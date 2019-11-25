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

#include "NFNodeView.h"
#include "NFUIModule.h"

void NFNodeAttri::Execute()
{
   if (inputPin)
   {
      imnodes::BeginInputAttribute(id);
      ImGui::Text("input pin");
   }
   else
   {
      imnodes::BeginOutputAttribute(id);
      ImGui::Text("output pin");
   }
 
   imnodes::EndAttribute(); 
}

void NFNode::Execute()
{
   imnodes::SetNodeName(id, name.c_str());
   imnodes::BeginNode(id);

   for (auto it : mAttris)
   {
     it->Execute();
   }

   imnodes::EndNode();
}

/////////////////

NFNodeView::NFNodeView(NFIPluginManager* p) : NFIView(p, NFViewType::NONE, GET_CLASS_NAME(NFNodeView))
{
   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();

}

int NFNodeView::GenerateId()
{
   int i = NFGetTimeS();
   i++;
   return i;
}

bool NFNodeView::Execute()
{
	//1. the project root folder is NFDataCfg

   imnodes::BeginNodeEditor();

   RenderNodes();
   RenderLinks();
/*
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

*/

   /////////////////////////////
   imnodes::EndNodeEditor();
   
   int start_attr, end_attr;
   if (imnodes::IsLinkCreated(&start_attr, &end_attr))
   {
      if (GetNodeByAttriId(start_attr) != GetNodeByAttriId(end_attr))
      {
         mLinks.push_back(NF_SHARE_PTR<NFNodeLink>(NF_NEW NFNodeLink(start_attr, end_attr)));
      }
   }

	return false;
}

void NFNodeView::RenderNodes()
{
   for (auto it : mNodes)
   {
      it->Execute();
   }
}

void NFNodeView::RenderLinks()
{
   int i = 0;
   for (auto it : mLinks)
   {
      ++i;
      imnodes::Link(i, it->start, it->end);
   }
}

void NFNodeView::AddNode(const int nodeId, const std::string& name)
{
   mNodes.push_back(NF_SHARE_PTR<NFNode>(NF_NEW NFNode(nodeId, name)));
}

void NFNodeView::AddNodeAttrIn(const int nodeId, const int attrId, const std::string& name)
{
   for (auto it : mNodes)
   {
      if (it->id == nodeId)
      {
         it->AddAttribute(attrId, name, true);
         return;
      }
   }
}

void NFNodeView::AddNodeAttrOut(const int nodeId, const int attrId, const std::string& name)
{
   for (auto it : mNodes)
   {
      if (it->id == nodeId)
      {
         it->AddAttribute(attrId, name, false);
         return;
      }
   }
}

void NFNodeView::DeleteNode(const int nodeId)
{
   for (auto it = mNodes.begin(); it != mNodes.end(); ++it)
   {
      if ((*it)->id == nodeId)
      {
         mNodes.erase(it);
         return;
      }
   }
}   

int NFNodeView::GetNodeByAttriId(const int attriId)
{
   for (auto it : mNodes)
   {
      NF_SHARE_PTR<NFNode> node = it;
      for (auto attri : node->mAttris)
      {
         if (attri->id == attriId)
         {
            return node->id;
         }
      }
   }

   return -1;
}

void NFNodeView::ResetOffestZero()
{
   //imnodes::EditorContext& context = imnodes::editor_context_get();
   //imnodes::editor_context_reset_panning(&context);
}