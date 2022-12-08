#include "finishpagewidget.h"
#include "ui_finishpagewidget.h"

FinishPageWidget::FinishPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinishPageWidget())
{
    ui->setupUi(this);
}
