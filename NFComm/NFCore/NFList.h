#pragma once
// -------------------------------------------------------------------------
//    @FileName      :    NFList.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2011-01-21 21:49
//    @Module    : 切记，不能在循环中自己删除自己
//
// -------------------------------------------------------------------------

#ifndef _NF_LIST_H_
#define _NF_LIST_H_

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include "NFPlatform.h"

template < typename idType >
class NFList
{
public:
    virtual ~NFList() {}
    bool                    Add(const idType& id);
    bool                    Find(const idType& id);
    bool                    Remove(const idType& id);
    bool                    ClearAll();

    bool                    First(idType& id);
	bool                    Next(idType& id);
    bool                    Get(const int32_t index, idType& id);
    int                       Count();

// 	idType& operator [] (const int32_t index)
// 	{
// 		if (it >= this->mtObjConfigList.size())
// 		{
// 			return normal;
// 		}
// 
// 		std::list<idType>::iterator it = this->mtObjConfigList.begin();
// 		std::advance(it + index);
// 
// 		return *it;
// 	}

protected:
	//idType normal;
    typedef     std::list<idType>   TLISTOBJCONFIGLIST;
    TLISTOBJCONFIGLIST          mtObjConfigList;
    typename std::list<idType>::iterator mCurIter;

private:
};

template < typename idType >
bool NFList<idType>::Add(const idType& id)
{
    if (!Find(id))
    {
        mtObjConfigList.push_back(id);
        return true;
    }

    return false;
}

template < typename idType >
bool NFList<idType>::Remove(const idType& id)
{
    if (Find(id))
    {
        mtObjConfigList.remove(id);
        return true;
    }

    return false;
}

template < typename idType >
bool NFList<idType>::ClearAll()
{
    mtObjConfigList.clear();
    return true;
}

template < typename idType >
bool NFList<idType>::First(idType& id)
{
    if (mtObjConfigList.size() <= 0)
    {
        return false;
    }

    mCurIter = mtObjConfigList.begin();
    if (mCurIter != mtObjConfigList.end())
    {
        id = *mCurIter;
        return true;
    }

    return false;
}

template < typename idType >
bool NFList<idType>::Next(idType& id)
{
    ++mCurIter;
    if (mCurIter != mtObjConfigList.end())
    {
        id = *mCurIter;
        return true;
    }

    return false;
}

template < typename idType >
bool NFList<idType>::Find(const idType& id)
{
    typename TLISTOBJCONFIGLIST::iterator it = std::find(mtObjConfigList.begin(), mtObjConfigList.end(), id);
    if (it != mtObjConfigList.end())
    {
        return true;
    }

    return false;
}

template < typename idType >
bool NFList<idType>::Get(const int32_t index, idType& id)
{
	if (index >= mtObjConfigList.size())
	{
		return false;;
	}
	
	std::list<idType>::iterator it = this->mtObjConfigList.begin();
	std::advance(it, index);

	id = *it;

	return true;
}

template < typename idType >
int NFList<idType>::Count()
{
    return (int)(mtObjConfigList.size());
}

#endif
