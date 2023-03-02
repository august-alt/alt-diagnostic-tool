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

#include "slotconnector.h"

SlotConnector::SlotConnector() {}

void SlotConnector::connectSignals(ADTExecutableRunner *runner, AbstractExecutablePage *page)
{
    connect(runner, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    connect(runner, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    connect(runner, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    connect(runner, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    connect(runner,
            SIGNAL(beginTask(ADTExecutable *)),
            page,
            SLOT(beginCurrentTask(ADTExecutable *)));
    connect(runner,
            SIGNAL(finishTask(ADTExecutable *)),
            page,
            SLOT(finishCurrentTask(ADTExecutable *)));
}

void SlotConnector::disconnectSignals(ADTExecutableRunner *runner, AbstractExecutablePage *page)
{
    disconnect(runner, SIGNAL(begin()), page, SLOT(beginAllTasks()));

    disconnect(runner, SIGNAL(finish()), page, SLOT(finishAllTasks()));

    disconnect(runner, SIGNAL(messageChanged(QString)), page, SLOT(messageChanged(QString)));

    disconnect(runner, SIGNAL(onProgressUpdate(int)), page, SLOT(onProgressUpdate(int)));

    disconnect(runner,
               SIGNAL(beginTask(ADTExecutable *)),
               page,
               SLOT(beginCurrentTask(ADTExecutable *)));
    disconnect(runner,
               SIGNAL(finishTask(ADTExecutable *)),
               page,
               SLOT(finishCurrentTask(ADTExecutable *)));
}
