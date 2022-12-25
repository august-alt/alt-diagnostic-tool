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

#include "adtwizard.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QPushButton>

#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

ADTWizard::ADTWizard(QString jsonFile, QWidget *parent)
    : QWizard(parent)
    , diagnosticTool(nullptr)
    , introPage(nullptr)
    , checkPage(nullptr)
    , repairPage(nullptr)
    , finishPage(nullptr)
{
    diagnosticTool.reset(new DiagnosticTool(LoadJSonFile(jsonFile)));

    introPage.reset(new IntroWizardPage());
    checkPage.reset(new CheckWizardPage(diagnosticTool.data()));
    repairPage.reset(new RepairWizardPage(diagnosticTool.data()));
    finishPage.reset(new FinishWizardPage());

    setPage(Intro_Page, introPage.data());
    setPage(Check_Page, checkPage.data());
    setPage(Repair_Page, repairPage.data());
    setPage(Finish_Page, finishPage.data());

    setStartId(Intro_Page);

    disconnect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(reject()));
    connect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));

    connect(this, SIGNAL(cancelPressed(int)), checkPage.get(), SLOT(cancelButtonPressed(int)));
    connect(this, SIGNAL(cancelPressed(int)), repairPage.get(), SLOT(cancelButtonPressed(int)));
}

void ADTWizard::cancelButtonPressed()
{
    emit cancelPressed(currentId());

    emit reject();
}

QJsonDocument ADTWizard::LoadJSonFile(QString file)
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

    return doc;
}
