// -------------------------------------------------------------------------
//    @FileName      :    NFIGridModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIGridModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_GRID_MODULE_H_
#define _NFI_GRID_MODULE_H_

#include <list>
#include <iostream>
#include <algorithm>
#include "NFComm/NFCore/NFIdentID.h"
#include "NFComm/NFCore/NFIDataList.h"

class NFCSceneGridInfo;

enum EGRID_DIRECTION
{
    EGRID_TOP,
    EGRID_DOWN,
    EGRID_LEFT,
    EGRID_RIGHT,

    EGRID_LEFT_TOP,
    EGRID_LEFT_DOWN,
    EGRID_RIGHT_TOP,
    EGRID_RIGHT_DOWN,

    EGRID_DIRECTION_MAXCOUNT,
};

enum EGRID_AROUND
{
    EGRID_SELF,
    EGRID_AROUND_9,
    EGRID_AROUND_16,
    EGRID_AROUND_25,
    EGRID_AROUND_ALL,
};

class NFIGridModule
{
public:
    virtual ~NFIGridModule(){}
    // the event that a object are moving
    virtual const NFIDENTID OnObjectMove(const NFIDENTID& self, const int& sceneID,
                                         const NFIDENTID& lastGrid, const int nX, const int nY, const int nZ) = 0;

    // the event that a object has entried
    virtual const NFIDENTID OnObjectEntry(const NFIDENTID& self, const int& sceneID,
                                          const int nX, const int nY, const int nZ) = 0;

    // the event that a object has leaved
    virtual const NFIDENTID OnObjectLeave(const NFIDENTID& self, const int& sceneID,
                                          const NFIDENTID& lastGrid) = 0;

    //////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
    //virtual const NFIDENTID ComputerGridID(const int nX, const int nY, const int nZ) = 0;

    // get the step lenth each two grid
    virtual const NFIDENTID GetStepLenth(const NFIDENTID& selfGrid, const NFIDENTID& otherGrid) = 0;

    // get some grids that around this grid
    virtual const int GetAroundGrid(const NFIDENTID& selfGrid, NFIDataList& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get some grids that around this grid
    virtual const int GetAroundGrid(NFCSceneGridInfo* pGridInfo, NFIDataList& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get some objects that around this grid
    virtual const int GetAroundObject(const NFIDENTID& selfGrid, NFIDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get some objects that around this grid
    virtual const int GetAroundObject(NFCSceneGridInfo* pGridInfo, NFIDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get a grid who connected it by direction
    virtual NFCSceneGridInfo* GetConnectGrid(const NFIDENTID& selfGrid, EGRID_DIRECTION eDirection) = 0;

    // get the pointer of this grid
    virtual NFCSceneGridInfo* GetGridInfo(const NFIDENTID& selfGrid) = 0;

protected:
private:
};

#endif
