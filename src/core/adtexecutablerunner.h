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
#ifndef ADTEXECUTABLERUNNER_H
#define ADTEXECUTABLERUNNER_H

#include "adtexecutablerunnerprivate.h"

#include <memory>
#include <QDBusServiceWatcher>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>

class ADTExecutableRunner : public QObject
{
public:
    Q_OBJECT

public:
    ADTExecutableRunner(QJsonDocument QJsonDocument,
                        QString &serviceName,
                        QString &path,
                        QString &interfaceName);

    int getAmountOfTasks();

    void cancelTasks();

    void resetStopFlag();

    bool isAnyErrorsInTask();

    ADTExecutable *getTask(int id);

public slots:
    void runTasks();

private:
    void executeTask(std::unique_ptr<ADTExecutable> &task);

private:
    std::unique_ptr<ADTExecutableRunnerPrivate> d;

    volatile bool stopFlag;

    std::unique_ptr<QDBusConnection> dbus;
    std::unique_ptr<QDBusInterface> dbusInterface;

    QString dbusServiceName;
    QString dbusPath;
    QString dbusInterfaceName;
    QString dbusStdOutSignalName;
    QString dbusStdErrSignalName;

private:
    void connectExecutableSignals(std::unique_ptr<ADTExecutable> &task);
    void disconnectExecutableSignals(std::unique_ptr<ADTExecutable> &task);

signals:
    void onProgressUpdate(int progress);
    void messageChanged(QString);
    void onError();

    void beginTask(ADTExecutable *task);
    void finishTask(ADTExecutable *task);

    void begin();
    void finish();
};

#endif //ADTEXECUTABLERUNNER_H
