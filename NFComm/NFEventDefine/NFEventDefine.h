// -------------------------------------------------------------------------
//    @FileName         :    NFEventDefine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFEventDefine
//
// -------------------------------------------------------------------------

#ifndef NF_EVENTDEFINE_H
#define NF_EVENTDEFINE_H

#include <iostream>
///

enum NFEventDefine
{
    ////////////////Has Self---logic//////////////////////////////////////////////////////////
    //arg[0]:resultID[EGameErrorCode]
    NFED_ON_GENERAL_MESSAGE,

    //arg[0]:x,y,z
    NFED_ON_CLIENT_REQUIRE_MOVE,
    NFED_ON_CLIENT_MOVE_RESULT,

    //arg[0]:player_id key_value skillID target(1-N)
    NFED_ON_CLIENT_REQUIRE_USE_SKILL,
    NFED_ON_CLIENT_USE_SKILL_RESULT,

    //arg[0]:skillID, fx, fy, fz, TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_SKILL_POS,
    NFED_ON_CLIENT_USE_SKILL_POS_RESULT,

    //arg[0]:itemID,TargetObject
    NFED_ON_CLIENT_REQUIRE_USE_ITEM,

    //arg[0]:itemID, fx, fy, fz, TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS,

    //arg[0]:object,type,sceneID,line
    NFED_ON_CLIENT_ENTER_SCENE,
    //arg[0]:object
    NFED_ON_CLIENT_LEAVE_SCENE,

    //arg[0]:object,type,sceneID,line
    NFED_ON_OBJECT_ENTER_SCENE_BEFORE,
    NFED_ON_OBJECT_ENTER_SCENE_RESULT,
    
    NFED_ON_OBJECT_BE_KILLED,

    // 通知副本奖励
    NFED_ON_NOTICE_ECTYPE_AWARD,

};

#endif