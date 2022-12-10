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

#include <QDebug>
#include <QThread>

CheckWizardPage::CheckWizardPage(DiagnosticTool *diagTool, QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::CheckWizardPage)
    , diagnosticTool(diagTool)
{
    ui->setupUi(this);

    ui->mainProgressBar->setMinimum(0);
    ui->mainProgressBar->setMaximum(100);
    ui->mainProgressBar->setValue(0);
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
    QThread *workingThread = new QThread();

    diagnosticTool->moveToThread(workingThread);

    connect(workingThread, SIGNAL(started()), diagnosticTool, SLOT(runChecks()));

    connect(workingThread, SIGNAL(finished()), workingThread, SLOT(deleteLater()));

    connect(diagnosticTool, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    connect(diagnosticTool, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));

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
