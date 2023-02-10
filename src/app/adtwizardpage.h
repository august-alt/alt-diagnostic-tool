#ifndef ADTWIZARDPAGE_H
#define ADTWIZARDPAGE_H

#include "ui_checkwizardpage.h"

template<typename TUi>
class ADTWizardPage
{
public:
    ADTWizardPage();
    virtual ~ADTWizardPage();

public:
    TUi *ui;

private:
    ADTWizardPage(const ADTWizardPage &) = delete;
    ADTWizardPage(ADTWizardPage &&)      = delete;
    ADTWizardPage &operator=(const ADTWizardPage &) = delete;
    ADTWizardPage &operator=(ADTWizardPage &&) = delete;
};

#include <adtwizardpage.inl>

#endif // ADTWIZARDPAGE_H
