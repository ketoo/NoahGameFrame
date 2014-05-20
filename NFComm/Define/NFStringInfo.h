#ifndef __NF_STRING_INFO_H__
#define __NF_STRING_INFO_H__

// desc:所有NFED_ON_SHOW_STRING事件使用的字串
// 字串格式中自带格式 格式全部使用%s
// 例如： %s对%s说:你好

// 所有字串按照模块区分，枚举ID自己控制区间

enum ResultInfo
{
    EGEC_UNKOWN_ERROR                  = 0,       // 未知错误
    EGEC_SUCCESS                                = 1,       // 操作成功
    EGEC_SIGNATURE_SUCCESS            = 2,       // 签名验证成功
    EGEC_ACCOUNTPWD_INVALID   = 3,          // 账号不存在或者密码错误
    EGEC_ACCOUNT_INVALID      = 4,              // 认证时无效账号
    EGEC_ACCOUNT_USING        = 5,                // 账号正在使用
    EGEC_ACCOUNT_LOCKED       = 6,              // 账号已封停
    EGEC_CHARACTER_EXIST      = 7,              // 创建角色时角色名重复
    EGEC_CHARACTER_SUCCESS      = 8,          // 创建角色成功
    EGEC_CHARACTER_FAILD      = 9,              // 创建角色失败
    EGEC_DELETECHAR_SUCCESS      = 10,          // 删除角色成功
    EGEC_DELETECHAR_FAILD      = 11,              // 删除角色失败

    EGEC_SVRZONEID_INVALID    = 12,             // 创建角色时游戏区代码无效
    EGEC_CHARACTER_NUMOUT     = 13,         // 创建角色时角色个数超出设定值（一个账号最多建4个角色）
    EGEC_CHARACTER_INVALID    = 14,           // 角色名无效（非法角色名）
    EGEC_CHARACTER_NOTEXIST   = 15,          // 角色不存在
    EGEC_CHARACTER_USING      = 16,             // 角色已在游戏中
    EGEC_CHARACTER_LOCKED     = 17,           // 角色已封停

    EGEC_ENTERGAME_SUCCESS = 18,        //进入游戏世界成功
    EGEC_ENTERGAME_FAILD = 19,              //进入游戏世界失败

    EGEC_ACCPETTASK_SUCCESS = 20,        //接任务成功
    EGEC_ACCPETTASK_FAILD = 21,              //接任务失败

    EGEC_PULLDOWNTASK_SUCCESS = 22,        //交任务成功
    EGEC_PULLDOWNTASK_FAILD = 23,              //交任务失败

};

#endif // __NF_STRING_INFO_H__