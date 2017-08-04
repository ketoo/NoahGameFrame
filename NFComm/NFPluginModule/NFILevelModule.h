// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_LEVEL_MODULE_H
#define NFI_LEVEL_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFILevelModule
    : public NFIModule
{
public:

    virtual int AddExp(const NFGUID& self, const int64_t nExp) = 0;

};

#endif