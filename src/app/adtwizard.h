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

#include "../core/adtexecutablerunner.h"

#include "checkwizardpage.h"
#include "finishwizardpage.h"
#include "introwizardpage.h"
#include "repairwizardpage.h"
#include "slotconnector.h"

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

    ADTWizard(QJsonDocument &checksData,
              QJsonDocument &resolversData,
              QString &serviceName,
              QString &path,
              QString &intefaceName,
              QWidget *parent = nullptr);

    virtual int nextId() const override;

    bool isServiceActive();

private slots:
    void cancelButtonPressed();

    void currentIdChanged(int currentPageId);

    void dBusServiceUnregistered();
    void dBusServiceRegistered();

private:
    std::unique_ptr<ADTExecutableRunner> checks;
    std::unique_ptr<ADTExecutableRunner> resolvers;

    QScopedPointer<IntroWizardPage> introPage;
    QScopedPointer<CheckWizardPage> checkPage;
    QScopedPointer<RepairWizardPage> repairPage;
    QScopedPointer<FinishWizardPage> finishPage;

    QScopedPointer<SlotConnector> slotConnector;

    std::unique_ptr<QDBusServiceWatcher> serviceWatcher;

    int previousPage;

    bool isServiceRegistered = true;

private:
    void connectSlotInCurrentPage(int currentPageId);
    void disconnectSlotInPreviousPage();
};

#endif // ADTWIZARD_H
