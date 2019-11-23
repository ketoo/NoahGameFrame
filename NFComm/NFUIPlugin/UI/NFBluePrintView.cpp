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

bool NFBluePrintView::Execute()
{
	//1. the project root folder is NFDataCfg

   imnodes::BeginNodeEditor();
   /////////////////////////////


   const int hardcoded_node_id = 1;
   imnodes::SetNodeName(hardcoded_node_id, "empty node");

   imnodes::BeginNode(hardcoded_node_id);
   imnodes::BeginInputAttribute(hardcoded_node_id);
   ImGui::Text("prop 1");
   ImGui::Text("prop 2");
   ImGui::Text("prop 3");
   imnodes::EndAttribute();
   imnodes::EndNode();


   const int hardcoded_node_id1 = 2;
   imnodes::SetNodeName(hardcoded_node_id1, "output node");
   imnodes::BeginNode(hardcoded_node_id1);

   const int output_attr_id = 2;
   imnodes::BeginOutputAttribute(output_attr_id);
   // in between Begin|EndAttribute calls, you can call ImGui
   // UI functions
   ImGui::Text("output pin");
   imnodes::EndAttribute();

   imnodes::EndNode();




   /////////////////////////////
   imnodes::EndNodeEditor();

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

void NFBluePrintView::SubRender()
{
   ImGui::Text(this->name.c_str());
}