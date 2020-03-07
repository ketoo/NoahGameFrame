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

#ifndef NF_SCENE_H
#define NF_SCENE_H

#include "NFMeshLoader.h"
#include "NFSceneNode.h"

class NFScene
    : public NFIModule
{
public:
    NFGUID AddNode(const std::string& name, const NFVector3& pos);
    NFGUID AddNode(NF_SHARE_PTR<NFSceneNode> parent, const std::string& name, const NFVector3& pos);

    NF_SHARE_PTR<NFSceneNode> FindNode(const NFGUID id);
    void RemoveNode(const NFGUID id);



    virtual bool Awake()
    {
        float bmin[3], bmax[3];
        mesh = meshLoader.LoadObjAndConvert(bmin, bmax, "./BAC_Batman70s_rocksteady/batman.obj");

        return true;
    }

    virtual bool Init()
    {
        /*
        glm::mat4 projection_matrix = glm::perspective(pCamera->GetFovy(), (float)pCamera->GetViewportWidth() / (float)pCamera->GetViewportHeight(), pCamera->GetZNear(), pCamera->GetZFar());
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, &projection_matrix[0][0]);

        glm::mat4 view_matrix = glm::lookAt(pCamera->GetCameraEye(), pCamera->GetCameraEye() + pCamera->GetCameraForward(), pCamera->GetCameraUp());
        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &view_matrix[0][0]);
        */

        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool ReadyExecute()
    {
        return true;
    }

    virtual bool Execute()
    {
        mesh->Draw();

        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Finalize()
    {
        return true;
    }


private:
    NFSceneNode rootNode;
    //for test
    NFMeshLoader meshLoader;
    NF_SHARE_PTR<NFMesh> mesh;

    NFMapEx<NFGUID, NFSceneNode> mNodes;
};

#endif // !NF_SCENE