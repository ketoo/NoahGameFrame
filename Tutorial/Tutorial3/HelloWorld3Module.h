// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld3
//
// -------------------------------------------------------------------------

#ifndef _NFC_HELLO_WORLD3_H_
#define _NFC_HELLO_WORLD3_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class HelloWorld3Module
    : public NFILogicModule
{
public:
    HelloWorld3Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const NFIDENTID& self, const int event, const NFIValueList& arg);
    int OnClassCallBackEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFIValueList& arg);
    int OnPropertyCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIValueList& oldVarList, const NFIValueList& newVarList, const NFIValueList& argVarList );
    int OnPropertyStrCallBackEvent( const NFIDENTID& self, const std::string& strProperty, const NFIValueList& oldVarList, const NFIValueList& newVarList, const NFIValueList& argVarList );

protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
