#ifndef ADTWIZARDPAGE_H
#define ADTWIZARDPAGE_H

#include "ui_checkwizardpage.h"

template<typename TUi>
class ADTWizardPage
{
public:
    ADTWizardPage();

public:
    TUi *ui;
};

#include <adtwizardpage.inl>

#endif // ADTWIZARDPAGE_H
