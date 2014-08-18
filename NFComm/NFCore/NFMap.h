// -------------------------------------------------------------------------
//    @FileName         :    NFMap.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    切记，不能在循环中自己删除自己
//
// -------------------------------------------------------------------------

#ifndef _NF_MAP_H_
#define _NF_MAP_H_

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>

template <typename T , typename TD>
class NFMap
{
public:
    typedef std::map<T, TD*> NFMapOBJECT;

    NFMap() {};
    virtual ~NFMap()
    {
        //mObjectList.clear();
        //新的NFMap只管理元素，不管理元素所占的内存，谁new 谁delete
        //DeleteAllElement();
    };

    virtual bool AddElement(const T& name, TD* data)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename NFMapOBJECT::value_type(name, data));
            // mObjectList[name] = data;
            return true;
        }

        return false;
    }

    virtual TD* RemoveElement(const T& name)
    {
        TD* pData = NULL;
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            itr = mObjectList.erase(itr);
        }

        return pData;
    }

    virtual TD* GetElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* First()
    {
        if (mObjectList.size() <= 0 )
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* First(T& name)
    {
        if (mObjectList.size() <= 0 )
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
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
