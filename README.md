NoahGameFrame
=============
######Copyright © http://www.yowoyo.com

##What is it?

NF is a lightweight, fast, scalable, distributed plugin framework.

##The features:
-------------------

<p>1: it is easy to use interface oriented design minimise the effort;</p>
<p>2: extensible plugin framework makes getting your application running is quick and simple;</p>
<p>3: clean, uncluttered design, stable engine used in several commercial products;</p>
<p>4: using the actor model has very high performance(by theron);</p>
<p>5: based on the event-driven and attribute-driver can make business more clearly and easy to maintenance;</p>
<p>6: based on the standard c + + development, cross-platform support;</p>
<p>7: with existing c++, c# game client for rapid development;</p>
<p>8: Cross-platform,;</p>


##Get the Sources:
-------------------

git clone https://github.com/ketoo/NoahGameFrame.git

or

svn checkout https://github.com/ketoo/NoahGameFrame


##Dependencies:
-------------------

Need build sub librares:

* Boost.System
* Boost.Regex
* Boost.DateTime
* Boost.Timer
* Boost.Chrono
* Boost.Thread

##Supported Compilers
-------------------

* GCC >= 4.8
* VC >= 9.0 (sp1)

##How to build:
-------------------


##Tutorial:
-------------------
Hello world
-----------

```cpp

// -------------------------------------------------------------------------
//    @FileName      :    HelloWorld1.h
//    @Author           :    ketoo
//    @Date             :    2014-05-14 08:51
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

    system("PAUSE");

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
















