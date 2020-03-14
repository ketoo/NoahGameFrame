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
#ifndef NF_TREE_VIEW_H
#define NF_TREE_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"

class NFTreeView;

class NFLeafNode
{
private:
	NFLeafNode(){}

public:
	NFLeafNode(NFTreeView* treeView, const int id, const std::string& name, const NFGUID guid)
   {
      this->id = id;
      this->name = name;
      this->guid = guid;
	  this->mTreeView = treeView;
   }

   void Execute();

   int id;
   std::string name;
   NFGUID guid;
   NFTreeView* mTreeView;
};

class NFTreeNode
{
private:
	NFTreeNode(){}

public:
	NFTreeNode(NFTreeView* treeView, const int id, const std::string& name, const NFGUID guid);

	void Execute();

	bool AddTreeNode(const NFGUID guid, const std::string& name);
	NF_SHARE_PTR<NFTreeNode> FindTreeNode(const NFGUID guid);
	bool DeleteTreeNode(const NFGUID guid);

   void AddLeaf(const int id, const std::string& name, const NFGUID guid)
   {
	  if (!mLeaves.ExistElement(guid))
	  {
		  mLeaves.AddElement(guid, NF_SHARE_PTR<NFLeafNode>(NF_NEW NFLeafNode(mTreeView, id, name, guid)));
	  }
   }

   void DeleteLeaf(const NFGUID guid)
   {
	   mLeaves.RemoveElement(guid);
   }

   std::string name;
   int id;
   NFGUID guid;

   NFMapEx<NFGUID, NFTreeNode> mSubTrees;
   NFMapEx<NFGUID, NFLeafNode> mLeaves;

   NFTreeView* mTreeView;
};

class NFTreeView
{
public:
	NFTreeView();
	~NFTreeView();

	void SetName(const std::string& name);
	void SetSelectedNode(const NFGUID& nodeId, bool doubleClick);
	void SetSelectedNodeFunctor(std::function<void(const NFGUID&, const bool)> functor);
	const NFGUID GetSelectedNode() const;

	void AddTreeNode(const NFGUID guid, const std::string& name);
	NF_SHARE_PTR<NFTreeNode> FindTreeNode(const NFGUID guid);
	bool DeleteTreeNode(const NFGUID guid);

	void AddSubTreeNode(const NFGUID guid, const NFGUID subId, const std::string& name);
	void AddTreeLeafNode(const NFGUID guid, const NFGUID leafId, const std::string& name);
	void DeleteTreeLeafNode(const NFGUID leafId);

	virtual bool Execute();

	virtual void Clear();

private:
	//maybe tree node, maybe leaf node
	NFGUID mSelectedNode;
	std::string name;

	std::function<void(const NFGUID&, const bool)> mSelectedFuntor;

	std::map<NFGUID, NF_SHARE_PTR<NFTreeNode>> mTrees;
};

#endif