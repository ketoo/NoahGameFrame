// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPMatchModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPMatchModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PVP_MATCH_MODULE_H
#define NFC_PVP_MATCH_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIPVPMatchModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPVPMatchRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFITeamModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"

class NFCPVPMatchModule
    : public NFIPVPMatchModule
{
public:
    NFCPVPMatchModule( NFIPluginManager* p )
    {
        pPluginManager = p;
		mnLastCheckTime = 0;
    }

    virtual ~NFCPVPMatchModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool ApplyPVP(const NFGUID& self, const int nPVPMode, const int nScore);
    virtual bool TeamApplyPVP(const NFGUID& xTeam, const NFIDataList& varMemberList, int nPVPMODE, const int nScore);
    virtual int GetGradeByScore(const int nScore);
    virtual int GetMemberCount(const int nPVPMode);

protected:
    NFGUID CreateRoom(const NFGUID& self, const int nPVPMode, const int nGrade);
    bool DestroyRoom(const NFGUID& self, const NFGUID& xRoomID);
    bool PlayerEnterRoom(const NFGUID& self, const int nRedOrBlue, const NFGUID& xRoomID);
    bool PlayerListEnterRoom(const std::vector<NFGUID>& xPlayerList, const int nRedOrBlue, const NFGUID& xRoomID);
    bool PlayerLeaveRoom(const NFGUID& self, const NFGUID& xRoomID);
	bool PlayerListLeaveRoom(const std::vector<NFGUID>& xPlayerList, const NFGUID& xRoomID);

    int NeedPlayerCount(const NFMsg::PVPRoomInfo& xRoomInfo, const int nRedOrebulue);
	bool UpdateRoomStatus(NFMsg::PVPRoomInfo& xRoomInfo, const int nTargetStatus = -1);

    void ProecessWaitRoom();
    void ProecessRoomBeginFight();
    void ProcessSingePlayerRoom();

	//msg handle
	void OnReqPVPApplyMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAckCreatePVPEctypeProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	bool BroadcastMsgToRoom(const NFGUID& self, const NFGUID& xTeam, const uint16_t nMsgID, google::protobuf::Message& xData);
private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIPVPMatchRedisModule* m_pPVPMatchRedisModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFITeamModule* m_pTeamModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;

	NFINT64 mnLastCheckTime;

};


#endif
