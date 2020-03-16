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
#ifndef NF_GOD_VIEW_H
#define NF_GOD_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFINavigationDataModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFNodeSystem.h"
#include "NFTreeView.h"

class NFGodView : public NFIView
{
public:
	NFGodView(NFIPluginManager* p, NFViewType vt);
   ~NFGodView();
   
	virtual bool Execute();

   virtual void SubRender();

   NFGUID GetCurrentObjectID();
   void SetCurrentObjectID(const NFGUID& id);

private:
	void RenderScene(const int sceneID, const int groupID);
	void HandlerSelected(const NFGUID& id, const bool doubleClick);
    void DrawMapData();
    void DrawToolBar();
    void OnMapClicked(const NFVector3& pos);

    bool HandlerNodeHovered(const NFGUID& id);

    void RenderSceneObjectNode(const int sceneID, const int groupID);
    void UpdateSceneObjectNodePosition(const int sceneID, const int groupID);

    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    NFVector2 ToMapGridPos(const NFVector3& pos);

private:
    std::vector<int> mLayerColor;
    int mStairColor;
    int mTextColor;
    int mImmovable;
    NFVector2 mClickedPos;

private:
	int mGroupID = -1;
	int mSceneID = -1;
	NFGUID mCurrentObjectID;
   
	NodeSystem::NFNodeSystem mNodeSystem;
	NFTreeView mTreeView;
private:
	NFIUIModule* m_pUIModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFISceneModule* m_pSceneModule;
    NFIKernelModule* m_pKernelModule;
    NFINavigationDataModule* m_pNavigationDataModule;
};

#endif