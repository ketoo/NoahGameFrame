// -------------------------------------------------------------------------
//    @FileName         ��    NFISceneInfoModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-04-16
//    @Module           ��    NFISceneInfoModule
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