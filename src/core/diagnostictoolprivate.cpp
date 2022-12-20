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

#include "diagnostictoolprivate.h"
#include "adtjsonconverter.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <qfile.h>

DiagnosticToolPrivate::DiagnosticToolPrivate(QJsonDocument &document)
    : checks(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , resolvers(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , currentDocument(std::make_unique<QJsonDocument>(document))
{
    parseExecutors(currentDocument.get());
}

void DiagnosticToolPrivate::parseExecutors(QJsonDocument *doc)
{
    QJsonObject object = doc->object();

    if (object.contains("checks") && object["checks"].isArray())
    {
        checks->clear();

        QJsonArray checksArray = object["checks"].toArray();

        for (int i = 0; i < checksArray.size(); i++)
        {
            auto check      = std::make_unique<ADTExecutable>();
            QJsonObject obj = checksArray[i].toObject();
            ADTJsonConverter::JSonToObject(*check.get(), &obj);
            checks->emplace_back(std::move(check));
        }
    }

    if (object.contains("resolvers") && object["resolvers"].isArray())
    {
        resolvers->clear();

        QJsonArray resolversArray = object["resolvers"].toArray();

        for (int i = 0; i < resolversArray.size(); i++)
        {
            auto resolver   = std::make_unique<ADTExecutable>();
            QJsonObject obj = resolversArray[i].toObject();
            ADTJsonConverter::JSonToObject(*resolver.get(), &obj);
            resolvers->emplace_back(std::move(resolver));
        }
    }
}
