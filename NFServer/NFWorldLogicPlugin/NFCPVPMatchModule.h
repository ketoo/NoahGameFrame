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

class NFCPVPMatchModule
    : public NFIPVPMatchModule
{
public:
    NFCPVPMatchModule( NFIPluginManager* p )
    {
        pPluginManager = p;
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
private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIPVPMatchRedisModule* m_pPVPMatchRedisModule;
};


#endif
