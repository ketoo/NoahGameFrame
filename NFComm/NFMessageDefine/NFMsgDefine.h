// -------------------------------------------------------------------------
//    @FileName			:   NFMsgDefine.cphp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFMsgDefine
// -------------------------------------------------------------------------

#ifndef NF_MSG_DEFINE_H
#define NF_MSG_DEFINE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFList.hpp"
#include "NFLimit.h"
#include "NFDefine.pb.h"
#include "NFMsgBase.pb.h"
#include "NFMsgPreGame.pb.h"
#include "NFMsgShare.pb.h"
#include "NFMsgExtra.pb.h"
#include "NFMsgFriend.pb.h"
#include "NFMsgTeam.pb.h"
#include "NFProtocolDefine.hpp"

#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
// -------------------------------------------------------------------------
#pragma pack(push,1)

enum E_CHECK_TYPE
{
    ECT_SAVE        = 0, 
    ECT_PRIVATE     = 1, 
    ECT_PUBLIC      = 2, 
};

// bool RecordToString( NFIRecord* pRecord, NFMsg::ObjectRecordBase& recordBase, E_CHECK_TYPE nCheckType, const bool bCheck );
// bool RecordToString( NFIRecordManager* pRecordManager, NFMsg::ObjectRecordList& recordList, E_CHECK_TYPE nCheckType, const bool bCheck );
//
// bool RecordFormString( NFIRecord* pRecord, const NFMsg::ObjectRecordBase& recordBase );
// bool RecordFormString( NFIRecordManager* pRecordManager, const NFMsg::ObjectRecordList& recordList );
//
// bool PropertyToString( NFIPropertyManager* pProManager, NFMsg::ObjectPropertyList& propertyDB, E_CHECK_TYPE nCheckType, const bool bCheck );
// bool PropertyListToString( NFList<NFIPropertyManager*>& pProManagerList,NFMsg::MultiObjectPropertyList& propertyDB, E_CHECK_TYPE nCheckType, const bool bCheck );
//
// bool PropertyFormString( NFIPropertyManager* pProManager, const NFMsg::ObjectPropertyList& propertyDB );

#pragma pack(pop)
#endif
