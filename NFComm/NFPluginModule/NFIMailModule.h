/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#ifndef NFI_MAIL_MODULE_H
#define NFI_MAIL_MODULE_H

#include "NFIModule.h"

class NFIMailModule : public NFIModule
{
public:
    virtual bool SendMail(const NFGUID& reciever, const int nMailType, const NFDataList& xItemList, const std::string& strNailContent = NULL_STR) = 0;
    virtual int OnDrawMail(const NFGUID& self, const NFDataList& var) = 0;
    virtual int OnOpenMail(const NFGUID& self, const NFDataList& var) = 0;
    virtual int OnDeleteMail(const NFGUID& self, const NFDataList& var) = 0;
};

#endif // !NFI_MAIL_MODULE_H