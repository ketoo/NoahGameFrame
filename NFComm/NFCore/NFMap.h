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

template <typename idType , typename dataType>
class NFMap
{
public:
    typedef std::map<idType, dataType*> NFMapOBJECT;

    NFMap() {};
    virtual ~NFMap()
    {
        //mObjectList.clear();
        //新的NFMap只管理元素，不管理元素所占的内存，谁new 谁delete
        //DeleteAllElement();
    };

    virtual bool AddElement(const idType& name, dataType* data)
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

    virtual dataType* RemoveElement(const idType& name)
    {
        dataType* pData = NULL;
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            mObjectList.erase(itr);
        }

        return pData;
    }

    virtual dataType* GetElement(const idType& name)
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

    virtual dataType* First()
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

    virtual dataType* Next()
    {
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

    virtual dataType* First(idType& name)
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

    virtual dataType* Next(idType& name)
    {
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
