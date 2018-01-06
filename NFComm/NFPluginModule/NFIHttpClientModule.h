// -------------------------------------------------------------------------
//    @FileName      	:   NFIHttpClientModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2017-07-07
//    @Module           :   NFIHttpClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_HTTP_CLIENT_MODULE_H
#define NFI_HTTP_CLIENT_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFNetPlugin/NFIHttpClient.h"

class NFIHttpClientModule
    : public NFIModule
{
public:
    virtual ~NFIHttpClientModule() {};

    template<typename BaseType>
    bool DoGet ( const std::string& strUri,BaseType* pBase,
						void ( BaseType::*handleRecieve ) ( const int state_code, const std::string& strRespData ) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2 ) ) );
        return DoGet ( strUri, std::map<std::string, std::string>(), pd );
    }
    template<typename BaseType>
    bool DoGet ( const std::string& strUri, const std::map<std::string, std::string>& xHeaders,
                      BaseType* pBase,
                      void ( BaseType::*handleRecieve ) ( const int state_code, const std::string& strRespData) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2) ) );
        return DoGet( strUri, xHeaders, pd );
    }


    template<typename BaseType>
    bool DoPost ( const std::string& strUri, const std::string& strPostData,
                       BaseType* pBase,
                       void ( BaseType::*handleRecieve ) ( const int state_code, const std::string& strRespData ) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2) ) );
        return DoPost( strUri, std::map<std::string, std::string>(), strPostData, pd );
    }


    template<typename BaseType>
    bool DoPost( const std::string& strUri, const std::string& strPostData, const std::map<std::string, std::string>& xHeaders,
                       BaseType* pBase,
                       void ( BaseType::*handleRecieve ) ( const int state_code, const std::string& strRespData ) )
    {
        HTTP_RESP_FUNCTOR_PTR pd ( new HTTP_RESP_FUNCTOR ( std::bind ( handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2) ) );
        return DoPost( strUri, xHeaders, strPostData, pd );
    }

protected:
    virtual bool DoGet( const std::string& strUrl,
                              const std::map<std::string, std::string>& xHeaders,
                              HTTP_RESP_FUNCTOR_PTR pCB) = 0 ;

    virtual bool DoPost( const std::string& strUrl,
                               const std::map<std::string, std::string>& xHeaders,
                               const std::string& strPostData,
                               HTTP_RESP_FUNCTOR_PTR pCB) = 0;
};

#endif
