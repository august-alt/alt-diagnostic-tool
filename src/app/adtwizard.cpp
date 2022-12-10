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
#include "checkwizardpage.h"
#include "finishwizardpage.h"
#include "introwizardpage.h"
#include "repairwizardpage.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>

ADTWizard::ADTWizard(QString jsonFile, QWidget *parent)
    : QWizard(parent)
    , diagnosticTool(nullptr)
{
    auto doc = LoadJSonFile(jsonFile);

    diagnosticTool = std::make_unique<DiagnosticTool>(*doc.get());

    setPage(Intro_Page, new IntroWizardPage);
    setPage(Check_Page, new CheckWizardPage(diagnosticTool.get()));
    setPage(Repair_Page, new RepairWizardPage(diagnosticTool.get()));
    setPage(Finish_Page, new FinishWizardPage);

    setStartId(Intro_Page);
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
