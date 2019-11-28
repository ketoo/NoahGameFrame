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
   NFNodeAttri(const int id, const std::string& name, const bool inputPin, const NFGUID guid)
   {
      this->id = id;
      this->name = name;
      this->inputPin = inputPin;
      this->guid = guid;
   }

   void Execute();

   int id;
   bool inputPin;
   std::string name;
   NFGUID guid;
};

class NFNode
{
private:
   NFNode(){}

public:
	NFNode(const int id, const std::string& name, const NFGUID guid, const NFVector2 vec)
   {
      this->id = id;
      this->name = name;
      this->guid = guid;
      this->initPos = vec;
   }

	void Execute();

   void AddAttribute(const int id, const std::string& name, const bool inputPin, const NFGUID guid)
   {
      auto ptr = NF_SHARE_PTR<NFNodeAttri>(NF_NEW NFNodeAttri(id, name, inputPin, guid));
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

   void DeleteAttribute(const NFGUID guid)
   {
      for (auto it = mAttris.begin(); it != mAttris.end(); ++it)
      {
         if ((*it)->guid == guid)
         {
            mAttris.erase(it);
            return;
         }
      }
   }

   std::string name;
   int id;
   NFGUID guid;
   NFVector2 initPos;

   std::list<NF_SHARE_PTR<NFNodeAttri>> mAttris;

private:
   bool first = true;
};

class NFNodeLink
{
private:
   NFNodeLink(){}

public:
   NFNodeLink(NFGUID startID, NFGUID endID, int start_attr, int end_attr)
   {
      this->start = startID;
      this->end = endID;

      this->start_attr = start_attr;
      this->end_attr = end_attr;
   }

   int start_attr;
   int end_attr;

   NFGUID start;
   NFGUID end;
};

class NFNodeView : public NFIView
{
public:
	NFNodeView(NFIPluginManager* p);
   ~NFNodeView();

	virtual bool Execute();

   void CleanNodes();

   //////////////////////////////////////
   const NFGUID GetNodeGUID(const int nodeId);
   const int GetNodeID(const NFGUID guid);

   const NFGUID GetAttriGUID(const int attriId);
   const int GetAttriID(const NFGUID guid);


   int GenerateId();

   void AddNode(const NFGUID guid, const std::string& name, const NFVector2 vec = NFVector2());
   void AddNodeAttrIn(const NFGUID guid, const NFGUID attrId, const std::string& name);
   void AddNodeAttrOut(const NFGUID guid, const NFGUID attrId, const std::string& name);
   void DeleteNode(const NFGUID guid);

   NFGUID GetNodeByAttriId(const NFGUID attriId);
   void ResetOffest(const NFVector2& pos);
   
private:
   void RenderNodes();
   void RenderLinks();
   
   void CheckNewLinkStatus();

private:
   NFIUIModule* m_pUIModule;
   void* m_pEditorContext;

   std::map<NFGUID, NF_SHARE_PTR<NFNode>> mNodes;
   std::list<NF_SHARE_PTR<NFNodeLink>> mLinks;
};

#endif