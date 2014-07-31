// -------------------------------------------------------------------------
//    @FileName      :    NFCGridModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGridModule
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGridModule.h"
#include "NFComm/NFCore/NFCDataList.h"

NFCGridModule::NFCGridModule(const int& sceneID, const int nSceneWidth)
{
    //     if (sceneID.IsNull()
    //         || nSceneWidth <= 0)
    //     {
    //         assert(0);
    //     }

    mSceneID = sceneID;
    mnSceneWidth = nSceneWidth;

    Init();
}

NFCGridModule::~NFCGridModule()
{
    Final();
}

const NFIDENTID NFCGridModule::OnObjectMove(const NFIDENTID& self, const int& sceneID, const NFIDENTID& lastGrid, const int nX, const int nY, const int nZ)
{
    if (sceneID != mSceneID)
    {
        return 0;
    }
    const NFIDENTID& currGrid = ComputerGridID(nX, nY, nZ);
    if (currGrid == lastGrid)
    {
        return lastGrid;
    }

    OnMoveOut(self, currGrid, lastGrid);
    OnMoveIn(self, currGrid, lastGrid);

    return currGrid;
}

const NFIDENTID NFCGridModule::OnObjectEntry(const NFIDENTID& self, const int& sceneID, const int nX, const int nY, const int nZ)
{
    NFIDENTID gridID = ComputerGridID(nX, nY, nZ);
    NFCSceneGridInfo* pGridInfo = GetGridInfo(gridID);
    if (pGridInfo)
    {
        OnMoveIn(self, gridID, gridID);
    }

    return gridID;
}

const NFIDENTID NFCGridModule::OnObjectLeave(const NFIDENTID& self, const int& sceneID, const NFIDENTID& lastGrid)
{

    OnMoveOut(self, lastGrid, lastGrid);

    return 0;
}

const NFIDENTID NFCGridModule::ComputerGridID(const int nX, const int nY, const int nZ)
{
    int nGridXIndex = nX / nGridWidth;
    int nGridZIndex = nZ / nGridWidth;

    return NFIDENTID(nGridXIndex, nGridZIndex);
}

const NFIDENTID NFCGridModule::GetStepLenth(const NFIDENTID& selfGrid, const NFIDENTID& otherGrid)
{
    return abs(otherGrid.nIdent - selfGrid.nIdent) + abs(otherGrid.nSerial - selfGrid.nSerial);
}

const int NFCGridModule::GetAroundGrid(const NFIDENTID& selfGrid, NFIDataList& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    NFCSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    return GetAroundGrid(pGridInfo, gridList, eAround);
}

const int NFCGridModule::GetAroundGrid(NFCSceneGridInfo* pGridInfo, NFIDataList& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    int nObjectCount = 0;

    if (!pGridInfo)
    {
        return nObjectCount;
    }

    gridList.AddPointer(pGridInfo);

    nObjectCount += pGridInfo->Count();

    switch (eAround)
    {
        case EGRID_AROUND_9:
        {
            for (int i = 0; i < EGRID_DIRECTION_MAXCOUNT; i++)
            {
                NFCSceneGridInfo* pInfo = pGridInfo->GetConnectGrid((EGRID_DIRECTION)i);
                if (pInfo)
                {
                    gridList.AddPointer(pInfo);
                    //gridList.push_back( pInfo );
                    nObjectCount += pInfo->Count();
                }
            }
        }
        break;
        case EGRID_AROUND_16:
            break;
        case EGRID_AROUND_25:
            break;
        case EGRID_AROUND_ALL:
            break;
        default:
            break;
    }

    return nObjectCount;
}

const int NFCGridModule::GetAroundObject(const NFIDENTID& selfGrid, NFIDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    NFCSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    if (pGridInfo)
    {
        return GetAroundObject(pGridInfo, objectList, eAround);
    }
    return 0;
}

const int NFCGridModule::GetAroundObject(NFCSceneGridInfo* pGridInfo, NFIDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    if (!pGridInfo)
    {
        return 0;
    }

    NFCDataList gridList;
    if (GetAroundGrid(pGridInfo, gridList, eAround) > 0)
    {
        for (int i = 0; i < gridList.GetCount(); i++)
        {
            NFCSceneGridInfo* pGridInfo = (NFCSceneGridInfo*)(gridList.PointerVal(i));
            if (pGridInfo)
            {
                NFIDENTID ident = 0;
                bool bRet = pGridInfo->First(ident);
                while (bRet)
                {
                    objectList.AddObject(ident);
                    bRet = pGridInfo->Next(ident);
                }
            }
        }
    }

    return objectList.GetCount();
}

NFCSceneGridInfo* NFCGridModule::GetConnectGrid(const NFIDENTID& selfGrid, EGRID_DIRECTION eDirection)
{
    NFCSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    if (pGridInfo)
    {
        return pGridInfo->GetConnectGrid(eDirection);
    }

    return NULL;
}

NFCSceneGridInfo* NFCGridModule::GetGridInfo(const NFIDENTID& selfGrid)
{
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(selfGrid);
    if (it != mtGridInfoMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFCGridModule::Init()
{
    // init all grid
    for (int nWidthPos = 0; nWidthPos < mnSceneWidth; nWidthPos += nGridWidth)
    {
        for (int nHeightPos = 0; nHeightPos < mnSceneWidth; nHeightPos += nGridWidth)
        {
            NFIDENTID gridID = ComputerGridID(nWidthPos, 0, nHeightPos);
            if (!GetGridInfo(gridID))
            {
                RegisterGrid(gridID);
            }
        }
    }
    //     for (int nWidthPos = -mnSceneWidth + 1; nWidthPos < mnSceneWidth; nWidthPos++)
    //     {
    //         for (int nHeightPos = -mnSceneWidth + 1; nHeightPos < mnSceneWidth; nHeightPos++)
    //         {
    //             NFIDENTID gridID = ComputerGridID(nWidthPos, 0, nHeightPos);
    //             if (!GetGridInfo(gridID))
    //             {
    //                 RegisterGrid(gridID);
    //             }
    //         }
    //     }

    // connect all grid
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.begin();
    for (it; it != mtGridInfoMap.end(); it++)
    {
        NFCSceneGridInfo* aroundGrid[EGRID_DIRECTION_MAXCOUNT] = { 0 };

        for (int  i = 0; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            aroundGrid[i] = GetConnectGrid(it->first, (EGRID_DIRECTION)i);
        }

        it->second->Init(aroundGrid);
    }

    return true;
}

bool NFCGridModule::Final()
{
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.begin();
    for (it; it != mtGridInfoMap.end(); it++)
    {
        delete it->second;
    }

    mtGridInfoMap.clear();

    return true;
}

bool NFCGridModule::RegisterGrid(const NFIDENTID& grid)
{
    if (!GetGridInfo(grid))
    {
        NFCSceneGridInfo* pInfo = NF_NEW NFCSceneGridInfo(grid);
        mtGridInfoMap.insert(TMAP_GRID_INFO::value_type(grid, pInfo));

        return true;
    }

    return false;
}

bool NFCGridModule::OnMoveIn(const NFIDENTID& self, const NFIDENTID& grid, const NFIDENTID& lastGrid)
{
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(grid);
    if (it != mtGridInfoMap.end())
    {
        return it->second->Add(self);
    }

    return false;
}

bool NFCGridModule::OnMoveOut(const NFIDENTID& self, const NFIDENTID& grid, const NFIDENTID& lastGrid)
{
    //grid为现在的格子
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(grid);
    if (it != mtGridInfoMap.end())
    {
        return it->second->Remove(self);
    }

    return true;
}

