// -------------------------------------------------------------------------
//    @FileName         :    NFLine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-10-09
//    @Module           :    NFLine
//
// -------------------------------------------------------------------------

#ifndef NF_VETOR2_H
#define NF_VETOR2_H

#include <iostream>
#include <string>


class NFLine
{
public:
    NFLine(const NFVector3& vPointStart, const NFVector3& vDirection, const int nLength)
    {
    }
    
    NFLine(const NFVector3& vPointStart, const NFVector3& vPointEnd)
    {
    }
   
    NFLine(const NFVector3& vPoint, const NFVector3& vDirection, const int nLength)
    {
    }

    NFVector3 PointStart() const
    {
        return NFVector3::Zero();
    }

    NFVector3 Direction() const
    {
        return NFVector3::Zero();
    }

    NFVector3 PointEnd() const
    {
        return NFVector3::Zero();
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
    NFVector3 mvStartPoint;
    NFVector3 mvEndPoint;


	
};

#endif