// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld3
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD3_H
#define NFC_HELLO_WORLD3_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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
    int OnEvent(const NFGUID& self, const int event, const NFIDataList& arg);
    int OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFIDataList& arg);
    int OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVarList, const NFIDataList::TData& newVarList );
    int OnPropertyStrCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVarList, const NFIDataList::TData& newVarList );

    int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
    unsigned long mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
