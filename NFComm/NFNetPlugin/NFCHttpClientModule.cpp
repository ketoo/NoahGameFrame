// -------------------------------------------------------------------------
//    @FileName      	:   NFCHttpClientModul.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-07-07
//    @Module           :   NFCHttpClientModule
//
// -------------------------------------------------------------------------

#include "NFCHttpClientModule.h"
#include "NFCHttpClient.h"

#define DEFAULT_USER_AGENT "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Mobile Safari/537.36"

NFCHttpClientModule::NFCHttpClientModule(NFIPluginManager* p)
{
    pPluginManager = p;
    m_pHttpClient = new NFCHttpClient();
    m_xDefaultHttpHeaders =
            {
                    {"Connection",    "close"},
                    {"Content-Type",  "text/plain;text/html;application/x-www-form-urlencoded;charset=utf-8"},
                    {"User-Agent", DEFAULT_USER_AGENT},
                    {"Cache-Control", "no-cache"}
            };
}

NFCHttpClientModule::~NFCHttpClientModule()
{
    delete m_pHttpClient;
    m_pHttpClient = NULL;
}

bool NFCHttpClientModule::Init()
{
    m_pHttpClient->Init();

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

bool NFCHttpClientModule::PerformGet(const std::string& strUri,
                                     const std::map<std::string, std::string>& xHeaders,
                                     HTTP_RESP_FUNCTOR_PTR pCB,
                                     const std::string& strUserData)
{
    return m_pHttpClient->PerformGet(strUri, pCB, strUserData, xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}

bool NFCHttpClientModule::PerformPost(const std::string& strUri,
                                      const std::map<std::string, std::string>& xHeaders,
                                      const std::string& strPostData,
                                      HTTP_RESP_FUNCTOR_PTR pCB,
                                      const std::string& strUserData)
{
    return m_pHttpClient->PerformPost(strUri, strPostData, pCB, strUserData,
                                      xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}
