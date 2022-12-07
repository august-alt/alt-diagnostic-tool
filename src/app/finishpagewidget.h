#ifndef FINISHPAGEWIDGET_H
#define FINISHPAGEWIDGET_H

#include <QWidget>

namespace Ui
{
class FinishPageWidget;
}

class FinishPageWidget : public QWidget
{
    Q_OBJECT

public:
    FinishPageWidget(QWidget *parent = nullptr);

private:
    Ui::FinishPageWidget *ui;

private:
    FinishPageWidget(const FinishPageWidget &) = delete;            // copy ctor
    FinishPageWidget(FinishPageWidget &&)      = delete;            // move ctor
    FinishPageWidget &operator=(const FinishPageWidget &) = delete; // copy assignment
    FinishPageWidget &operator=(FinishPageWidget &&) = delete;      // move assignment
};

#endif //FINISHPAGEWIDGET_H
