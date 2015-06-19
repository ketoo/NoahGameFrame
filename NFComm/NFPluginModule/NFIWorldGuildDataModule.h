// -------------------------------------------------------------------------
//    @FileName         :    NFIWorldGuildDataModule.h
//    @Author           :    flyicegood
//    @Date             :    2015-06-06 16:51
//    @Module           :    NFIWorldGuildDataModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLD_GUILD_DATA_MODULE_H_
#define _NFI_WORLD_GUILD_DATA_MODULE_H_

#include "NFILogicModule.h"

class NFIWorldGuildDataModule
    : public NFILogicModule
{
public:

	virtual bool ExitGuild(const NFIDENTID& self, const std::string& strName, bool& bExit) = 0;
    virtual void CheckLoadGuild(const NFIDENTID& self, const NFIDENTID& xGuild) = 0;

    virtual const NFIDENTID CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName) = 0;
    virtual const bool DeleteGuild(const NFIDENTID& xGuild) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFIDENTID& xGuild) = 0;

    virtual bool GetGuild(const NFIDENTID& self, NFIDENTID& xGuild) = 0;
    virtual bool SetGuild( const NFIDENTID& self, const NFIDENTID& xGuild ) = 0;

protected:
private:
};

#endif