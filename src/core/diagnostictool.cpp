#include "diagnostictool.h"

#include <QJsonDocument>
#include <QThread>

#include <QtWidgets/QApplication>

DiagnosticTool::DiagnosticTool(QJsonDocument &document)
    : d(std::make_unique<DiagnosticToolPrivate>(document))
{}

void DiagnosticTool::runChecks()
{
    emit begin();

    int checkSize = d->checks.get()->size();

    int percentByOneCheck = 100 / checkSize;

    int progress = 0;

    for (int i = 0; i < checkSize; i++)
    {
        ADTExecutable *currentTask = d->checks.get()->at(i).get();

        emit messageChanged(currentTask->m_name);

        QThread::sleep(3);

        emit progressChanged(progress + (i + 1) * percentByOneCheck);
    }

    emit progressChanged(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();
}

void DiagnosticTool::runResolvers()
{
    emit begin();

    int resolversSize = d->resolvers.get()->size();

    int percentByOneResolver = 100 / resolversSize;

    int progress = 0;

    for (int i = 0; i < resolversSize; i++)
    {
        ADTExecutable *currentTask = d->resolvers.get()->at(i).get();

        emit messageChanged(currentTask->m_name);

        QThread::sleep(3);

        emit progressChanged(progress + (i + 1) * percentByOneResolver);
    }

    emit progressChanged(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();
}

void DiagnosticTool::cancelTask(bool stop) {}
