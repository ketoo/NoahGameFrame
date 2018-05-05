// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpServerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFCHttpServerModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HTTP_SERVER_MODULE_H
#define NFC_HTTP_SERVER_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFNetPlugin/NFIHttpServer.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpServerModule
        : public NFIHttpServerModule
{

public:
    NFCHttpServerModule(NFIPluginManager* p);

    virtual ~NFCHttpServerModule();

public:

    virtual int InitServer(const unsigned short nPort);

    virtual bool Execute();

    virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK,
                const std::string& reason = "OK");

private:
	virtual bool OnReceiveNetPack(const NFHttpRequest& req);
	virtual NFWebStatus OnFilterPack(const NFHttpRequest& req);

	virtual bool AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR_PTR& cb);
	virtual bool AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR_PTR& cb);

private:
	NFIHttpServer* m_pHttpServer;
	NFILogModule* mLogModule;

	NFMapEx<NFHttpType, std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>> mMsgCBMap;
	std::map<std::string, HTTP_FILTER_FUNCTOR_PTR> mMsgFliterMap;

    HTTP_RECEIVE_FUNCTOR_PTR mComMsgCBList;
};

#endif
