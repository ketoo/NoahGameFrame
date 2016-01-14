// -------------------------------------------------------------------------
//    @FileName      :    NFIWorldChatGroupModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFIWorldChatGroupModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLD_CHAT_GROUP_MODULE_H
#define _NFI_WORLD_CHAT_GROUP_MODULE_H

#include "NFILogicModule.h"

class NFIWorldChatGroupModule
    : public NFILogicModule
{
public:
    virtual bool JoinGroup(const NFGUID& self, const NFGUID& xGroupID) = 0;
    virtual const NFGUID CreateGroup(const NFGUID& self) = 0;
    virtual bool QuitGroup(const NFGUID& self, const NFGUID& xGroupID) = 0;
    virtual bool DeleteGroup(const NFGUID& self, const NFGUID& xGroupID) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGroup(const NFGUID& self) = 0;
    virtual bool GetOnlineMember( const NFGUID& self, const NFGUID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList) = 0;
    virtual bool Online(const NFGUID& self, const NFGUID& xGroupID, const int& nGameID) = 0;
    virtual bool Offeline(const NFGUID& self, const NFGUID& xGroupID) = 0;
protected:

private:
};

#endif