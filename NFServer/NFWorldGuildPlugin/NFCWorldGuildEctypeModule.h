// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildEctypeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildEctypeModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_GUILD_MODULE_H
#define NFC_WORLD_GUILD_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildEctypeModule.h"

class NFCWorldGuildEctypeModule
    : public NFIWorldGuildEctypeModule
{
public:
    NFCWorldGuildEctypeModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	
    virtual bool AfterInit();

protected:
	int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
	int OnObjectEctypeServerEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );
protected:
	void OnReqGuildEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSetGuildEctypeInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	int OnGuildObjectEnter( const NFGUID& xGuid, const int nServerID);
	int OnGuildObjectLeave( const NFGUID& xGuid, const int nServerID);
	int OnGuildObjectPropertyEnter( const NFGUID& xGuid, const int nServerID);
	int OnGuildObjectRecordEnter( const NFGUID& xGuid, const int nServerID);

	int OnGuildObjectPropertyCommonEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	int OnGuildObjectRecordCommonEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );

protected:
    NFIKernelModule* m_pKernelModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFIWorldGuildModule* m_pWorldGuildModule;
	NFIWorldGuildDataModule* m_pWorldGuildDataModule;
};

#endif