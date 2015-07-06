// -------------------------------------------------------------------------
//    @FileName         :    NFIObjectSaveModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFIObjectSaveModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFI_OBJECTSAVE_MODULE_H_
#define _NFI_OBJECTSAVE_MODULE_H_

#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIObjectSaveModule : public NFILogicModule
{
public:
    virtual bool RegisterAutoSave(const std::string& strClassName) = 0;
    virtual const bool LoadDataFormNoSql( const NFIDENTID& self, const std::string& strClassName) = 0;
    virtual const bool SaveDataToNoSql( const NFIDENTID& self) = 0;

};

#endif