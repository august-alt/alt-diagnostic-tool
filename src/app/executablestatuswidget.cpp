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
