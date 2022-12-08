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

ADTWizard::ADTWizard(QWidget *parent)
    : QWizard(parent)
{
    setPage(Intro_Page, new IntroWizardPage);
    setPage(Check_Page, new CheckWizardPage);
    setPage(Repair_Page, new RepairWizardPage);
    setPage(Finish_Page, new FinishWizardPage);

    setStartId(Intro_Page);
}
