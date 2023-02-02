#include "slotconnector.h"

SlotConnector::SlotConnector() {}

void SlotConnector::connectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page)
{
    connect(diagTool, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    connect(diagTool, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    connect(diagTool, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    connect(diagTool, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    connect(diagTool,
            SIGNAL(beginTask(ADTExecutable *)),
            page,
            SLOT(beginCurrentTask(ADTExecutable *)));
    connect(diagTool,
            SIGNAL(finishTask(ADTExecutable *)),
            page,
            SLOT(finishCurrentTask(ADTExecutable *)));
}

void SlotConnector::disconnectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page)
{
    disconnect(diagTool, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    disconnect(diagTool, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    disconnect(diagTool, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    disconnect(diagTool, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    disconnect(diagTool,
               SIGNAL(beginTask(ADTExecutable *)),
               page,
               SLOT(beginCurrentTask(ADTExecutable *)));
    disconnect(diagTool,
               SIGNAL(finishTask(ADTExecutable *)),
               page,
               SLOT(finishCurrentTask(ADTExecutable *)));
}
