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

      //ImGui::PushItemWidth(160);
      ImGui::Text(" ");
      this->nodeView->RenderAttriPin(this);

      imnodes::EndAttribute();
   }
   else
   {
      imnodes::BeginOutputAttribute(id);

      //ImGui::SameLine(ImGui::GetWindowWidth() - 30);
      ImGui::Text(" ");
      this->nodeView->RenderAttriPin(this);

      imnodes::EndAttribute();
   }

   ImGui::Separator();
}

void NFNode::Execute()
{
   imnodes::SetNodeName(id, name.c_str());
   //ImGui::PushItemWidth(200);
   imnodes::BeginNode(id);

   if (first)
   {
      first = false;
      imnodes::SetNodeOriginPos(id, ImVec2(initPos.X(), initPos.Y()));
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

bool NFNodeView::Execute()
{
	//1. the project root folder is NFDataCfg
   EditorContextSet((imnodes::EditorContext*)m_pEditorContext);

   imnodes::BeginNodeEditor();

   RenderNodes();
   RenderLinks();

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

void NFNodeView::SetUpNewLinkCallBack(std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> functor)
{
    mTryNewLinkFunctor = functor;
}

void NFNodeView::SetUpNodeAttriRenderCallBack(std::function<void(NFNodeAttri*)> functor)
{
    mNodeAttriRenderFunctor = functor;
}

void NFNodeView::RenderAttriPin(NFNodeAttri* nodeAttri)
{
    if (mNodeAttriRenderFunctor)
    {
        mNodeAttriRenderFunctor(nodeAttri);
    }
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
   for (auto it : mLinks)
   {
      int start = GetAttriID(it->startAttr);
      int end = GetAttriID(it->endAttr);
      
      imnodes::Link(it->index, start, end);
   }
}

void NFNodeView::AddNode(const NFGUID guid, const std::string& name, const NFVector2 vec)
{
   if (mNodes.find(guid) == mNodes.end())
   {
       auto node = NF_SHARE_PTR<NFNode>(NF_NEW NFNode(GenerateNodeId(), name, guid, vec));
       node->nodeView = this;
       mNodes.insert(std::pair<NFGUID, NF_SHARE_PTR<NFNode>>(guid, node));
   }
}

void NFNodeView::AddNodeAttrIn(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& desc)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddAttribute(GeneratePinId(), name, true, attrId, desc);
         return;
      }
   }
}

void NFNodeView::AddNodeAttrOut(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& desc)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddAttribute(GeneratePinId(), name, false, attrId, desc);
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

void NFNodeView::AddLink(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
    if (GetLink(startNode, endNode, startPin, endPin))
    {
        return;
    }

    auto link = NF_SHARE_PTR<NFDataLink>(NF_NEW NFDataLink(startNode, endNode, startPin, endPin, GenerateLinkId()));
    mLinks.push_back(link);
}

NF_SHARE_PTR<NFDataLink> NFNodeView::GetLink(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
    for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
    {
        if ((*it)->startNode == startNode
            && (*it)->endNode == endNode
            && (*it)->startAttr == startPin
            && (*it)->endAttr == endPin)
        {
            return *it;
        }
    }

    return nullptr;
}

void NFNodeView::DeleteLink(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
    for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
    {
        if ((*it)->startNode == startNode
            && (*it)->endNode == endNode
            && (*it)->startAttr == startPin
            && (*it)->endAttr == endPin)
        {
            mLinks.erase(it);
            return;
        }
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

void NFNodeView::SetNodeDraggable(const NFGUID guid, const bool dragable)
{
    if (mNodes.find(guid) != mNodes.end())
    {
        int id = GetNodeID(guid);
        if (id >= 0)
        {
            EditorContextSet((imnodes::EditorContext*)m_pEditorContext);
            imnodes::SetNodeDraggable(id, dragable);
        }
    }
}

void NFNodeView::SetNodePosition(const NFGUID guid, const NFVector2 vec)
{
    if (mNodes.find(guid) != mNodes.end())
    {
        int id = GetNodeID(guid);
        if (id >= 0)
        {
            EditorContextSet((imnodes::EditorContext*)m_pEditorContext);
            imnodes::SetNodeOriginPos(id, ImVec2(vec.X(), vec.Y()));
        }
    }
}

void NFNodeView::ResetOffest(const NFVector2& pos)
{
   EditorContextSet((imnodes::EditorContext*)m_pEditorContext);
   imnodes::EditorContextResetPanning(ImVec2(pos.X(), pos.Y()));
}

void NFNodeView::MoveToNode(const NFGUID guid)
{
    int id = GetNodeID(guid);
    if (id >= 0)
    {
        EditorContextSet((imnodes::EditorContext*)m_pEditorContext);
        imnodes::EditorContextMoveToNode(id);
    }
}

void NFNodeView::CheckNewLinkStatus()
{
   int start_attr, end_attr;
   if (imnodes::IsLinkCreated(&start_attr, &end_attr))
   {
      NFGUID startID = GetAttriGUID(start_attr);
      NFGUID endID = GetAttriGUID(end_attr);
      NFGUID startNodeID = GetNodeByAttriId(startID);
      NFGUID endEndID = GetNodeByAttriId(endID);
      if (startNodeID == endEndID || startNodeID.IsNull() || endEndID.IsNull())
      {
          //debug error
          return;
      }

      if (mTryNewLinkFunctor)
      {
          mTryNewLinkFunctor(startNodeID, endEndID, startID, endID);
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
            return attri->guid;
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
            return attri->id;
         }
      }
   }

   return -1;
}