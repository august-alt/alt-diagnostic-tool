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
#include "adtjsonloader.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ADTJsonLoader::ADTJsonLoader() {}

QJsonDocument ADTJsonLoader::loadDocument(QString file, QString jsonArrayName)
{
    QFile jsonFile(file);

    QJsonDocument doc;

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Can't open json file!";
        return doc;
    }

    QByteArray fileData = jsonFile.readAll();

    jsonFile.close();

    doc = (QJsonDocument::fromJson(fileData));

    QJsonObject object = doc.object();

    if (object.contains(jsonArrayName) && object[jsonArrayName].isArray())
    {
        return QJsonDocument(object[jsonArrayName].toArray());
    }

    qWarning() << "Can't find in " << file << " json array with name " << jsonArrayName;

    QJsonDocument emptyDoc;

    return emptyDoc;
}
