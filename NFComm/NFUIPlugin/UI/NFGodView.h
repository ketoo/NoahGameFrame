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
#ifndef NF_GOD_VIEW_H
#define NF_GOD_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFNodeView.h"

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
   void RenderScene(const int sceneID);
   void RenderScene(const int sceneID, const int groupID);

   void RenderSceneObjectNode(const int sceneID, const int groupID);
private:
   int mnSceneID = -1;
   int mnGroupID = -1;

   NFGUID mCurrentObjectID;
   
   NFNodeView* m_pNodeView;
private:
   NFIUIModule* m_pUIModule;
   NFIClassModule* m_pClassModule;
   NFIElementModule* m_pElementModule;
   NFISceneModule* m_pSceneModule;
   NFIKernelModule* m_pKernelModule;
};

#endif