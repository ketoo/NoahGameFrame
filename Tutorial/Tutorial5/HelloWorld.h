// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD_H
#define NFC_HELLO_WORLD_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIHelloWorld
	: public NFIModule
{

};

class NFCHelloWorld
    : public NFIHelloWorld
{
public:
    NFCHelloWorld(NFIPluginManager* p)
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
