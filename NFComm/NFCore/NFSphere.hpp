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

#ifndef NF_SPHERE_H
#define NF_SPHERE_H

#include <iostream>
#include <string>
#include "NFSpaceNode.hpp"

class NFLine;
class NFBox;
class NFPlane;
class NFVector3;

class NFSphere : public NFSpaceNode
{
public:
    NFSphere()
    {
        mvCenter = NFVector3::Zero();
        mfRadius = 0.0f;
    }

    NFSphere(const NFVector3&  vCenter, float fRadius)
    {
        this->mvCenter = vCenter;
        this->mfRadius = fRadius;
    }

    virtual ~NFSphere() {}

    bool operator==(const NFSphere& other) const
    {
        return (mvCenter == other.mvCenter) && (mfRadius == other.mfRadius);
    }

    bool operator!=(const NFSphere& other) const
    {
        return !((mvCenter == other.mvCenter) && (mfRadius == other.mfRadius));
    }

    void operator*=(const float fScale)
    {
    }

    void operator/=(const float fScale)
    {
    }

    bool Contains(const NFVector3& v)
    {
        return false;
    }

    bool CollideWithLine(const NFLine& l)
    {
        return false;
    }
    
    bool CollideWithPlane(const NFPlane& p)
    {
        return false;
    }

    bool CollideWithBox(const NFBox& b)
    {
        return false;
    }

    bool CollideWithSphere(const NFSphere& s)
    {
        return false;
    }
private:
    NFVector3        mvCenter;
    float            mfRadius;

};

#endif