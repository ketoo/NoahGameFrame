#pragma once
// -------------------------------------------------------------------------
//    @FileName			:    NFResponseLogin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFResponseLogin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIResponse.h"

class NFResponseLogin : public NFIResponse
{
public:
	std::string jwt;

};

AJSON(NFResponseLogin, jwt, code, message)