// -------------------------------------------------------------------------
//    @FileName			:    NFCCreateRoleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-03-29
//    @Module           :    NFCCreateRoleModule
//
// -------------------------------------------------------------------------

#ifndef NFC_CREATEROLE_MODULE_H
#define NFC_CREATEROLE_MODULE_H

#include "NFComm/NFPluginModule/NFICreateRoleModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCCreateRoleModule
    : public NFICreateRoleModule
{
public:
    NFCCreateRoleModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCCreateRoleModule() {};

    virtual bool Init();
    virtual bool Shut();
	virtual bool ReadyExecute();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	void OnReqiureRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCreateRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnDeleteRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	/*
    int OnLoadRoleBeginEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnLoadRoleFinalEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnCreateRoleEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnDeleteRoleEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnAcountDisConnectEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);
	*/

private:

    NFIGameLogicModule* m_pGameLogicModule;
    NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif
