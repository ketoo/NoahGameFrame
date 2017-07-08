// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpClientModul.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-07-07
//    @Module           :   NFCHttpClientModule
//
// -------------------------------------------------------------------------

#include "NFCHttpClientModule.h"
#include "NFCHttpClient.h"

NFCHttpClientModule::NFCHttpClientModule(NFIPluginManager * p)
{
	pPluginManager = p;
	m_pHttpClient = new NFCHttpClient();
}

NFCHttpClientModule::~NFCHttpClientModule()
{
	delete m_pHttpClient;
	m_pHttpClient = NULL;
}

bool NFCHttpClientModule::Init()
{
	m_pHttpClient->Initialization();

	return true;
}

bool NFCHttpClientModule::Execute()
{
	m_pHttpClient->Execute();
	return true;
}

bool NFCHttpClientModule::Shut()
{
	m_pHttpClient->Final();

	return true;
}

bool NFCHttpClientModule::PerformGet(const std::string & strUri, const std::string & strUserData, HTTP_RESP_FUNCTOR_PTR pCB)
{
	return m_pHttpClient->PerformGet(strUri, strUserData, pCB);
}

bool NFCHttpClientModule::PerformPost(const std::string & strUri, const std::string & strUserData, const std::string & strPostData, HTTP_RESP_FUNCTOR_PTR pCB)
{
	return m_pHttpClient->PerformPost(strUri, strUserData, strPostData, pCB);
}
