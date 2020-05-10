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

#include "NFNodeView.h"
#include "NFUIModule.h"
#define STB_IMAGE_IMPLEMENTATION
#include "imnodes/stb_image.h"

void BEGIN_EDITOR(const std::string& name)
{
    imnodes::BeginNodeEditor();
}

void END_EDITOR()
{
    imnodes::EndNodeEditor();
}

void BEGIN_INPUT_PIN(int id, NFPinShape shape)
{
    imnodes::BeginInputAttribute(id, (imnodes::PinShape)shape);
}

void END_INPUT_PIN()
{
    imnodes::EndAttribute();
}

void BEGIN_OUT_PIN(int id, NFPinShape shape)
{
    imnodes::BeginOutputAttribute(id, (imnodes::PinShape)shape);
}

void END_OUT_PIN()
{
    imnodes::EndAttribute();
}

void BEGIN_NODE(const int id, const bool title, const std::string& name, ImTextureID user_texture_id, const NFVector2& iconSize)
{
    imnodes::BeginNode(id);

    if (title)
    {
        imnodes::BeginNodeTitleBar();
        if (user_texture_id != NULL)
        {
            ImGui::Image((void*)(intptr_t)user_texture_id, ImVec2(iconSize.X(), iconSize.Y()));
            ImGui::SameLine();
        }

        ImGui::TextUnformatted(name.c_str());
        imnodes::EndNodeTitleBar();
    }
}

void END_NODE()
{
    imnodes::EndNode();
}

void NODE_LINK(const int linkId, const int startPinId, const int endPinId)
{
    imnodes::Link(linkId, startPinId, endPinId);
}

void SET_NODE_DRAGGABLE(const int id, const bool draggable)
{
    imnodes::SetNodeDraggable(id, draggable);
}

void SET_NODE_SCREEN_POSITION(const int id, const ImVec2 pos)
{
    imnodes::SetNodeScreenSpacePos(id, pos);
}

void SET_NODE_POSITION(const int id, const ImVec2 pos)
{
    imnodes::SetNodeGridSpacePos(id, pos);
}

void FOCUS_ON_NODE(int id)
{
    imnodes::EditorContextMoveToNode(id);
}

bool IS_LINK_CREATED(int* const started_at, int* const ended_at)
{
    return imnodes::IsLinkCreated(started_at, ended_at);
}

void IS_LINK_HOVERED(int* const link)
{
    imnodes::IsLinkHovered(link);
}

void IS_NODE_HOVERED(int* const link)
{
    imnodes::IsNodeHovered(link);
}

void SET_CURRENT_CONTEXT(void* p)
{
    EditorContextSet((imnodes::EditorContext*)p);
}

void PUSH_COLOR(int style, int color)
{
    imnodes::PushColorStyle((imnodes::ColorStyle)style, color);
}

void POP_COLOR()
{
    imnodes::PopColorStyle();
}
//////////////////////////


bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void* ShowImage(const char* filename, int width, int height)
{
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile(filename, &my_image_texture, &my_image_width, &my_image_height);
    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(width, height));

    return (void*)(intptr_t)my_image_texture;
}

void NFNodePin::Execute()
{
	if (this->iconTextureId == nullptr && !this->image.empty())
	{
		int my_image_width = 0;
		int my_image_height = 0;
		GLuint my_image_texture = 0;
		bool ret = LoadTextureFromFile(this->image.c_str(), &my_image_texture, &my_image_width, &my_image_height);

		this->iconTextureId = (void*)(intptr_t)my_image_texture;
	}
	else
	{
		if (!this->newImage.empty() && this->newImage != this->image)
		{
			this->image = this->newImage;
			this->newImage = "";

			int my_image_width = 0;
			int my_image_height = 0;
			GLuint my_image_texture = 0;
			bool ret = LoadTextureFromFile(this->image.c_str(), &my_image_texture, &my_image_width, &my_image_height);

			this->iconTextureId = (void*)(intptr_t)my_image_texture;
		}
	}


   if (this->inputPin)
   {
        PUSH_COLOR(imnodes::ColorStyle::ColorStyle_Pin, color);
		if (shape != NFPinShape::PinShape_NONE)
		{
			BEGIN_INPUT_PIN(id, shape);
		}

        POP_COLOR();

	   //ImGui::Text(this->name.c_str());

        //ShowImage(this->image.c_str(), 20, 20);
		ImGui::Image(this->iconTextureId, ImVec2(this->imageSize.X(), this->imageSize.Y()));

        ImGui::SameLine();
        ImGui::PushItemWidth(60);
        ImGui::Text(this->name.c_str());
        ImGui::PopItemWidth();


        this->nodeView->RenderForPin(this);
	   if (shape != NFPinShape::PinShape_NONE)
	   {
		   END_INPUT_PIN();
	   }
   }
   else
   {
        PUSH_COLOR(imnodes::ColorStyle::ColorStyle_Pin, color);

        BEGIN_OUT_PIN(id, shape);

        POP_COLOR();

        //ImGui::Text(str.c_str());
        //ImGui::SameLine();
        //ImGui::SetAlignment(ImGui_Alignment_Right);

        ImGui::Indent(100);
        ImGui::PushItemWidth(60);
        ImGui::Text(this->name.c_str());
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::Indent(70);


	   ImGui::Image(this->iconTextureId, ImVec2(this->imageSize.X(), this->imageSize.Y()));
        //this->iconTextureId = ShowImage(this->image.c_str(), 20, 20);

        ImGui::SameLine();

        this->nodeView->RenderForPin(this);

        END_OUT_PIN();
   }
}

