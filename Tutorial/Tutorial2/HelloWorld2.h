// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld2.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld2
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD2_H
#define NFC_HELLO_WORLD2_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
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

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnPropertyCallBackEvent(const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVarList, const NFIDataList::TData& newVarList);



};

#endif
