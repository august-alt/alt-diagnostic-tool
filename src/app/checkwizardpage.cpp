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
#include "../core/diagnostictool.h"
#include "ui_checkwizardpage.h"
#include <adtwizard.h>

#include <QDebug>

CheckWizardPage::CheckWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::CheckWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteChecks(false)
    , workingThread(nullptr)
{
    ui->setupUi(this);

    ui->mainProgressBar->setMinimum(0);
    ui->mainProgressBar->setMaximum(100);
    ui->mainProgressBar->setValue(0);
}

bool CheckWizardPage::isComplete() const
{
    return isCompleteChecks;
}

int CheckWizardPage::nextId() const
{
    if (ui->runRepairRadioButton->isChecked())
    {
        return ADTWizard::Repair_Page;
    }
    if (ui->finishRadioButton->isChecked())
    {
        return ADTWizard::Finish_Page;
    }

    return ADTWizard::Check_Page;
}

void CheckWizardPage::showEvent(QShowEvent *event)
{
    if (isOpening)
    {
        return;
    }
    isOpening = true;

    QWizardPage::showEvent(event);

    runChecks();
}

void CheckWizardPage::runChecks()
{
    workingThread = new QThread();

    connect(diagnosticTool, SIGNAL(begin()), this, SLOT(disableNextButton()));

    connect(diagnosticTool, SIGNAL(finish()), this, SLOT(enableNextButton()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void CheckWizardPage::progressChanged(int progress)
{
    ui->mainProgressBar->setValue(progress);
}

void CheckWizardPage::messageChanged(QString message)
{
    ui->currentStatusLabel->setText("Running check number: " + message);
}

void CheckWizardPage::disableNextButton()
{
    isCompleteChecks = false;

    emit completeChanged();
}

void CheckWizardPage::enableNextButton()
{
    isCompleteChecks = true;

    emit completeChanged();
}

void CheckWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Check_Page)
    {
        diagnosticTool->cancelTask();

        workingThread->wait();
    }
}
