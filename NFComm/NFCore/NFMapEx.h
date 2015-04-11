// -------------------------------------------------------------------------
//    @FileName         :    NFMapEx.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    这个是智能指针专用,切记，不能在循环中自己删除自己
//
// -------------------------------------------------------------------------

#ifndef _NF_MAPEX_H_
#define _NF_MAPEX_H_

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "NFComm/NFPluginModule/NFPlatform.h"

template <typename T , typename TD>
class NFMapEx
{
public:
    typedef std::map<T, NF_SHARE_PTR<TD> > NFMapOBJECT;

    NFMapEx() {};
    virtual ~NFMapEx()
    {
    };

    virtual bool AddElement(const T& name, const NF_SHARE_PTR<TD> data)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename NFMapOBJECT::value_type(name, data));
            return true;
        }

        return false;
    }

    virtual bool SetElement(const T& name, const NF_SHARE_PTR<TD> data)
    {
        mObjectList[name] = data;

        return true;
    }

    virtual bool RemoveElement(const T& name)
    {
        NF_SHARE_PTR<TD> pData(NULL);
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            mObjectList.erase(itr);

            return true;
        }

        return false;
    }

    virtual NF_SHARE_PTR<TD> GetElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>(NULL);
        }
    }

    virtual NF_SHARE_PTR<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>(NULL);
        }
    }

    virtual NF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>(NULL);
        }
    }

    virtual NF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>(NULL);
        }
    }

    virtual NF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>(NULL);
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>(NULL);
        }
    }

    int Count()
    {
        return mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }
private:
    NFMapOBJECT     mObjectList;
    typename NFMapOBJECT::iterator mObjectCurIter;
};


#endif
