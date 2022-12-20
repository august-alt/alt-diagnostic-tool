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

#ifndef ADTWIZARD_H
#define ADTWIZARD_H

#include "../core/diagnostictool.h"

#include "checkwizardpage.h"
#include "finishwizardpage.h"
#include "introwizardpage.h"
#include "repairwizardpage.h"

#include <QScopedPointer>
#include <QWizard>
#include <QtDBus/QDBusConnection>
#include <QtXml/QDomDocument>

class ADTWizard : public QWizard
{
public:
    Q_OBJECT

public:
    enum
    {
        Intro_Page,
        Check_Page,
        Repair_Page,
        Finish_Page
    };

    ADTWizard(QString jsonFile, QWidget *parent = nullptr);

private slots:
    void cancelButtonPressed();

signals:
    void cancelPressed(int currentPage);

private:
    QScopedPointer<DiagnosticTool> diagnosticTool;

    QScopedPointer<IntroWizardPage> introPage;
    QScopedPointer<CheckWizardPage> checkPage;
    QScopedPointer<RepairWizardPage> repairPage;
    QScopedPointer<FinishWizardPage> finishPage;

private:
    QJsonDocument LoadJSonFile(QString file);
};

#endif // ADTWIZARD_H
