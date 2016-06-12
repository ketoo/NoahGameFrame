// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPMatchModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPMatchModule
//
// -------------------------------------------------------------------------

#include "NFCPVPMatchModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"

bool NFCPVPMatchModule::Init()
{
	
    return true;
}


bool NFCPVPMatchModule::Shut()
{
    return true;
}

bool NFCPVPMatchModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCPVPMatchModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>();
    
    return true;
}

bool NFCPVPMatchModule::ApplyPVP(const NFGUID& self, const int nPVPMode, const int nScore)
{
    NF_SHARE_PTR<NFGUID> pRoomID =mxPlayerRoomInfo.GetElement(self);
    if (pRoomID)
    {
        //aready in room 
        return false;
    }
    
    int nGrade = GetGradeByScore(nScore);
    NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> > pGradeData = GetRoomList(nPVPMode, nGrade);
    if (!pGradeData)
    {
        return false;
    }

    NFGUID xFindRoomID;
    bool bFind = false;
    bool bRed = false;
    for (NF_SHARE_PTR<NFGUID> pData = pGradeData->First(xFindRoomID); pData != NULL; pData = pGradeData->Next(xFindRoomID))
    {
        if (!xFindRoomID.IsNull())
        {
            NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xFindRoomID);
            if (pRoomData)
            {
                if (pRoomData->mxRedPlayer.Count() < pRoomData->nMaxPalyer)
                {
                    bFind = true;
                    break;
                }
                else if (pRoomData->mxBluePlayer.Count() < pRoomData->nMaxPalyer)
                {
                    bFind = true;
                    break;
                }
            }
        }
    }

    if (!bFind)
    {
        xFindRoomID = CreateRoom(self, nPVPMode, nGrade);
        if (xFindRoomID.IsNull())
        {
            return false;
        }
    }

    if (!PlayerEnterRoom(self, bRed, xFindRoomID))
    {
        return false;
    }

    return true;
}

bool NFCPVPMatchModule::TeamApplyPVP(const NFGUID& xTeam, const NFIDataList& varMemberList, int nPVPMode, const int nScore)
{
    int nMemberCount = varMemberList.GetCount();
    for (int i = 0; i < varMemberList.GetCount(); i++)
    {
        const NFGUID& xPlayerID = varMemberList.Object(i);
        if (xPlayerID.IsNull())
        {
            return false;
        }
    }

    int nPVPModeMaxMember = GetMemberCount(nPVPMode);
    if (nPVPModeMaxMember < 0)
    {
        return false;
    }

    if (nMemberCount > nPVPModeMaxMember)
    {
        return false;
    }
    
    int nGrade = GetGradeByScore(nScore);
    NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> > pGradeData = GetRoomList(nPVPMode, nGrade);
    if (!pGradeData)
    {
        return false;
    }

    NFGUID xFindRoomID;
    bool bFind = false;
    bool bRed = false;
    for (NF_SHARE_PTR<NFGUID> pData = pGradeData->First(xFindRoomID); pData != NULL; pData = pGradeData->Next(xFindRoomID))
    {
        if (!xFindRoomID.IsNull())
        {
            NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xFindRoomID);
            if (!pRoomData)
            {
                if ((pRoomData->mxRedPlayer.Count() + nMemberCount) < pRoomData->nMaxPalyer)
                {
                    bFind = true;
                    break;
                }
                else if ((pRoomData->mxBluePlayer.Count() + nMemberCount) < pRoomData->nMaxPalyer)
                {
                    bFind = true;
                    break;
                }
            }
        }
    }

    if (!bFind)
    {
        xFindRoomID = CreateRoom(NFGUID(), nPVPMode, nGrade);
        if (xFindRoomID.IsNull())
        {
            return false;
        }
    }

    for (int i = 0; i < varMemberList.GetCount(); i++)
    {
        const NFGUID& xPlayerID = varMemberList.Object(i);
        if (!xPlayerID.IsNull())
        {
            PlayerEnterRoom(xPlayerID, bRed, xFindRoomID);
        }
    }

    return true;
}

int NFCPVPMatchModule::GetGradeByScore(const int nScore)
{
    return 1;
}

int NFCPVPMatchModule::GetMemberCount(const int nPVPMode)
{
    switch (nPVPMode)
    {
    case EPVPMODE_ONE:
        return 1;
        break;
    case EPVPMODE_TWO:
        return 2;
        break;
    case EPVPMODE_THREE:
        return 3;
        break;
    default:
        return -1;
        break;
    }

    return -1;
}

