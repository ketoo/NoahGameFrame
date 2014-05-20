// -------------------------------------------------------------------------
//    @FileName         £º    NFISceneInfoModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-04-16
//    @Module           £º    NFISceneInfoModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SCENEINFO_MODULE_H_
#define _NFI_SCENEINFO_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFISceneInfoModule
    : public NFILogicModule
{

public:
    virtual bool LoadSceneInfo(const std::string& strFileName) = 0;

};

#endif