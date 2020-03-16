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
#ifndef NF_NODE_SYSTEM_H
#define NF_NODE_SYSTEM_H

#include "NFComm/NFPluginModule/NFIModule.h"

/*https://www.rapidtables.com/web/color/RGB_Color.html
*/

namespace NodeSystem
{
    class NFNode
    {
    private:
       NFNode(){}

    public:
	    NFNode(const NFGUID id, const int size, const std::string& name, const NFVector2 vec, const int color)
       {
          this->id = id;
          this->color = color;
          this->size = size;
          this->name = name;
          this->pos = vec;
       }

	    void Execute(const NFVector2& offset);

       std::string name;
       NFGUID id;
       int color;
       int size;
       NFVector2 pos;
       bool selected = false;
    };

    class NFNodeSystem
    {
    public:
        NFNodeSystem();
       ~NFNodeSystem();

	    void Execute();


        void SetBeginRenderCallBack(std::function<void()> functor);

        void CleanNodes();

        std::shared_ptr<NFNode> AddNode(const NFGUID nodeId, const std::string& name, const NFVector2 vec = NFVector2(), const int color = -100000);
        void DeleteNode(const NFGUID nodeId);
        std::shared_ptr<NFNode> FindNode(const const NFGUID nodeId);

        void ResetToCenter();
        void MoveToNode(const NFGUID nodeId);
        void SelectNode(const NFGUID nodeId);

        void SetBackGroundColor(const int color);
        void SetNodeSize(const int size);
        int GetNodeSize();
        NFVector2 GetOffset();

        void DrawRect(const NFVector2& p_min, const NFVector2& p_max, int col);
        void DrawRectFilled(const NFVector2& p_min, const NFVector2& p_max, int col);
        void DrawText(const NFVector2& pos, int col, const std::string& text);

        void DrawCircle(const NFVector2& center, const float radius, const int col);
        void DrawCircleFilled(const NFVector2& center, const float radius, int const col);

        void DrawArrow(const NFVector2& start, const NFVector2& end, int col);

    private:
        bool init = false;

        std::function<void()> mBeginRenderFunctor;

        int idIndex = 0;
        int nodeSize = 10;
        int backgroundColor;

        NFGUID selectedObject;

        NFVector2 offset;
        std::map<NFGUID, NF_SHARE_PTR<NFNode>> mNodes;
    };

}
#endif