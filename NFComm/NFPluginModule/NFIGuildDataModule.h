// -------------------------------------------------------------------------
//    @FileName         :    NFIGuildDataModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 16:51
//    @Module           :    NFIGuildDataModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GUILD_DATA_MODULE_H
#define NFI_GUILD_DATA_MODULE_H

#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIGuildDataModule
    : public NFIModule
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

        NFGUID      mxGuildID    ;
        std::string     mstrGuildName  ;
        std::string     mnGuildIcon  ;
        NFINT32         mnGuildMemberCount  ;
        NFINT32         mnGuildMemberMaxCount  ;
        NFINT32         mnGuildHonor ;
        NFINT32         mnGuildRank  ;
    };

    virtual bool ExitGuild(const NFGUID& self, const std::string& strName, bool& bExit) = 0;
    virtual void CheckLoadGuild(const NFGUID& self, const NFGUID& xGuild) = 0;
    virtual bool GetPlayerInfo(const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP) = 0;

    virtual const NFGUID CreateGuild(const NFGUID& xPlayeID, const std::string& strName, const std::string& strRoleName,
                                     const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1, const int nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT) = 0;
    virtual const bool DeleteGuild(const NFGUID& xGuild) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFGUID& xGuild) = 0;

    virtual bool GetPlayerGuild(const NFGUID& self, NFGUID& xGuild) = 0;
    virtual bool SearchGuild(const NFGUID& self, const std::string& strName, std::vector<SearchGuildObject>& xList) = 0;
    virtual bool GetPlayerGameID(const NFGUID& self, int& nGameID) = 0;

protected:
private:
};

#endif