// -------------------------------------------------------------------------
//    @FileName			:    NFCPVPMatchRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPVPMatchRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PVPMATCH_REDIS_MODULE_H
#define NFC_PVPMATCH_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPVPMatchRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFCPVPMatchRedisModule : public NFIPVPMatchRedisModule
{
public:
	NFCPVPMatchRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

    virtual bool GetPlayerRoomID(const NFGUID& self, NFGUID& xRoomID);
    virtual bool GetPlayerRoomIDList(const std::vector<NFGUID>& xPlayerList, std::vector<NFGUID>& vecRoomIDList);
    virtual bool GetRoomInfo(const NFGUID& xRoomID, NFMsg::PVPRoomInfo& xRoomInfo);
    virtual bool GetRoomInfoList(const std::vector<NFGUID>& xRoomIDList, std::vector<NFMsg::PVPRoomInfo>& vecRoomInfoList);
    virtual bool GetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, std::vector<NFGUID>& xRoomIDList);

    virtual bool SetPlayerRoomID(const NFGUID& self, const NFGUID& xRoomID);
    virtual bool SetPlayerRoomIDList(const std::vector<NFGUID>& xPlayerList, std::vector<NFGUID>& vecRoomIDList);
    virtual bool SetRoomInfo(const NFGUID& xRoomID, NFMsg::PVPRoomInfo& xRoomInfo);
    virtual bool SetRoomInfoList(const std::vector<NFGUID>& xRoomIDList, std::vector<NFMsg::PVPRoomInfo>& vecRoomInfoList);
    virtual bool SetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const NFGUID& xRoomID);

    virtual bool DeletePlayerRoomID(const NFGUID& self, const NFGUID& xRoomID);
    virtual bool DeletePlayerRoomIDList(const std::vector<NFGUID>& xPlayerList);
    virtual bool DeleteRoomInfo(const NFGUID& xRoomID);
    virtual bool DeleteRoomInfoList(const std::vector<NFGUID>& xRoomIDList);
    virtual bool DeleteStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const NFGUID& xRoomID);
    virtual bool DeletetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const std::vector<NFGUID>& xRoomIDList);

    virtual bool PushSinglePlayer(const NFGUID& self, const int nPVPMode, const int nGrade );
    virtual bool PopSinglePlayer(NFGUID& self, const int nPVPMode, const int nGrade);
    virtual bool PopSinglePlayerList( const int nPVPMode, const int nGrade, const int nCount, std::vector<NFGUID>& xPlayerList);
    virtual int GetSinglePlayerCount(NFGUID& self, const int nPVPMode, const int nGrade);

protected:
    std::string MakePlayerRoomKey();
    std::string MakePvpRoomRedisKey();
    std::string MakeSingleWaitRedisKey(const int nPvpMode, const int nGrade);
    std::string MakeStatusRoomIDRedisKey(const int nPvpMode, const int nGrade, const int nStatus);

private:
	NFINoSqlModule* m_pNoSqlModule;

};


#endif
