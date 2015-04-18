// -------------------------------------------------------------------------
//    @FileName         :    NFISceneProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFISceneProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SCENEPROCESS_MODULE_H_
#define _NFI_SCENEPROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

enum E_SCENE_TYPE
{
	SCENE_TYPE_NORMAL,
    SCENE_TYPE_MAINLINE_CLONE,

    SCENE_TYPE_ERROR,
};

class NFISceneProcessModule
    : public NFILogicModule
{

public:
    virtual bool IsCloneScene(const int nSceneID) = 0;
    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID) = 0;
};

#endif