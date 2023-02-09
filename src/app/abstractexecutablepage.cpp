#include "abstractexecutablepage.h"

#include <QPushButton>
#include <QStyle>
#include <QThread>

AbstractExecutablePage::AbstractExecutablePage(ADTExecutableRunner *run, QWidget *parent)
    : QWizardPage(parent)
    , pageUi(new ADTWizardPage<Ui::CheckWizardPage>())
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
    pageUi->ui->setupUi(this);

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

    pageUi->ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);
    pageUi->ui->detailsScrollAreaWidgetContents->setLayout(detailsLayout);

    pageUi->ui->mainProgressBar->setMinimum(0);
    pageUi->ui->mainProgressBar->setMaximum(100);
    pageUi->ui->mainProgressBar->setValue(0);
}

AbstractExecutablePage::~AbstractExecutablePage()
{
    delete pageUi;
}

void AbstractExecutablePage::runTasks()
{
    cleanUpUi();

    //hideFinishRadiobuttons();

    runner->resetStopFlag();

    workingThread = new QThread();

    connect(workingThread, SIGNAL(started()), runner, SLOT(runTasks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    runner->moveToThread(workingThread);

    workingThread->start();
}

void AbstractExecutablePage::beginAllTasks()
{
    disableButtonsBeforeChecks();
}

void AbstractExecutablePage::finishAllTasks()
{
    //showFinishRadiobuttons();

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
    pageUi->ui->mainProgressBar->setValue(progress);
}

void AbstractExecutablePage::messageChanged(QString message)
{
    pageUi->ui->currentStatusLabel->setText("Running task number: " + message);
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
    pageUi->ui->mainProgressBar->setValue(0);

    delete pageUi->ui->summaryScrollAreaWidgetContents;

    pageUi->ui->summaryScrollAreaWidgetContents = new QWidget();

    pageUi->ui->summaryScrollArea->setWidget(pageUi->ui->summaryScrollAreaWidgetContents);

    summaryLayout = new QVBoxLayout();

    summaryLayout->setAlignment(Qt::AlignTop);
    summaryLayout->insertStretch(0, 10);

    pageUi->ui->summaryScrollAreaWidgetContents->setLayout(summaryLayout);

    pageUi->ui->stackedWidget->setCurrentIndex(0);
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

    exchangeWidgetsInStackedWidget();
}

void AbstractExecutablePage::exchangeWidgetsInStackedWidget()
{
    pageUi->ui->stackedWidget->currentIndex() == 0 ? pageUi->ui->stackedWidget->setCurrentIndex(1)
                                                   : pageUi->ui->stackedWidget->setCurrentIndex(0);
}
