// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpNetModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFCHttpNetModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HTTPNET_MODULE_H
#define NFC_HTTPNET_MODULE_H

#include <iostream>
#include "NFComm/NFNetPlugin/NFCHttpNet.h"
#include "NFComm/NFPluginModule/NFIHttpNetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCHttpNetModule
	: public NFIHttpNetModule
{

public:
	NFCHttpNetModule(NFIPluginManager* p);
	virtual ~NFCHttpNetModule();
	
public:

	virtual int InitServer(const unsigned short nPort);
	virtual bool Execute();

	virtual bool AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR_PTR& cb);
	virtual bool AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb);

	virtual bool ResponseMsg(const NFHttpRequest & req, const int nCommand, const std::string& strMsg);
	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK");
	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName);

private:
	void OnReceiveNetPack(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl);

private:
	NFILogModule* mLogModule;
	NFIHttpNet* mpHttpNet;

	std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
	std::list<HTTP_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};

#endif
