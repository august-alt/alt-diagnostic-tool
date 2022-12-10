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
#include "ui_repairwizardpage.h"

#include <QThread>

RepairWizardPage::RepairWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::RepairWizardPage)
    , diagnosticTool(diagTool)
{
    ui->setupUi(this);

    ui->repairProgressBar->setMinimum(0);
    ui->repairProgressBar->setMaximum(100);
    ui->repairProgressBar->setValue(0);
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
    QThread *workingThread = new QThread();

    diagnosticTool->moveToThread(workingThread);

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runResolvers()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));

    workingThread->start();
}

void RepairWizardPage::progressChanged(int progress)
{
    ui->repairProgressBar->setValue(progress);
}

void RepairWizardPage::messageChanged(QString message)
{
    ui->statusLabel->setText("Running resolver number: " + message);
}
