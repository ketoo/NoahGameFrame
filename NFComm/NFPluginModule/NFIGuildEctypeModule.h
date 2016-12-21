// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_GUILDECTYPE_MODULE_H
#define NFI_GUILDECTYPE_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFIBuffConfigModule.h"


class NFIGuildEctypeModule
    : public NFIModule
{
public:
    virtual bool CreateGuilEctype(const NFGUID& self, const NFGUID& guild) = 0;
    virtual bool DestroyGuildEctype(const NFGUID& self, const NFGUID& guild) = 0;
    virtual bool ApplyEnterGuilEctype(const NFGUID& self, const NFGUID& guild) = 0;
};

#endif