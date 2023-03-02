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
#include "adtexecutablerunner.h"

#include <QJsonDocument>
#include <QThread>
#include <QtWidgets/QApplication>

ADTExecutableRunner::ADTExecutableRunner(QJsonDocument document,
                                         QString &serviceName,
                                         QString &path,
                                         QString &interfaceName)
    : d(std::make_unique<ADTExecutableRunnerPrivate>(document))
    , stopFlag(false)
    , dbus(std::make_unique<QDBusConnection>(QDBusConnection::systemBus()))
    , dbusInterface(std::make_unique<QDBusInterface>(serviceName, path, interfaceName, *dbus.get()))
    , dbusServiceName(serviceName)
    , dbusPath(path)
    , dbusInterfaceName(interfaceName)
    , dbusStdOutSignalName("executor_stdout")
    , dbusStdErrSignalName("executor_stderr")

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
    dbusInterface->connection().connect(dbusServiceName,
                                        dbusPath,
                                        dbusInterfaceName,
                                        dbusStdOutSignalName,
                                        task.get(),
                                        SLOT(getStdout(QString)));
    dbusInterface->connection().connect(dbusServiceName,
                                        dbusPath,
                                        dbusInterfaceName,
                                        dbusStdErrSignalName,
                                        task.get(),
                                        SLOT(getStderr(QString)));
}

void ADTExecutableRunner::disconnectExecutableSignals(std::unique_ptr<ADTExecutable> &task)
{
    dbusInterface->connection().disconnect(dbusServiceName,
                                           dbusPath,
                                           dbusInterfaceName,
                                           dbusStdOutSignalName,
                                           task.get(),
                                           SLOT(getStdout(QString)));

    dbusInterface->connection().disconnect(dbusServiceName,
                                           dbusPath,
                                           dbusInterfaceName,
                                           dbusStdErrSignalName,
                                           task.get(),
                                           SLOT(getStderr(QString)));
}
