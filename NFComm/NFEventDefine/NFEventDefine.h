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
    // Client to LoginServer
    //arg[0]:address,account, arg[1]:password
    NFED_ON_CLIENT_LOGIN = 10,
	//state,address,account
    NFED_ON_CLIENT_LOGIN_RESULTS,

    //arg[0]:address,account
    NFED_ON_CLIENT_LOGOUT,
    NFED_ON_CLIENT_LOGOUT_RESULTS,

    //arg[0]:address,account
    NFED_ON_CLIENT_DISCONNECT,

    //arg[0]:_nWorldID,ClientAddress, _nLoginID, _szAccount
    NFED_ON_CLIENT_SELECT_SERVER,
    //arg[0]:_nWorldID,_Ident, _nLoginID, _szWorldAddress, _nPort, _szWorldKey
    NFED_ON_CLIENT_SELECT_SERVER_RESULTS,

    //arg[0]:_szAccount, nWoridID
    NFED_ON_CLIENT_EXIT_SERVER,

    //arg[0]:address,account
    NFED_ON_CLIENT_CONECTTO_GAMESERVER,
    NFED_ON_CLIENT_CONECTTO_GAMESERVER_RESULTS,

    //arg[1]:szAccount
    NFED_ON_DATABASE_SERVER_LOADROE_BEGIN,
    //array[5]:szAccount,roleName1,roleName2,roleName3,roleName4
    NFED_ON_DATABASE_SERVER_LOADROE_FINAL_RESULTS,

    //arg[0]:address,roleName,roleSex,roleJob,roleRace,roleCamp,
    NFED_ON_CLIENT_CREATEROLE,
    //arg[0]:address,roleName,roleSex,roleJob,roleRace,roleCamp,
    NFED_ON_CLIENT_CREATEROLE_RESULTS,

    //arg[0]:account,roleName
    NFED_ON_CLIENT_DELETEROLE,

    //arg[0]:gaeserverid,account,roleName,sceneIndex
    NFED_ON_CLIENT_SELECTROLE_ENTER,
    //arg[0]:address[other game server address]
    NFED_ON_CLIENT_SELECT_ENTER_RESULTS,

    //arg[0]:account,roleName,ident
    NFED_ON_CLIENT_LEAVE_GAME,

    //arg[0]:gsID,account,roleName,SceneID
    NFED_ON_CLIENT_WANTO_SWAP_GS,
	NFED_ON_KICK_FROM_SERVER,
    ////////////////Has Self//////////////////////////////////////////////////////////
    //arg[0]:resultID[EGameErrorCode]
    NFED_ON_GENERAL_MESSAGE,
    //arg[0]:nPlayerID, nMover, nMoveType,fTargetX, fTargetY,fTargetZ;
    NFED_ON_SERVER_MOVE_START,
    //arg[0]:fX, fY,fZ;
    NFED_ON_SERVER_MOVEING,
    //arg[0]:fX, fY,fZ;
    NFED_ON_SERVER_MOVE_END,

    //arg[0]:x,y,z
    NFED_ON_CLIENT_REQUIRE_MOVE,

    //arg[0]:player_id key_value skillID target(1-N)
    NFED_ON_CLIENT_REQUIRE_USE_SKILL,

    //arg[0]:skillID, fx, fy, fz, TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_SKILL_POS,

    //arg[0]:itemID,TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_ITEM,

    //arg[0]:itemID, fx, fy, fz, TargetObjectList
    NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS,

    //arg[0]:skillID,result,TargetObjectList
    NFED_ON_CLIENT_USE_SKILL_RESULT,

    //arg[0]:skillID, fx, fy, fz, result,TargetObjectList
    NFED_ON_CLIENT_USE_SKILL_POS_RESULT,

    //arg[0]:itemID,TargetObjectList
    NFED_ON_CLIENT_USE_ITEM_RESULT,

    //arg[0]:itemID, fx, fy, fz, result,TargetObjectList
    NFED_ON_CLIENT_USE_ITEM_POS_RESULT,

    //arg[0]:originTableName, targetTableName, originRow, targetRow
    NFED_ON_CLIENT_SWAP_TABLE,

    //申请接任务
    //arg[0]:task_id
    NFED_ON_CLIENT_ACCEPT_TASK,
    //申请交任务
    //arg[0]:task_id
    NFED_ON_CLIENT_PULLDOWN,

    //申请关卡结算
    //arg[0]:
    NFED_ON_CLIENT_PULL_CUSTOM,

    ///////////////Only Sserver///////////////////////////////////////////////////////////
    //arg[0]:oldGridID, newGridID
    NFED_ON_OBJECT_MOVE_SWAP_GRID,

    //arg[0]:skillName[ItemName]
    NFED_ON_OBJECT_COOLDOWN,

    //arg[0]:attack object
    NFED_ON_OBJECT_BE_KILLED,

    //arg[0]:object,type,sceneID,line
    NFED_ON_OBJECT_ENTER_SCENE,
    //arg[0]:object,type,sceneID,line
    NFED_ON_OBJECT_ENTER_SCENE_BEFORE,
    NFED_ON_OBJECT_ENTER_SCENE_RESULT,
    //arg[0]:object
    NFED_ON_OBJECT_LEAVE_SCENE,

    // 售出物品事件arg[0] index arg[1] guid
    NFED_ON_SELL_ITEM,

    // arg[0] stringID arg[1..N]参数是字串中需要的参数
    NFED_ON_SHOW_STRING,

    // 拆分物品事件 arg[0] src_index arg[1] src_guid arg[2] target_index
    NFED_ON_SPLIT_ITEM,

    // 背包整理事件 arg[0] pack_type
    NFED_ON_SORT_PACK,

    // 回购物品 arg[0] index arg[1] guid
    NFED_ON_BUYBACK_ITEM,

    // 获得掉落物品 npc_id item_id
    NFED_REQ_PICK_DROP_ITEM,


    // 排行榜数据更新 rank_type op_type
    NFED_ON_UPDATE_RANK_DATA,

    // player_id rank_type op_type value
    NFED_ON_UPDATE_RANK_DATA_RESULT,
    //master server's event
    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_WORLD_REGISTERED,

    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_WORLD_UNREGISTERED,

    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_WORLD_REFRESH_INFO,

    ///
    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_LOGIN_REGISTERED,

    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_LOGIN_UNREGISTERED,

    //arg[0]:worldID, worldName, worldMaxOnline, worldOnlineCount
    //NFED_ON_LOGIN_REFRESH_INFO,

    //////////////////////////////////////////////////////////////////////////
    //server's event
    //arg[0]:self, sender
    //NFED_ON_CREATE_OBJECT = 1000,             //创建一个对象

    //arg[0]:self, sender
    //NFED_ON_DESTROY_OBJECT,                           //删除一个对象

    //arg[0]:self, containerID
    //NFED_ON_ENTRY_OBJECT,                         //进入容器

    //arg[0]:self, tableName,row,col,oldValue,newValue
    //NFED_ON_RECORD_CHANGED,                       //对象表内容改变

    //////////////////////////////////////////////////////////////////////////
    //  NFED_ON_VISIBLE_OBJECT,                     //对象显示与否
    //  NFED_ON_SELECT_OBJECT,                      //对象被选中
    //  NFED_ON_BEATTACK_OBJECT,                    //对象被攻击
    //  NFED_ON_BEKILLED_OBJECT,                    //对象被杀死
    //  NFED_ON_RELIVE_OBJECT,                      //对象复活
    //
    //  NFED_ON_SKILL_BEGIN = 200,                  //开始一个技能
    //  NFED_ON_SKILL_END,                          //结束一个技能
    //  NFED_ON_SKILL_BREAK,                        //技能被打断
    //
    //
    //  NFED_ON_SPRING = 11000,
    //  NFED_ON_ENDNSPRING,
    //  NFED_ON_ADDBUFF,
    //  NFED_ON_REMOVEBUFF,
    //  NFED_ON_ADDSKILL,
    //  NFED_ON_REMOVESKILL,
    //  NFED_ON_ADDITEM,
    //  NFED_ON_REMOVEITEM,
    //
    ////arg[0]:x,y,z
    //     NFED_ON_STARTMOVE,
    //     NFED_ON_MOVEING,
    //     NFED_ON_ENDMOVE,
    //
    //  NFED_ON_CREATESCENE = 20000,
    //  NFED_ON_DELSCENE,
    //  NFED_ON_ENTRYSCENE,
    //  NFED_ON_LEAVESCENE,


};

#endif
