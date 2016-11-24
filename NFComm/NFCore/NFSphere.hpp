// -------------------------------------------------------------------------
//    @FileName         :    NFSphere.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-10-09
//    @Module           :    NFSphere
//
// -------------------------------------------------------------------------

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