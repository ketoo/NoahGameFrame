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
#ifndef NF_NODE_VIEW_H
#define NF_NODE_VIEW_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"

#include "imgui/imgui.h"
#include "imnodes/imnodes.h"

/*https://www.rapidtables.com/web/color/RGB_Color.html
*/

enum NFColor
{
	DEFAULT = -1788294,
	WORKFLOW = -10625828,
	PININ = -1788294,
	PINOUT = -5263441,

	EXECUTER = -11944749,
	VARIABLE = -3240504,
	MODIFIER = -11814298,
	BRANCH = -13475794,
	MONITOR = -4271476,
	DEBUGER = -10770958,
	ARITHMETIC = -11251634,
	CUSTOM = -2051992
};

enum NFPinShape
{
	PinShape_NONE = 0,
    PinShape_Circle,
    PinShape_CircleFilled,
    PinShape_Triangle,
    PinShape_TriangleFilled,
    PinShape_Quad,
    PinShape_QuadFilled
};

void* ShowImage(const char* filename, int width, int height);

class NFNodeView;

class NFNodePin
{
private:
   NFNodePin(){}

public:
   NFNodePin(const int id, const std::string& name, const std::string& image, const bool inputPin, const NFGUID guid, const NFColor color, const NFPinShape shape = NFPinShape::PinShape_Circle)
   {
       this->color = color;
        this->id = id;
        this->name = name;
        this->image = image;
        this->inputPin = inputPin;
        this->guid = guid;
        this->shape = shape;
	   	iconTextureId = nullptr;
	   	imageSize = NFVector2(20.0f, 20.0f);
   }

    void Execute();
    void UpdateShape();

   int id;
   int color;
   bool inputPin;
   std::string name;
	std::string newImage;
	std::string image;
   NFVector2 imageSize;
   void* iconTextureId;

   NFPinShape shape;
   NFGUID guid;
   NFGUID nodeId;
   NFGUID linkId;
   NFNodeView* nodeView;
};

class NFNode
{
private:
   NFNode(){}

public:
	NFNode(const int id, const std::string& name, const NFGUID guid, const NFVector2 vec, const NFColor color)
   {
      this->id = id;
      this->color = color;
      this->name = name;
      this->guid = guid;
      this->pos = vec;
      this->iconTextureId = nullptr;
      this->iconSize = NFVector2(20.0f, 20.0f);
   }

	void Execute();

   void AddPin(const int id, const std::string& name, const std::string& image, const bool inputPin, const NFGUID guid, NFColor color, NFPinShape shape = NFPinShape::PinShape_Circle)
   {
      auto ptr = NF_SHARE_PTR<NFNodePin>(NF_NEW NFNodePin(id, name, image, inputPin, guid, color, shape));
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

   NF_SHARE_PTR<NFNodePin> GetPin(const NFGUID guid)
   {
       for (auto it = mAttris.begin(); it != mAttris.end(); ++it)
       {
           if ((*it)->guid == guid)
           {
               return *it;
           }
       }

       return nullptr;
   }

    NF_SHARE_PTR<NFNodePin> GetPin(const int id)
    {
        for (auto it = mAttris.begin(); it != mAttris.end(); ++it)
        {
            if ((*it)->id == id)
            {
                return *it;
            }
        }

        return nullptr;
    }

   std::string name;
   int id;
   int color;
   bool title = true;
   NFGUID guid;
   NFVector2 pos;
   NFNodeView* nodeView;

   NFVector2 iconSize;
   std::string iconPath;
   void* iconTextureId;

   std::list<NF_SHARE_PTR<NFNodePin>> mAttris;

private:
   bool first = true;
};

class NFNodeView
{
public:
	NFNodeView();
   ~NFNodeView();

	virtual bool Execute();

   void CleanNodes();

   void SetUpNewLinkCallBack(std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> functor);
   void SetUpDeleteLinkCallBack(std::function<bool(const NFGUID&)> functor);

   void SetHoverNodeCallBack(std::function<bool(const NFGUID&)> functor);
   void SetBeginRenderCallBack(std::function<void()> functor);

   void SetPinRenderCallBack(std::function<void(NFNodePin*)> functor);

   void SetNodeRenderBeforePinInCallBack(std::function<void(NFNode*)> functor);
   void SetNodeRenderAfterPinInCallBack(std::function<void(NFNode*)> functor);
   void SetNodeRenderBeforePinOutCallBack(std::function<void(NFNode*)> functor);
   void SetNodeRenderAfterPinOutCallBack(std::function<void(NFNode*)> functor);


   void RenderForPin(NFNodePin* nodeAttri);

   void NodeRenderBeforePinIn(NFNode* node);
   void NodeRenderAfterPinIn(NFNode* node);
   void NodeRenderBeforePinOut(NFNode* node);
   void NodeRenderAfterPinOut(NFNode* node);

   //////////////////////////////////////
   const NFGUID GetNodeGUID(const int nodeId);
   const int GetNodeID(const NFGUID guid);

   const NFGUID GetAttriGUID(const int attriId);
   const int GetAttriID(const NFGUID guid);

	NF_SHARE_PTR<NFNode> AddNode(const NFGUID guid, const std::string& name, NFColor color = NFColor::DEFAULT, const NFVector2 vec = NFVector2());
   void AddPinIn(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& image, NFColor color = NFColor::PININ, NFPinShape shape = NFPinShape::PinShape_Circle);
   void AddPinOut(const NFGUID guid, const NFGUID attrId, const std::string& name, const std::string& image, NFColor color = NFColor::PINOUT, NFPinShape shape = NFPinShape::PinShape_Circle);
   void ModifyPinColor(const NFGUID attrId, NFColor color);
   void DeleteNode(const NFGUID guid);
   NF_SHARE_PTR<NFNode> FindNode(const NFGUID guid);

   void AddLink(const NFGUID& selfID, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin, const int color);
   NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
   NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& id);
   void DeleteLink(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
   void DeleteLink(const NFGUID& id);

   NFGUID GetNodeByAttriId(const NFGUID attriId);
   void SetNodeDraggable(const NFGUID guid, const bool dragable);
   void SetNodePosition(const NFGUID guid, const NFVector2 vec = NFVector2());

   void ResetOffset(const NFVector2& pos);
   void MoveToNode(const NFGUID guid);

   void SetCurrentContext();

private:
   void RenderNodes();
   void RenderLinks();
   
   void CheckNewLinkStatus();
   void CheckDeleteLinkStatus();
   void CheckHoverNodeStatus();

private:

    std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> mTryNewLinkFunctor;
    std::function<bool(const NFGUID&)> mTryDeleteLinkFunctor;
    std::function<bool(const NFGUID&)> mHoverNodeFunctor;
    std::function<void()> mBeginRenderFunctor;
    std::function<void(NFNodePin*)> mPinRenderFunctor;

    std::function<void(NFNode*)> mNodeRenderBeforePinInFunctor;
    std::function<void(NFNode*)> mNodeRenderAfterPinInFunctor;
    std::function<void(NFNode*)> mNodeRenderBeforePinOutFunctor;
    std::function<void(NFNode*)> mNodeRenderAfterPinOutFunctor;

    NFIUIModule* m_pUIModule;

    void* m_pEditorContext;

    std::map<NFGUID, NF_SHARE_PTR<NFNode>> mNodes;
    std::list<NF_SHARE_PTR<NFDataLink>> mLinks;
};

#endif