// -------------------------------------------------------------------------
//    @FileName			:    NFIResponse.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFIResponse
//
// -------------------------------------------------------------------------

#ifndef NFI_RESPONSE_H
#define NFI_RESPONSE_H

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "Dependencies/ajson/ajson.hpp"

class NFIResponse
{
public:
	enum ResponseType
	{
		RES_TYPE_SUCCESS,
		RES_TYPE_FAILED,
		RES_TYPE_AUTH_FAILED,
	};

public:

	ResponseType code;
	std::string message;

};

AJSON(NFIResponse, code, message)
#endif