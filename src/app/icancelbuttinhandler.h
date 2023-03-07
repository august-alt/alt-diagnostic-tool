#ifndef ICANCELBUTTONHANDLER_H
#define ICANCELBUTTONHANDLER_H

class ICancelButtonHandler
{
public:
    ICancelButtonHandler()          = default;
    virtual ~ICancelButtonHandler() = default;

    virtual void cancelButtonPressed() = 0;
};

#endif
