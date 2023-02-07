#include "adtexecutablerunner.h"

#include <QJsonDocument>
#include <QThread>
#include <QtWidgets/QApplication>

ADTExecutableRunner::ADTExecutableRunner(QJsonDocument document)
    : d(std::make_unique<ADTExecutableRunnerPrivate>(document))
    , stopFlag(false)
    , dbus(std::make_unique<QDBusConnection>(QDBusConnection::systemBus()))
    , dbusInterface(std::make_unique<QDBusInterface>("ru.basealt.alterator",
                                                     "/ru/basealt/alterator/executor",
                                                     "ru.basealt.alterator.executor",
                                                     *dbus.get()))
{}

void ADTExecutableRunner::runTasks()
{
    emit begin();

    int taskSize = d->tasks->size();

    if (taskSize == 0)
    {
        emit onProgressUpdate(100);

        this->moveToThread(QApplication::instance()->thread());

        emit finish();

        QThread::currentThread()->quit();

        return;
    }

    int percentByOneCheck = 100 / taskSize;

    int progress = 0;

    auto &tasks = (*d->tasks.get());

    for (auto &task : tasks)
    {
        if (stopFlag)
        {
            break;
        }

        emit messageChanged(task.get()->m_name);

        executeTask(task);
        QThread::sleep(1); //delay, in order to see the progress

        progress = progress + percentByOneCheck;

        emit onProgressUpdate(progress);
    }

    emit onProgressUpdate(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();

    QThread::currentThread()->quit();
}

ADTExecutable *ADTExecutableRunner::getTask(int id)
{
    for (auto &task : *d->tasks)
    {
        if (task.get()->m_id == id)
        {
            return task.get();
        }
    }

    return nullptr;
}

void ADTExecutableRunner::executeTask(std::unique_ptr<ADTExecutable> &task)
{
    //There is no backend at the moment, so we use the test command and signals

    connectExecutableSignals(task);

    emit beginTask(task.get());

    QDBusReply<int> reply = dbusInterface->call("test1", "\"dev\"");

    disconnectExecutableSignals(task);

    task.get()->m_exit_code = reply.value();

    emit finishTask(task.get());
}

int ADTExecutableRunner::getAmountOfTasks()
{
    return d->tasks.get()->size();
}

void ADTExecutableRunner::cancelTasks()
{
    stopFlag = true;
}

void ADTExecutableRunner::resetStopFlag()
{
    stopFlag = false;
}

bool ADTExecutableRunner::isAnyErrorsInTask()
{
    for (auto &task : *d->tasks)
    {
        if (task.get()->m_exit_code != 0)
        {
            return true;
        }
    }

    return false;
}

void ADTExecutableRunner::connectExecutableSignals(std::unique_ptr<ADTExecutable> &task)
{
    dbusInterface->connection().connect(QLatin1String("ru.basealt.alterator"),
                                        QLatin1String("/ru/basealt/alterator/executor"),
                                        QLatin1String("ru.basealt.alterator.executor"),
                                        QLatin1String("executor_stdout"),
                                        task.get(),
                                        SLOT(getStdout(QString)));
    dbusInterface->connection().connect("ru.basealt.alterator",
                                        "/ru/basealt/alterator/executor",
                                        "ru.basealt.alterator.executor",
                                        "executor_stderr",
                                        task.get(),
                                        SLOT(getStderr(QString)));
}

void ADTExecutableRunner::disconnectExecutableSignals(std::unique_ptr<ADTExecutable> &task)
{
    dbusInterface->connection().disconnect("ru.basealt.alterator",
                                           "/ru/basealt/alterator/executor",
                                           "ru.basealt.alterator.executor",
                                           "executor_stdout",
                                           task.get(),
                                           SLOT(getStdout(QString)));

    dbusInterface->connection().disconnect("ru.basealt.alterator",
                                           "/ru/basealt/alterator/executor",
                                           "ru.basealt.alterator.executor",
                                           "executor_stderr",
                                           task.get(),
                                           SLOT(getStderr(QString)));
}
