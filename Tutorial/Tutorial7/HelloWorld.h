// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld1.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-19
//    @Module           :	 HelloWorld1
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD_H
#define NFC_HELLO_WORLD_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINavigationModule.h"

/*
IN THIS PLUGIN:
YOU WILL KNOW HOW TO USE THE "IScriptModule" TO CALL LUA SCRIPT
*/


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
