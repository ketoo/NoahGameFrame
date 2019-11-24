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

#include "NFBluePrintView.h"
#include "NFUIModule.h"

NFBluePrintView::NFBluePrintView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFBluePrintView))
{
   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();

}



void NFBluePrintView::Begin()
{
	ed::Config config;
	config.SettingsFile = "BasicInteraction.json";
	g_Context = ed::CreateEditor(&config);
}

bool NFBluePrintView::Execute()
{
	////1. the project root folder is NFDataCfg

 //  imnodes::BeginNodeEditor();
 //  /////////////////////////////


 //  const int hardcoded_node_id = 1;
 //  imnodes::SetNodeName(hardcoded_node_id, "empty node");

 //  imnodes::BeginNode(hardcoded_node_id);
 //  imnodes::BeginInputAttribute(hardcoded_node_id);
 //  ImGui::Text("prop 1");
 //  ImGui::Text("prop 2");
 //  ImGui::Text("prop 3");
 //  imnodes::EndAttribute();
 //  imnodes::EndNode();


 //  const int hardcoded_node_id1 = 2;
 //  imnodes::SetNodeName(hardcoded_node_id1, "output node");
 //  imnodes::BeginNode(hardcoded_node_id1);

 //  const int output_attr_id = 2;
 //  imnodes::BeginOutputAttribute(output_attr_id);
 //  // in between Begin|EndAttribute calls, you can call ImGui
 //  // UI functions
 //  ImGui::Text("output pin");
 //  imnodes::EndAttribute();

 //  imnodes::EndNode();

	Application_Frame();


 //  /////////////////////////////
 //  imnodes::EndNodeEditor();

   if (ImGui::IsWindowFocused())
   {
      NF_SHARE_PTR<NFIView> pView = m_pUIModule->GetView(NFViewType::HierachyView);
      if (pView)
      {
         pView->OccupySubRender(this);
      }
   }

	return false;
}

void NFBluePrintView::End()
{
	ed::DestroyEditor(g_Context);
}

void NFBluePrintView::SubRender()
{
   ImGui::Text(this->name.c_str());
}

void NFBluePrintView::Application_Frame()
{

	auto& io = ImGui::GetIO();

	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

	ImGui::Separator();

	ed::SetCurrentEditor(g_Context);

	// Start interaction with editor.
	ed::Begin("My Editor", ImVec2(0.0, 0.0f));

	int uniqueId = 1;

	//
	// 1) Commit known data to editor
	//

	// Submit Node A
	ed::NodeId nodeA_Id = uniqueId++;
	ed::PinId  nodeA_InputPinId = uniqueId++;
	ed::PinId  nodeA_OutputPinId = uniqueId++;

	if (g_FirstFrame)
		ed::SetNodePosition(nodeA_Id, ImVec2(10, 10));
	ed::BeginNode(nodeA_Id);
	ImGui::Text("Node A");
	ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
	ImGui::Text("-> In");
	ed::EndPin();
	ImGui::SameLine();
	ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ed::EndNode();

	// Submit Node B
	ed::NodeId nodeB_Id = uniqueId++;
	ed::PinId  nodeB_InputPinId1 = uniqueId++;
	ed::PinId  nodeB_InputPinId2 = uniqueId++;
	ed::PinId  nodeB_OutputPinId = uniqueId++;

	if (g_FirstFrame)
		ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));
	ed::BeginNode(nodeB_Id);
	ImGui::Text("Node B");
	ImGuiEx_BeginColumn();
	ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
	ImGui::Text("-> In1");
	ed::EndPin();
	ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
	ImGui::Text("-> In2");
	ed::EndPin();
	ImGuiEx_NextColumn();
	ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ImGuiEx_EndColumn();
	ed::EndNode();

	// Submit Links
	for (auto& linkInfo : g_Links)
		ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

	//
	// 2) Handle interactions
	//

	// Handle creation action, returns true if editor want to create new object (node or link)
	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{
			// QueryNewLink returns true if editor want to create new link between pins.
			//
			// Link can be created only for two valid pins, it is up to you to
			// validate if connection make sense. Editor is happy to make any.
			//
			// Link always goes from input to output. User may choose to drag
			// link from output pin or input pin. This determine which pin ids
			// are valid and which are not:
			//   * input valid, output invalid - user started to drag new ling from input pin
			//   * input invalid, output valid - user started to drag new ling from output pin
			//   * input valid, output valid   - user dragged link over other pin, can be validated

			if (inputPinId && outputPinId) // both are valid, let's accept link
			{
				// ed::AcceptNewItem() return true when user release mouse button.
				if (ed::AcceptNewItem())
				{
					// Since we accepted new link, lets add one to our list of links.
					g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

					// Draw new link.
					ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
				}

				// You may choose to reject connection between these nodes
				// by calling ed::RejectNewItem(). This will allow editor to give
				// visual feedback by changing link thickness and color.
			}
		}
	}
	ed::EndCreate(); // Wraps up object creation action handling.


	// Handle deletion action
	if (ed::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ed::AcceptDeletedItem())
			{
				// Then remove link from your data.
				for (auto& link : g_Links)
				{
					if (link.Id == deletedLinkId)
					{
						g_Links.erase(&link);
						break;
					}
				}
			}

			// You may reject link deletion by calling:
			// ed::RejectDeletedItem();
		}
	}
	ed::EndDelete(); // Wrap up deletion action



	// End of interaction with editor.
	ed::End();

	if (g_FirstFrame)
		ed::NavigateToContent(0.0f);

	ed::SetCurrentEditor(nullptr);

	g_FirstFrame = false;

	// ImGui::ShowMetricsWindow();

}

const char* Application_GetName()
{
	return "Basic Interaction";
}


void NFBluePrintView::ImGuiEx_BeginColumn()
{
	ImGui::BeginGroup();
}

void NFBluePrintView::ImGuiEx_NextColumn()
{
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
}

void NFBluePrintView::ImGuiEx_EndColumn()
{
	ImGui::EndGroup();
}