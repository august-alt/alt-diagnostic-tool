/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "executablestatuswidget.h"
#include "ui_executablestatuswidget.h"

#include <QStyle>

ExecutableStatusWidget::ExecutableStatusWidget(int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExecutableStatusWidget)
    , currentId(id)
{
    ui->setupUi(this);

    QIcon icon = style()->standardIcon(QStyle::SP_BrowserReload);

    ui->iconLabel->setPixmap(icon.pixmap(QSize(16, 16)));
}

ExecutableStatusWidget::~ExecutableStatusWidget()
{
    delete ui;
}

void ExecutableStatusWidget::setText(QString text)
{
    ui->statusTextLabel->setText(text);
}

void ExecutableStatusWidget::setIcon(QIcon &icon)
{
    ui->iconLabel->setPixmap(icon.pixmap(QSize(16, 16)));
}

void ExecutableStatusWidget::setId(int id)
{
    currentId = id;
}

int ExecutableStatusWidget::getId()
{
    return currentId;
}

void ExecutableStatusWidget::on_detailedPushButton_clicked()
{
    emit onDetailsButtonPressed(currentId);
}
