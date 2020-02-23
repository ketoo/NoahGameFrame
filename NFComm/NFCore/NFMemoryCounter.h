/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: yu.tang
   
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

#ifndef NF_MEMORY_COUNTER_H
#define NF_MEMORY_COUNTER_H

#include <iostream>
#include <string>
#include <map>
#include "NFComm/NFPluginModule/NFPlatform.h"

class NFMemoryCounter
{
private:
	NFMemoryCounter() {}

    std::string mstrClassName;

    struct Data
    {
        Data(NFMemoryCounter* p, int d)
            :deep(d)
        {
            data.insert(std::map<NFMemoryCounter*, int>::value_type(p, d));
        }

        std::map<NFMemoryCounter*, int> data;
        int deep = 0;
    };


public:
	static std::map<std::string, Data>* mxCounter;

	NFMemoryCounter(const std::string& strClassName, const int deep = 0)
	{
		mstrClassName = strClassName;

        if (!mxCounter)
        {
            mxCounter = NF_NEW std::map<std::string, Data>();
        }
		
        auto it = mxCounter->find(mstrClassName);
        if (it == mxCounter->end())
        {
            mxCounter->insert(std::map<std::string, Data>::value_type(mstrClassName, Data(this, deep)));
        }
        else
        {
            it->second.data.insert(std::map<NFMemoryCounter*, int>::value_type(this, deep));
        }
	}

	virtual ~NFMemoryCounter()
	{
        auto it = mxCounter->find(mstrClassName);
        if (it != mxCounter->end())
        {
            auto it2 = it->second.data.find(this);
            if (it2 != it->second.data.end())
            {
                it->second.data.erase(it2);
            }
        }
	}

    virtual void ToMemoryCounterString(std::string& info) = 0;

    static void PrintMemoryInfo(std::string& info, const int deep = 0)
    {
        for (auto it = mxCounter->begin(); it != mxCounter->end(); ++it)
        {
            info.append(it->first);
            info.append("=>");
            info.append(std::to_string(it->second.data.size()));
            info.append("\n");

            if (deep && it->second.deep)
            {
                for  (auto data : it->second.data)
                {
                    data.first->ToMemoryCounterString(info);
                }
            }
        }
    }
};

#endif