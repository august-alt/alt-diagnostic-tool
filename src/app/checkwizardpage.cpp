/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "checkwizardpage.h"
#include "adtwizard.h"
#include "ui_checkwizardpage.h"

#include <QPushButton>
#include <QStyle>

CheckWizardPage::CheckWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::CheckWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteChecks(false)
    , workingThread(nullptr)
    , currentIconLabel(nullptr)
    , currentTextLabel(nullptr)
    , summaryLayout(nullptr)
    , detailsLayout(nullptr)
    , detailsText(nullptr)
    , currentCheckDetailsButton(nullptr)
    , backToSummaryLogsButton(nullptr)
{
    ui->setupUi(this);

    summaryLayout = new QVBoxLayout();
    detailsLayout = new QVBoxLayout();

    summaryLayout->addStretch(10);

    backToSummaryLogsButton = new QPushButton();
    backToSummaryLogsButton->setText("Back");

    connect(backToSummaryLogsButton,
            SIGNAL(clicked()),
            this,
            SLOT(onbackToSummaryLogsButton_clicked()));

    QHBoxLayout *detailsHButtonLayout = new QHBoxLayout();
    detailsHButtonLayout->addStretch();
    detailsHButtonLayout->addWidget(backToSummaryLogsButton);

    detailsText = new QPlainTextEdit();
    detailsLayout->addWidget(detailsText);
    detailsLayout->insertLayout(10, detailsHButtonLayout);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);
    ui->detailsScrollAreaWidgetContents->setLayout(detailsLayout);

    ui->mainProgressBar->setMinimum(0);
    ui->mainProgressBar->setMaximum(100);
    ui->mainProgressBar->setValue(0);
}

bool CheckWizardPage::isComplete() const
{
    return isCompleteChecks;
}

int CheckWizardPage::nextId() const
{
    if (ui->runRepairRadioButton->isChecked())
    {
        return ADTWizard::Repair_Page;
    }
    if (ui->finishRadioButton->isChecked())
    {
        return ADTWizard::Finish_Page;
    }

    return ADTWizard::Check_Page;
}

void CheckWizardPage::showEvent(QShowEvent *event)
{
    if (isOpening)
    {
        return;
    }
    isOpening = true;

    QWizardPage::showEvent(event);

    runChecks();
}

void CheckWizardPage::runChecks()
{
    workingThread = new QThread();

    connect(diagnosticTool, SIGNAL(begin()), this, SLOT(disableNextButton()));

    connect(diagnosticTool, SIGNAL(finish()), this, SLOT(enableNextButton()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    connect(diagnosticTool,
            SIGNAL(beginTask(ADTExecutable *)),
            this,
            SLOT(beginCheck(ADTExecutable *)));
    connect(diagnosticTool,
            SIGNAL(finishTask(ADTExecutable *)),
            this,
            SLOT(finishCheck(ADTExecutable *)));

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void CheckWizardPage::onProgressUpdate(int progress)
{
    ui->mainProgressBar->setValue(progress);
}

void CheckWizardPage::messageChanged(QString message)
{
    ui->currentStatusLabel->setText("Running check number: " + message);
}

void CheckWizardPage::disableNextButton()
{
    isCompleteChecks = false;

    emit completeChanged();
}

void CheckWizardPage::enableNextButton()
{
    isCompleteChecks = true;

    emit completeChanged();
}

void CheckWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Check_Page)
    {
        diagnosticTool->cancelTask();

        if (!isCompleteChecks)
        {
            workingThread->wait();
        }
    }
}

void CheckWizardPage::beginCheck(ADTExecutable *check)
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    currentIconLabel          = new QLabel();
    currentTextLabel          = new QLabel();
    currentCheckDetailsButton = new QPushButton();

    currentCheckDetailsButton->setText("Details");

    currentCheckDetailsButton->setProperty("taskId", QVariant(check->m_id));

    connect(currentCheckDetailsButton,
            SIGNAL(clicked()),
            this,
            SLOT(currentCheckDetailsButton_clicked()));

    QIcon icon = style()->standardIcon(QStyle::SP_BrowserReload);
    currentIconLabel->setPixmap(icon.pixmap(QSize(16, 16)));

    currentTextLabel->setText("Running " + check->m_name + " check...");

    hLayout->addWidget(currentIconLabel);
    hLayout->addWidget(currentTextLabel);
    hLayout->addStretch(10);
    hLayout->addWidget(currentCheckDetailsButton);

    summaryLayout->insertLayout(0, hLayout);
}

void CheckWizardPage::finishCheck(ADTExecutable *check)
{
    if (currentIconLabel == nullptr || currentTextLabel == nullptr)
    {
        return;
    }

    QIcon icon = style()->standardIcon(QStyle::SP_DialogApplyButton);
    currentTextLabel->setText("Check " + check->m_name + " completed");

    if (check->m_exit_code != 0)
    {
        icon = style()->standardIcon(QStyle::SP_DialogCloseButton);
        currentTextLabel->setText("Check " + check->m_name + " failed");
    }

    currentIconLabel->setPixmap(icon.pixmap(QSize(16, 16)));
}

void CheckWizardPage::onbackToSummaryLogsButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void CheckWizardPage::currentCheckDetailsButton_clicked()
{
    QPushButton *senderPtr = dynamic_cast<QPushButton *>(sender());
    if (senderPtr != nullptr)
    {
        QString id = senderPtr->property("taskId").toString();

        ADTExecutable *check = diagnosticTool->getCheck(id.toInt());

        if (check != nullptr)
        {
            detailsText->clear();

            detailsText->appendPlainText(check->m_stdout);
            detailsText->appendPlainText(check->m_stderr);
        }

        onbackToSummaryLogsButton_clicked();
    }
}