void NFNodePin::UpdateShape()
{
    if (linkId.IsNull())
    {
        switch (shape)
        {
            case NFPinShape::PinShape_CircleFilled:
                shape = NFPinShape::PinShape_Circle;
                break;
            case NFPinShape::PinShape_TriangleFilled:
                shape = NFPinShape::PinShape_Triangle;
                break;
            case NFPinShape::PinShape_QuadFilled:
                shape = NFPinShape::PinShape_Quad;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (shape)
        {
            case NFPinShape::PinShape_Circle:
                shape = NFPinShape::PinShape_CircleFilled;
                break;
            case NFPinShape::PinShape_Triangle:
                shape = NFPinShape::PinShape_TriangleFilled;
                break;
            case NFPinShape::PinShape_Quad:
                shape = NFPinShape::PinShape_QuadFilled;
                break;
            default:
                break;
        }
    }
}

void NFNode::Execute()
{
   //ImGui::PushItemWidth(200);
	if (this->iconTextureId == nullptr
		&& !this->iconPath.empty())
	{
		int my_image_width = 0;
		int my_image_height = 0;
		GLuint my_image_texture = 0;
		bool ret = LoadTextureFromFile(this->iconPath.c_str(), &my_image_texture, &my_image_width, &my_image_height);

		this->iconTextureId = (void*)(intptr_t)my_image_texture;
	}

    if (title)
    {
        PUSH_COLOR(imnodes::ColorStyle::ColorStyle_TitleBar, color);
    }
    else
    {
        PUSH_COLOR(imnodes::ColorStyle::ColorStyle_NodeBackground, color);
    }

    PUSH_COLOR(imnodes::ColorStyle::ColorStyle_TitleBarHovered, color + 1000);
    PUSH_COLOR(imnodes::ColorStyle::ColorStyle_TitleBarSelected, color + 1000);

    BEGIN_NODE(id, title, name, this->iconTextureId, this->iconSize);

    POP_COLOR();
    POP_COLOR();
    POP_COLOR();

   if (first)
   {
        first = false;
        if (pos.IsZero())
        {
            SET_NODE_SCREEN_POSITION(id, ImVec2(400, 300));
        }
        else
        {
            SET_NODE_POSITION(id, ImVec2(pos.X(), pos.Y()));
        }
        
   }

   this->nodeView->NodeRenderBeforePinIn(this);

   for (auto it : mAttris)
   {
       if (it->inputPin)
       {
           it->Execute();
       }
   }

   this->nodeView->NodeRenderAfterPinIn(this);

   if (mAttris.size() > 0)
   {
       ImGui::Button("", ImVec2(180, 1));
   }

   this->nodeView->NodeRenderBeforePinOut(this);

   for (auto it : mAttris)
   {
       if (!it->inputPin)
       {
           it->Execute();
       }
   }

   this->nodeView->NodeRenderAfterPinOut(this);

   END_NODE();
}

/////////////////

NFNodeView::NFNodeView()
{
    m_pEditorContext = imnodes::EditorContextCreate();
    imnodes::Style& nodeStyle = imnodes::GetStyle();
    nodeStyle.pin_offset = 10.0f;
}

NFNodeView::~NFNodeView()
{
   imnodes::EditorContextFree((imnodes::EditorContext*)m_pEditorContext);

   m_pEditorContext = nullptr;
}

bool NFNodeView::Execute()
{
	//1. the project root folder is NFDataCfg
    SET_CURRENT_CONTEXT(m_pEditorContext);

    BEGIN_EDITOR("My Editor");

    if (mBeginRenderFunctor)
    {
        mBeginRenderFunctor();
    }

    RenderNodes();
    RenderLinks();

    END_EDITOR();

    CheckNewLinkStatus();
    CheckDeleteLinkStatus();
    CheckHoverNodeStatus();

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

void NFNodeView::SetUpDeleteLinkCallBack(std::function<bool(const NFGUID&)> functor)
{
    mTryDeleteLinkFunctor = functor;
}

void NFNodeView::SetHoverNodeCallBack(std::function<bool(const NFGUID&)> functor)
{
    mHoverNodeFunctor = functor;
}

void NFNodeView::SetBeginRenderCallBack(std::function<void()> functor)
{
    mBeginRenderFunctor = functor;
}

void NFNodeView::SetPinRenderCallBack(std::function<void(NFNodePin*)> functor)
{
    mPinRenderFunctor = functor;
}

void NFNodeView::SetNodeRenderBeforePinInCallBack(std::function<void(NFNode*)> functor)
{
    mNodeRenderBeforePinInFunctor = functor;
}

void NFNodeView::SetNodeRenderAfterPinInCallBack(std::function<void(NFNode*)> functor)
{
    mNodeRenderAfterPinInFunctor = functor;
}

void NFNodeView::SetNodeRenderBeforePinOutCallBack(std::function<void(NFNode*)> functor)
{
    mNodeRenderBeforePinOutFunctor = functor;
}

void NFNodeView::SetNodeRenderAfterPinOutCallBack(std::function<void(NFNode*)> functor)
{
    mNodeRenderAfterPinOutFunctor = functor;
}

void NFNodeView::RenderForPin(NFNodePin* nodeAttri)
{
    if (mPinRenderFunctor)
    {
        mPinRenderFunctor(nodeAttri);
    }
}

void NFNodeView::NodeRenderBeforePinIn(NFNode* node)
{
    if (mNodeRenderBeforePinInFunctor)
    {
        mNodeRenderBeforePinInFunctor(node);
    }
}

void NFNodeView::NodeRenderAfterPinIn(NFNode* node)
{
    if (mNodeRenderAfterPinInFunctor)
    {
        mNodeRenderAfterPinInFunctor(node);
    }
}

void NFNodeView::NodeRenderBeforePinOut(NFNode* node)
{
    if (mNodeRenderBeforePinOutFunctor)
    {
        mNodeRenderBeforePinOutFunctor(node);
    }
}

void NFNodeView::NodeRenderAfterPinOut(NFNode* node)
{
    if (mNodeRenderAfterPinOutFunctor)
    {
        mNodeRenderAfterPinOutFunctor(node);
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

      PUSH_COLOR(imnodes::ColorStyle::ColorStyle_Link, it->color);

      NODE_LINK(it->index, start, end);

      POP_COLOR();
   }
}

NF_SHARE_PTR<NFNode> NFNodeView::AddNode(const NFGUID guid, const std::string& name, NFColor color, const NFVector2 vec)
{
   if (mNodes.find(guid) == mNodes.end())
   {
       auto node = NF_SHARE_PTR<NFNode>(NF_NEW NFNode(NFIView::GenerateNodeId(), name, guid, vec, color));
       node->nodeView = this;
       mNodes.insert(std::make_pair(guid, node));

       return node;
   }

   return nullptr;
}

void NFNodeView::AddPinIn(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& image, NFColor color, NFPinShape shape)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddPin(NFIView::GeneratePinId(), name, image, true, attrId, color, shape);
         return;
      }
   }
}

