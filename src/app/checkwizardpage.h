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

#ifndef CHECKWIZARDPAGE_H
#define CHECKWIZARDPAGE_H

#include "abstractexecutablepage.h"
#include "executablestatuswidget.h"

class CheckWizardPage : public AbstractExecutablePage
{
    Q_OBJECT

public:
    CheckWizardPage(ADTExecutableRunner *run, QWidget *parent = nullptr);

    bool isAnyErrorsInTasks() override;

private:
    void showFinishRadiobuttons();
    void hideFinishRadiobuttons();
    void setRadiobuttonSizePolicy();

public slots:

    void cancelButtonPressed(int currentPage);

private:
    CheckWizardPage(const CheckWizardPage &) = delete;
    CheckWizardPage(CheckWizardPage &&)      = delete;
    CheckWizardPage &operator=(const CheckWizardPage &) = delete;
    CheckWizardPage &operator=(CheckWizardPage &&) = delete;
};

#endif // CHECKWIZARDPAGE_H
