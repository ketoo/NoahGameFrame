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
      ImGui::Text(name.c_str());
   }
   else
   {
      imnodes::BeginOutputAttribute(id);
      ImGui::Text(name.c_str());
   }
 
   imnodes::EndAttribute(); 
}

void NFNode::Execute()
{
   imnodes::SetNodeName(id, name.c_str());
   imnodes::BeginNode(id);

   if (first)
   {
      first = false;
      imnodes::SetNodePos(id, ImVec2(initPos.X(), initPos.Y()));
   }

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
   m_pEditorContext = imnodes::EditorContextCreate();
}

NFNodeView::~NFNodeView()
{
   imnodes::EditorContextFree((imnodes::EditorContext*)m_pEditorContext);
   m_pEditorContext = nullptr;
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
   EditorContextSet((imnodes::EditorContext*)m_pEditorContext);

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
   
   CheckNewLinkStatus();
  
	return false;
}

void NFNodeView::CleanNodes()
{
   mNodes.clear();
   mLinks.clear();
}

void NFNodeView::RenderNodes()
{
   for (auto it : mNodes)
   {
      it.second->Execute();
   }
}

void NFNodeView::RenderLinks()
{
   int i = 0;
   for (auto it : mLinks)
   {
      ++i;
      imnodes::Link(i, it->start_attr, it->end_attr);
   }
}

void NFNodeView::AddNode(const NFGUID guid, const std::string& name, const NFVector2 vec)
{
   if (mNodes.find(guid) == mNodes.end())
   {
      mNodes.insert(std::pair<NFGUID, NF_SHARE_PTR<NFNode>>(guid, NF_SHARE_PTR<NFNode>(NF_NEW NFNode(GenerateId(), name, guid, vec))));
   }
}

void NFNodeView::AddNodeAttrIn(const NFGUID guid, const NFGUID attrId, const std::string& name)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddAttribute(GenerateId(), name, true, attrId);
         return;
      }
   }
}

void NFNodeView::AddNodeAttrOut(const NFGUID guid, const NFGUID attrId, const std::string& name)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddAttribute(GenerateId(), name, false, attrId);
         return;
      }
   }
}

void NFNodeView::DeleteNode(const NFGUID guid)
{
   auto it = mNodes.find(guid);
   if (it != mNodes.end())
   {
      mNodes.erase(it);
   }
}   

NFGUID NFNodeView::GetNodeByAttriId(const NFGUID attriId)
{
   for (auto it : mNodes)
   {
      NF_SHARE_PTR<NFNode> node = it.second;
      for (auto attri : node->mAttris)
      {
         if (attri->guid == attriId)
         {
            return node->guid;
         }
      }
   }

   return NFGUID();
}

void NFNodeView::ResetOffest(const NFVector2& pos)
{
   EditorContextSet((imnodes::EditorContext*)m_pEditorContext);
   imnodes::EditorContextResetPanning(ImVec2(pos.X(), pos.Y()));
}

void NFNodeView::CheckNewLinkStatus()
{
   int start_attr, end_attr;
   if (imnodes::IsLinkCreated(&start_attr, &end_attr))
   {
      NFGUID startID = GetAttriGUID(start_attr);
      NFGUID endID = GetAttriGUID(end_attr);
      if (GetNodeByAttriId(startID) != GetNodeByAttriId(endID))
      {
         mLinks.push_back(NF_SHARE_PTR<NFNodeLink>(NF_NEW NFNodeLink(startID, endID, start_attr, end_attr)));
      }
   }
}

const NFGUID NFNodeView::GetNodeGUID(const int nodeId)
{
   for (auto it : mNodes)
   {
      if (it.second->id == nodeId)
      {
         return it.second->guid;
      }
   }

   return NFGUID();
}

const int NFNodeView::GetNodeID(const NFGUID guid)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         return it.second->id;
      }
   }

   return -1;
}

const NFGUID NFNodeView::GetAttriGUID(const int attriId)
{
   for (auto it : mNodes)
   {
      NF_SHARE_PTR<NFNode> node = it.second;
      for (auto attri : node->mAttris)
      {
         if (attri->id == attriId)
         {
            return node->guid;
         }
      }
   }

   return NFGUID();
}

const int NFNodeView::GetAttriID(const NFGUID guid)
{
   for (auto it : mNodes)
   {
      NF_SHARE_PTR<NFNode> node = it.second;
      for (auto attri : node->mAttris)
      {
         if (attri->guid == guid)
         {
            return node->id;
         }
      }
   }

   return -1;
}