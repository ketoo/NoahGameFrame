//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H
#define NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H


/*
状态回复（status reply）的第一个字节是 "+"           //For Simple Strings the first byte of the reply is "+"
错误回复（error reply）的第一个字节是 "-"            //For Errors the first byte of the reply is "-"
整数回复（integer reply）的第一个字节是 ":"          //For Integers the first byte of the reply is ":"
批量回复（bulk reply）的第一个字节是 "$"             //For Bulk Strings the first byte of the reply is "$"
多条批量回复（multi bulk reply）的第一个字节是 "*"    //For Arrays the first byte of the reply is "*"
 */
enum NFREDIS_RESULT_STATUS
{
    NFREDIS_RESULT_STATUS_OK,
    NFREDIS_RESULT_STATUS_UNKNOW,
    NFREDIS_RESULT_STATUS_IMCOMPLETE,
};

enum NFREDIS_RESP_TYPE
{
    NFREDIS_RESP_UNKNOW,
    NFREDIS_RESP_NIL,
    NFREDIS_RESP_STATUS,
    NFREDIS_RESP_ERROR,
    NFREDIS_RESP_INT,
    NFREDIS_RESP_BULK,
    NFREDIS_RESP_ARRAY,
};

static const std::string NFREDIS_CRLF = "\r\n";	///结束标志 terminated by CRLF
static const int NFREDIS_SIZEOF_CRLF = 2;	///the size of CRLF
static const std::string NFREDIS_STATUS_OK = "OK";

static const char* NFREDIS_STATUS_REPLY = "+";    //状态回复（status reply）的第一个字节是 "+"
static const char* NFREDIS_ERROR_REPLY = "-";	    //错误回复（error reply）的第一个字节是 "-"
static const char* NFREDIS_INT_REPLY = ":";	    //整数回复（integer reply）的第一个字节是 ":"
static const char* NFREDIS_BULK_REPLY = "$";	    //批量回复（bulk reply）的第一个字节是 "$"
static const char* NFREDIS_ARRAY_REPLY = "*";	    //多条批量回复（multi bulk reply）的第一个字节是 "*"

#define GET_NAME(functionName)   (#functionName)

#endif //NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H
