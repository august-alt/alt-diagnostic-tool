#ifndef SLOTCONNECTOR_H
#define SLOTCONNECTOR_H

#include "../core/adtexecutablerunner.h"

#include "abstractexecutablepage.h"

class SlotConnector : public QObject
{
public:
    Q_OBJECT

public:
    SlotConnector();

    void connectSignals(ADTExecutableRunner *diagTool, AbstractExecutablePage *page);
    void disconnectSignals(ADTExecutableRunner *diagTool, AbstractExecutablePage *page);
};

#endif // SLOTCONNECTOR_H
