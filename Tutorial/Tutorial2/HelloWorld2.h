// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld2.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld2
//
// -------------------------------------------------------------------------

#ifndef _NFC_HELLO_WORLD2_H_
#define _NFC_HELLO_WORLD2_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"

class HelloWorld2
    : public NFILogicModule
{
public:
    HelloWorld2(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnPropertyCallBackEvent(const NFIDENTID& self, const std::string& strProperty, const NFIValueList& oldVarList, const NFIValueList& newVarList, const NFIValueList& argVarList);



};

#endif
