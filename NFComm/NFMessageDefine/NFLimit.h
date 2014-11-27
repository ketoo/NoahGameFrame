// -------------------------------------------------------------------------
//    @FileName      :    NFLimit.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2010-10-5 12:05
//    @Module    :    极限定义值
//
// -------------------------------------------------------------------------

#ifndef _NF_LIMIT_H_
#define _NF_LIMIT_H_

#include "NFMsgBase.pb.h"
#include "NFComm/NFCore/NFIdentID.h"

#pragma pack(push,1)

//定义极限值
//#define PT_MAX_GC_SIZE          (0x8000)        // 网关到客户端的最大的封包大小32K
//#define PT_MAX_CG_SIZE          (0x2000)        // 客户端到网关的最大的封包大小8K

#define NF_NAME_MAXLEN          32              // 最大的名字长度
#define NF_BROAD_MAXLEN         64             // 最大广播数量
#define NF_PATH_MAXLEN          128            // 属性中最大的路径等长度
#define NF_ACCOUNT_MAXROLE      4               //一个账号下面最多几个角色
#define NF_ROLE_MAXPKG          150             //一个角色下面最多几个背包格子
#define NF_ROLE_DEFAULTPKG      60              //创建人物默认赋予背包格子数

#define NF_VIEWGRID_UNIT        200             //一个格子，有多少个标准单位（10个标准单位为1米）
#define NF_VIEWSIZE_MAXLEN      9               //最远距离,以自己为中心，可以看几个格子内的信息，如下图,原点是自己的位置，黑框表示可视范围

//#define NF_SCENE_LEFTLENPOS       -32000      //场景最左边距离
//#define NF_SCENE_RIGHTPOS     32000           //场景最右边距离

//#define NF_MAX_ONLINE         3000            //最大在线人数

/*
■■■
■⊙■
■■■
*/

#pragma pack(pop)

#endif