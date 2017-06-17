// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpNetModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFCHttpNetModule
//
// -------------------------------------------------------------------------

#include "NFCHttpNetModule.h"

NFCHttpNetModule::NFCHttpNetModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mpHttpNet = NULL;
}
NFCHttpNetModule::~NFCHttpNetModule()
{
	if (mpHttpNet)
	{
		mpHttpNet->Final();
		delete mpHttpNet;
		mpHttpNet = NULL;
	}
}

bool NFCHttpNetModule::Execute()
{
	if (mpHttpNet)
	{
		mpHttpNet->Execute();
	}

	return true;
}

int NFCHttpNetModule::InitServer(const unsigned short nPort)
{
	mpHttpNet = new NFCHttpNet(this, &NFCHttpNetModule::OnReceiveNetPack);
	std::cout << "Open http port:" << nPort << std::endl;
	return mpHttpNet->InitServer(nPort);
}

void NFCHttpNetModule::OnReceiveNetPack(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl)
{
	auto it = mMsgCBMap.find(strCommand);
	if (mMsgCBMap.end() != it)
	{
		HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(req, strCommand, strUrl);
	}
	else
	{
		if (mComMsgCBList.size() > 0)
		{
			for (std::list<HTTP_RECEIVE_FUNCTOR_PTR>::iterator it = mComMsgCBList.begin(); it != mComMsgCBList.end(); ++it)
			{
				HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
				HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
				pFunc->operator()(req, strCommand, strUrl);
			}
		}
		else
		{
			std::cout << "no http receiver:" << strCommand << std::endl;
		}
	}
}

bool NFCHttpNetModule::AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
	if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
	{
		mMsgCBMap.insert(std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));
		return true;
	}
	return false;
}

bool NFCHttpNetModule::AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
	mComMsgCBList.push_back(cb);
	return true;
}

bool NFCHttpNetModule::ResponseMsg(const NFHttpRequest & req, const int nCommand, const std::string & strMsg)
{
	return false;
}

bool NFCHttpNetModule::ResponseMsg(const NFHttpRequest & req, const std::string& strMsg, NFWebStatus code, const std::string& strReason)
{
	return mpHttpNet->ResponseMsg(req, strMsg, code, strReason);
}

bool NFCHttpNetModule::ResponseFile(const NFHttpRequest & req, const std::string & strPath, const std::string & strFileName)
{
	return mpHttpNet->ResponseFile(req, strPath, strFileName);
}