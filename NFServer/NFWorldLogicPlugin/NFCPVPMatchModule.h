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
    bool PlayerLeaveRoom(const NFGUID& self, const NFGUID& xRoomID);
    NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> > GetRoomList(const int nPVPMode, const int nGrade);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;

private:
    NFMapEx<int, NFMapEx< int , NFMapEx<NFGUID, NFGUID> > >mxWaitRoom; //pvpmode <--->grade<-(roomid <--> roomid)>
    NFMapEx<NFGUID, NFGUID> mxPlayerRoomInfo; //PlayerID <--> RoomID;
    NFMapEx<NFGUID, PVPRoom> mxRoomInfo; //RoomID <--> RoomInfo;
};


#endif
