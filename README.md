# NoahGameFrame
#####  http://www.noahframe.com
##### Forum: http://bbs.noahframe.com/forum.php

[![Join the chat at https://gitter.im/ketoo/NoahGameFrame](https://badges.gitter.im/ketoo/NoahGameFrame.svg)](https://gitter.im/ketoo/NoahGameFrame?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

#####QQ Group：341159815

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

## Toturial 

[Toturial](http://bbs.noahframe.com/forum.php?mod=forumdisplay&fid=39) : http://bbs.noahframe.com/forum.php?mod=forumdisplay&fid=39

### FAQ OR DISCUSS
[FAQ](http://blog.noahframe.com/?p=229) : http://blog.noahframe.com/?p=229

### Unity3D client
[U3D Client](https://github.com/ketoo/NoahGameFrame/tree/master/NFClient/Unity3D)

## Architecture

### App Architecture:
![App Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_app_arch.png)

### Server Architecture
![Server Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_server_arch.png)

## Get the Sources:

git clone https://github.com/ketoo/NoahGameFrame.git

or

svn checkout https://github.com/ketoo/NoahGameFrame


## Dependencies

- libevent 2.0.22
- easylogging++ 9.80
- mysql++ 3.2.2
- google protobuf 2.5.0
- redis-cpp-client 
- Theron 6.00.01
- curl 7.37.1

## Supported Compilers

* GCC >= 4.8 (**Tested in Ubuntu 15.04**)
* MSVC >= VS2015 (**Tested in Win7/10**)

## Build and Install
### MSVC >= 2015

1. Git pull all source
2. Run **install4vs.bat**
3. Open the solution: **NoahFrame.sln**, build FileProcessTool project
4. Run **GenerateConfigXML.bat** to generate configuration files
5. Open the solution: **NoahFrame.sln**
6. Build the solution(if u build failed, please build again(not rebuild))
7. Run the binary file by **_Out/rund.bat**

### CodeBlocks >= 13.1(Only in linux)

1. Git pull all source
2. Run **install.sh**
3. Open the solution with CodeBlocks: **NF.workspace**
4. Check **C++11** option in CodeBlocks compiler setting
5. Build **NF.workspace**
6. Run **sh ./GenerateConfigXML.sh** to generate configuration files
7. Run the binary file by **_Out/rund.sh**

### CMake
1. Git pull all source
2. Install cmake[>= 3.1] please choose options for installing: **Add CMake to the system PATH for all users and restart your computer**
3. Install VS2015 or gcc[>= 4.8]
4. Run **install4cmake.bat** or **install4cmake.sh** to build NF
5. Run the binary file by **_Out/rund.bat** or **_Out/rund.sh**

### JAVA Project
WebSite:  https://github.com/NFGameTeam/NFrame-java

### C# Project
WebSite:  https://github.com/ketoo/NFrame



## Documents & Tutorial
[English](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_EN_US.md)

[Mandarin](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_ZH_CN.md)

## License
The NFrame project is currently available under the [Apache License](https://github.com/ketoo/NoahGameFrame/blob/develop/LICENSE).


Tutorial:
-------------------
### [01-Hello world, add a module](https://github.com/ketoo/NoahGameFrame/tree/develop/Tutorial/Tutorial1)


```cpp

// -------------------------------------------------------------------------
//    @FileName      	:    HelloWorld1.h
//    @Author           :    ketoo
//    @Date             :    2014-05-01 08:51
//    @Module           :   HelloWorld1
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD1_H
#define NFC_HELLO_WORLD1_H

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

    virtual bool Execute();

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

bool HelloWorld1::Execute()
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
### About The Author

* Mail: 342006@qq.com
* GitHub: https://github.com/ketoo

-------------------

### Amazing  open source projects:
**breeze**
* Auther: zsummer
* Github: https://github.com/zsummer/breeze
* Description:A fast, scalable, distributed game server framework for C++


**gce**
* Auther: nousxiong
* GitHub: https://github.com/nousxiong/gce
* Description: The Game Communication Environment (GCE) is an actor model framework for online game development.




