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
#include "NFComm/NFNetPlugin/NFCHttpServer.h"
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

    virtual bool AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool ResponseMsg(const NFHttpRequest& req, const int nCommand, const std::string& strMsg);

    virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK,
                const std::string& reason = "OK");

    virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName);

private:
    bool OnReceiveNetPack(const NFHttpRequest& req);

private:
    NFILogModule* mLogModule;
    NFIHttpServer* m_pHttpServer;

    std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
    std::list<HTTP_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};

#endif
