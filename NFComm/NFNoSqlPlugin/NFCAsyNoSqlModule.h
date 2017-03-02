// -------------------------------------------------------------------------
//    @FileName			:    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_NOSQL_MODULE_H
#define NFC_ASY_NOSQL_MODULE_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIAsyNoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFINoSqlDriverManager.h"

class NFCNoSqlComponent : public NFIComponent
{
public:
	NFCNoSqlComponent(NFGUID self, const std::string& strName) : NFIComponent(self, strName)
	{
	}

	NFCNoSqlComponent(NFIPluginManager* pPluginManager) : NFIComponent(NFGUID(), "")
	{
	}

	virtual ~NFCNoSqlComponent()
	{

	}

	virtual bool Init();
	virtual bool AfterInit();
	virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);

protected:
	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();

	NF_SHARE_PTR<NFINoSqlDriverManager> m_pNoSqlDriverManager;
};

class NFCAsyNoSqlModule
    : public NFIAsyNoSqlModule
{
public:

	NFCAsyNoSqlModule(NFIPluginManager* p);
    virtual ~NFCAsyNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	virtual bool StartActorPool(const int nCount);
	virtual bool CloseActorPool();

	int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

protected:

	NFMapEx<int, int> mActorList; //actorid <-->Used


	NFIActorModule* m_pActorModule;
};

#endif