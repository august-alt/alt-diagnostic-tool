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

#include "diagnostictool.h"
#include "adtexecutable.h"

#include <QJsonDocument>
#include <QThread>

#include <QtWidgets/QApplication>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

DiagnosticTool::DiagnosticTool(QJsonDocument document)
    : d(nullptr)
    , stopFlag(false)
    , dbus(std::make_unique<QDBusConnection>(QDBusConnection::systemBus()))
    , dbusInterface(std::make_unique<QDBusInterface>("ru.basealt.alterator",
                                                     "/ru/basealt/alterator/executor",
                                                     "ru.basealt.alterator.executor",
                                                     *dbus.get()))

{
    d.reset(new DiagnosticToolPrivate(document));
}

void DiagnosticTool::runChecks()
{
    emit begin();

    int checkSize = d->checks->size();

    if (checkSize == 0)
        return;

    int percentByOneCheck = 100 / checkSize;

    int progress = 0;

    auto &checks = (*d->checks.get());

    for (auto &check : checks)
    {
        if (stopFlag)
        {
            break;
        }

        emit messageChanged(check.get()->m_name);

        executeCommand(check);
        QThread::sleep(1); //delay, in order to see the progress

        progress = progress + percentByOneCheck;

        emit onProgressUpdate(progress);
    }

    emit onProgressUpdate(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();

    QThread::currentThread()->quit();
}

void DiagnosticTool::runResolvers()
{
    emit begin();

    int resolversSize = d->resolvers->size();

    if (resolversSize == 0)
        return;

    int percentByOneResolver = 100 / resolversSize;

    int progress = 0;

    auto &resolvers = (*d->resolvers.get());

    for (auto &resolver : resolvers)
    {
        if (stopFlag)
        {
            break;
        }

        emit messageChanged(resolver.get()->m_name);

        executeCommand(resolver);
        QThread::sleep(1); //delay, in order to see the progress

        progress = progress + percentByOneResolver;

        emit onProgressUpdate(progress);
    }

    emit onProgressUpdate(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();

    QThread::currentThread()->quit();
}

void DiagnosticTool::executeCommand(std::unique_ptr<ADTExecutable> &task)
{
    //There is no backend at the moment, so we use the test command and signals

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

    QDBusReply<int> reply = dbusInterface->call("test1", "\"dev\"");

    dbusInterface->connection().disconnect("ru.basealt",
                                           "/ru/basealt/alterator/executor",
                                           "ru.basealt.alterator.executor",
                                           "executor_stdout",
                                           task.get(),
                                           SLOT(getStdout(QString)));
    dbusInterface->connection().disconnect("ru.basealt",
                                           "/ru/basealt/alterator/executor",
                                           "ru.basealt.alterator.executor",
                                           "executor_stderr",
                                           task.get(),
                                           SLOT(getStderr(QString)));
}

void DiagnosticTool::cancelTask()
{
    stopFlag = true;
}

unsigned int DiagnosticTool::getAmountOfChecks()
{
    return d->checks->size();
}

unsigned int DiagnosticTool::getAmountOfResolvers()
{
    return d->resolvers->size();
}
