# NoahGameFrame
##### Copyright © http://www.noahframe.com

#####交流QQ群：341159815

## What is it?

NF is a lightweight, fast, scalable, distributed plugin framework.Greatly inspired by OGRE and Bigworld.

## Features
*  it is easy to use interface oriented design minimise the effort
*  extensible plugin framework makes getting your application running is quick and simple
*  clean, uncluttered design, stable engine used in several commercial products
*  using the actor model has very high performance(by theron);</p>
*  based on the event-driven and attribute-driver can make business more clearly and easy to maintenance
*  based on the standard c + + development, cross-platform support
*  with existing c++, c# game client for rapid development
*  Cross-platform


## Introduction
[English PDF](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/NFrame_Introduction_EN_US.pdf) / [English PPT](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/NFrame_Introduction_EN_US.ppt)

[中文简体 PDF](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/NFrame_Introduction_ZH_CN.pdf) / [中文简体 PPT](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/NFrame_Introduction_ZH_CN.ppt)


## Architecture

### App Architecture:
![App Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_app_arch.png)

### Server Architecture
![Server Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_server_arch.png)

### Event Driver:
![Event Driver](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Event_driver.png)

## Get the Sources:

git clone https://github.com/ketoo/NoahGameFrame.git

or

svn checkout https://github.com/ketoo/NoahGameFrame


## Dependencies

Need build sub librares:

- Boost.System
- Boost.Chrono
- Boost.Thread
- libevent-2.0.21-stable
- protobuf-2.5.0
- redis-cplusplus-client
- Theron-6.00.01
- easylogging++

## Supported Compilers

* GCC >= 4.8
* VC >= 9.0 (sp1)

## Build and Install
### Windows[VS2012]

1. Download all source;
2. Extract nessasery lib: 	**Dependencies/boost_1_53_0.zip** to **Dependencies/**
3. Extract nessasery lib: 	**Dependencies/lib.zip** to **Dependencies/**
4. Extract nessasery lib: 	**Dependencies/gperftools-2.1.zip** to **Dependencies/**
5. Extract nessasery lib: 	**Dependencies/curl-7.37.1.zip** to **Dependencies/**
6. Open the solution: **NoahFrame.sln**
7. Build the solution
8. Copy files:libmysql.dll mysqlpp_d.dll 	to **_Out/Server/Debug/NFGameServer1/**
9. Copy files:libmysql.dll mysqlpp.dll 		to **_Out/Server/Release/NFGameServer1/**
10. Copy files:libmysql.dll mysqlpp_d.dll 	to **_Out/Server/Debug/NFLoginServer/**
11. Copy files:libmysql.dll mysqlpp.dll 	to **_Out/Server/Release/NFLoginServer/**
12. Copy files:libmysql.dll mysqlpp_d.dll 	to **_Out/Server/Debug/NFWorldServer/**
13. Copy files:libmysql.dll mysqlpp.dll 	to **_Out/Server/Release/NFWorldServer/**
14. Run the binary file by **_Out/Server/Debug/Run.bat**

### Windows[VS2015]
Who can do this?

### Linux[CodeBlock 13.1]

1. Download all source;
2. Extract nessasery lib: **Dependencies/boost_1_53_0.zip** to **Dependencies/**
3. Extract nessasery lib: 	**Dependencies/lib.zip** to **Dependencies/**
4. Extract nessasery lib: 	**Dependencies/gperftools-2.1.zip** to **Dependencies/**
5. Extract nessasery lib: 	**Dependencies/curl-7.37.1.zip** to **Dependencies/**
6. Open the solution with CodeBlocks: **NF.workspace**
7. Add **c++11** optional for every project of this solution
8. Build library: **NFNet, NFCore, NFEventTriggerPlugin, NFKernelPlugin, Theron, libprotocolbuf[libprotocolbuf-lite], NFLogPlugin, NFPluginLoader**
9. Build all other plugin
10. Run the binary file by **_Out/Server/Debug/Run.sh**


### JAVA Project
WebSite:  https://github.com/NFGameTeam/NFrame-java

### C# Project
WebSite:  https://github.com/ketoo/NFrame



## Documents & Tutorial
[English](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_EN_US.md)

[中文简体](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_ZH_CN.md)

## License
The NFrame project is currently available under the [Apache License](https://github.com/ketoo/NoahGameFrame/blob/develop/LICENSE).


Tutorial:
-------------------
### [01-Hello world, add a module](https://github.com/ketoo/NoahGameFrame/tree/develop/Tutorial/Tutorial1)


```cpp

// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld1.h
//    @Author           :    ketoo
//    @Date             :    2014-05-01 08:51
//    @Module           :   HelloWorld1
//
// -------------------------------------------------------------------------

#ifndef _NFC_HELLO_WORLD1_H_
#define _NFC_HELLO_WORLD1_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"

class HelloWorld1
    : public NFILogicModule
{
public:
    HelloWorld1(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool BeforeShut();
    virtual bool Shut();

protected:

};

#endif


#include "HelloWorld1.h"

bool HelloWorld1::Init()
{
    // Use this for initialization
	
    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool HelloWorld1::AfterInit()
{
    // AfterInit is called after Init
	
    std::cout << "Hello, world1, AfterInit" << std::endl;

    return true;
}

bool HelloWorld1::Execute( const float fLasFrametime, const float fStartedTime )
{
    // Execute is called once per frame
	
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

bool HelloWorld1::BeforeShut()
{
    //before final
	
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld1::Shut()
{
    //final
	
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}

```


-------------------
### [02-Hello world, test data driver](https://github.com/ketoo/NoahGameFrame/tree/develop/Tutorial/Tutorial2)

* how to use the world's most advanced data engine 

-------------------
### [03-Hello world, test heartbeat and event system](https://github.com/ketoo/NoahGameFrame/tree/develop/Tutorial/Tutorial3)

* how to use the synchronous events

-------------------
### [04-Hello actor, test actor model(async event system)](https://github.com/ketoo/NoahGameFrame/tree/develop/Tutorial/Tutorial4ActorPlugin)

* how to use the asynchronous events
* use multiple cpus to get high performance

-------------------









