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

#ifndef ADTEXECUTABLE_H
#define ADTEXECUTABLE_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class ADTExecutable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id MEMBER m_id)
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(QString category MEMBER m_category)
    Q_PROPERTY(QString command MEMBER m_command)
    Q_PROPERTY(QString args MEMBER m_args)

public:
    int m_id;
    QString m_name;
    QString m_category;
    QString m_command;
    QString m_args;

    QString m_stdout;
    QString m_stderr;

public:
    ADTExecutable();

public slots:
    void getStdout(QString out);
    void getStderr(QString err);
};

#endif //ADTEXECUTABLE_H
