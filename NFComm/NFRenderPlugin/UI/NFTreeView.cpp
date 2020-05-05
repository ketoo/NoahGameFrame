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
#include "NFTreeView.h"

void NFLeafNode::Execute()
{
	bool selected = false;
	if (mTreeView->GetSelectedNode() == guid)
	{
		//selected = true;
	}

	ImGui::Checkbox("", &selected);
	ImGui::SameLine();
	ImGui::Bullet();
	ImGui::SameLine();

	if (ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			mTreeView->SetSelectedNode(guid, true);
		}
	}
}

NFTreeNode::NFTreeNode(NFTreeView* treeView, const int id, const std::string& name, const NFGUID guid)
{
	this->id = id;
	this->name = name;
	this->guid = guid;
	this->mTreeView = treeView;
}

void NFTreeNode::Execute()
{
	//base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;

	bool selected = false;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;// ImGuiTreeNodeFlags_OpenOnArrow;
	if (mTreeView->GetSelectedNode() == guid)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
		selected = true;
	}

	ImGui::Checkbox("", &selected);
	ImGui::SameLine();
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		mTreeView->SetSelectedNode(guid, true);
	}
	else
	{
		if (ImGui::IsItemClicked())
		{
			mTreeView->SetSelectedNode(guid, false);
		}
	}

	if (node_open)
	{
		auto tree = mSubTrees.First();
		while (tree)
		{
			tree->Execute();
			tree = mSubTrees.Next();
		}
		auto leaf = mLeaves.First();
		while (leaf)
		{
			leaf->Execute();
			leaf = mLeaves.Next();
		}

		ImGui::TreePop();
	}
}

bool NFTreeNode::AddTreeNode(const NFGUID guid, const std::string& name)
{
	if (!mSubTrees.ExistElement(guid))
	{
		return mSubTrees.AddElement(guid, NF_SHARE_PTR<NFTreeNode>(NF_NEW NFTreeNode(this->mTreeView, NFIView::GenerateNodeId(), name, guid)));
	}

	return false;
}

NF_SHARE_PTR<NFTreeNode> NFTreeNode::FindTreeNode(const NFGUID guid)
{
	auto node = mSubTrees.GetElement(guid);
	if (node)
	{
		return node;
	}

	node = mSubTrees.First();
	while (node)
	{
		auto p = node->FindTreeNode(guid);
		if (p)
		{
			return p;
		}

		node = mSubTrees.Next();
	}

	return nullptr;
}

bool NFTreeNode::DeleteTreeNode(const NFGUID guid)
{
	if (mSubTrees.ExistElement(guid))
	{
		return mSubTrees.RemoveElement(guid);
	}

	auto node = mSubTrees.First();
	while (node)
	{
		bool b = node->DeleteTreeNode(guid);
		if (b)
		{
			return b;
		}

		node = mSubTrees.Next();
	}

	return false;
}

NFTreeView::NFTreeView()
{
}

NFTreeView::~NFTreeView()
{
	mTrees.clear();
}

void NFTreeView::SetName(const std::string& name)
{
	this->name = name;
}

void NFTreeView::SetSelectedNode(const NFGUID& nodeId, bool doubleClick)
{
	mSelectedNode = nodeId;
	if (mSelectedFuntor)
	{
		mSelectedFuntor(mSelectedNode, doubleClick);
	}
}

void NFTreeView::SetSelectedNodeFunctor(std::function<void(const NFGUID&, const bool)> functor)
{
	mSelectedFuntor = functor;
}

const NFGUID NFTreeView::GetSelectedNode() const
{
	return mSelectedNode;
}

void NFTreeView::AddTreeNode(const NFGUID guid, const std::string& name)
{
	if (mTrees.find(guid) == mTrees.end())
	{
		mTrees.insert(std::make_pair(guid, NF_SHARE_PTR<NFTreeNode>(NF_NEW NFTreeNode(this, NFIView::GenerateNodeId(), name, guid))));
	}
}

NF_SHARE_PTR<NFTreeNode> NFTreeView::FindTreeNode(const NFGUID guid)
{
	auto it = mTrees.find(guid);
	if (it != mTrees.end())
	{
		return it->second;
	}

	for (auto tree : mTrees)
	{
		auto subNode = tree.second->FindTreeNode(guid);
		if (subNode)
		{
			return subNode;
		}
	}

	return nullptr;
}

bool NFTreeView::DeleteTreeNode(const NFGUID guid)
{
	auto it = mTrees.find(guid);
	if (it != mTrees.end())
	{
		mTrees.erase(it);
		return true;
	}

	for (auto tree : mTrees)
	{
		tree.second->DeleteTreeNode(guid);
	}

	return true;
}

void NFTreeView::AddSubTreeNode(const NFGUID guid, const NFGUID subId, const std::string& name)
{
	auto node = FindTreeNode(guid);
	if (node)
	{
		node->AddTreeNode(subId, name);
	}
}

void NFTreeView::AddTreeLeafNode(const NFGUID guid, const NFGUID leafId, const std::string& name)
{
	auto node = FindTreeNode(guid);
	if (node)
	{
		node->AddLeaf(NFIView::GeneratePinId(), name, leafId);
	}
}

void NFTreeView::DeleteTreeLeafNode(const NFGUID leafId)
{
	for (auto it : mTrees)
	{
		it.second->DeleteLeaf(leafId);
	}
}

bool NFTreeView::Execute()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(name.c_str(), node_flags))
	{
		for (auto it : mTrees)
		{
			it.second->Execute();
		}

		ImGui::TreePop();
	}

	return false;
}


void NFTreeView::Clear()
{
	mTrees.clear();
}