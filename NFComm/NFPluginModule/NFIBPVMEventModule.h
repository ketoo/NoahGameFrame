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
#ifndef NFI_BLUE_PRINT_GAME_EVENT_MODULE_H
#define NFI_BLUE_PRINT_GAME_EVENT_MODULE_H

#include <iostream>
#include <map>
#include <list>
#include "NFIModule.h"
#include "NFIKernelModule.h"
#include "NFIClassModule.h"
#include "NFIElementModule.h"

//block id, monitor id, event id, map data
typedef std::function<int(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&)> BLUEPRINT_EVENT_FUNCTOR;

class NFIBPVMEventModule
    : public NFIModule
{
public:
    ////////////////////////////////
    template<typename BaseType>
    bool RegisterGameEventCallBack(const NFGUID blockID, const int nEventID, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterGameEventCallBack(blockID, nEventID, monitorID, functor);
    }

    virtual bool RegisterGameEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;
    
    ////////////////////////////////
    
    template<typename BaseType>
    bool RegisterNetEventCallBack(const NFGUID blockID, const int nEventID, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterNetEventCallBack(blockID, nEventID, monitorID, functor);
    }

    virtual bool RegisterNetEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;
    
    ////////////////////////////////
   
    template<typename BaseType>
    bool RegisterNetMsgEventCallBack(const NFGUID blockID, const int nEventID, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterNetMsgEventCallBack(blockID, nEventID, monitorID, functor);
    }

    virtual bool RegisterNetMsgEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;
   
    ////////////////////////////////

    template<typename BaseType>
    bool RegisterGameObjectEventCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterGameObjectEventCallBack(blockID, className, monitorID, functor);
    }

    virtual bool RegisterGameObjectEventCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;

    /*
    ////////////////////////////////
    template<typename BaseType>
    bool RegisterGameObjectPropCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterGameObjectPropCallBack(blockID, className, monitorID, functor);
    }

    virtual bool RegisterGameObjectPropCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;

    ////////////////////////////////

    template<typename BaseType>
    bool RegisterGameObjectRecordCallBack(const NFGUID blockID, const int nEventID, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterGameObjectRecordCallBack(blockID, nEventID, monitorID, functor);
    }

    virtual bool RegisterGameObjectRecordCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;

    ////////////////////////////////
    template<typename BaseType>
    bool RegisterObjectSceneCallBack(const NFGUID blockID, const int nEventID, const NFGUID monitorID, BaseType* pBase, int (BaseType::* handler)(const NFGUID&, const NFGUID&, const int, const NFMapEx<std::string, NFData>&))
    {
        BLUEPRINT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        return RegisterObjectSceneCallBack(blockID, nEventID, monitorID, functor);
    }

    virtual bool RegisterObjectSceneCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor) = 0;
    */

    ////////////////////////////////

	virtual bool UnRegisterAllCallBack(const NFGUID blockID) = 0;

};

#endif