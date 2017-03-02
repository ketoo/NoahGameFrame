// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld1.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld1
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD1_H
#define NFC_HELLO_WORLD1_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIHelloWorld1
	: public NFIModule
{
};

class NFCHelloWorld1
    : public NFIHelloWorld1
{
public:
    NFCHelloWorld1(NFIPluginManager* p)
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
