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
#include "../core/adtjsonloader.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QPushButton>

#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

ADTWizard::ADTWizard(QJsonDocument checksData, QJsonDocument resolversData, QWidget *parent)
    : QWizard(parent)
    , checks(nullptr)
    , resolvers(nullptr)
    , introPage(nullptr)
    , checkPage(nullptr)
    , repairPage(nullptr)
    , finishPage(nullptr)
    , slotConnector(nullptr)
    , previousPage(0)
{
    checks    = std::make_unique<ADTExecutableRunner>(checksData);
    resolvers = std::make_unique<ADTExecutableRunner>(resolversData);

    introPage.reset(new IntroWizardPage());
    checkPage.reset(new CheckWizardPage(checks.get()));
    repairPage.reset(new RepairWizardPage(resolvers.get()));
    finishPage.reset(new FinishWizardPage());

    slotConnector.reset(new SlotConnector);

    setPage(Intro_Page, introPage.data());
    setPage(Check_Page, checkPage.data());
    setPage(Repair_Page, repairPage.data());
    setPage(Finish_Page, finishPage.data());

    setStartId(Intro_Page);

    disconnect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(reject()));
    connect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));

    connect(this, SIGNAL(cancelPressed(int)), introPage.get(), SLOT(cancelButtonPressed(int)));
    connect(this, SIGNAL(cancelPressed(int)), checkPage.get(), SLOT(cancelButtonPressed(int)));
    connect(this, SIGNAL(cancelPressed(int)), repairPage.get(), SLOT(cancelButtonPressed(int)));

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(currentIdChanged(int)));
}

int ADTWizard::nextId() const
{
    auto repairPg = static_cast<CheckWizardPage *>(page(ADTWizard::Repair_Page));
    auto checkPg  = static_cast<RepairWizardPage *>(page(ADTWizard::Check_Page));

    int currentPage = currentId();

    switch (currentPage)
    {
    case Intro_Page:

        if (checkPg->getAmountOfTasks() != 0)
        {
            return Check_Page;
        }
        else
        {
            return Finish_Page;
        }
        break;

    case Check_Page:

        if (repairPg->getAmountOfTasks() != 0 && checkPg->isAnyErrorsInTasks())
        {
            return Repair_Page;
        }
        else
        {
            return Finish_Page;
        }
        break;

    case Repair_Page:

        return Finish_Page;

    case Finish_Page:
        return -1;
    }

    return -1;
}

void ADTWizard::cancelButtonPressed()
{
    emit cancelPressed(currentId());
}

void ADTWizard::currentIdChanged(int currentPageId)
{
    connectSlotInCurrentPage(currentPageId);
    disconnectSlotInPreviousPage();

    previousPage = currentPageId;

    switch (currentPageId)
    {
    case Check_Page:
        checkPage.data()->runTasks();
        break;

    case Repair_Page:
        repairPage.data()->runTasks();
        break;

    default:
        break;
    }
}

void ADTWizard::connectSlotInCurrentPage(int currentPageId)
{
    switch (currentPageId)
    {
    case ADTWizard::Intro_Page:
        break;

    case ADTWizard::Check_Page:

        slotConnector->connectSignals(checks.get(),
                                      static_cast<AbstractExecutablePage *>(checkPage.get()));
        break;

    case ADTWizard::Repair_Page:
        slotConnector->connectSignals(resolvers.get(),
                                      static_cast<AbstractExecutablePage *>(repairPage.get()));
        break;

    case ADTWizard::FinishButton:
        break;
    }
}

void ADTWizard::disconnectSlotInPreviousPage()
{
    switch (previousPage)
    {
    case ADTWizard::Intro_Page:
        break;

    case ADTWizard::Check_Page:

        slotConnector->disconnectSignals(checks.get(),
                                         static_cast<AbstractExecutablePage *>(checkPage.get()));
        break;

    case ADTWizard::Repair_Page:
        slotConnector->disconnectSignals(resolvers.get(),
                                         static_cast<AbstractExecutablePage *>(repairPage.get()));
        break;

    case ADTWizard::FinishButton:
        break;
    }
}
