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
#include "adtexecutablerunnerprivate.h"
#include "adtjsonconverter.h"

#include <QJsonArray>
#include <QJsonObject>
#include <qjsondocument.h>

ADTExecutableRunnerPrivate::ADTExecutableRunnerPrivate(QJsonDocument &document)
    : tasks(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , jsonObject(std::make_unique<QJsonDocument>(document))
{
    parseTasks(jsonObject.get());
}

void ADTExecutableRunnerPrivate::parseTasks(QJsonDocument *doc)
{
    if (!doc->isArray())
    {
        return;
    }

    QJsonArray taskArray = doc->array();

    if (taskArray.isEmpty())
    {
        return;
    }

    tasks->clear();

    for (int i = 0; i < taskArray.size(); i++)
    {
        auto task       = std::make_unique<ADTExecutable>();
        QJsonObject obj = taskArray[i].toObject();
        ADTJsonConverter::JSonToObject(*task.get(), &obj);
        tasks->push_back(std::move(task));
    }
}
