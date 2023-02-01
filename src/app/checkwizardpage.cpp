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
            SLOT(exchangeWidgetsInStackedWidget()));

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

    ui->finishRadioButton->setChecked(true);
    setRadiobuttonSizePolicy();
    hideFinishRadiobuttons();
}

bool CheckWizardPage::isComplete() const
{
    return isCompleteChecks;
}

int CheckWizardPage::nextId() const
{
    if (diagnosticTool->anyErrorsInChecks() && diagnosticTool->hasAnyResolvers()
        && ui->runRepairRadioButton->isChecked())
    {
        return ADTWizard::Repair_Page;
    }
    else
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

    connect(wizard(), SIGNAL(currentIdChanged(int)), this, SLOT(currentIdChanged(int)));
}

void CheckWizardPage::runChecks()
{
    cleanUpUi();

    hideFinishRadiobuttons();

    workingThread = new QThread();

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void CheckWizardPage::connectSlotsToDiagnosticTool()
{
    connect(diagnosticTool, SIGNAL(begin()), this, SLOT(beginAllChecks()));

    connect(diagnosticTool, SIGNAL(finish()), this, SLOT(finishAllChecks()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    connect(diagnosticTool,
            SIGNAL(beginTask(ADTExecutable *)),
            this,
            SLOT(beginCurrentCheck(ADTExecutable *)));
    connect(diagnosticTool,
            SIGNAL(finishTask(ADTExecutable *)),
            this,
            SLOT(finishCurrentCheck(ADTExecutable *)));
}

void CheckWizardPage::disconnectSlotToDiagnosticTool()
{
    disconnect(diagnosticTool, SIGNAL(begin()), this, SLOT(beginAllChecks()));

    disconnect(diagnosticTool, SIGNAL(finish()), this, SLOT(finishAllChecks()));

    disconnect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    disconnect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    disconnect(diagnosticTool,
               SIGNAL(beginTask(ADTExecutable *)),
               this,
               SLOT(beginCurrentCheck(ADTExecutable *)));
    disconnect(diagnosticTool,
               SIGNAL(finishTask(ADTExecutable *)),
               this,
               SLOT(finishCurrentCheck(ADTExecutable *)));
}

void CheckWizardPage::enableButtonsAfterChecks()
{
    isCompleteChecks = true;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(false);

    wizard()->button(QWizard::BackButton)->setEnabled(true);
}

void CheckWizardPage::disableButtonsBeforeChecks()
{
    isCompleteChecks = false;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(true);

    wizard()->button(QWizard::BackButton)->setEnabled(false);
}

void CheckWizardPage::showFinishRadiobuttons()
{
    if (diagnosticTool->anyErrorsInChecks() && diagnosticTool->hasAnyResolvers())
    {
        ui->finishRadioButton->setVisible(true);
        ui->runRepairRadioButton->setVisible(true);
    }
}

void CheckWizardPage::hideFinishRadiobuttons()
{
    ui->finishRadioButton->setVisible(false);
    ui->runRepairRadioButton->setVisible(false);
}

void CheckWizardPage::setRadiobuttonSizePolicy()
{
    QSizePolicy finishRadioButtonPolicy = ui->finishRadioButton->sizePolicy();
    finishRadioButtonPolicy.setRetainSizeWhenHidden(true);
    ui->finishRadioButton->setSizePolicy(finishRadioButtonPolicy);

    QSizePolicy runRepairRadioButtonpolicy = ui->runRepairRadioButton->sizePolicy();
    runRepairRadioButtonpolicy.setRetainSizeWhenHidden(true);
    ui->runRepairRadioButton->setSizePolicy(runRepairRadioButtonpolicy);
}

void CheckWizardPage::onProgressUpdate(int progress)
{
    ui->mainProgressBar->setValue(progress);
}

void CheckWizardPage::messageChanged(QString message)
{
    ui->currentStatusLabel->setText("Running check number: " + message);
}

void CheckWizardPage::beginAllChecks()
{
    disableButtonsBeforeChecks();
}

void CheckWizardPage::finishAllChecks()
{
    showFinishRadiobuttons();

    disconnect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    disconnectSlotToDiagnosticTool();

    enableButtonsAfterChecks();
}

void CheckWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Check_Page)
    {
        diagnosticTool->cancelTask();

        if (!isCompleteChecks)
        {
            workingThread->wait();

            enableButtonsAfterChecks();

            showFinishRadiobuttons();
        }

        ui->stackedWidget->setCurrentIndex(0);
    }
}

void CheckWizardPage::beginCurrentCheck(ADTExecutable *check)
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

void CheckWizardPage::finishCurrentCheck(ADTExecutable *check)
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

void CheckWizardPage::exchangeWidgetsInStackedWidget()
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

void CheckWizardPage::currentIdChanged(int id)
{
    if (id == ADTWizard::Check_Page)
    {
        connectSlotsToDiagnosticTool();

        diagnosticTool->resetStopFlag();

        runChecks();
    }
}

void CheckWizardPage::cleanUpUi()
{
    ui->mainProgressBar->setValue(0);

    delete ui->summaryScrollAreaWidgetContents;

    ui->summaryScrollAreaWidgetContents = new QWidget();

    ui->summaryScrollArea->setWidget(ui->summaryScrollAreaWidgetContents);

    summaryLayout = new QVBoxLayout();

    summaryLayout->addStretch(10);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);

    ui->stackedWidget->setCurrentIndex(0);
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

        exchangeWidgetsInStackedWidget();
    }
}
