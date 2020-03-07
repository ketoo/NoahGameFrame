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

#include "NFLightComponent.h"

NFLightComponent::NFLightComponent()
:type(NFLightType::AMBIANT),
color(1.0f, 1.0f, 1.0f),
alpha(255)
{

}

NFLightComponent::~NFLightComponent()
{

}

void NFLightComponent::SetType(NFLightType pType)
{
    this->type = pType;
}

void NFLightComponent::SetColor(const NFVector3& color, const int alpha)
{
    this->color = color;
}

void NFLightComponent::SetAlpha(float alpha)
{
    this->alpha = alpha;
}

NFLightComponent::NFLightType NFLightComponent::GetType()
{
    return this->type;;
}

const NFVector3& NFLightComponent::GetColor()
{
    return this->color;
}

int NFLightComponent::GetAlpha()
{
    return this->alpha;
}