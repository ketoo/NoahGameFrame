/*
			This file is part of:
				NoahFrame
		https://github.com/ketoo/NoahGameFrame

	Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

	File creator: lvsheng.huang

	NoahFrame is open-source software and you can redistribute it and/or modify
	it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef NFI_CELL_MODULE_H
#define NFI_CELL_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include <array>
#include "NFIModule.h"
#include "NFGUID.h"

class NFSceneCellInfo;

enum ECELL_DIRECTION
{
    ECELL_TOP = 0,
    ECELL_DOWN = 1,
    ECELL_LEFT = 2,
    ECELL_RIGHT = 3,

    ECELL_LEFT_TOP = 4,
    ECELL_LEFT_DOWN = 5,
    ECELL_RIGHT_TOP = 6,
    ECELL_RIGHT_DOWN = 7,

    ECELL_DIRECTION_MAXCOUNT = 8,
};

enum ECELL_AROUND
{
    ECELL_SELF,
    ECELL_AROUND_9,
    ECELL_AROUND_16,
    ECELL_AROUND_25,
    ECELL_AROUND_ALL,
};
//id, scene_id, group_id, move_out_cell_id, move_in_cell_id
typedef std::function<int(const NFGUID&, const int&, const int&, const NFGUID&, const NFGUID&)> CELL_MOVE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<CELL_MOVE_EVENT_FUNCTOR> CELL_MOVE_EVENT_FUNCTOR_PTR;//EVENT

class NFICellModule
	: public NFIModule
{
public:
    virtual ~NFICellModule() {}

	virtual const NFGUID ComputeCellID(const int nX, const int nY, const int nZ) = 0;
	virtual const NFGUID ComputeCellID(const NFVector3& vec) = 0;

	virtual const bool CreateGroupCell(const int& sceneID, const int& groupID) = 0;
	virtual const bool DestroyGroupCell(const int& sceneID, const int& groupID) = 0;

    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toCell) = 0;

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& toCell) = 0;

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell) = 0;


	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const bool bPlayer, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const bool bPlayer, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, const std::string& strClassName, NFDataList& list, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, const std::string& strClassName, NFDataList& list, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9) = 0;


    //////////////////////////////////////////////////////////////////////////

	template<typename BaseType>
	bool AddMoveEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int&, const int&, const NFGUID&, const NFGUID&))
	{
		CELL_MOVE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr(new CELL_MOVE_EVENT_FUNCTOR(functor));

		AddMoveEventCallBack(functorPtr);
		return true;
	}
	template<typename BaseType>
	bool AddMoveInEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int&, const int&, const NFGUID&, const NFGUID&))
	{
		CELL_MOVE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr(new CELL_MOVE_EVENT_FUNCTOR(functor));

		AddMoveInEventCallBack(functorPtr);
		return true;
	}

	template<typename BaseType>
	bool AddMoveOutEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int&, const int&, const NFGUID&, const NFGUID&))
	{
		CELL_MOVE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr(new CELL_MOVE_EVENT_FUNCTOR(functor));

		AddMoveOutEventCallBack(functorPtr);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
protected:
	virtual int AddMoveEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddMoveInEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddMoveOutEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr) = 0;

private:
};

#endif
