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

#include "repairwizardpage.h"
#include "adtwizard.h"
#include "ui_repairwizardpage.h"

#include <QPushButton>
#include <QStyle>

RepairWizardPage::RepairWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::RepairWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteResolvers(false)
    , workingThread(nullptr)
    , currentIconLabel(nullptr)
    , currentTextLabel(nullptr)
    , summaryLayout(nullptr)
    , detailsLayout(nullptr)
    , detailsText(nullptr)
    , currentResolvDetailsButton(nullptr)
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

    ui->repairProgressBar->setMinimum(0);
    ui->repairProgressBar->setMaximum(100);
    ui->repairProgressBar->setValue(0);
}

bool RepairWizardPage::isComplete() const
{
    return isCompleteResolvers;
}

void RepairWizardPage::showEvent(QShowEvent *event)
{
    if (isOpening)
    {
        return;
    }
    isOpening = true;

    QWizardPage::showEvent(event);

    runResolvers();
}

void RepairWizardPage::runResolvers()
{
    workingThread = new QThread();

    connect(diagnosticTool, SIGNAL(begin()), this, SLOT(disableNextButton()));

    connect(diagnosticTool, SIGNAL(finish()), this, SLOT(enableNextButton()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    connect(diagnosticTool,
            SIGNAL(beginTask(ADTExecutable *)),
            this,
            SLOT(beginResolv(ADTExecutable *)));
    connect(diagnosticTool,
            SIGNAL(finishTask(ADTExecutable *)),
            this,
            SLOT(finishResolv(ADTExecutable *)));

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runResolvers()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void RepairWizardPage::onProgressUpdate(int progress)
{
    ui->repairProgressBar->setValue(progress);
}

void RepairWizardPage::messageChanged(QString message)
{
    ui->statusLabel->setText("Running resolver: " + message);
}

void RepairWizardPage::disableNextButton()
{
    isCompleteResolvers = false;

    emit completeChanged();
}

void RepairWizardPage::enableNextButton()
{
    isCompleteResolvers = true;

    emit completeChanged();
}

void RepairWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Repair_Page)
    {
        diagnosticTool->cancelTask();

        if (!isCompleteResolvers)
        {
            workingThread->wait();
        }
    }
}

void RepairWizardPage::beginResolv(ADTExecutable *resolv)
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    currentIconLabel          = new QLabel();
    currentTextLabel          = new QLabel();
    currentResolvDetailsButton = new QPushButton();

    currentResolvDetailsButton->setText("Details");

    currentResolvDetailsButton->setProperty("taskId", QVariant(resolv->m_id));

    connect(currentResolvDetailsButton,
            SIGNAL(clicked()),
            this,
            SLOT(currentResolvDetailsButton_clicked()));

    QIcon icon = style()->standardIcon(QStyle::SP_BrowserReload);
    currentIconLabel->setPixmap(icon.pixmap(QSize(16, 16)));

    currentTextLabel->setText("Running " + resolv->m_name + " check...");

    hLayout->addWidget(currentIconLabel);
    hLayout->addWidget(currentTextLabel);
    hLayout->addStretch(10);
    hLayout->addWidget(currentResolvDetailsButton);

    summaryLayout->insertLayout(0, hLayout);
}

void RepairWizardPage::finishResolv(ADTExecutable *resolv)
{
    if (currentIconLabel == nullptr || currentTextLabel == nullptr)
    {
        return;
    }

    QIcon icon = style()->standardIcon(QStyle::SP_DialogApplyButton);
    currentTextLabel->setText("Resolver " + resolv->m_name + " completed");

    if (resolv->m_exit_code != 0)
    {
        icon = style()->standardIcon(QStyle::SP_DialogCloseButton);
        currentTextLabel->setText("Resolver " + resolv->m_name + " failed");
    }

    currentIconLabel->setPixmap(icon.pixmap(QSize(16, 16)));
}

void RepairWizardPage::onbackToSummaryLogsButton_clicked()
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

void RepairWizardPage::currentResolvDetailsButton_clicked()
{
    QPushButton *senderPtr = dynamic_cast<QPushButton *>(sender());
    if (senderPtr != nullptr)
    {
        QString id = senderPtr->property("taskId").toString();

        ADTExecutable *resolv = diagnosticTool->getResolv(id.toInt());

        if (resolv != nullptr)
        {
            detailsText->clear();

            detailsText->appendPlainText(resolv->m_stdout);
            detailsText->appendPlainText(resolv->m_stderr);
        }

        onbackToSummaryLogsButton_clicked();
    }
}
