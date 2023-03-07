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
#include "ui_checkwizardpage.h"

#include <QThread>

CheckWizardPage::CheckWizardPage(ADTExecutableRunner *run, QWidget *parent)
    : AbstractExecutablePage(run, parent)
{
    ui->finishRadioButton->setChecked(true);
    setRadiobuttonSizePolicy();
    hideFinishRadiobuttons();

    connect(this,
            &AbstractExecutablePage::enableUiElements,
            this,
            &CheckWizardPage::onEnableUiElements);
}

bool CheckWizardPage::isAnyErrorsInTasks()
{
    if (ui->runRepairRadioButton->isChecked())
    {
        return true;
    }
    else
    {
        return AbstractExecutablePage::isAnyErrorsInTasks();
    }
}

void CheckWizardPage::showFinishRadiobuttons()
{
    if (runner->isAnyErrorsInTask())
    {
        ui->finishRadioButton->setVisible(true);
        ui->runRepairRadioButton->setVisible(true);
    }
}

void CheckWizardPage::hideFinishRadiobuttons()
{
    ui->finishRadioButton->setVisible(false);
    ui->runRepairRadioButton->setVisible(false);
}

void CheckWizardPage::setRadiobuttonSizePolicy()
{
    QSizePolicy finishRadioButtonPolicy = ui->finishRadioButton->sizePolicy();
    finishRadioButtonPolicy.setRetainSizeWhenHidden(true);
    ui->finishRadioButton->setSizePolicy(finishRadioButtonPolicy);

    QSizePolicy runRepairRadioButtonpolicy = ui->runRepairRadioButton->sizePolicy();
    runRepairRadioButtonpolicy.setRetainSizeWhenHidden(true);
    ui->runRepairRadioButton->setSizePolicy(runRepairRadioButtonpolicy);
}

void CheckWizardPage::onEnableUiElements()
{
    showFinishRadiobuttons();
}
