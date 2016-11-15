// -------------------------------------------------------------------------
//    @FileName         :    NFMapEx.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    这个是智能指针专用,切记，不能在循环中自己删除自己
//
// -------------------------------------------------------------------------

#ifndef NF_MAPEX_H
#define NF_MAPEX_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "NFCConsistentHash.hpp"
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

	virtual bool ExistElement(const T& name)
	{
		typename NFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr != mObjectList.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
/*
	virtual NF_SHARE_PTR<TD> AddElement(const T& name)
	{
		typename NFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr == mObjectList.end())
		{
			NF_SHARE_PTR<TD> data(NF_NEW TD());
			mObjectList.insert(typename NFMapOBJECT::value_type(name, data));
			return data;
		}

		return NF_SHARE_PTR<TD>();
	}
	*/
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

    virtual bool RemoveElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            mObjectList.erase(itr);

            return true;
        }

        return false;
    }

    virtual TD* GetElementNude(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second.get();
        }
        else
        {
            return NULL;
        }
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
            return NF_SHARE_PTR<TD>();
        }
    }

    virtual TD* FirstNude(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* NextNude(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* FirstNude()
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* NextNude()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual NF_SHARE_PTR<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return NF_SHARE_PTR<TD>();
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>();
        }
    }

    virtual NF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>();
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>();
        }
    }

    virtual NF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NF_SHARE_PTR<TD>();
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>();
        }
    }

    virtual NF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>();
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NF_SHARE_PTR<TD>();
        }
    }

    int Count()
    {
        return (int)mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }

protected:
    NFMapOBJECT     mObjectList;
    typename NFMapOBJECT::iterator mObjectCurIter;
};

template <typename T, typename TD>
class NFCConsistentHashMapEx : public NFMapEx<T, TD>
{
public:
	virtual void InitHashNodeWeith(const int nWeigh = 500)
	{
		mnWeigh = nWeigh;
	}

	virtual NF_SHARE_PTR<TD> GetElementBySuit()
	{
		NFCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNode(vNode))
		{
			typename NFMapOBJECT::iterator itr = mObjectList.find(vNode.mxData);
			if (itr != mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual NF_SHARE_PTR<TD> GetElementBySuit(const T& name)
	{
		NFCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNode(name, vNode))
		{
			typename NFMapOBJECT::iterator itr = mObjectList.find(vNode.mxData);
			if (itr != mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual bool AddElement(const T& name, const NF_SHARE_PTR<TD> data)
	{
		typename NFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr == mObjectList.end())
		{
			mObjectList.insert(typename NFMapOBJECT::value_type(name, data));

			mxConsistentHash.Insert(name);

			return true;
		}

		return false;
	}

	virtual bool RemoveElement(const T& name)
	{
		typename NFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr != mObjectList.end())
		{
			mObjectList.erase(itr);
			mxConsistentHash.Erase(name);

			return true;
		}

		return false;
	}

	bool ClearAll()
	{
		mObjectList.clear();
		mxConsistentHash.ClearAll();
		return true;
	}

private:
	int mnWeigh = 0;
	NFCConsistentHash<T> mxConsistentHash;
};
#endif