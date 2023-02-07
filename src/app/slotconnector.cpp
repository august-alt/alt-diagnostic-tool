#include "slotconnector.h"

SlotConnector::SlotConnector() {}

void SlotConnector::connectSignals(ADTExecutableRunner *runner, AbstractExecutablePage *page)
{
    connect(runner, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    connect(runner, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    connect(runner, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    connect(runner, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    connect(runner,
            SIGNAL(beginTask(ADTExecutable *)),
            page,
            SLOT(beginCurrentTask(ADTExecutable *)));
    connect(runner,
            SIGNAL(finishTask(ADTExecutable *)),
            page,
            SLOT(finishCurrentTask(ADTExecutable *)));
}

void SlotConnector::disconnectSignals(ADTExecutableRunner *runner, AbstractExecutablePage *page)
{
    disconnect(runner, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    disconnect(runner, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    disconnect(runner, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    disconnect(runner, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    disconnect(runner,
               SIGNAL(beginTask(ADTExecutable *)),
               page,
               SLOT(beginCurrentTask(ADTExecutable *)));
    disconnect(runner,
               SIGNAL(finishTask(ADTExecutable *)),
               page,
               SLOT(finishCurrentTask(ADTExecutable *)));
}
