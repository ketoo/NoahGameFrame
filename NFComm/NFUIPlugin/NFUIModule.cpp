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

#include "NFUIModule.h"
#include "UI/NFGodView.h"
#include "UI/NFSceneView.h"
#include "UI/NFHierachyView.h"
#include "UI/NFConsoleView.h"
#include "UI/NFProfileView.h"
#include "UI/NFBluePrintView.h"
#include "UI/NFProjectView.h"
#include "UI/NFGameView.h"

/*
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
*/


bool NFUIModule::Awake()
{

	// Setup Dear ImGui context
	/*
		IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	*/


	return true;
}

bool NFUIModule::Init()
{
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFGodView(pPluginManager, NFIView::NFViewType::GodView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFSceneView(pPluginManager, NFIView::NFViewType::SceneView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFHierachyView(pPluginManager, NFIView::NFViewType::HierachyView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFConsoleView(pPluginManager, NFIView::NFViewType::ConsoleView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFProfileView(pPluginManager, NFIView::NFViewType::InspectorView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFBluePrintView(pPluginManager, NFIView::NFViewType::BluePrintView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFProjectView(pPluginManager, NFIView::NFViewType::ProjectView)));
    mViewList.push_back(NF_SHARE_PTR<NFIView>(NF_NEW NFGameView(pPluginManager, NFIView::NFViewType::GameView)));
   
	return true;
}

bool NFUIModule::AfterInit()
{


	return true;
}

bool NFUIModule::CheckConfig()
{

	return true;
}

bool NFUIModule::ReadyExecute()
{


	return true;
}

bool NFUIModule::Execute()
{
	//ImGui::NewFrame();

	for(NF_SHARE_PTR<NFIView> item : mViewList)
	{
		item->Execute();
	}

	//ImGui::End();

	// Render dear imgui into screen
    //ImGui::Render();

	return true;
}

bool NFUIModule::BeforeShut()
{

	return true;
}

bool NFUIModule::Shut()
{
	//ImGui::DestroyContext();

	return true;
}

bool NFUIModule::Finalize()
{


	return true;
}

bool NFUIModule::OnReloadPlugin()
{

	return true;
}

