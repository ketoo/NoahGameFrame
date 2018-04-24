// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpNetModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFCHttpNetModule
//
// -------------------------------------------------------------------------

#include "NFCHttpServerModule.h"

NFCHttpServerModule::NFCHttpServerModule(NFIPluginManager* p)
{
    pPluginManager = p;
    m_pHttpServer = NULL;
}

NFCHttpServerModule::~NFCHttpServerModule()
{
    if (m_pHttpServer)
    {
        m_pHttpServer->Final();
        delete m_pHttpServer;
        m_pHttpServer = NULL;
    }
}

bool NFCHttpServerModule::Execute()
{
    if (m_pHttpServer)
    {
        m_pHttpServer->Execute();
    }

    return true;
}

int NFCHttpServerModule::InitServer(const unsigned short nPort)
{
    m_pHttpServer = new NFCHttpServer(this, &NFCHttpServerModule::OnReceiveNetPack);
    std::cout << "Open http port:" << nPort << std::endl;
    return m_pHttpServer->InitServer(nPort);
}

bool NFCHttpServerModule::OnReceiveNetPack(const NFHttpRequest& req)
{
    auto it = mMsgCBMap.find(req.path);
    if (mMsgCBMap.end() != it)
    {
        HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
        HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
        pFunc->operator()(req);
    }
    else
    {
        if (mComMsgCBList.size() > 0)
        {
            for (std::list<HTTP_RECEIVE_FUNCTOR_PTR>::iterator it = mComMsgCBList.begin();
                 it != mComMsgCBList.end(); ++it)
            {
                HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
                HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
                pFunc->operator()(req);
            }
        }
		else
        {
            std::cout << "no http receiver:" << req.path << std::endl;
        }
    }

	return true;
}

bool NFCHttpServerModule::AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
    if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
    {
        mMsgCBMap.insert(std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));
        return true;
    }
    return false;
}

bool NFCHttpServerModule::AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
    mComMsgCBList.push_back(cb);
    return true;
}

bool NFCHttpServerModule::AddFilterCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
    m_pHttpServer->AddFilter(cb);
    return true;
}

bool NFCHttpServerModule::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
                                      const std::string& strReason)
{
    return m_pHttpServer->ResponseMsg(req, strMsg, code, strReason);
}
