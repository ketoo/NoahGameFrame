/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NFI_PLAYER_MYSQL_MODULE_H
#define NFI_PLAYER_MYSQL_MODULE_H

#include <iostream>
#include "NFIModule.h"

typedef std::function<void(const NFGUID& self, const int nRet, const std::string&strUseData)> LOADDATA_RETURN_FUNCTOR;


class NFIPlayerMysqlModule
    : public NFIModule
{

public:

	virtual const NFGUID CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex) = 0;
	virtual const bool DeleteRole(const std::string& strAccount, const NFGUID xID) = 0;
	virtual const NFGUID GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVeec) = 0;

	virtual bool RegisterAutoSave(const std::string& strClassName) = 0;
	virtual const bool LoadDataFormSql( const NFGUID& self , const std::string& strClassName) = 0;
	virtual const bool SaveDataToSql( const NFGUID& self) = 0;

    template<typename BaseType>
    bool LoadDataFormSqlAsy(const NFGUID& self, const std::string& strClassName, BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::string&),  const std::string&strUseData)
    {
        const LOADDATA_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        return LoadDataFormSqlAsy(self, strClassName, FunReturnRsp, strUseData);
    }

private:
    virtual const bool LoadDataFormSqlAsy( const NFGUID& self , const std::string& strClassName, const LOADDATA_RETURN_FUNCTOR& xFun, const std::string& strUseData) = 0;


};

#endif