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

#ifndef NF_LIMIT_H
#define NF_LIMIT_H

#pragma pack(push,1)

#define NF_NAME_MAXLEN          32              
#define NF_BROAD_MAXLEN         64             
#define NF_PATH_MAXLEN          128            
#define NF_ACCOUNT_MAXROLE      4               
#define NF_ROLE_MAXPKG          150             
#define NF_ROLE_DEFAULTPKG      60              

#define NF_VIEWGRID_UNIT        200             
#define NF_VIEWSIZE_MAXLEN      9               

//#define NF_SCENE_LEFTLENPOS       -32000      
//#define NF_SCENE_RIGHTPOS     32000           

//#define NF_MAX_ONLINE         3000            


#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN

#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID

#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN

#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID

#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif

class _NFExport DoNothing
{
public:
	void DonOTHING()
	{

	}
};


#pragma pack(pop)



#endif