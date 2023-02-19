/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "abstractexecutablepage.h"

#include <QPushButton>
#include <QStyle>
#include <QThread>

const int MAIN_PROGRESSBAR_MINIMUM = 0;
const int MAIN_PROGRESSBAR_MAXIMUM = 100;
const int LAYOUT_STRETCH_INDEX     = 0;
const int LAYOUT_STRETCH_FACTOR    = 10;
const int LAYOUT_INDEX             = 10;

AbstractExecutablePage::AbstractExecutablePage(ADTExecutableRunner *run, QWidget *parent)
    : QWizardPage(parent)
    , ui()
    , runner(run)
    , isCompleteTasks(false)
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
    backToSummaryLogsButton->setText(tr("Back"));

    connect(backToSummaryLogsButton, SIGNAL(clicked()), this, SLOT(toggleWidgetsInStackedWidget()));

    QHBoxLayout *detailsHButtonLayout = new QHBoxLayout();
    detailsHButtonLayout->addStretch();
    detailsHButtonLayout->addWidget(backToSummaryLogsButton);

    detailsText = new QPlainTextEdit();
    detailsLayout->addWidget(detailsText);
    detailsLayout->insertLayout(LAYOUT_INDEX, detailsHButtonLayout);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);
    ui->detailsScrollAreaWidgetContents->setLayout(detailsLayout);

    ui->mainProgressBar->setMinimum(MAIN_PROGRESSBAR_MINIMUM);
    ui->mainProgressBar->setMaximum(MAIN_PROGRESSBAR_MAXIMUM);
    ui->mainProgressBar->setValue(MAIN_PROGRESSBAR_MINIMUM);
}

AbstractExecutablePage::~AbstractExecutablePage() {}

void AbstractExecutablePage::runTasks()
{
    cleanUpUi();

    runner->resetStopFlag();

    workingThread = new QThread();

    connect(workingThread, SIGNAL(started()), runner, SLOT(runTasks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    runner->moveToThread(workingThread);

    workingThread->start();
}

bool AbstractExecutablePage::isComplete() const
{
    return isCompleteTasks;
}

int AbstractExecutablePage::getAmountOfTasks()
{
    return runner->getAmountOfTasks();
}

bool AbstractExecutablePage::isAnyErrorsInTasks()
{
    return runner->isAnyErrorsInTask();
}

void AbstractExecutablePage::beginAllTasks()
{
    disableButtonsBeforeChecks();
}

void AbstractExecutablePage::finishAllTasks()
{
    QObject::disconnect(workingThread, SIGNAL(started()), runner, SLOT(runTasks()));

    enableButtonsAfterChecks();
}

void AbstractExecutablePage::beginCurrentTask(ADTExecutable *task)
{
    ExecutableStatusWidget *statusWidget = new ExecutableStatusWidget(task->m_id);
    connect(statusWidget,
            SIGNAL(onDetailsButtonPressed(int)),
            this,
            SLOT(currentTaskDetailsButton_clicked(int)));

    statusWidget->setText(tr("Running ") + task->m_name + tr(" task..."));

    summaryLayout->insertWidget(0, statusWidget, Qt::AlignTop);

    currentCheckWidget = statusWidget;
}

void AbstractExecutablePage::finishCurrentTask(ADTExecutable *task)
{
    QIcon icon = style()->standardIcon(QStyle::SP_DialogApplyButton);
    currentCheckWidget->setText(tr("Task ") + task->m_name + tr(" completed"));

    if (task->m_exit_code != 0)
    {
        icon = style()->standardIcon(QStyle::SP_DialogCloseButton);
        currentCheckWidget->setText(tr("Task ") + task->m_name + tr(" failed"));
    }
    currentCheckWidget->setIcon(icon);
}

void AbstractExecutablePage::onProgressUpdate(int progress)
{
    ui->mainProgressBar->setValue(progress);
}

void AbstractExecutablePage::messageChanged(QString message)
{
    ui->currentStatusLabel->setText(tr("Running task number: ") + message);
}

void AbstractExecutablePage::enableButtonsAfterChecks()
{
    isCompleteTasks = true;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(false);

    wizard()->button(QWizard::BackButton)->setEnabled(true);
}

void AbstractExecutablePage::disableButtonsBeforeChecks()
{
    isCompleteTasks = false;

    emit completeChanged();

    wizard()->button(QWizard::CancelButton)->setEnabled(true);

    wizard()->button(QWizard::BackButton)->setEnabled(false);
}

void AbstractExecutablePage::cleanUpUi()
{
    ui->mainProgressBar->setValue(MAIN_PROGRESSBAR_MINIMUM);

    delete ui->summaryScrollAreaWidgetContents;

    ui->summaryScrollAreaWidgetContents = new QWidget();

    ui->summaryScrollArea->setWidget(ui->summaryScrollAreaWidgetContents);

    summaryLayout = new QVBoxLayout();

    summaryLayout->setAlignment(Qt::AlignTop);
    summaryLayout->insertStretch(LAYOUT_STRETCH_INDEX, LAYOUT_STRETCH_FACTOR);

    ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);

    ui->stackedWidget->setCurrentIndex(0);
}

void AbstractExecutablePage::currentTaskDetailsButton_clicked(int id)
{
    ADTExecutable *task = runner->getTask(id);

    if (task != nullptr)
    {
        detailsText->clear();

        detailsText->appendPlainText(task->m_stdout);
        detailsText->appendPlainText(task->m_stderr);
    }

    toggleWidgetsInStackedWidget();
}

void AbstractExecutablePage::toggleWidgetsInStackedWidget()
{
    ui->stackedWidget->currentIndex() == 0 ? ui->stackedWidget->setCurrentIndex(1)
                                           : ui->stackedWidget->setCurrentIndex(0);
}
