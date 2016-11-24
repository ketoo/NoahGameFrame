// -------------------------------------------------------------------------
//    @FileName         :    NFRay.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-10-09
//    @Module           :    NFRay
//
// -------------------------------------------------------------------------

#ifndef NF_RAY_H
#define NF_RAY_H

#include <iostream>
#include <string>
#include "NFVector3.hpp"

class NFLine;
class NFBox;
class NFPlane;
class NFSphere;
class NFVector3;

class NFRay
{
public:
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
    NFLine* m_pLine;

};

#endif