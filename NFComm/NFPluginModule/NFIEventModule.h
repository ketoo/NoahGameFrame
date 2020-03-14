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

#ifndef NFI_EVENT_MODULE_H
#define NFI_EVENT_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIEventModule
    : public NFIModule
{
public:
protected:

	typedef std::function<int(const NFGUID&, const int, const NFDataList&)> OBJECT_EVENT_FUNCTOR;
	typedef std::function<int(const int, const NFDataList&)> MODULE_EVENT_FUNCTOR;

public:
	// only be used in module
    virtual bool DoEvent(const int nEventID, const NFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const int nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const int nEventID) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const int nEventID, BaseType* pBase, int (BaseType::*handler)(const int, const NFDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		return AddEventCallBack(nEventID, functor);
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////
	// can be used for object
    virtual bool DoEvent(const NFGUID self, const int nEventID, const NFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const NFGUID self,const int nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const NFGUID self,const int nEventID) = 0;
	virtual bool RemoveEventCallBack(const NFGUID self) = 0;
	
	template<typename BaseType>
	bool AddEventCallBack(const NFGUID& self, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const NFDataList&))
	{
		OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		return AddEventCallBack(self, nEventID, functor);
	}

    //can be used for common event
    template<typename BaseType>
    bool AddCommonEventCallBack(BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const int, const NFDataList&))
    {
        OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        return AddCommonEventCallBack( functor);
    }


protected:

	virtual bool AddEventCallBack(const int nEventID, const MODULE_EVENT_FUNCTOR cb) = 0;
    virtual bool AddEventCallBack(const NFGUID self, const int nEventID, const OBJECT_EVENT_FUNCTOR cb) = 0;
    virtual bool AddCommonEventCallBack(const OBJECT_EVENT_FUNCTOR cb) = 0;

};

#endif