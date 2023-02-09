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

#include "checkwizardpage.h"
#include "adtwizard.h"
#include "executablestatuswidget.h"
#include "ui_checkwizardpage.h"

#include <QPushButton>
#include <QStyle>

CheckWizardPage::CheckWizardPage(ADTExecutableRunner *run, QWidget *parent)
    : AbstractExecutablePage(run, parent)
{
    pageUi->ui->finishRadioButton->setChecked(true);
    setRadiobuttonSizePolicy();
    hideFinishRadiobuttons();
}

bool CheckWizardPage::isComplete() const
{
    return isCompleteTasks;
}

int CheckWizardPage::nextId() const
{
    if (runner->isAnyErrorsInTask() && pageUi->ui->runRepairRadioButton->isChecked())
    {
        return ADTWizard::Repair_Page;
    }
    else
    {
        return ADTWizard::Finish_Page;
    }

    return ADTWizard::Check_Page;
}

void CheckWizardPage::showFinishRadiobuttons()
{
    if (runner->isAnyErrorsInTask())
    {
        pageUi->ui->finishRadioButton->setVisible(true);
        pageUi->ui->runRepairRadioButton->setVisible(true);
    }
}

void CheckWizardPage::hideFinishRadiobuttons()
{
    pageUi->ui->finishRadioButton->setVisible(false);
    pageUi->ui->runRepairRadioButton->setVisible(false);
}

void CheckWizardPage::setRadiobuttonSizePolicy()
{
    QSizePolicy finishRadioButtonPolicy = pageUi->ui->finishRadioButton->sizePolicy();
    finishRadioButtonPolicy.setRetainSizeWhenHidden(true);
    pageUi->ui->finishRadioButton->setSizePolicy(finishRadioButtonPolicy);

    QSizePolicy runRepairRadioButtonpolicy = pageUi->ui->runRepairRadioButton->sizePolicy();
    runRepairRadioButtonpolicy.setRetainSizeWhenHidden(true);
    pageUi->ui->runRepairRadioButton->setSizePolicy(runRepairRadioButtonpolicy);
}

void CheckWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Check_Page)
    {
        runner->cancelTasks();

        if (!isCompleteTasks)
        {
            workingThread->wait();

            enableButtonsAfterChecks();

            showFinishRadiobuttons();
        }

        pageUi->ui->stackedWidget->setCurrentIndex(0);
    }
}
