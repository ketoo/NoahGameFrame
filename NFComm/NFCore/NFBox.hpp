// -------------------------------------------------------------------------
//    @FileName         :    NFBox.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-10-09
//    @Module           :    NFBox
//
// -------------------------------------------------------------------------

#ifndef NF_BOX_H
#define NF_BOX_H

#include <iostream>
#include <string>

class NFLine;
class NFBox;
class NFPlane;
class NFVector3;

class NFBox
{
    NFVector3 GetCorner(int i) const
    {
        if(i < 8)
        {
            return mvCorner[i];
        }
    }

    NFVector3 Corner(int i) const
    {
        return GetCorner(i);
    }

private:
    NFVector3 mvCorner[8];

    NFVector3 mvRadius[3];

    NFVector3 mvCenter;
};

#endif