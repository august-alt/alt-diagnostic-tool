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
    ADTWizardPage(const ADTWizardPage &) = delete;            // copy ctor
    ADTWizardPage(ADTWizardPage &&)      = delete;            // move ctor
    ADTWizardPage &operator=(const ADTWizardPage &) = delete; // copy assignment
    ADTWizardPage &operator=(ADTWizardPage &&) = delete;      // move assignm
};

#include <adtwizardpage.inl>

#endif // ADTWIZARDPAGE_H
