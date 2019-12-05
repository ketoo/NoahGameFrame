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
#ifndef NF_TREE_VIEW_H
#define NF_TREE_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"

class NFLeafNode
{
private:
	NFLeafNode(){}

public:
	NFLeafNode(const int id, const std::string& name, const NFGUID guid)
   {
      this->id = id;
      this->name = name;
      this->guid = guid;
   }

   void Execute();

   int id;
   std::string name;
   NFGUID guid;
};

class NFTreeNode
{
private:
	NFTreeNode(){}

public:
	NFTreeNode(const int id, const std::string& name, const NFGUID guid);

	void Execute();

   void AddLeaf(const int id, const std::string& name, const NFGUID guid)
   {
	  if (mLeaves.find(guid) == mLeaves.end())
	  {
		  mLeaves.insert(std::pair<NFGUID, NF_SHARE_PTR<NFLeafNode>>(guid, NF_SHARE_PTR<NFLeafNode>(NF_NEW NFLeafNode(id, name, guid))));
	  }
   }

   void DeleteLeaf(const NFGUID guid)
   {
	   auto it = mLeaves.find(guid);
	   if (it != mLeaves.end())
	   {
		   mLeaves.erase(it);
	   }
   }

   int base_flags = 0;
   bool selected = false;

   std::string name;
   int id;
   NFGUID guid;

   std::map<NFGUID, NF_SHARE_PTR<NFTreeNode>> mSubTrees;
   std::map<NFGUID, NF_SHARE_PTR<NFLeafNode>> mLeaves;
};

class NFTreeView : public NFIView
{
public:
	NFTreeView(NFIPluginManager* p);
	~NFTreeView();

	void SetName(const std::string& name);

	void AddTreeNode(const NFGUID guid, const std::string& name);
	void DeleteTreNode(const NFGUID guid);

	void AddSubTreeNode(const NFGUID guid, const NFGUID subId, const std::string& name);
	void DelSubTreeNode(const NFGUID guid, const NFGUID subId);

	void AddTreeLeafNode(const NFGUID guid, const NFGUID leafId, const std::string& name);
	void DeleteTreeLeafNode(const NFGUID guid, const NFGUID leafId);

	NFGUID GetTreeNodeByLeaf(const NFGUID leafId);

	virtual bool Execute();

private:
	std::string mstrName;
   NFIUIModule* m_pUIModule;
   std::map<NFGUID, NF_SHARE_PTR<NFTreeNode>> mTrees;
};

#endif