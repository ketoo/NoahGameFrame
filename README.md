# NoahGameFrame
##### Copyright © http://www.yowoyo.com

#####交流QQ群：341159815

## What is it?

NF is a lightweight, fast, scalable, distributed plugin framework.

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
4. Open the solution: **NoahFrame.sln**
5. Build the solution
6. Run the binary file by **_Out/Server/Debug/Run.bat**

### Linux[CodeBlock 13.1]

1. Download all source;
2. Extract nessasery lib: **Dependencies/boost_1_53_0.zip** to **Dependencies/**
3. Extract nessasery lib: **Dependencies/lib.zip** to **Dependencies/**
4. Open the solution with CodeBlocks: **NF.workspace**
5. Add **c++11** optional for every project of this solution
6. Build library: **NFNet, NFCore, NFEventTriggerPlugin, NFKernelPlugin, Theron, libprotocolbuf[libprotocolbuf-lite], NFLogPlugin, NFPluginLoader**
7. Build all other plugin
8. Run the binary file by **_Out/Server/Debug/Run.sh**

## Documents & Tutorial
[English](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_EN_US.md)

[中文简体](https://github.com/ketoo/NoahGameFrame/blob/develop/doc_ZH_CN.md)

## License
The NFrame project is currently available under the [Apache License](https://github.com/ketoo/NoahGameFrame/blob/develop/LICENSE).