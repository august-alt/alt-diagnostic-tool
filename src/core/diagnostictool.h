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

#ifndef DIAGNOSTICTOOL_H
#define DIAGNOSTICTOOL_H

#include "diagnostictoolprivate.h"

#include <QJsonDocument>
#include <QScopedPointer>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>

class DiagnosticTool : public QObject
{
public:
    Q_OBJECT

public:
    DiagnosticTool(QJsonDocument document);

    void cancelTask();

    unsigned int getAmountOfChecks();
    unsigned int getAmountOfResolvers();

public slots:
    void runChecks();
    void runResolvers();

    void executeCommand(std::unique_ptr<ADTExecutable> &task);

private:
    QScopedPointer<DiagnosticToolPrivate> d;

    volatile bool stopFlag;

    std::unique_ptr<QDBusConnection> dbus;
    std::unique_ptr<QDBusInterface> dbusInterface;

signals:
    void onProgressUpdate(int progress);
    void messageChanged(QString);
    void onError();

    void begin();
    void finish();
};

#endif // DIAGNOSTICTOOL_H
