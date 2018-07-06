/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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


#ifndef NFC_HATE_MODULE_H
#define NFC_HATE_MODULE_H

#include <string>
#include <iostream>
#include <map>
#include "NFComm/NFPluginModule/NFIHateModule.h"

class NFCHateModule
    : public NFIHateModule
{
public:
    NFCHateModule(NFIPluginManager* pm)
    {
        pPluginManager = pm;
    }

    virtual bool ClearHate(const NFGUID& self);
    //仇恨列表是否为空
    virtual bool Empty(const NFGUID& self);

    //添加仇恨对象
    virtual bool AddHate(const NFGUID& self, const NFGUID& other, const int nValue);

    //设置仇恨值
    virtual bool SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue);

    //把他人的仇恨copy到自己身上,BOSS,小弟
    virtual bool CopyHateList(const NFGUID& self, const NFGUID& other);

    //对仇恨排序，最高在前面
    virtual void CompSortList(const NFGUID& self);

    //得到最高仇恨的一个对象
    virtual const NFGUID& QueryMaxHateObject(const NFGUID& self);

    //得到最高仇恨的数个对象
    virtual bool QueryMaxHateObject(const NFGUID& self, std::list<std::string>& HateList, const int nCount = 1);

    //查询仇恨值
    virtual int QueryHateValue(const NFGUID& self, const NFGUID& other);

protected:

    typedef std::map<NFGUID, int> THATEMAP;
    typedef std::map<NFGUID, THATEMAP*> TOBJECTHATEMAP;

    TOBJECTHATEMAP mtObjectHateMap;

private:
};

#endif