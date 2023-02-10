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

#ifndef EXECUTABLESTATUSWIDGET_H
#define EXECUTABLESTATUSWIDGET_H

#include <QWidget>

namespace Ui
{
class ExecutableStatusWidget;
}

class ExecutableStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExecutableStatusWidget(int id, QWidget *parent = nullptr);
    ~ExecutableStatusWidget();

public:
    void setId(int id);
    int getId();

    void setText(QString text);

    void setIcon(QIcon &icon);

signals:
    void onDetailsButtonPressed(int id);

private slots:
    void on_detailedPushButton_clicked();

private:
    Ui::ExecutableStatusWidget *ui;

    int currentId;

private:
    ExecutableStatusWidget(const ExecutableStatusWidget &) = delete;
    ExecutableStatusWidget(ExecutableStatusWidget &&)      = delete;
    ExecutableStatusWidget &operator=(const ExecutableStatusWidget &) = delete;
    ExecutableStatusWidget &operator=(ExecutableStatusWidget &&) = delete;
};

#endif // EXECUTABLESTATUSWIDGET_H
