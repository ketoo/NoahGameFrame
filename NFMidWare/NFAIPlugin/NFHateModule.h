/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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


#ifndef NF_HATE_MODULE_H
#define NF_HATE_MODULE_H

#include <string>
#include <iostream>
#include <map>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"
#include "NFComm/NFPluginModule/NFIFriendModule.h"

class NFHateModule
    : public NFIHateModule
{
public:
    NFHateModule(NFIPluginManager* pm)
    {
        pPluginManager = pm;
    }
	virtual bool Init();

	virtual bool Shut();

	virtual bool AfterInit();

	virtual bool BeforeShut();

	virtual bool Execute();

    virtual bool ClearHateObjects(const NFGUID& self);

    //添加仇恨对象
    virtual bool AddHate(const NFGUID& self, const NFGUID& other, const int nValue);

    //设置仇恨值
    virtual bool EndSpring(const NFGUID& other);
	virtual bool OnSpring(const NFGUID& self, const NFGUID& other);

    //把他人的仇恨copy到自己身上,BOSS,小弟
    virtual bool CopyHateList(const NFGUID& self, const NFGUID& other);

    //对仇恨排序，最高在前面
    //virtual void CompSortList(const NFGUID& self);

    //得到最高仇恨的一个对象
    virtual NFGUID QueryMaxHateObject(const NFGUID& self);

    //查询仇恨值
    virtual int QueryHateValue(const NFGUID& self, const NFGUID& other);

protected:
	bool SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue);
	int OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnPositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar);
	int OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar);

protected:
	struct ObjectHateData
	{
		std::map<NFGUID, int> hateValue;
	};

	struct SceneHateData
	{
		int sceneID;
		int groupID;
		NFMapEx<NFGUID, ObjectHateData> hateValue;
	};
	//by <sceneid, groupid> as guid
	//scene_group_guid, NFMapEx<NFGUID, ObjectHateData>
    NFMapEx<NFGUID, SceneHateData> mtObjectHateMap;

private:
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIElementModule* m_pElementModule;
	NFIFriendModule* m_pFriendModule;
};

#endif