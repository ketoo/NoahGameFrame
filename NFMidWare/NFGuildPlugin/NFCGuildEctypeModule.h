// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildEctypeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCGuildEctypeModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILDECTYPE_MODULE_H
#define NFC_GUILDECTYPE_MODULE_H

#include "NFComm/NFPluginModule/NFIGuildEctypeModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"

class NFCGuildEctypeModule
    : public NFIGuildEctypeModule
{
public:
	struct GuildEctypeInfo
	{
		GuildEctypeInfo()
		{
			nEctypeID = 0;
			nGroupID = 0;
		}

		int nEctypeID;
		int nGroupID;
	};
public:
    NFCGuildEctypeModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGuildEctypeModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool ApplyEnterGuilEctype(const NFGUID& self, const NFGUID& guild);
	virtual bool BindGuildEcType(const NFGUID& self, const NFGUID& guild, const int nEctypeID, const int nGroup);

protected:
	void OnAckGuildEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSetGuildEctypeInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
	void OnCloneObjectEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneObjectLeaveProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneObjectPropertyEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneObjectRecordEntryProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClonePropertyIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClonePropertyFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClonePropertyStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClonePropertyDoubleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClonePropertyObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneAddRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRemoveRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneSwapRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordDoubleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordClearProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCloneRecordSortProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnRecordEnterUnPack(NF_SHARE_PTR<NFIRecord> pRecord, const NFMsg::ObjectRecordBase& xRecordData);


private:
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerToWorldModule* m_pGameServerToWorldModule;
	NFICommonConfigModule* m_pCommonConfigModule;

	NFMapEx<NFGUID, GuildEctypeInfo> mmGuilEctype;
	NFIKernelModule* m_pKernelModule;
};


#endif
