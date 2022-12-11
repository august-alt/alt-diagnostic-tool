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

ADTWizard::ADTWizard(QString jsonFile, QWidget *parent)
    : QWizard(parent)
    , diagnosticTool(nullptr)
    , introPage(nullptr)
    , checkPage(nullptr)
    , repairPage(nullptr)
    , finishPage(nullptr)
{
    auto doc = LoadJSonFile(jsonFile);

    diagnosticTool = std::make_unique<DiagnosticTool>(*doc.get());

    introPage  = std::make_unique<IntroWizardPage>();
    checkPage  = std::make_unique<CheckWizardPage>(diagnosticTool.get());
    repairPage = std::make_unique<RepairWizardPage>(diagnosticTool.get());
    finishPage = std::make_unique<FinishWizardPage>();

    setPage(Intro_Page, introPage.get());
    setPage(Check_Page, checkPage.get());
    setPage(Repair_Page, repairPage.get());
    setPage(Finish_Page, finishPage.get());

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

std::unique_ptr<QJsonDocument> ADTWizard::LoadJSonFile(QString file)
{
    QFile jsonFile(file);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Can't open json file!";
        return nullptr;
    }

    QByteArray fileData = jsonFile.readAll();

    jsonFile.close();

    return std::make_unique<QJsonDocument>(QJsonDocument::fromJson(fileData));
}
