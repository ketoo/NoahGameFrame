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
#ifndef NF_NODE_VIEW_H
#define NF_NODE_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"

class NFNodeAttri
{
private:
   NFNodeAttri(){}

public:
   NFNodeAttri(const int id, const std::string& name, const bool inputPin)
   {
      this->id = id;
      this->name = name;
      this->inputPin = inputPin;
   }

   void Execute();

   int id;
   bool inputPin;
   std::string name;
};

class NFNode
{
private:
   NFNode(){}

public:
	NFNode(const int id, const std::string& name)
   {
      this->id = id;
      this->name = name;
   }

	void Execute();

   void AddAttribute(const int id, const std::string& name, const bool inputPin)
   {
      auto ptr = NF_SHARE_PTR<NFNodeAttri>(NF_NEW NFNodeAttri(id, name, inputPin));
      mAttris.push_back(ptr);
   }

   void DeleteAttribute(const int id)
   {
      for (auto it = mAttris.begin(); it != mAttris.end(); ++it)
      {
         if ((*it)->id == id)
         {
            mAttris.erase(it);
            return;
         }
      }
   }

   std::string name;
   int id;

   std::list<NF_SHARE_PTR<NFNodeAttri>> mAttris;
};

class NFNodeLink
{
private:
   NFNodeLink(){}

public:
   NFNodeLink(int start_attr, int end_attr)
   {
      start = start_attr;
      end = end_attr;

   }

   int start;
   int end;
};

class NFNodeView : public NFIView
{
public:
	NFNodeView(NFIPluginManager* p);

	virtual bool Execute();

   int GenerateId();

   void AddNode(const int nodeId, const std::string& name);
   void AddNodeAttrIn(const int nodeId, const int attrId, const std::string& name);
   void AddNodeAttrOut(const int nodeId, const int attrId, const std::string& name);
   void DeleteNode(const int nodeId);

   int GetNodeByAttriId(const int attriId);
   void ResetOffestZero();
   
private:
   void RenderNodes();
   void RenderLinks();

private:
   NFIUIModule* m_pUIModule;

   std::list<NF_SHARE_PTR<NFNode>> mNodes;
   std::list<NF_SHARE_PTR<NFNodeLink>> mLinks;
};

#endif