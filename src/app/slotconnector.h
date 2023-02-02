#ifndef SLOTCONNECTOR_H
#define SLOTCONNECTOR_H

#include "../core/diagnostictool.h"

#include "abstractexecutablepage.h"

class SlotConnector : public QObject
{
public:
    Q_OBJECT

public:
    SlotConnector();

    void connectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page);
    void disconnectSignals(DiagnosticTool *diagTool, AbstractExecutablePage *page);
};

#endif // SLOTCONNECTOR_H
