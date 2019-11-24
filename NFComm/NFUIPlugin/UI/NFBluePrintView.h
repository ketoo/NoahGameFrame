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

// Struct to hold basic information about connection between
// pins. Note that connection (aka. link) has its own ID.
// This is useful later with dealing with selections, deletion
// or other operations.
struct LinkInfo
{
	ax::NodeEditor::LinkId Id;
	ax::NodeEditor::PinId  InputId;
	ax::NodeEditor::PinId  OutputId;
};

class NFNodeData
{
public:
	NFNodeData()
	{
		ax::NodeEditor::Config config;
		config.SettingsFile = "BasicInteraction.json";
		g_Context = ax::NodeEditor::CreateEditor(&config);
	}

	virtual ~NFNodeData()
	{
		ax::NodeEditorDestroyEditor(g_Context);
	}

	void Render()
	{
		auto& io = ImGui::GetIO();

		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

		ImGui::Separator();

		ax::NodeEditorSetCurrentEditor(g_Context);

		// Start interaction with editor.
		ax::NodeEditorBegin("My Editor", ImVec2(0.0, 0.0f));

		int uniqueId = 1;

		//
		// 1) Commit known data to editor
		//

		// Submit Node A
		ax::NodeEditorNodeId nodeA_Id = uniqueId++;
		ax::NodeEditorPinId  nodeA_InputPinId = uniqueId++;
		ax::NodeEditorPinId  nodeA_OutputPinId = uniqueId++;

		if (g_FirstFrame)
			ax::NodeEditorSetNodePosition(nodeA_Id, ImVec2(10, 10));
		ax::NodeEditorBeginNode(nodeA_Id);
		ImGui::Text("Node A");
		ax::NodeEditorBeginPin(nodeA_InputPinId, ax::NodeEditorPinKind::Input);
		ImGui::Text("-> In");
		ax::NodeEditorEndPin();
		ImGui::SameLine();
		ax::NodeEditorBeginPin(nodeA_OutputPinId, ax::NodeEditorPinKind::Output);
		ImGui::Text("Out ->");
		ax::NodeEditorEndPin();
		ax::NodeEditorEndNode();

		// Submit Node B
		ax::NodeEditorNodeId nodeB_Id = uniqueId++;
		ax::NodeEditorPinId  nodeB_InputPinId1 = uniqueId++;
		ax::NodeEditorPinId  nodeB_InputPinId2 = uniqueId++;
		ax::NodeEditorPinId  nodeB_OutputPinId = uniqueId++;

		if (g_FirstFrame)
			ax::NodeEditorSetNodePosition(nodeB_Id, ImVec2(210, 60));
		ax::NodeEditorBeginNode(nodeB_Id);
		ImGui::Text("Node B");

		ImGui::BeginGroup();
		ax::NodeEditorBeginPin(nodeB_InputPinId1, ax::NodeEditorPinKind::Input);
		ImGui::Text("-> In1");
		ax::NodeEditorEndPin();
		ax::NodeEditorBeginPin(nodeB_InputPinId2, ax::NodeEditorPinKind::Input);
		ImGui::Text("-> In2");
		ax::NodeEditorEndPin();

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ax::NodeEditorBeginPin(nodeB_OutputPinId, ax::NodeEditorPinKind::Output);
		ImGui::Text("Out ->");
		ax::NodeEditorEndPin();
		ImGui::EndGroup();

		ax::NodeEditorEndNode();

		// Submit Links
		for (auto& linkInfo : g_Links)
			ax::NodeEditorLink(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ax::NodeEditorBeginCreate())
		{
			ax::NodeEditorPinId inputPinId, outputPinId;
			if (ax::NodeEditorQueryNewLink(&inputPinId, &outputPinId))
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
					// ax::NodeEditorAcceptNewItem() return true when user release mouse button.
					if (ax::NodeEditorAcceptNewItem())
					{
						// Since we accepted new link, lets add one to our list of links.
						g_Links.push_back({ ax::NodeEditorLinkId(g_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ax::NodeEditorLink(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
					}

					// You may choose to reject connection between these nodes
					// by calling ax::NodeEditorRejectNewItem(). This will allow editor to give
					// visual feedback by changing link thickness and color.
				}
			}
		}
		ax::NodeEditorEndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ax::NodeEditorBeginDelete())
		{
			// There may be many links marked for deletion, let's loop over them.
			ax::NodeEditorLinkId deletedLinkId;
			while (ax::NodeEditorQueryDeletedLink(&deletedLinkId))
			{
				// If you agree that link can be deleted, accept deletion.
				if (ax::NodeEditorAcceptDeletedItem())
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
				// ax::NodeEditorRejectDeletedItem();
			}
		}
		ax::NodeEditorEndDelete(); // Wrap up deletion action



		// End of interaction with editor.
		ax::NodeEditorEnd();

		if (g_FirstFrame)
			ax::NodeEditorNavigateToContent(0.0f);

		ax::NodeEditorSetCurrentEditor(nullptr);

		g_FirstFrame = false;

	}

	ax::NodeEditorEditorContext* g_Context = nullptr;    // Editor context, required to trace a editor state.
	bool                 g_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
	ImVector<LinkInfo>   g_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
	int g_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
};

class NFBluePrintView : public NFIView
{
public:
	NFBluePrintView(NFIPluginManager* p, NFViewType vt);

	virtual bool Execute();

	virtual void SubRender();


 
private:
	NFNodeData mNodeData;

private:
	NFIUIModule* m_pUIModule;
};

#endif