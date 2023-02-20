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

const int LAST_PAGE_INDEX = -1;

ADTWizard::ADTWizard(QJsonDocument checksData, QJsonDocument resolversData, QWidget *parent)
    : QWizard(parent)
    , checks(new ADTExecutableRunner(checksData))
    , resolvers(new ADTExecutableRunner(resolversData))
    , introPage(new IntroWizardPage())
    , checkPage(new CheckWizardPage(checks.get()))
    , repairPage(new RepairWizardPage(resolvers.get()))
    , finishPage(new FinishWizardPage())
    , slotConnector(new SlotConnector())
    , previousPage(0)
{
    setPage(Intro_Page, introPage.data());
    setPage(Check_Page, checkPage.data());
    setPage(Repair_Page, repairPage.data());
    setPage(Finish_Page, finishPage.data());

    setStartId(Intro_Page);

    disconnect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(reject()));
    connect(button(QWizard::CancelButton),
            &QPushButton::clicked,
            this,
            &ADTWizard::cancelButtonPressed);

    connect(this, &ADTWizard::cancelPressed, introPage.get(), &IntroWizardPage::cancelButtonPressed);
    connect(this, &ADTWizard::cancelPressed, checkPage.get(), &CheckWizardPage::cancelButtonPressed);
    connect(this,
            &ADTWizard::cancelPressed,
            repairPage.get(),
            &RepairWizardPage::cancelButtonPressed);

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
        return (checkPg->getAmountOfTasks() != 0 ? Check_Page : Finish_Page);

    case Check_Page:
        return (repairPg->getAmountOfTasks() != 0 && checkPg->isAnyErrorsInTasks() ? Repair_Page
                                                                                   : Finish_Page);

    case Repair_Page:
        return Finish_Page;

    case Finish_Page:
        return LAST_PAGE_INDEX;
    }

    return LAST_PAGE_INDEX;
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
    case ADTWizard::Check_Page:

        slotConnector->connectSignals(checks.get(),
                                      static_cast<AbstractExecutablePage *>(checkPage.get()));
        break;

    case ADTWizard::Repair_Page:
        slotConnector->connectSignals(resolvers.get(),
                                      static_cast<AbstractExecutablePage *>(repairPage.get()));
        break;

    case ADTWizard::Intro_Page:
    case ADTWizard::FinishButton:
    default:
        break;
    }
}

void ADTWizard::disconnectSlotInPreviousPage()
{
    switch (previousPage)
    {
    case ADTWizard::Check_Page:

        slotConnector->disconnectSignals(checks.get(),
                                         static_cast<AbstractExecutablePage *>(checkPage.get()));
        break;

    case ADTWizard::Repair_Page:
        slotConnector->disconnectSignals(resolvers.get(),
                                         static_cast<AbstractExecutablePage *>(repairPage.get()));
        break;

    case ADTWizard::Intro_Page:
    case ADTWizard::FinishButton:
    default:
        break;
    }
}
