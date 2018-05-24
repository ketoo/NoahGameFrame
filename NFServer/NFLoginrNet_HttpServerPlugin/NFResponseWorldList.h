#pragma once
// -------------------------------------------------------------------------
//    @FileName			:    NFResponseWorldList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFResponseWorldList
//
// -------------------------------------------------------------------------

#include <list>
#include "NFComm/NFPluginModule/NFIResponse.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

class NFResponseWorldList : public NFIResponse
{
public:
	class NFWorld
	{
	public:
		int id;
		std::string name;
		NFMsg::EServerState state;
		int count;
	};

	std::list<NFWorld> world;
};

AJSON(NFResponseWorldList::NFWorld, id, name, state, count)
AJSON(NFResponseWorldList, world, code, message)