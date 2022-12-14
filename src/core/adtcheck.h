#ifndef ADTCHECK_H
#define ADTCHECK_H

#include "adtexecutable.h"

#include <vector>
#include <QMap>

class ADTCheck : public ADTExecutable
{
public:
    ADTCheck();

public:
    std::vector<int> resolverIds;
};

#endif // ADTCHECK_H
