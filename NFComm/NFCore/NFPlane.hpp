// -------------------------------------------------------------------------
//    @FileName         :    NFPlane.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-10-09
//    @Module           :    NFPlane
//
// -------------------------------------------------------------------------

#ifndef NF_PLANE_H
#define NF_PLANE_H

#include <iostream>
#include <string>


class NFPlane
{

private:
    NFVector3 mvCorner[4];

    NFVector3 mvRadius[2];

    NFVector3 mvCenter;
};

#endif