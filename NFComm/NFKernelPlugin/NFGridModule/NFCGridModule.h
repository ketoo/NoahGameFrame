// -------------------------------------------------------------------------
//    @FileName      :    NFCGridModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCGridModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_GRID_MODULE_H_
#define _NFC_GRID_MODULE_H_

#include <iostream>
#include <assert.h>
#include <map>
#include "NFIGridModule.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFIDataList.h"

class NFCSceneGridInfo
    : public NFList<NFIDENTID>
{
public:

    NFCSceneGridInfo(const NFIDENTID& gridID)
    {
        mGridID = gridID;
        for (int i = EGRID_TOP; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            mAroundGrid[i] = NULL;
        }
    }

    virtual ~NFCSceneGridInfo()
    {
        // TODO
    }

    void Init(NFCSceneGridInfo** pGridArray)
    {
        for (int i = EGRID_TOP; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            mAroundGrid[i] = pGridArray[i];
        }
    }

    void Finale()
    {
    }

    NFCSceneGridInfo* GetConnectGrid(EGRID_DIRECTION eDirection)
    {
        return mAroundGrid[eDirection];
    }

protected:
private:
    NFCSceneGridInfo* mAroundGrid[EGRID_DIRECTION_MAXCOUNT];
    NFIDENTID mGridID;
};

class NFCGridModule
    : public NFIGridModule
{
public:
    NFCGridModule(const int& sceneID, const int nSceneWidth);

    virtual ~NFCGridModule();

    // the event that a object are moving
    virtual const NFIDENTID OnObjectMove(const NFIDENTID& self, const int& sceneID,
                                         const NFIDENTID& lastGrid, const int nX, const int nY, const int nZ);

    // the event that a object has entried
    virtual const NFIDENTID OnObjectEntry(const NFIDENTID& self, const int& sceneID,
                                          const int nX, const int nY, const int nZ);

    // the event that a object has leaved
    virtual const NFIDENTID OnObjectLeave(const NFIDENTID& self, const int& sceneID,
                                          const NFIDENTID& lastGrid);

    //////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
    static const NFIDENTID ComputerGridID(const int nX, const int nY, const int nZ);

    // get the step lenth each two grid
    virtual const NFIDENTID GetStepLenth(const NFIDENTID& selfGrid, const NFIDENTID& otherGrid);

    // get some grids that around this grid
    virtual const int GetAroundGrid(const NFIDENTID& selfGrid, NFIDataList& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9);

    // get some grids that around this grid
    virtual const int GetAroundGrid(NFCSceneGridInfo* pGridInfo, NFIDataList& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9);

    // get some objects that around this grid
    virtual const int GetAroundObject(const NFIDENTID& selfGrid, NFIDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9);

    // get some objects that around this grid
    virtual const int GetAroundObject(NFCSceneGridInfo* pGridInfo, NFIDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9);

    // get a grid who connected it by direction
    virtual NFCSceneGridInfo* GetConnectGrid(const NFIDENTID& selfGrid, EGRID_DIRECTION eDirection);

    // get the pointer of this grid
    virtual NFCSceneGridInfo* GetGridInfo(const NFIDENTID& selfGrid);

protected:
private:

    bool Init();

    bool Final();

    bool RegisterGrid(const NFIDENTID& grid);

    bool OnMoveIn(const NFIDENTID& self, const NFIDENTID& grid, const NFIDENTID& lastGrid);

    bool OnMoveOut(const NFIDENTID& self, const NFIDENTID& grid, const NFIDENTID& lastGrid);

private:
    const static int nGridWidth = 10;

    int mnSceneWidth;
    int mSceneID;

    typedef std::map<NFIDENTID, NFCSceneGridInfo*> TMAP_GRID_INFO;
    TMAP_GRID_INFO mtGridInfoMap;

};

#endif
