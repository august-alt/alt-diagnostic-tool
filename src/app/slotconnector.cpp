#include "slotconnector.h"

SlotConnector::SlotConnector() {}

void SlotConnector::connectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page)
{
    QObject::connect(diagTool, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    QObject::connect(diagTool, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    QObject::connect(diagTool, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    QObject::connect(diagTool, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    QObject::connect(diagTool,
                     SIGNAL(beginTask(ADTExecutable *)),
                     page,
                     SLOT(beginCurrentTask(ADTExecutable *)));
    QObject::connect(diagTool,
                     SIGNAL(finishTask(ADTExecutable *)),
                     page,
                     SLOT(finishCurrentTask(ADTExecutable *)));
}

void SlotConnector::disconnectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page) {}
