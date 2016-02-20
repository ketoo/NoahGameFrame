#include "HelloWorld7.h"

bool HelloWorld7::Init()
{
    //初始化
    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool HelloWorld7::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world1, AfterInit" << std::endl;

    m_pUrlClientModule = pPluginManager->FindModule<NFIUrlClientModule>("NFCUrlClientModule");

    assert(NULL != m_pUrlClientModule);

    if (!m_pUrlClientModule->StartActorPool(50))
    {
        return false;
    }

    std::string strUrl;
    std::map<std::string, std::string> mxGetParams;
    std::map<std::string, std::string> mxPostParams;
    std::map<std::string, std::string> mxCookies;
    float fTimeOutSec = 30;
    std::string strRsp;


    strUrl = "http://www.baidu.com";
    mxGetParams["accounttype"]="common";
    mxGetParams["appid"]="598844";

    mxCookies["session_id"]="openid";

    std::ostringstream osstreamParams;
    osstreamParams<< strUrl <<" http Get Params: ";

    for (std::map<std::string, std::string>::iterator iter = mxGetParams.begin(); iter != mxGetParams.end(); ++iter)
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";

    }

    osstreamParams <<"Post Params: ";
    for (std::map<std::string, std::string>::iterator iter = mxPostParams.begin(); iter != mxPostParams.end(); ++iter)
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";
    }

    osstreamParams <<"Cookies Params: ";
    for (std::map<std::string, std::string>::iterator iter = mxCookies.begin(); iter != mxCookies.end(); iter ++ )
    {
        osstreamParams << iter->first << ":"<< iter->second<<" ";
    }

    std::string strUseData ="test data";

    m_pUrlClientModule->HttpRequestPostAs(NFGUID(2,3), strUrl, mxGetParams, mxPostParams, mxCookies, fTimeOutSec, this, &HelloWorld7::handleRsp, strUseData);
    return true;
}

bool HelloWorld7::Execute()
{
    //每帧执行
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

void HelloWorld7::handleRsp(const NFGUID& self , const int nRet, const std::string& strData, const std::string&strUseData)
{
    std::cout << "Ret " << nRet << " Result Data:" << strData << " Use Data:" << strUseData;
}

bool HelloWorld7::BeforeShut()
{
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;

    system("PAUSE");

    //反初始化之前
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld7::Shut()
{
    //反初始化
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}
