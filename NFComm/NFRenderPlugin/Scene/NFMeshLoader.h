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

#ifndef NF_MESH_LOADER
#define NF_MESH_LOADER

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/fwd.hpp"
#include "tiny_obj_loader.h"
#include "NFMeshComponent.h"
#include "NFComm/NFCore/NFVector3.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"


class NFMesh
{
public:
    void Draw()
    {

    }

public:

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
};

class NFMeshLoader
{
public:
    NF_SHARE_PTR<NFMesh> LoadMesh(const std::string& path);

private:
};

#endif // !NF_MESH_LOADER