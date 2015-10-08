// -------------------------------------------------------------------------
//    @FileName      :    NFIWorldChatGroupModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFIWorldChatGroupModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLD_CHAT_GROUP_MODULE_H_
#define _NFI_WORLD_CHAT_GROUP_MODULE_H_

#include "NFILogicModule.h"

class NFIWorldChatGroupModule
    : public NFILogicModule
{
public:
    virtual bool JoinGroup(const NFIDENTID& self, const NFIDENTID& xGroupID) = 0;
    virtual const NFIDENTID& CreateGroup(const NFIDENTID& self) = 0;
    virtual bool QuitGroup(const NFIDENTID& self, const NFIDENTID& xGroupID) = 0;
    virtual bool DeleteGroup(const NFIDENTID& self, const NFIDENTID& xGroupID) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGroup(const NFIDENTID& self) = 0;
    virtual bool GetOnlineMember( const NFIDENTID& self, const NFIDENTID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList) = 0;
    virtual bool Online(const NFIDENTID& self, const NFIDENTID& xGroupID, const int& nGameID) = 0;
    virtual bool Offeline(const NFIDENTID& self, const NFIDENTID& xGroupID) = 0;
protected:

private:
};

#endif