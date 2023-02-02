#ifndef SLOTCONNECTOR_H
#define SLOTCONNECTOR_H

#include "../core/diagnostictool.h"

#include "abstractexecutablepage.h"

class SlotConnector
{
public:
    SlotConnector(DiagnosticTool *diagToll, AbstractExecutablePage *page);
};

#endif // SLOTCONNECTOR_H
