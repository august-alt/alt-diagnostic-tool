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

class DiagnosticTool : public QObject
{
public:
    Q_OBJECT

public:
    DiagnosticTool(QJsonDocument &document);
    void cancelTask();

public slots:
    void runChecks();
    void runResolvers();

private:
    std::unique_ptr<DiagnosticToolPrivate> d;

    volatile bool stopFlag;

signals:
    void progressChanged(int progress);
    void messageChanged(QString);

    void begin();
    void finish();
};

#endif // DIAGNOSTICTOOL_H
