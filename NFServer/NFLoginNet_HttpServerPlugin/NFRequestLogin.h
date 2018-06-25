#pragma once
// -------------------------------------------------------------------------
//    @FileName			:    NFResponseLogin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-04-29
//    @Module           :    NFResponseLogin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIRequest.h"

class NFRequestLogin : public NFIRequest
{
public:
	std::string user;
	std::string password;

};

AJSON(NFRequestLogin, user, password)