NFGUID NFCPVPMatchModule::CreateRoom(const NFGUID& self, const int nPVPMode, const int nGrade)
{
    NFGUID xRoomID = m_pUUIDModule->CreateGUID();
    if (xRoomID.IsNull())
    {
        return NFGUID();
    }

    NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xRoomID);
    if (!pRoomData)
    {
        pRoomData = NF_SHARE_PTR<PVPRoom>(NF_NEW PVPRoom());
        mxRoomInfo.AddElement(xRoomID, pRoomData);
    }

    pRoomData->mnCellStatus = 0;
    pRoomData->xCellID = xRoomID;
    pRoomData->mnPVPMode = nPVPMode;
    pRoomData->mnPVPGrade = nGrade;

    int nPVPModeMaxMember = GetMemberCount(nPVPMode);
    if (nPVPModeMaxMember < 0)
    {
        mxRoomInfo.RemoveElement(xRoomID);
        return NFGUID();
    }

    pRoomData->nMaxPalyer = nPVPModeMaxMember;

    NF_SHARE_PTR<NFMapEx< int, NFMapEx<NFGUID, NFGUID> > > pPVPmodeData = mxWaitRoom.GetElement(nPVPMode);
    if (!pPVPmodeData)
    {
        pPVPmodeData = NF_SHARE_PTR<NFMapEx< int, NFMapEx<NFGUID, NFGUID> > >(NF_NEW NFMapEx< int, NFMapEx<NFGUID, NFGUID> >());
        mxWaitRoom.AddElement(nPVPMode, pPVPmodeData);
    }

    NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID > >pGradeData = pPVPmodeData->GetElement(nGrade);
    if (!pGradeData)
    {
        pGradeData = NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID > >(NF_NEW NFMapEx<NFGUID, NFGUID >());
        pPVPmodeData->AddElement(nGrade, pGradeData);
    }

    NF_SHARE_PTR<NFGUID> pRoomID = pGradeData->GetElement(xRoomID);
    if (pRoomID)
    {
        pRoomID = NF_SHARE_PTR<NFGUID>(NF_NEW NFGUID());
        pGradeData->AddElement(xRoomID, pRoomID);
    }

    *pRoomID = xRoomID;

    return xRoomID;
}

bool NFCPVPMatchModule::DestroyRoom(const NFGUID& self, const NFGUID& xRoomID)
{
    NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xRoomID);
    if (!pRoomData)
    {
        return false;
    }

    NF_SHARE_PTR<NFMapEx< int, NFMapEx<NFGUID, NFGUID> > > pPVPmodeData = mxWaitRoom.GetElement(pRoomData->mnPVPMode);
    if (pPVPmodeData)
    {
        NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID > >pGradeData = pPVPmodeData->GetElement(pRoomData->mnPVPGrade);
        if (pGradeData)
        {
            pGradeData->RemoveElement(xRoomID);
        }
    }

    NFGUID xPlayerID;
    for (NF_SHARE_PTR<int> pData = pRoomData->mxBluePlayer.First(xPlayerID); pData != NULL; pData = pRoomData->mxBluePlayer.Next(xPlayerID))
    {
        mxPlayerRoomInfo.RemoveElement(xPlayerID);
    }

    for (NF_SHARE_PTR<int> pData = pRoomData->mxRedPlayer.First(xPlayerID); pData != NULL; pData = pRoomData->mxRedPlayer.Next(xPlayerID))
    {
        mxPlayerRoomInfo.RemoveElement(xPlayerID);
    }

    mxRoomInfo.RemoveElement(xRoomID);

    return false;
}

bool NFCPVPMatchModule::PlayerEnterRoom(const NFGUID& self, const int nRed, const NFGUID& xRoomID)
{
    if (!xRoomID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xRoomID);
    if (nRed )
    {
        if (pRoomData->mxRedPlayer.Count() < pRoomData->nMaxPalyer)
        {
            pRoomData->mxRedPlayer.AddElement(self, NF_SHARE_PTR<int>(NF_NEW int(0)));
            NF_SHARE_PTR<NFGUID> pSelfRoomID = mxPlayerRoomInfo.GetElement(self);
            if (!pSelfRoomID)
            {
                pSelfRoomID = NF_SHARE_PTR<NFGUID>(NF_NEW NFGUID());
            }
            *pSelfRoomID = xRoomID;

            return true;
        }
    }
    else
    {
        if (pRoomData->mxBluePlayer.Count() < pRoomData->nMaxPalyer)
        {
            pRoomData->mxBluePlayer.AddElement(self, NF_SHARE_PTR<int>(NF_NEW int(0)));
            NF_SHARE_PTR<NFGUID> pSelfRoomID = mxPlayerRoomInfo.GetElement(self);
            if (!pSelfRoomID)
            {
                pSelfRoomID = NF_SHARE_PTR<NFGUID>(NF_NEW NFGUID());
            }
            *pSelfRoomID = xRoomID;

            return true;
        }
    }
    
    return false;
}

bool NFCPVPMatchModule::PlayerLeaveRoom(const NFGUID& self, const NFGUID& xRoomID)
{
    if (!xRoomID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<PVPRoom> pRoomData = mxRoomInfo.GetElement(xRoomID);
    if (pRoomData)
    {
        pRoomData->mxRedPlayer.RemoveElement(self);
        pRoomData->mxRedPlayer.RemoveElement(self);
    }

    mxPlayerRoomInfo.RemoveElement(self);

    return true;
}

NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> > NFCPVPMatchModule::GetRoomList(const int nPVPMode, const int nGrade)
{
    NF_SHARE_PTR<NFMapEx< int, NFMapEx<NFGUID, NFGUID> > > pPvPModeData = mxWaitRoom.GetElement(nPVPMode);
    if (pPvPModeData.get())
    {
        pPvPModeData = NF_SHARE_PTR<NFMapEx< int, NFMapEx<NFGUID, NFGUID> > >(NF_NEW NFMapEx< int, NFMapEx<NFGUID, NFGUID> >());
        mxWaitRoom.AddElement(nPVPMode, pPvPModeData);
    }

    NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> > pGradeData = pPvPModeData->GetElement(nGrade);
    if (pGradeData.get())
    {
        pGradeData = NF_SHARE_PTR<NFMapEx<NFGUID, NFGUID> >(NF_NEW NFMapEx<NFGUID, NFGUID>());
        pPvPModeData->AddElement(nGrade, pGradeData);
    }

    return pGradeData;
}
