// -------------------------------------------------------------------------
//    @FileName      ：    NFMsgDefine.h
//    @Author      ：    LvSheng.Huang
//    @Date    ：    2010-10-5 12:05
//    @Module    ：    消息协议声明
//
// -------------------------------------------------------------------------

#ifndef _NF_MSGDEFINE_H_
#define _NF_MSGDEFINE_H_

#include "NFLimit.h"
#include "NFDefine.pb.h"
#include "NFMsgBase.pb.h"
#include "NFMsgBaseEx.pb.h"
#include "NFMsgPreGame.pb.h"
#include "NFMsgShare.pb.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFList.h"

// -------------------------------------------------------------------------
#pragma pack(push,1)

enum E_CHECK_TYPE
{
    ECT_SAVE        = 0, // 保存
    ECT_PRIVATE     = 1, // 私有
    ECT_PUBLIC      = 2, // 公有
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
