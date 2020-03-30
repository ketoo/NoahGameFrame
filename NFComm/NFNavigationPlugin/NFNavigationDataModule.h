/*
    Copyright 2009 - 2019 NoahGameFrame/NoahFrame
    
   This source file is part of NoahGameFrame/NoahFrame.
   NoahGameFrame/NoahFrame is open-source software and you can redistribute it and/or modify
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

#ifndef NF_MAP_DATA_MODULE_H
#define NF_MAP_DATA_MODULE_H

#include <memory>
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFINavigationDataModule.h"
////////////////////////////////////////////////////////////////////////////


class NFNavigationDataModule
    : public NFINavigationDataModule
{
public:
    NFNavigationDataModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init() override;
    virtual bool Shut() override;
    virtual bool Execute() override;

    virtual bool AfterInit() override;

    virtual const std::string& GetDefaultMapData(const int scene);

	virtual const NF_SHARE_PTR<GroupNavigationData> GetMapData(const int scene);
    virtual const NF_SHARE_PTR<GroupNavigationData> GetMapData(const int scene, const int group);
    virtual const NF_SHARE_PTR<NFVoxel> GetMapData(const int scene, const int group, const int x, int z);

    //modify map data in run time
    virtual bool SetMapDataOccupyItem(const int scene, const int group, const int x, const int z, const std::string& item);
    virtual bool SetMapDataMovable(const int scene, const int group, const int x, int z, const int movable);
    virtual bool SetMapDataOccupy(const int scene, const int group, const int x, int z, const NFGUID occupy);
    virtual bool SetMapDataLayer(const int scene, const int group, const int x, int z, const int layer);
protected:
    //<scene,group> ==>data
    NFMapEx<NFGUID, GroupNavigationData> mGroupNavigationData;

    //////////////////////////////////////////////////////////////////////////

    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneModule* m_pSceneModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIGameServerToWorldModule* m_pNetClientModule;
};
#endif
