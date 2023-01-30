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

#ifndef FINISHWIZARDPAGE_H
#define FINISHWIZARDPAGE_H

#include <QWizardPage>

namespace Ui
{
class FinishWizardPage;
}

class FinishWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    FinishWizardPage(QWidget *parent = nullptr);

    void initializePage() override;

private:
    Ui::FinishWizardPage *ui;

private:
    FinishWizardPage(const FinishWizardPage &) = delete;            // copy ctor
    FinishWizardPage(FinishWizardPage &&)      = delete;            // move ctor
    FinishWizardPage &operator=(const FinishWizardPage &) = delete; // copy assignment
    FinishWizardPage &operator=(FinishWizardPage &&) = delete;      // move assignment
};

#endif // FINISHWIZARDPAGE_H
