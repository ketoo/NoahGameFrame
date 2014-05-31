// -------------------------------------------------------------------------
//    @FileName         ：    NFIScriptModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-08
//    @Module           ：    NFIScriptModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SCRIPT_MODULE_H_
#define _NFI_SCRIPT_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIScriptModule
    : public NFILogicModule
{

public:
    //call script
    virtual bool DoScript(const NFIDENTID& self, const std::string& strScriptName, const std::string& strFunction, const NFIValueList& arg){return false;};

    //此为此单脚本的初始化顺序，并不是模块
    virtual bool Init(const NFIDENTID& self){return false;};

    virtual bool AfterInit(const NFIDENTID& self){return false;};

    virtual bool BeforeShut(const NFIDENTID& self){return false;};

    virtual bool Shut(const NFIDENTID& self){return false;};

    virtual bool Execute(const NFIDENTID& self, const float fLasFrametime, const float fStartedTime){return false;};

    //becall from script
    //
};

#endif