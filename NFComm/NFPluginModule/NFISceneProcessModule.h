// -------------------------------------------------------------------------
//    @FileName         :    NFISceneProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFISceneProcessModule
//
// -------------------------------------------------------------------------

#ifndef NFI_SCENEPROCESS_MODULE_H
#define NFI_SCENEPROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"

enum E_SCENE_TYPE
{
	SCENE_TYPE_NORMAL,
	SCENE_TYPE_SINGLE_CLONE_SCENE,
	SCENE_TYPE_MULTI_CLONE_SCENE,
    SCENE_TYPE_ERROR,
};

class NFISceneProcessModule
    : public NFIModule
{

public:
    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID) = 0;
	virtual bool RequestEnterScene(const NFGUID & self, const int nSceneID, const int nType, const NFDataList & argList) = 0;
	virtual bool RequestEnterScene(const NFGUID& self, const int nSceneID, const int nGrupID, const int nType, const NFDataList& argList) = 0;
};

#endif