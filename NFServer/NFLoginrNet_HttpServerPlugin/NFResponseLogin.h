#pragma once
// -------------------------------------------------------------------------
//    @FileName			:    NFResponsetLogin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFResponsetLogin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIResponse.h"

class NFResponsetLogin : public NFIResponse
{
public:
	std::string jwt;

};

AJSON(NFResponsetLogin, jwt, code, message)