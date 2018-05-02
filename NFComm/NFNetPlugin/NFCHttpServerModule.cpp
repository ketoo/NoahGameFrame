// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpNetModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-06-17
//    @Module           :   NFCHttpNetModule
//
// -------------------------------------------------------------------------

#include "NFCHttpServer.h"
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

	if (mFilter)
	{
		m_pHttpServer->AddFilter(mFilter);
	}

    return m_pHttpServer->InitServer(nPort);
}

bool NFCHttpServerModule::OnReceiveNetPack(const NFHttpRequest& req)
{
	bool bHandled = false;
	auto it = mMsgCBMap.GetElement(req.type);
	if (it)
	{
		auto itPath = it->find(req.path);
		if (it->end() != itPath)
		{
			HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = itPath->second;
			HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(req);

			bHandled = true;
		}
	}

    if (!bHandled)
    {
        if (mComMsgCBList.size() > 0)
        {
			for (auto it : mComMsgCBList)
			{
				HTTP_RECEIVE_FUNCTOR* pFunc = it.get();
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

bool NFCHttpServerModule::AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
	auto it = mMsgCBMap.GetElement(eRequestType);
	if (!it)
	{
		mMsgCBMap.AddElement(eRequestType, NF_SHARE_PTR<std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>>(NF_NEW std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>()));
	}

	it = mMsgCBMap.GetElement(eRequestType);
	auto itPath = it->find(strCommand);
	if (it->end() == itPath)
	{
		it->insert(std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));

		return true;
	}
	else
	{
		std::cout << eRequestType << " " << strCommand << "" << std::endl;
	}

    return false;
}

bool NFCHttpServerModule::AddComMsgCB(const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
    mComMsgCBList.push_back(cb);
    return true;
}

bool NFCHttpServerModule::AddFilterCB(const HTTP_FILTER_FUNCTOR_PTR& cb)
{
	mFilter = cb;
	if (m_pHttpServer)
	{
		m_pHttpServer->AddFilter(cb);
	}

    return true;
}

bool NFCHttpServerModule::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
                                      const std::string& strReason)
{
    return m_pHttpServer->ResponseMsg(req, strMsg, code, strReason);
}
