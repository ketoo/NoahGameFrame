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
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"

class NFNodeView;

class NFNodePin
{
private:
   NFNodePin(){}

public:
   NFNodePin(const int id, const std::string& name, const bool inputPin, const NFGUID guid)
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
   NFGUID nodeId;
   NFNodeView* nodeView;
   NF_SHARE_PTR<NFDataLink> link;
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

   void AddPin(const int id, const std::string& name, const bool inputPin, const NFGUID guid)
   {
      auto ptr = NF_SHARE_PTR<NFNodePin>(NF_NEW NFNodePin(id, name, inputPin, guid));
      ptr->nodeView = this->nodeView;
      ptr->nodeId = this->guid;
      mAttris.push_back(ptr);
   }

   void RemovePin(const int id)
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

   void RemovePin(const NFGUID guid)
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
   NFNodeView* nodeView;

   std::list<NF_SHARE_PTR<NFNodePin>> mAttris;

private:
   bool first = true;
};

class NFNodeView : public NFIView
{
public:
	NFNodeView(NFIPluginManager* p);
   ~NFNodeView();

	virtual bool Execute();

   void CleanNodes();

   void SetUpNewLinkCallBack(std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> functor);
   void SetUpDeleteLinkCallBack(std::function<bool(const NFGUID&)> functor);
   void SetPinRenderCallBack(std::function<void(NFNodePin*)> functor);
   void RenderForPin(NFNodePin* nodeAttri);

   //////////////////////////////////////
   const NFGUID GetNodeGUID(const int nodeId);
   const int GetNodeID(const NFGUID guid);

   const NFGUID GetAttriGUID(const int attriId);
   const int GetAttriID(const NFGUID guid);

   void AddNode(const NFGUID guid, const std::string& name, const NFVector2 vec = NFVector2());
   void AddPinIn(const NFGUID guid, const NFGUID attrId, const std::string& name);
   void AddPinOut(const NFGUID guid, const NFGUID attrId, const std::string& name);
   void DeleteNode(const NFGUID guid);

   void AddLink(const NFGUID& selfID, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
   NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
   NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& id);
   void DeleteLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
   void DeleteLink(const NFGUID& id);

   NFGUID GetNodeByAttriId(const NFGUID attriId);
   void SetNodeDraggable(const NFGUID guid, const bool dragable);
   void SetNodePosition(const NFGUID guid, const NFVector2 vec = NFVector2());

   void ResetOffest(const NFVector2& pos);
   void MoveToNode(const NFGUID guid);


private:
   void RenderNodes();
   void RenderLinks();
   
   void CheckNewLinkStatus();
   void CheckDeleteLinkStatus();

private:

    std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> mTryNewLinkFunctor;
    std::function<bool(const NFGUID&)> mTryDeleteLinkFunctor;
    std::function<void(NFNodePin*)> mPinRenderFunctor;

    NFIUIModule* m_pUIModule;

    void* m_pEditorContext;

    std::map<NFGUID, NF_SHARE_PTR<NFNode>> mNodes;
    std::list<NF_SHARE_PTR<NFDataLink>> mLinks;
};

#endif