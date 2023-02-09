#include "adtwizardpage.h"

template<typename TUi>
ADTWizardPage<TUi>::ADTWizardPage()
    : ui(new TUi)
{}

template<typename TUi>
ADTWizardPage<TUi>::~ADTWizardPage()
{
    delete ui;
}
