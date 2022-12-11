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

#include <QJsonDocument>
#include <QThread>

#include <QtWidgets/QApplication>

DiagnosticTool::DiagnosticTool(QJsonDocument &document)
    : d(std::make_unique<DiagnosticToolPrivate>(document))
    , stopFlag(false)
{}

void DiagnosticTool::runChecks()
{
    emit begin();

    int checkSize = d->checks.get()->size();

    int percentByOneCheck = 100 / checkSize;

    int progress = 0;

    for (int i = 0; i < checkSize; i++)
    {
        if (stopFlag)
            break;

        ADTExecutable *currentTask = d->checks.get()->at(i).get();

        emit messageChanged(currentTask->m_name);

        QThread::sleep(1); //job simulation

        emit onProgressUpdate(progress + (i + 1) * percentByOneCheck);
    }

    emit onProgressUpdate(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();

    QThread::currentThread()->quit();
}

void DiagnosticTool::runResolvers()
{
    emit begin();

    int resolversSize = d->resolvers.get()->size();

    int percentByOneResolver = 100 / resolversSize;

    int progress = 0;

    for (int i = 0; i < resolversSize; i++)
    {
        if (stopFlag)
            break;

        ADTExecutable *currentTask = d->resolvers.get()->at(i).get();

        emit messageChanged(currentTask->m_name);

        QThread::sleep(1); //job simulation

        emit onProgressUpdate(progress + (i + 1) * percentByOneResolver);
    }

    emit onProgressUpdate(100);

    this->moveToThread(QApplication::instance()->thread());

    emit finish();

    QThread::currentThread()->quit();
}

void DiagnosticTool::cancelTask()
{
    stopFlag = true;
}

unsigned int DiagnosticTool::getAmountOfChecks()
{
    return d->checks.get()->size();
}

unsigned int DiagnosticTool::getAmountOfResolvers()
{
    return d->resolvers.get()->size();
}
