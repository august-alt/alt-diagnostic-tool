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
    : AbstractExecutablePage(parent)
    , ui(new Ui::RepairWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteResolvers(false)
    , workingThread(nullptr)
    , currentResolvWidget(nullptr)
    , summaryLayout(nullptr)
    , detailsLayout(nullptr)
    , detailsText(nullptr)
    , currentResolvDetailsButton(nullptr)
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

    connect(wizard(), SIGNAL(currentIdChanged(int)), this, SLOT(currentIdChanged(int)));
}

void RepairWizardPage::runResolvers()
{
    cleanUpUi();

    workingThread = new QThread();

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runResolvers()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void RepairWizardPage::enableButtonsAfterChecks()
{
    isCompleteResolvers = true;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(false);

    wizard()->button(QWizard::BackButton)->setEnabled(true);
}

void RepairWizardPage::disableButtonsBeforeChecks()
{
    isCompleteResolvers = false;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(true);

    wizard()->button(QWizard::BackButton)->setEnabled(false);
}

void RepairWizardPage::onProgressUpdate(int progress)
{
    ui->repairProgressBar->setValue(progress);
}

void RepairWizardPage::messageChanged(QString message)
{
    ui->statusLabel->setText("Running resolver: " + message);
}

void RepairWizardPage::beginAllTasks()
{
    disableButtonsBeforeChecks();
}

void RepairWizardPage::finishAllTasks()
{
    disconnect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    enableButtonsAfterChecks();
}

void RepairWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Check_Page)
    {
        diagnosticTool->cancelTask();

        if (!isCompleteResolvers)
        {
            workingThread->wait();

            enableButtonsAfterChecks();
        }

        ui->stackedWidget->setCurrentIndex(0);
    }
}

void RepairWizardPage::beginCurrentTask(ADTExecutable *task)
{
    ExecutableStatusWidget *statusWidget = new ExecutableStatusWidget(task->m_id);
    connect(statusWidget,
            SIGNAL(onDetailsButtonPressed(int)),
            this,
            SLOT(currentResolvDetailsButton_clicked(int)));

    statusWidget->setText("Running " + task->m_name + " resolver...");

    summaryLayout->insertWidget(0, statusWidget, Qt::AlignTop);

    currentResolvWidget = statusWidget;
}

void RepairWizardPage::finishCurrentTask(ADTExecutable *task)
{
    QIcon icon = style()->standardIcon(QStyle::SP_DialogApplyButton);
    currentResolvWidget->setText("Resolver " + task->m_name + " completed");

    if (task->m_exit_code != 0)
    {
        icon = style()->standardIcon(QStyle::SP_DialogCloseButton);
        currentResolvWidget->setText("Resolver " + task->m_name + " failed");
    }
    currentResolvWidget->setIcon(icon);
}

void RepairWizardPage::currentResolvDetailsButton_clicked(int id)
{
    ADTExecutable *resolv = diagnosticTool->getResolv(id);

    if (resolv != nullptr)
    {
        detailsText->clear();

        detailsText->appendPlainText(resolv->m_stdout);
        detailsText->appendPlainText(resolv->m_stderr);
    }

    exchangeWidgetsInStackedWidget();
}

void RepairWizardPage::exchangeWidgetsInStackedWidget()
{
    ui->stackedWidget->currentIndex() == 0 ? ui->stackedWidget->setCurrentIndex(1)
                                           : ui->stackedWidget->setCurrentIndex(0);
}

void RepairWizardPage::currentIdChanged(int id)
{
    if (id == ADTWizard::Repair_Page)
    {
        diagnosticTool->resetStopFlag();

        runResolvers();
    }
}

void RepairWizardPage::cleanUpUi()
{
    ui->repairProgressBar->setValue(0);

    delete ui->summaryScrollAreaWidgetContents;

    ui->summaryScrollAreaWidgetContents = new QWidget();

    ui->summaryScrollArea->setWidget(ui->summaryScrollAreaWidgetContents);

    summaryLayout = new QVBoxLayout();

    summaryLayout->setAlignment(Qt::AlignTop);
    summaryLayout->insertStretch(0, 10);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);

    ui->stackedWidget->setCurrentIndex(0);
}
