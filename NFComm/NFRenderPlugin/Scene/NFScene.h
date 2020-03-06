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

#ifndef NF_SCENE
#define NF_SCENE

#include "NFSceneNode.h"

class NFScene
    : public NFIModule
{
    NFGUID AddNode(const std::string& name, const NFVector3& pos);
    NFGUID AddNode(NF_SHARE_PTR<NFSceneNode> parent, const std::string& name, const NFVector3& pos);

    NF_SHARE_PTR<NFSceneNode> FindNode(const NFGUID id);
    void RemoveNode(const NFGUID id);



private:
    NFSceneNode rootNode;

    NFMapEx<NFGUID, NFSceneNode> mNodes;
};

#endif // !NF_SCENE