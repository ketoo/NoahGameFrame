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

#ifndef NF_LIGHT_COMPONENT_H
#define NF_LIGHT_COMPONENT_H


#include "NFSceneNode.h"
#include "NFComm/NFCore/NFVector2.hpp"
#include "NFComm/NFCore/NFVector3.hpp"

class NFLightComponent
    : public NFSceneNode
{
public:
    enum NFLightType
    {
        AMBIANT = 0,
        POINT_LIGHT = 1,
        DIRECTIONNAL = 2,
        SPOT_LIGHT = 3,
        SILOUHETTE = 4
    };

    NFLightComponent();
    ~NFLightComponent();

    void SetType(const NFLightType type);
    void SetColor(const NFVector3& color, const int alpha);
    void SetAlpha(float alpha);

    NFLightType GetType();
    const NFVector3& GetColor();
    int GetAlpha();

private:
    NFLightType type;
    NFVector3 color;
    int alpha;//[0-255]
};

#endif // !NF_LIGHT