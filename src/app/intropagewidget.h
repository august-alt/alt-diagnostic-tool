#ifndef INTROHPAGEWIDGET_H
#define INTROHPAGEWIDGET_H

#include <QWidget>

namespace Ui
{
class IntroPageWidget;
}

class IntroPageWidget : public QWidget
{
    Q_OBJECT

public:
    IntroPageWidget(QWidget *parent = nullptr);

private:
    Ui::IntroPageWidget *ui;

private:
    IntroPageWidget(const IntroPageWidget &) = delete;            // copy ctor
    IntroPageWidget(IntroPageWidget &&)      = delete;            // move ctor
    IntroPageWidget &operator=(const IntroPageWidget &) = delete; // copy assignment
    IntroPageWidget &operator=(IntroPageWidget &&) = delete;      // move assignment
};

#endif //INTROHPAGEWIDGET_H
