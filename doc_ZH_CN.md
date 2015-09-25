# 欢迎使用NFrame敏捷服务器开发框架

关键字：文档，介绍，入门，教程

**NFrame** 是一个使用C++语言开发的、支持高并发、高性能的跨平台敏捷服务器开发解决方案。
旨在帮助中小企业降低开发门槛，快速完成项目功能。采用敏捷开发中的分层设计思路，将功能拆分为多个插件模块，让开发人员集中处理单一功能，提高团队效率。特点概述：

* 通用的抽象对象系统
* 数据驱动 (Property & record)
* 事件驱动 (Event)
* 可扩展的App、插件化、模块化 (Plugin & Module)
* 面向接口编程 (IOD)
* 高性能、高并发 (网络、Actor、逻辑)
* Component组件 (脚本系统)
* 分布式服务器架构
* 高稳定性、简易部署、支持扩展、跨平台
* 可视化配置、配套工具
* 配套客户端(Unity3D客户端(已完成)、Cocos2D客户端(暂未开始))
* 企业定制化服务(存储方案、逻辑模块、新架构)


## 目录
<pre>
<a href="#简述">简述</a>
<a href="#特性详细介绍">特性详细介绍</a>
    <a href="#通用的抽象对象系统logicclass">通用的抽象对象系统(LogicClass)</a>
    <a href="#数据驱动">数据驱动</a>
    <a href="#事件驱动">事件驱动</a>
    <a href="#面向接口编程iod">面向接口编程(IOD)</a>
    <a href="#高性能高并发">高性能高并发</a>
    <a href="#组件系统component">组件系统(Component)</a>
    <a href="#分布式服务器架构">分布式服务器架构</a>
<a href="#使用教程">使用教程</a>
    <a href="#教程1添加一个模块">教程1:添加一个模块</a>
    <a href="#教程2数据驱动">教程2:数据驱动</a>
    <a href="#教程3事件系统">教程3:事件系统</a>
    <a href="#教程4异步事件系统actor">教程4:异步事件系统，actor</a>
<a href="#结束">结束</a>
</pre>

## 简述
NFrame采用成熟的敏捷开发思想——分层设计，分层的程序设计带来的好处是显而易见的，由于层间松散的耦合关系，使得我们可以专注于本层的设计，而不必关心其他层的设计，也不必担心自己的设计会影响其它层，对提高软件质量大有裨益。而且分层设计使得程序结构清晰，升级和维护都变得十分容易，更改层的具体实现代码，只要层接口保持稳定，其他层可以不必修改。即使层的接口发生变化，也只影响上层和下层，修改工作量小而且错误可以控制，不会带来意外的风险。NFrame同时使用了将应用程序设计成三层架构，最顶层是App，中间层是各种插件，插件下是各种对应的具化的模块功能。这种设计的优点是对应模块只处理自己的事务，降低耦合，通过接口与其他模块交互，将模块的风险降到最低。

**App设计架构图**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_app_arch.png)

## 特性详细介绍
### 通用的抽象对象系统(LogicClass)
- 丰富的基础属性类型
- 对象属性的可配置性(Excel可以定义所有属性)
- 对象初始数据的可配置性(Excel可以预设值所有属性的值)
- 可动态增减属性(服务器运行过程中可以程序添加属性)
- 无需在代码中再声明任何业务类(Excel直接声明)
- 通用的设置/获取信息接口

### 数据驱动
相对于传统的服务器开发，NFrame使用了一种全新的数据定义和使用的方法，我们称之为**属性(Property)**和**表(Record)**。

**属性(Property)**主要用来存储用户的基本数据，例如：姓名、性别、年龄、等级 等数据，主要表现为一个名称对应一个数据。

**表(Record)**主要用来存储一些记录，例如：道具列表、任务列表 等数据，主要表现为一个记录里包含多条数据。

NFrame使用了此种模型来定义应用中的所有数据，避免了以往传统服务器中数据结构定义混乱，接口不统一、别人无法接手等问题。

**一个Property和Record的例子：**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Property_Sample.png)
> NF属性配置例子(Excel编辑)

![NF Record Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Record_Sample.png)
> NF表配置例子(Excel编辑)

### 事件驱动
事件驱动灵感来源与处理器的处理流程，旨为只提供流水线式的处理逻辑模块，而本身不保存和涉留对象的数据。

事件驱动包含：**Property驱动**，**Record驱动**，**Event驱动**，**Heartbeat驱动**

通过**Property Driver**，所有只要注册过属性观测者的Processer Function均会得到所关注Property的变化通知，以便做出对应的逻辑处理。

通过**Record Driver**，所有只要注册过Record的观测者的Processer Function均会得到所关注Record的变化通知，以便做出对应的逻辑处理。

通过**Event System**，所有只要注册过Event的观测者的Processer Function均会得到所关注的事件通知Processer可以产生新的事件或属性驱动，以便驱动其他逻辑模块处理逻辑。

通过**Heartbeat System**，所有只要注册过同名心跳的观测者的Processer Function均会定时处理逻辑，以便延时/定时处理逻辑。