void NFNodeView::AddPinOut(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& image, NFColor color, NFPinShape shape)
{
   for (auto it : mNodes)
   {
      if (it.second->guid == guid)
      {
         it.second->AddPin(NFIView::GeneratePinId(), name, image, false, attrId, color, shape);
         return;
      }
   }
}

void NFNodeView::ModifyPinColor(const NFGUID attrId, NFColor color)
{
    NFGUID nodeID = GetNodeByAttriId(attrId);
    auto it = mNodes.find(nodeID);
    if (it != mNodes.end())
    {
        auto pin = it->second->GetPin(attrId);
        if (pin)
        {
            pin->color = color;
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

void NFNodeView::AddLink(const NFGUID& selfID, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin, const int color)
{
    if (GetLink(startNode, endNode, startPin, endPin))
    {
        return;
    }

    auto startNodeObject = this->FindNode(startNode);
    auto endNodeObject = this->FindNode(endNode);
    if (startNodeObject && endNodeObject)
    {
        auto startPinObject = startNodeObject->GetPin(startPin);
        auto endPinObject = endNodeObject->GetPin(endPin);
        if (startPinObject && endPinObject)
        {
            startPinObject->linkId = selfID;
            endPinObject->linkId = selfID;

            startPinObject->UpdateShape();
            endPinObject->UpdateShape();

            auto link = NF_SHARE_PTR<NFDataLink>(NF_NEW NFDataLink(selfID, startNode, endNode, startPin, endPin, NFIView::GenerateLinkId(), color));
            mLinks.push_back(link);
        }
    }
}

NF_SHARE_PTR<NFDataLink> NFNodeView::GetLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
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

NF_SHARE_PTR<NFDataLink> NFNodeView::GetLink(const NFGUID& id)
{
    for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
    {
        if ((*it)->selfID == id)
        {
            return *it;
        }
    }

    return nullptr;
}

void NFNodeView::DeleteLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
{
    for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
    {
        if ((*it)->startNode == startNode
            && (*it)->endNode == endNode
            && (*it)->startAttr == startPin
            && (*it)->endAttr == endPin)
        {
            mLinks.erase(it);
            break;
        }
    }

    auto startNodeObject = this->FindNode(startNode);
    auto endNodeObject = this->FindNode(endNode);
    if (startNodeObject && endNodeObject)
    {
        auto startPinObject = startNodeObject->GetPin(startPin);
        auto endPinObject = endNodeObject->GetPin(endPin);
        if (startPinObject && endPinObject)
        {
            startPinObject->linkId = NFGUID();
            endPinObject->linkId = NFGUID();

            startPinObject->UpdateShape();
            endPinObject->UpdateShape();
        }
    }
}

void NFNodeView::DeleteLink(const NFGUID& id)
{
    for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
    {
        auto linkObject = (*it);
        if (linkObject->selfID == id)
        {
            mLinks.erase(it);


            auto startNodeObject = this->FindNode(linkObject->startNode);
            auto endNodeObject = this->FindNode(linkObject->endNode);
            if (startNodeObject && endNodeObject)
            {
                auto startPinObject = startNodeObject->GetPin(linkObject->startAttr);
                auto endPinObject = startNodeObject->GetPin(linkObject->endAttr);
                if (startPinObject && endPinObject)
                {
                    startPinObject->linkId = NFGUID();
                    endPinObject->linkId = NFGUID();

                    startPinObject->UpdateShape();
                    endPinObject->UpdateShape();
                }
            }

            break;
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
            SET_CURRENT_CONTEXT(m_pEditorContext);
			SET_NODE_DRAGGABLE(id, dragable);
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
            SET_CURRENT_CONTEXT(m_pEditorContext);
            SET_NODE_POSITION(id, ImVec2(vec.X(), vec.Y()));
        }
    }
}

void NFNodeView::ResetOffset(const NFVector2& pos)
{
    SET_CURRENT_CONTEXT(m_pEditorContext);
   imnodes::EditorContextResetPanning(ImVec2(pos.X(), pos.Y()));
}

void NFNodeView::MoveToNode(const NFGUID guid)
{
    int id = GetNodeID(guid);
    if (id >= 0)
    {
        SET_CURRENT_CONTEXT(m_pEditorContext);

        FOCUS_ON_NODE(id);
    }
}

void NFNodeView::SetCurrentContext()
{
    SET_CURRENT_CONTEXT(m_pEditorContext);
}

void NFNodeView::CheckNewLinkStatus()
{
    SET_CURRENT_CONTEXT(m_pEditorContext);

   int start_attr, end_attr;
   if (IS_LINK_CREATED(&start_attr, &end_attr))
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

void NFNodeView::CheckDeleteLinkStatus()
{
    SET_CURRENT_CONTEXT(m_pEditorContext);

    int selectedLink = -1;
    IS_LINK_HOVERED(&selectedLink);
    if (selectedLink >= 0)
    {
        if (ImGui::IsMouseDoubleClicked(0))
        {
            for (auto it = mLinks.begin(); it != mLinks.end(); ++it)
            {
                auto link = *it;
                if (link->index == selectedLink)
                {
                    //try to delete this link
                    if (mTryDeleteLinkFunctor)
                    {
                        mTryDeleteLinkFunctor(link->selfID);
                    }

                    return;
                }
            }
        }
    }
}

void NFNodeView::CheckHoverNodeStatus()
{
    int selectNode = -1;
    IS_NODE_HOVERED(&selectNode);
    if (selectNode >= 0)
    {
        for (auto it = mNodes.begin(); it != mNodes.end(); ++it)
        {
            auto node = it->second;
            if (node->id == selectNode)
            {
                if (mHoverNodeFunctor)
                {
                    mHoverNodeFunctor(node->guid);
                }

                return;
            }
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

NF_SHARE_PTR<NFNode> NFNodeView::FindNode(const NFGUID guid)
{
    auto it = mNodes.find(guid);
    if (it != mNodes.end())
    {
        return it->second;
    }

    return nullptr;
}
