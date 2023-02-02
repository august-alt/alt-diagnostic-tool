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
#include "executablestatuswidget.h"
#include "ui_checkwizardpage.h"

#include <QPushButton>
#include <QStyle>

CheckWizardPage::CheckWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : AbstractExecutablePage(parent)
    , ui(new Ui::CheckWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteChecks(false)
    , workingThread(nullptr)
    , currentCheckWidget(nullptr)
    , summaryLayout(nullptr)
    , detailsLayout(nullptr)
    , detailsText(nullptr)
    , currentCheckDetailsButton(nullptr)
    , backToSummaryLogsButton(nullptr)
{
    ui->setupUi(this);

    summaryLayout = new QVBoxLayout();
    detailsLayout = new QVBoxLayout();

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

void CheckWizardPage::beginAllTasks()
{
    disableButtonsBeforeChecks();
}

void CheckWizardPage::finishAllTasks()
{
    showFinishRadiobuttons();

    disconnect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

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

void CheckWizardPage::beginCurrentTask(ADTExecutable *task)
{
    ExecutableStatusWidget *statusWidget = new ExecutableStatusWidget(task->m_id);
    connect(statusWidget,
            SIGNAL(onDetailsButtonPressed(int)),
            this,
            SLOT(currentCheckDetailsButton_clicked(int)));

    statusWidget->setText("Running " + task->m_name + " check...");

    summaryLayout->insertWidget(0, statusWidget, Qt::AlignTop);

    currentCheckWidget = statusWidget;
}

void CheckWizardPage::finishCurrentTask(ADTExecutable *task)
{
    QIcon icon = style()->standardIcon(QStyle::SP_DialogApplyButton);
    currentCheckWidget->setText("Check " + task->m_name + " completed");

    if (task->m_exit_code != 0)
    {
        icon = style()->standardIcon(QStyle::SP_DialogCloseButton);
        currentCheckWidget->setText("Check " + task->m_name + " failed");
    }
    currentCheckWidget->setIcon(icon);
}

void CheckWizardPage::exchangeWidgetsInStackedWidget()
{
    ui->stackedWidget->currentIndex() == 0 ? ui->stackedWidget->setCurrentIndex(1)
                                           : ui->stackedWidget->setCurrentIndex(0);
}

void CheckWizardPage::currentIdChanged(int id)
{
    if (id == ADTWizard::Check_Page)
    {
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

    summaryLayout->setAlignment(Qt::AlignTop);
    summaryLayout->insertStretch(0, 10);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);

    ui->stackedWidget->setCurrentIndex(0);
}

void CheckWizardPage::currentCheckDetailsButton_clicked(int id)
{
    ADTExecutable *check = diagnosticTool->getCheck(id);

    if (check != nullptr)
    {
        detailsText->clear();

        detailsText->appendPlainText(check->m_stdout);
        detailsText->appendPlainText(check->m_stderr);
    }

    exchangeWidgetsInStackedWidget();
}
