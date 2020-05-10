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

#include "NFNodeSystem.h"
#include "gl3w/GL/glcorearb.h"
#include "gl3w/GL/gl3w.h"
#include "imgui/imgui.h"

namespace NodeSystem
{
    void NFNode::Execute(const NFVector2& offset)
    {
        ImVec2 disPlayPos(pos.X() + offset.X(), pos.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddCircleFilled(disPlayPos,5, color);

        ImGui::GetWindowDrawList()->AddText(ImVec2(disPlayPos.x - this->size, disPlayPos.y - this->size), ImColor(200, 0, 0, 255), name.c_str());
        if (selected)
        {
			ImGui::GetWindowDrawList()->AddCircle(disPlayPos, 6, ImColor(255, 255, 0, 255));
        }
    }

    NFNodeSystem::NFNodeSystem()
    {
    }

    NFNodeSystem::~NFNodeSystem()
    {
    }

    void NFNodeSystem::Execute()
    {
        ImGui::BeginGroup();

        ImGuiIO& io = ImGui::GetIO();
        if (!init)
        {
            init = true;

            ImVec2 backSize = ImGui::GetWindowSize();
            offset = NFVector2(backSize.x / 2, io.DisplaySize.y - backSize.y / 2);
        }

        //process offset when moving mouse
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(1))
        {
            this->offset.SetX(this->offset.X() + io.MouseDelta.x);
            this->offset.SetY(this->offset.Y() + io.MouseDelta.y);
        }

        //draw grid back ground
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(), io.DisplaySize, ImColor(40, 40, 50, 200));

        //draw grid line
        for (int i = 0 + ((int)this->offset.X() % this->nodeSize); i < io.DisplaySize.x; i += this->nodeSize)
        {
            ImGui::GetWindowDrawList()->AddLine(ImVec2(i, 0), ImVec2(i, io.DisplaySize.y), ImColor(200, 200, 200, 20));
        }

        for (int j = 0 + ((int)this->offset.Y() % this->nodeSize); j < io.DisplaySize.y; j += this->nodeSize)
        {
            ImGui::GetWindowDrawList()->AddLine(ImVec2(0, j), ImVec2(io.DisplaySize.x, j), ImColor(200, 200, 200, 20));
        }

        if (mBeginRenderFunctor)
        {
            mBeginRenderFunctor();
        }

        //draw objects
        for (auto item : mNodes)
        {
            item.second->Execute(this->offset);
        }

        ImGui::EndGroup();
    }

    void NFNodeSystem::SetBeginRenderCallBack(std::function<void()> functor)
    {
        mBeginRenderFunctor = functor;
    }

    void NFNodeSystem::CleanNodes()
    {
        mNodes.clear();
    }

    std::shared_ptr<NFNode> NFNodeSystem::AddNode(const NFGUID nodeId, const std::string& name, const NFVector2 vec, const int color)
    {
        if (mNodes.find(nodeId) == mNodes.end())
        {
            auto node = std::shared_ptr<NFNode>(new NFNode(nodeId, nodeSize, name, vec, color));
            mNodes.insert(std::make_pair(nodeId, node));

            return node;
        }

        return nullptr;
    }

    void NFNodeSystem::DeleteNode(const NFGUID nodeId)
    {
        auto it = mNodes.find(nodeId);
        if (it != mNodes.end())
        {
            mNodes.erase(it);
        }
    }

    NF_SHARE_PTR<NFNode> NFNodeSystem::FindNode(const const NFGUID nodeId)
    {
        auto it = mNodes.find(nodeId);
        if (it != mNodes.end())
        {
            return it->second;
        }

        return nullptr;
    }

    void NFNodeSystem::ResetToCenter()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 backSize = ImGui::GetWindowSize();
        this->offset = NFVector2(backSize.x / 2, io.DisplaySize.y - backSize.y / 2);
    }

    void NFNodeSystem::MoveToNode(const NFGUID nodeId)
    {
        auto itNew = mNodes.find(nodeId);
        if (itNew != mNodes.end())
        {
            ResetToCenter();
            this->offset -= itNew->second->pos;
        }
    }

    void NFNodeSystem::SelectNode(const NFGUID nodeId)
    {
        auto it = mNodes.find(selectedObject);
        if (it != mNodes.end())
        {
            it->second->selected = false;
        }

        auto itNew = mNodes.find(nodeId);
        if (itNew != mNodes.end())
        {
            selectedObject = nodeId;
            itNew->second->selected = true;
        }
    }

    void NFNodeSystem::SetBackGroundColor(const int color)
    {
        this->backgroundColor = color;
    }

    void NFNodeSystem::SetNodeSize(const int size)
    {
        this->nodeSize = nodeSize;
    }

    int NFNodeSystem::GetNodeSize()
    {
        return this->nodeSize;
    }

    NFVector2 NFNodeSystem::GetOffset()
    {
        return offset;
    }

	void NFNodeSystem::DrawRect(const NFVector2& p_min, const NFVector2& p_max, int col)
	{
        ImVec2 disPlayPos(p_min.X() + offset.X(), p_min.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddRect(disPlayPos, ImVec2(offset.X() + p_max.X(), offset.Y() + p_max.Y()), col);
	}

	void NFNodeSystem::DrawRectFilled(const NFVector2& p_min, const NFVector2& p_max, int col)
	{
        ImVec2 disPlayPos(p_min.X() + offset.X(), p_min.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddRectFilled(disPlayPos, ImVec2(offset.X() + p_max.X(), offset.Y() + p_max.Y()), col);
	}

	void NFNodeSystem::DrawText(const NFVector2& pos, int col, const std::string& text)
	{
        ImVec2 disPlayPos(pos.X() + offset.X(), pos.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddText(disPlayPos, col, text.c_str());
	}

    void NFNodeSystem::DrawCircle(const NFVector2& center, const float radius, const int col)
    {
        ImVec2 disPlayPos(center.X() + offset.X(), center.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddCircle(disPlayPos, radius, col);
    }

    void NFNodeSystem::DrawCircleFilled(const NFVector2& center, const float radius, int const col)
    {
        ImVec2 disPlayPos(center.X() + offset.X(), center.Y() + offset.Y());
        ImGui::GetWindowDrawList()->AddCircleFilled(disPlayPos, radius, col);

    }

    void NFNodeSystem::DrawArrow(const NFVector2& start, const NFVector2& end, int col)
    {
        ImVec2 disPlayPos(start.X() + offset.X(), start.Y() + offset.Y() - 2);

        //ImGui::GetWindowDrawList()->AddRectFilled(disPlayPos, ImVec2(disPlayPos.x + end.X(), disPlayPos.x + end.Y() + 2), col);
        //ImGui::GetWindowDrawList()->AddCircle(ImVec2(disPlayPos.x + end.X() * 0.75, disPlayPos.x + end.Y() * 0.75), nodeSize / 4, col);
    }
}