**NFrame事件驱动设计图**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Event_driver.png)

**NFrame事件驱动示例代码**

Property驱动示例：
```cpp
m_pKernelModule->AddPropertyCallBack(self, "Level", this, &NFCPropertyModule::OnObjectLevelEvent);

m_pKernelModule->SetPropertyInt(self, "Level", 100);

int NFCPropertyModule::OnObjectLevelEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{
    // do something
    return 0;
}
```
Record驱动代码示例：
```cpp
m_pKernelModule->AddRecordCallBack(self, "TaskList", this, &NFCHeroModule::OnTaskRecordEvent);

int NFCHeroModule::OnHeroRecordEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{
    NF_SHARED_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (nullptr == pObject)
    {
        return 1;
    }

    NF_SHARED_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return 1;
    }

    switch (nOpType)
    {
    case NFIRecord::Add:
        // TODO
        break;
    case NFIRecord::Del:
        // TODO
        break;
    case NFIRecord::UpData:
        // TODO
        break;
    default:
        break;
    }

    return 0;
}
```

Event驱动代码示例
```cpp
m_pEventProcessModule->AddEventCallBack(self, NF_EVENT_DO_SOMETHING, this, &NFCFightValueModule::OnDoSomethingEvent);

int NFCFightValueModule::OnRefreshFightValueEvent(const NFIDENTID& self, const int nEventID, const NFIDataList& var)
{
    // do something
	return 0;
}
```

HeartBeat驱动代码示例：
```cpp
m_pKernelModule->AddHeartBeat(self, "OnHeartBeat", this, &HelloWorld3Module::OnHeartBeat, 5.0f, 1000);

int HelloWorld3Module::OnHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    // do something
	return 0;
}
```

### 面向接口编程(IOD)
&#160; &#160; &#160; &#160;较于大多数OO式开发，NFrame支持更灵活的IO(接口)式开发，让你的开发更简单纯粹。
通过模块抽象基类的虚接口让模块的功能互相调用，真正做到了软件开发的低耦合高内聚。

**NFrame面向接口编程示例代码**
```cpp
class NFISceneProcessModule
    : public NFILogicModule
{

public:
    virtual bool IsCloneScene(const int nSceneID) = 0;
    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID) = 0;
};
```

### 高性能，高并发
NFrame由于设计上的分层独立从而使得架构上本身就性能较高。同时在网络通信上使用了久经考验的**LibEvent**作为网络底层，使用**google ProtoBuf**作为协议序列化库，LibEvent的高性能加上Protobuf的高压缩率，真实测试过单服承载8000以上用户高频率协议通讯。

NFrame在逻辑处理上使用了**Actor模**式，采用了**Theron**作为Actor基础类库，支持并发编程，充分利用CPU性能。 Theron作为知名工业级并发库，应用在许多工业级软件上，例如Matlab使用了Theron后，让其性能直接提高了**4.5x倍**([原文链接](http://www.theron-library.com/index.php?t=story&p=43))。

NFrame使用C++作为基础开发语言，相对于其他编程语言，在性能和效率上更是快人一步，良好的设计模式的应用让逻辑更加简单。

### 组件系统(Component)
- 提供类似Unity样式的组件组合模式，以丰富服务器后期脚本编辑以及对象行为扩展
- 系统在对象产生时自动识别以及实例化Component
- 通过事件系统，向Component注入事件消息，便于处理逻辑
- 特殊需求的特殊处理，灵活拓展对象的行为
- 不用关心其他模块，降低耦合
- 当前已经支持lua作为脚本嵌入到框架中使用(详见教程6)
- 后期将支持JavaScript，C#，python等脚本语言

### 分布式服务器架构
经过我们参考主流架构以及长期的项目经验积累，NFrame被设计为理论上可无限扩展的分布式架构，拥有着真正的无理论上限的系统规模和负载可扩展性。

**主要特性：**
1. 无限扩展
2. 高可靠性
3. 简易通讯
4. 业务去中心化
5. 自动摘除问题节点
6. 节点实时权重运算和评估
7. 负载均衡

服务器架构图：
![NF Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_server_arch.png)

## 使用教程
### 教程1:添加一个模块
```cpp
#include "NFComm/NFPluginModule/NFIPlugin.h"
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
```
```cpp
#include "HelloWorld1.h"

bool HelloWorld1::Init()
{
    //初始化
    std::cout << "Hello, world1, Init" << std::endl;
    return true;
}

bool HelloWorld1::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world1, AfterInit" << std::endl;
    return true;
}

bool HelloWorld1::Execute( const float fLasFrametime, const float fStartedTime )
{
    //每帧执行
    //std::cout << "Hello, world1, Execute" << std::endl;
    return true;
}

bool HelloWorld1::BeforeShut()
{
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;
	
    //反初始化之前
    std::cout << "Hello, world1, BeforeShut" << std::endl;
    return true;
}

bool HelloWorld1::Shut()
{
    //反初始化
    std::cout << "Hello, world1, Shut" << std::endl;
    return true;
}

```

### 教程2:数据驱动
### 教程3:事件系统
### 教程4:异步事件系统，actor
# 结束
