#pragma once
// -------------------------------------------------------------------------
//    @FileName			:    NFRequestSelectWorld.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFRequestSelectWorld
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIRequest.h"

class NFRequestSelectWorld : public NFIRequest
{
public:
	int id;

};

AJSON(NFRequestSelectWorld, id)