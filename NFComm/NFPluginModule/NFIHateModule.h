/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#ifndef NFI_HATE_MODULE_H
#define NFI_HATE_MODULE_H

#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class NFIHateModule
    : public NFIModule
{
public:
	/*
	virtual bool ClearHateObjects(const NFGUID& self) = 0;

	virtual bool AddHate(const NFGUID& self, const NFGUID& other, const int nValue) = 0;

	virtual bool EndSpring(const NFGUID& other) = 0;
	virtual bool EndSpring(const NFGUID& self, const NFGUID& other) = 0;
	virtual bool OnSpring(const NFGUID& self, const NFGUID& other) = 0;

	virtual bool CopyHateList(const NFGUID& self, const NFGUID& other) = 0;
	*/

	virtual NFGUID QueryMaxHateObject(const NFGUID& self) = 0;
   
    virtual int QueryHateValue(const NFGUID& self, const NFGUID& other) = 0;

protected:

private:
};

#endif