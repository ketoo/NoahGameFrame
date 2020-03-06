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

#ifndef NF_SCENE_NODE
#define NF_SCENE_NODE

#include <list>
#include <vector>
#include "NFComm/NFCore/NFVector3.hpp"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFActorPlugin/NFActor.h"

class NFSceneNode
{
public:
    const NFGUID GetID();

    void SetName(const std::string& name);
    const std::string& GetName();

    void SetParent(const NF_SHARE_PTR<NFSceneNode> parent);
    NF_SHARE_PTR<NFSceneNode> GetParent();

    void AddChild(const NF_SHARE_PTR<NFSceneNode> child);
    NF_SHARE_PTR<NFSceneNode> GetChild(const NFGUID childID);
    std::list<NF_SHARE_PTR<NFSceneNode>> GetChildren();

    void SetPosition(const NFVector3& pos);
    const NFVector3& GetPosition();

    void SetScale(const NFVector3& scale);
    const NFVector3& GetScale();

    void SetRotation(const NFVector3& rotation);
    const NFVector3& GetRotation();

    void SetDirection(const NFVector3& dir);
    const NFVector3& GetDirection();

    void SetTag(const std::string& tag);
    const std::string& GetTag();

    void SetLayer(const int layer);
    const int GetLayer();

    void LookAt(const NFVector3& pos);
    void LookAt(NF_SHARE_PTR<NFSceneNode> node);

    template <typename T>
    NF_SHARE_PTR<T> AddComponent()
    {
        NF_SHARE_PTR<NFIComponent> component = FindComponent(typeid(T).name());
        if (component)
        {
            return NULL;
        }

        {
            if (!TIsDerived<T, NFIComponent>::Result)
            {
                return NULL;
            }

            NF_SHARE_PTR<T> pComponent = NF_SHARE_PTR<T>(NF_NEW T());

            assert(NULL != pComponent);

            AddComponent(pComponent);

            return pComponent;
        }

        return nullptr;
    }



    template <typename T>
    NF_SHARE_PTR<T> FindComponent()
    {
        NF_SHARE_PTR<NFIComponent> component = FindComponent(typeid(T).name());
        if (component)
        {
            NF_SHARE_PTR<T> pT = std::dynamic_pointer_cast<T>(component);

            assert(NULL != pT);

            return pT;
        }

        return nullptr;
    }

    template <typename T>
    bool RemoveComponent()
    {
        return RemoveComponent(typeid(T).name());
    }

protected:
    bool AddComponent(NF_SHARE_PTR<NFIComponent> component);
    bool FindComponent(const std::string& name);
    bool RemoveComponent(const std::string& name);


protected:
    NFGUID id;
    std::string name;
    NF_SHARE_PTR<NFSceneNode> parent;
    std::map<NFGUID, NF_SHARE_PTR<NFSceneNode>> children;

    NFVector3 position;
    NFVector3 scale;
    NFVector3 rotation;
    NFVector3 direction;

    std::string tag;
    int layer;


    NFMapEx<std::string, NFIComponent> mComponent;
};

#endif // !NF_SCENE_NODE