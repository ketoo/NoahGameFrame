// -------------------------------------------------------------------------
//    @FileName         £º    NFIDataProcessModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-10-03
//    @Module           £º    NFIDataProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATAPROCESS_MODULE_H
#define _NFI_DATAPROCESS_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFIActorDataModule.h"

class NFIDataProcessModule
    : public NFIActorDataModule
{

public:

	virtual const NFGUID& CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex) = 0;
	virtual const bool DeleteRole(const std::string& strAccount, const NFGUID xID) = 0;
	virtual const NFGUID& GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVeec) = 0;

	virtual bool RegisterAutoSave(const std::string& strClassName) = 0;
	virtual const bool LoadDataFormSql( const NFGUID& self , const std::string& strClassName) = 0;
	virtual const bool SaveDataToSql( const NFGUID& self) = 0;

};

#endif