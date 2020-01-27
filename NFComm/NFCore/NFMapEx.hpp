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

#ifndef NF_MAPEX_H
#define NF_MAPEX_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

template <typename T , typename TD>
class NFMapEx
{
public:
    typedef std::map<T, NF_SHARE_PTR<TD> > NFMapOBJECT;

    NFMapEx()
	{
	};
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
		if (data == nullptr)
		{
			std::cout << "AddElement failed : " << std::endl;
			return false;
		}

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
            return nullptr;
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
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }

    int Count()
    {
        return (int)mObjectList.size();
    }


protected:
    NFMapOBJECT     mObjectList;
    typename NFMapOBJECT::iterator mObjectCurIter;
};

template <typename T, typename TD>
class NFConsistentHashMapEx : public NFMapEx<T, TD>
{
public:
	virtual NF_SHARE_PTR<TD> GetElementBySuitRandom()
	{
		NFVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeRandom(vNode))
		{
			typename NFMapEx<T, TD>::NFMapOBJECT::iterator itr = NFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != NFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual NF_SHARE_PTR<TD> GetElementBySuitConsistent()
	{
		NFVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeConsistent(vNode))
		{
			typename NFMapEx<T, TD>::NFMapOBJECT::iterator itr = NFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != NFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual NF_SHARE_PTR<TD> GetElementBySuit(const T& name)
	{
		NFVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNode(name, vNode))
		{
			typename NFMapEx<T, TD>::NFMapOBJECT::iterator itr = NFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != NFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual bool AddElement(const T& name, const NF_SHARE_PTR<TD> data) override
	{
		if (data == nullptr)
		{
			return false;
		}

		typename NFMapEx<T, TD>::NFMapOBJECT::iterator itr = NFMapEx<T, TD>::mObjectList.find(name);
		if (itr == NFMapEx<T, TD>::mObjectList.end())
		{
			NFMapEx<T, TD>::mObjectList.insert(typename NFMapEx<T, TD>::NFMapOBJECT::value_type(name, data));

			mxConsistentHash.Insert(name);

			return true;
		}

		return false;
	}

	virtual bool RemoveElement(const T& name) override
	{
		typename NFMapEx<T, TD>::NFMapOBJECT::iterator itr = NFMapEx<T, TD>::mObjectList.find(name);
		if (itr != NFMapEx<T, TD>::mObjectList.end())
		{
			NFMapEx<T, TD>::mObjectList.erase(itr);
			mxConsistentHash.Erase(name);

			return true;
		}

		return false;
	}

    virtual bool ClearAll() override
	{
		NFMapEx<T, TD>::mObjectList.clear();
		mxConsistentHash.ClearAll();
		return true;
	}

private:
	NFConsistentHash<T> mxConsistentHash;
};
#endif