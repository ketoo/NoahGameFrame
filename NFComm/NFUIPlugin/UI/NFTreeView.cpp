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
#include "NFTreeView.h"

void NFLeafNode::Execute()
{
}

NFTreeNode::NFTreeNode(const int id, const std::string& name, const NFGUID guid)
{
	base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;
	this->id = id;
	this->name = name;
	this->guid = guid;
}

void NFTreeNode::Execute()
{
	// Items 0..2 are Tree Node
	base_flags = 0;
	if (selected)
	{
		base_flags = ImGuiTreeNodeFlags_Selected;
	}

	ImGui::Checkbox("", &selected);
	ImGui::SameLine();
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, base_flags, name.c_str());
	if (ImGui::IsItemClicked())
	{
		selected = true;
	}
	else
	{
		selected = false;
	}

	if (node_open)
	{
		/*
		for (auto it : mSubTrees)
		{
			it.second->Execute();
		}
		for (auto it : mLeaves)
		{
			it.second->Execute();
		}
		*/

		/*
		static int test_flag1;
		ImGui::CheckboxFlags("", (unsigned int*)&test_flag1, 0);
		ImGui::SameLine();
		ImGui::BulletText("Blah blah\nBlah Blah");
		*/

		ImGui::TreePop();
	}

}

NFTreeView::NFTreeView(NFIPluginManager* p) : NFIView(p, NFViewType::NONE, GET_CLASS_NAME(NFTreeView))
{
	m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
}

NFTreeView::~NFTreeView()
{
}

void NFTreeView::SetName(const std::string& name)
{
	mstrName = name;
}

void NFTreeView::AddTreeNode(const NFGUID guid, const std::string& name)
{
	if (mTrees.find(guid) == mTrees.end())
	{
		mTrees.insert(std::pair<NFGUID, NF_SHARE_PTR<NFTreeNode>>(guid, NF_SHARE_PTR<NFTreeNode>(NF_NEW NFTreeNode(GenerateId(), name, guid))));
	}
}

void NFTreeView::DeleteTreNode(const NFGUID guid)
{
	auto it = mTrees.find(guid);
	if (it != mTrees.end())
	{
		mTrees.erase(it);
	}
}

void NFTreeView::AddSubTreeNode(const NFGUID guid, const NFGUID subId, const std::string& name)
{
}

void NFTreeView::DelSubTreeNode(const NFGUID guid, const NFGUID subId)
{
}

void NFTreeView::AddTreeLeafNode(const NFGUID guid, const NFGUID leafId, const std::string& name)
{
}

void NFTreeView::DeleteTreeLeafNode(const NFGUID guid, const NFGUID leafId)
{
}

NFGUID NFTreeView::GetTreeNodeByLeaf(const NFGUID leafId)
{
	return NFGUID();
}

bool NFTreeView::Execute()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		for (auto it : mTrees)
		{
			it.second->Execute();
		}

		ImGui::TreePop();
	}

	return false;
}
