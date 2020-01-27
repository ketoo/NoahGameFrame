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

#ifndef NF_MSG_DEFINE_H
#define NF_MSG_DEFINE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFList.hpp"
#include "NFLimit.h"
#include "NFDefine.pb.h"
#include "NFMsgBase.pb.h"
#include "NFMsgPreGame.pb.h"
#include "NFMsgShare.pb.h"
#include "NFProtocolDefine.hpp"

#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
// -------------------------------------------------------------------------
#pragma pack(push,1)

enum E_CHECK_TYPE
{
    ECT_SAVE        = 0, 
    ECT_PRIVATE     = 1, 
    ECT_PUBLIC      = 2, 
};
#pragma pack(pop)
#endif
