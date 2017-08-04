// -------------------------------------------------------------------------
//    @FileName      :    NFCPackModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-10
//    @Module           :    NFCPackModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GM_MODULE_H
#define NFC_GM_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGmModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"

class NFCGmModule
    : public NFIGmModule
{
public:
    NFCGmModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGmModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	void OnGMPropertyIntProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMPropertyStrProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMPropertyObjectProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMPropertyFloatProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMRecordIntProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMRecordStrProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMRecordObjectProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnGMRecordFloatProcess( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
    void OnGMNormalProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienGMProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void CheckAndAddRow(const NFGUID& self, std::string strRecordName, const int nRow);

private:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFILevelModule* m_pLevelModule;
    NFIPackModule* m_pPackModule;
    NFIHeroModule* m_pHeroModule;
};


#endif
