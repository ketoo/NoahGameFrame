// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld7.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld7
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD7_H
#define NFC_HELLO_WORLD7_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIUrlClientModule.h"

class HelloWorld7
    : public NFILogicModule
{
public:
    HelloWorld7(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

    void handleRsp(const NFGUID& self , const int nRet, const std::string& strData, const std::string&strUseData);
protected:

private:
    NFIUrlClientModule* m_pUrlClientModule;

};

#endif
