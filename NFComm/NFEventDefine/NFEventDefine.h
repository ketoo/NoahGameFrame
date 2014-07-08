// -------------------------------------------------------------------------
//    @FileName         :    NFEventDefine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFEventDefine
//
// -------------------------------------------------------------------------

#ifndef _NF_EVENTDEFINE_H_
#define _NF_EVENTDEFINE_H_

#include <iostream>
///

enum NFEventDefine
{
    ////////////prepare//////////////////////////////////////////////////////////////
    // Client to LoginServer
    //arg[0]:address,account, arg[1]:password
    NFED_ON_CLIENT_LOGIN = 10,
	//state,address,account
    NFED_ON_CLIENT_LOGIN_RESULTS,

    //arg[0]:_nWorldID,ClientAddress, _nLoginID, _szAccount
    NFED_ON_CLIENT_SELECT_SERVER,
    //arg[0]:_nWorldID,_Ident, _nLoginID, _szWorldAddress, _nPort, _szWorldKey
    NFED_ON_CLIENT_SELECT_SERVER_RESULTS,

    //arg[0]:gsID,account,roleName,SceneID
	NFED_ON_KICK_FROM_SERVER,

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

    //arg[0]:itemID,TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_ITEM,
    NFED_ON_CLIENT_USE_ITEM_RESULT,

    //arg[0]:itemID, fx, fy, fz, TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS,
    NFED_ON_CLIENT_USE_ITEM_POS_RESULT,

    //arg[0]:object,type,sceneID,line
    NFED_ON_CLIENT_ENTER_SCENE,
    //arg[0]:object
    NFED_ON_CLIENT_LEAVE_SCENE,

    //arg[0]:object,type,sceneID,line
    NFED_ON_OBJECT_ENTER_SCENE_BEFORE,
    NFED_ON_OBJECT_ENTER_SCENE_RESULT,
    
    NFED_ON_CLIENT_SWAP_TABLE,

    NFED_ON_OBJECT_BE_KILLED,
};

#endif