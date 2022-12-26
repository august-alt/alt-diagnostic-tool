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

#include "repairwizardpage.h"
#include "adtwizard.h"
#include "ui_repairwizardpage.h"

#include <QThread>

RepairWizardPage::RepairWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::RepairWizardPage)
    , diagnosticTool(diagTool)
    , isCompleteResolvers(false)
    , workingThread(nullptr)
{
    ui->setupUi(this);

    ui->repairProgressBar->setMinimum(0);
    ui->repairProgressBar->setMaximum(100);
    ui->repairProgressBar->setValue(0);

    ui->detailsScrollArea->setVisible(false);
}

bool RepairWizardPage::isComplete() const
{
    return isCompleteResolvers;
}

void RepairWizardPage::showEvent(QShowEvent *event)
{
    if (isOpening)
    {
        return;
    }
    isOpening = true;

    QWizardPage::showEvent(event);

    runResolvers();
}

void RepairWizardPage::runResolvers()
{
    workingThread = new QThread();

    connect(diagnosticTool, SIGNAL(begin()), this, SLOT(disableNextButton()));

    connect(diagnosticTool, SIGNAL(finish()), this, SLOT(enableNextButton()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(onProgressUpdate(int)), this, SLOT(onProgressUpdate(int)));

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runResolvers()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    diagnosticTool->moveToThread(workingThread);

    workingThread->start();
}

void RepairWizardPage::onProgressUpdate(int progress)
{
    ui->repairProgressBar->setValue(progress);
}

void RepairWizardPage::messageChanged(QString message)
{
    ui->statusLabel->setText("Running resolver number: " + message);
}

void RepairWizardPage::disableNextButton()
{
    isCompleteResolvers = false;

    emit completeChanged();
}

void RepairWizardPage::enableNextButton()
{
    isCompleteResolvers = true;

    emit completeChanged();
}

void RepairWizardPage::cancelButtonPressed(int currentPage)
{
    if (currentPage == ADTWizard::Repair_Page)
    {
        diagnosticTool->cancelTask();

        if (!isCompleteResolvers)
        {
            workingThread->wait();
        }
    }
}

void RepairWizardPage::on_detailsPushButton_clicked()
{
    if (ui->detailsScrollArea->isVisible())
    {
        ui->detailsScrollArea->setVisible(false);
    }
    else
    {
        ui->detailsScrollArea->setVisible(true);
    }
}
