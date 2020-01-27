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

#ifndef NF_LIST_H
#define NF_LIST_H

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include "NFComm/NFPluginModule/NFPlatform.h"

template < typename T >
class NFList
{
public:
    virtual ~NFList() {}
    bool                    Add(const T& id);
    bool                    Find(const T& id);
    bool                    Remove(const T& id);
    bool                    ClearAll();

    bool                    First(T& id);
    bool                    Next(T& id);
    bool                    Get(const int32_t index, T& id);
    int                     Count();

protected:
    //idType normal;
    typedef     std::list<T>   TLISTOBJCONFIGLIST;
    TLISTOBJCONFIGLIST          mtObjConfigList;
    typename std::list<T>::iterator mCurIter;

private:
};

template < typename T >
bool NFList<T>::Add(const T& id)
{
    //if (!Find(id))
    {
        mtObjConfigList.push_back(id);
        return true;
    }

    return false;
}

template < typename T >
bool NFList<T>::Remove(const T& id)
{
    if (Find(id))
    {
        mtObjConfigList.remove(id);
        return true;
    }

    return false;
}

template < typename T >
bool NFList<T>::ClearAll()
{
    mtObjConfigList.clear();
    return true;
}

template < typename T >
bool NFList<T>::First(T& id)
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

template < typename T >
bool NFList<T>::Next(T& id)
{
    if (mCurIter == mtObjConfigList.end())
    {
        return false;
    }

    ++mCurIter;
    if (mCurIter != mtObjConfigList.end())
    {
        id = *mCurIter;
        return true;
    }

    return false;
}

template < typename T >
bool NFList<T>::Find(const T& id)
{
    typename TLISTOBJCONFIGLIST::iterator it = std::find(mtObjConfigList.begin(), mtObjConfigList.end(), id);
    if (it != mtObjConfigList.end())
    {
        return true;
    }

    return false;
}

template < typename T >
bool NFList<T>::Get(const int32_t index, T& id)
{
    if (index >= mtObjConfigList.size())
    {
        return false;
    }

    typename std::list<T>::iterator it = this->mtObjConfigList.begin();
    std::advance(it, index);

    id = *it;

    return true;
}

template < typename T >
int NFList<T>::Count()
{
    return (int)(mtObjConfigList.size());
}

#endif
