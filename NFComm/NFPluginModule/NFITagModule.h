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



#ifndef NFI_TAG_MODULE_H
#define NFI_TAG_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFITagModule
    : public NFIModule
{

public:
    enum TAG_DEFINE
    {
        TAG_DEFAULT,
        TAG_BC,
        TAG_CHAT,
        TAG_TEAM,
    };

    virtual bool GetTagList(const std::string& strTag, NFList<NFGUID>& xList) = 0;
    virtual bool AddToTagList(const std::string& strTag, const NFGUID object) = 0;
    virtual bool RemoveFrmTagList(const std::string& strTag, const NFGUID object) = 0;

    virtual bool GetTagList(const TAG_DEFINE eTag, const std::string& strTag, NFList<NFGUID>& xList) = 0;
    virtual bool AddToTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object) = 0;
    virtual bool RemoveFrmTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object) = 0;

};

#endif