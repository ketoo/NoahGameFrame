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
#ifndef NF_BLUE_PRINT_VIEW_H
#define NF_BLUE_PRINT_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"

# include "imgui_node_editor.h"
# define IMGUI_DEFINE_MATH_OPERATORS
# include "ImGui/imgui_internal.h"
namespace ed = ax::NodeEditor;

// Struct to hold basic information about connection between
// pins. Note that connection (aka. link) has its own ID.
// This is useful later with dealing with selections, deletion
// or other operations.
struct LinkInfo
{
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};

class NFBluePrintView : public NFIView
{
public:
	NFBluePrintView(NFIPluginManager* p, NFViewType vt);

	virtual void Begin();
	virtual bool Execute();
	virtual void End();

	virtual void SubRender();

	ed::EditorContext* g_Context = nullptr;    // Editor context, required to trace a editor state.
	bool                 g_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
	ImVector<LinkInfo>   g_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
	int                  g_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
 
	void Application_Frame();
	void ImGuiEx_BeginColumn();
	void ImGuiEx_EndColumn();
	void ImGuiEx_NextColumn();

private:
	NFIUIModule* m_pUIModule;
};

#endif