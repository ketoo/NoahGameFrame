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
#include "NFComm/NFMessageDefine/NFRecordDefine.pb.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIWorldGuildDataModule
    : public NFILogicModule
{
public:   
    struct SearchGuildObject
    {
        SearchGuildObject()
        {
            mnGuildMemberCount  = 0;
            mnGuildMemberMaxCount  = 0;
            mnGuildHonor = 0;
            mnGuildRank  = 0;
        }

        NFIDENTID		mxGuildID    ;
        std::string		mstrGuildName  ;
        std::string		mnGuildIcon  ;
        NFINT32		    mnGuildMemberCount  ;
        NFINT32		    mnGuildMemberMaxCount  ;
        NFINT32		    mnGuildHonor ;
        NFINT32		    mnGuildRank  ;
    };

	virtual bool ExitGuild(const NFIDENTID& self, const std::string& strName, bool& bExit) = 0;
    virtual void CheckLoadGuild(const NFIDENTID& self, const NFIDENTID& xGuild) = 0;
    virtual bool GetPlayerInfo(const NFIDENTID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP) = 0;

    virtual const NFIDENTID CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName, const std::string& strRoleName, 
        const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1, const int nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT) = 0;
    virtual const bool DeleteGuild(const NFIDENTID& xGuild) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFIDENTID& xGuild) = 0;

    virtual bool GetGuild(const NFIDENTID& self, NFIDENTID& xGuild) = 0;
    virtual bool SetGuild( const NFIDENTID& self, const NFIDENTID& xGuild ) = 0;
    virtual bool SearchGuild(const NFIDENTID& self, const std::string& strName, std::vector<SearchGuildObject>& xList) = 0;

protected:
private:
};

#endif