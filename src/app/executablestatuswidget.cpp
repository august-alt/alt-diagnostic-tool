#include "executablestatuswidget.h"
#include "ui_executablestatuswidget.h"

#include <QStyle>

ExecutableStatusWidget::ExecutableStatusWidget(int _id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExecutableStatusWidget)
    , id(-1)
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

void ExecutableStatusWidget::setId(int _id)
{
    id = _id;
}

int ExecutableStatusWidget::getId()
{
    return id;
}

void ExecutableStatusWidget::on_detailedPushButton_clicked()
{
    emit onDetailsButtonPressed(id);
}
