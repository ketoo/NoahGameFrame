#include "NFSceneNode.h"
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

const NFGUID NFSceneNode::GetID()
{
    return this->id;
}

void NFSceneNode::SetName(const std::string& name)
{
    this->name = name;
}

const std::string& NFSceneNode::GetName()
{
    return this->name;// TODO: insert return statement here
}

void NFSceneNode::SetParent(const NF_SHARE_PTR<NFSceneNode> parent)
{
    this->parent = parent;
}

NF_SHARE_PTR<NFSceneNode> NFSceneNode::GetParent()
{
    return this->parent;
}

void NFSceneNode::AddChild(const NF_SHARE_PTR<NFSceneNode> child)
{

}

NF_SHARE_PTR<NFSceneNode> NFSceneNode::GetChild(const NFGUID childID)
{
    return NF_SHARE_PTR<NFSceneNode>();
}

std::list<NF_SHARE_PTR<NFSceneNode>> NFSceneNode::GetChildren()
{
    return std::list<NF_SHARE_PTR<NFSceneNode>>();
}

void NFSceneNode::SetPosition(const NFVector3& pos)
{
    this->position = pos;
}

const NFVector3& NFSceneNode::GetPosition()
{
    return this->position;
}

void NFSceneNode::SetScale(const NFVector3& scale)
{
    this->scale = scale;
}

const NFVector3& NFSceneNode::GetScale()
{
    return this->scale;
}

void NFSceneNode::SetRotation(const NFVector3& rotation)
{
    this->rotation = rotation;
}

const NFVector3& NFSceneNode::GetRotation()
{
    return this->rotation;
}

void NFSceneNode::SetDirection(const NFVector3& dir)
{
    this->direction = dir;
}

const NFVector3& NFSceneNode::GetDirection()
{
    return this->direction;
}

void NFSceneNode::SetTag(const std::string& tag)
{
    this->tag = tag;
}

const std::string& NFSceneNode::GetTag()
{
    return this->tag;
}

void NFSceneNode::SetLayer(const int layer)
{
    this->layer = layer;
}

const int NFSceneNode::GetLayer()
{
    return this->layer;
}

void NFSceneNode::LookAt(const NFVector3& pos)
{
}

void NFSceneNode::LookAt(NF_SHARE_PTR<NFSceneNode> node)
{
}

bool NFSceneNode::AddComponent(NF_SHARE_PTR<NFIComponent> component)
{
    return false;
}

bool NFSceneNode::FindComponent(const std::string& name)
{
    return false;
}

bool NFSceneNode::RemoveComponent(const std::string& name)
{
    return false;
}
