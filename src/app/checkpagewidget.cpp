#include "checkpagewidget.h"
#include "ui_checkpagewidget.h"

CheckPageWidget::CheckPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckPageWidget())
{
    ui->setupUi(this);
}
