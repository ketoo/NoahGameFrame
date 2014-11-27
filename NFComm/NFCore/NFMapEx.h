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

template <typename T , typename TD>
class NFMapEx
{
public:
    typedef std::map<T, std::shared_ptr<TD> > NFMapOBJECT;

    NFMapEx() {};
    virtual ~NFMapEx()
    {
    };

    virtual bool AddElement(const T& name, TD* data)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename NFMapOBJECT::value_type(name, std::shared_ptr<TD>(data)));
            // mObjectList[name] = data;
            return true;
        }

        return false;
    }

    virtual std::shared_ptr<TD> RemoveElement(const T& name)
    {
        std::shared_ptr<TD> pData(NULL);
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            itr = mObjectList.erase(itr);
        }

        return pData;
    }

    virtual std::shared_ptr<TD> GetElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return std::shared_ptr<TD>(NULL);
        }
    }

    virtual std::shared_ptr<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return std::shared_ptr<TD>(NULL);
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return std::shared_ptr<TD>(NULL);
        }
    }

    virtual std::shared_ptr<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return std::shared_ptr<TD>(NULL);
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return std::shared_ptr<TD>(NULL);
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return std::shared_ptr<TD>(NULL);
        }
    }

    virtual std::shared_ptr<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return std::shared_ptr<TD>(NULL);
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return std::shared_ptr<TD>(NULL);
        }
    }

    virtual std::shared_ptr<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return std::shared_ptr<TD>(NULL);
        }

        if (mObjectCurIter == mObjectList.end())
        {
            return std::shared_ptr<TD>(NULL);
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return std::shared_ptr<TD>(NULL);
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
