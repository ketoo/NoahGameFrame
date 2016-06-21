// -------------------------------------------------------------------------
//    @FileName			:    NFCUrlCode.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCUrlCode
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_URLCODE_H
#define NFC_URLCODE_H

#include <string>
#include "NFComm/NFPluginModule/NFIUrlCodeModule.h"

class NFCUrlCodeModule : public NFIUrlCodeModule
{
public:
    NFCUrlCodeModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

public:
    enum ErrorCode
    {
        ERR_NULL_PARAMS = -100,
        ERR_BUFFER_TOO_SMALL = -99,
        ERR_ENCODE = -98,
        OK = 0
    };

    enum CharsetCheck
    {
        NO_CHECK = 0,
        UTF8_CHECK = 1,
        GBK_CHECK = 2
    };

    struct EncodeURIDefineNode
    {
        size_t len;
        char ta[4];
    };


    struct DecodeURIDefineNode
    {
        int highVal;
        int lowVal;
    };

    virtual std::string EncodeURIValue(const std::string& strSource);
    virtual std::string DecodeURIValue(const std::string& strSource);

public:
    int EncodeURIValue(std::string& strResult, const std::string& pSourceBuffer);
    int EncodeURIValue(char* pResultBuffer, const char* pSourceBuffer, size_t nResultBufferSize);

    int DecodeURIValue(std::string& strResult, const std::string& strSourceBuffer);
    int DecodeURIValue(char* pResultBuffer, const char* strSourceBuffer, size_t nResultBufferSize);

private:
    static EncodeURIDefineNode mxURIEncodeMap[256];
    static DecodeURIDefineNode mxURIDecodeMap[103];
};

#endif
