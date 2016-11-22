// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_URLCODE_MODULE_H
#define NFI_URLCODE_MODULE_H

#include "NFIModule.h"

class NFIUrlCodeModule
    : public NFIModule
{
public:
    virtual std::string EncodeURIValue(const std::string& strSource) = 0;
    virtual std::string DecodeURIValue(const std::string& strSource) = 0;
};

#endif