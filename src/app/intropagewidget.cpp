#include "intropagewidget.h"
#include "ui_intropagewidget.h"

IntroPageWidget::IntroPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IntroPageWidget())
{
    ui->setupUi(this);
}
