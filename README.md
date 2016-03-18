# NoahGameFrame
##### Copyright © http://www.noahframe.com

[![Join the chat at https://gitter.im/ketoo/NoahGameFrame](https://badges.gitter.im/ketoo/NoahGameFrame.svg)](https://gitter.im/ketoo/NoahGameFrame?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

#####交流QQ群：341159815

## What is it?

NF is a lightweight, fast, scalable, distributed plugin framework.Greatly inspired by OGRE and Bigworld.

## Features
- Easy to use interface oriented design minimise the effort
- Extensible plugin framework makes getting your application running is quick and simple
- Clean, uncluttered design, stable engine used in several commercial products
- Using the actor model has very high performance(by Theron)
- Based on the event-driven and attribute-driver can make business more clearly and easy to maintenance
- Based on the standard c + + development, cross-platform support
- With existing c++, c# game client for rapid development
- Cross-platform



### FAQ
[FAQ](http://blog.noahframe.com/?p=229)

### Unity3D client
[U3D Client](https://github.com/ketoo/NoahGameFrame/tree/master/_Out/Server/ClientBin/source)

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

- libevent 2.0.22
- easylogging++ 9.80
- mysql++ 3.2.2
- google protobuf 2.5.0
- pybind11 1.2
- redis-cpp-client 
- Theron 6.00.01
- curl 7.37.1

## Supported Compilers

* GCC >= 4.8 (**Tested in Ubuntu 15.04**)
* MSVC >= VS2013 (**Tested in Win7/10**)

## Build and Install
### Windows[MSVC >= 2013]

1. Git pull all source
2. Run **install.bat**
3. Open the solution: **NoahFrame.sln**
4. Build the solution
5. Run **_Out/Server/NFDataCfg/Tool/FileProcess.exe** to generate configuration files
6. Run the binary file by **_Out/Server/Debug/Run.bat**

### Linux[CodeBlocks >= 13.1]

1. Git pull all source
2. Run **install.sh**
3. Open the solution with CodeBlocks: **NF.workspace**
4. Check **C++11** option in CodeBlocks compiler setting
5. Build **NF.workspace**
6. Run the binary file by **_Out/Server/Debug/rund.sh**


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









