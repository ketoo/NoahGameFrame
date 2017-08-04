// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld3
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD3_H
#define NFC_HELLO_WORLD3_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"

class NFIHelloWorld3Module
	: public NFIModule
{

};

class NFCHelloWorld3Module
    : public NFIHelloWorld3Module
{
public:
    NFCHelloWorld3Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const NFGUID& self, const NFEventDefine event, const NFDataList& arg);
    int OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFDataList& arg);
    int OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVarList, const NFData& newVarList);
    int OnPropertyStrCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVarList, const NFData& newVarList);

    int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
    int64_t mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
	NFIScheduleModule* m_pScheduleModule;
};

#endif
