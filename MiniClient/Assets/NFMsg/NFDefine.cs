//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: NFDefine.proto
namespace NFMsg
{
    [global::ProtoBuf.ProtoContract(Name=@"EGameEventCode")]
    public enum EGameEventCode
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_UNKOWN_ERROR", Value=0)]
      EGEC_UNKOWN_ERROR = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_SUCCESS", Value=1)]
      EGEC_SUCCESS = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ACCOUNT_EXIST", Value=2)]
      EGEC_ACCOUNT_EXIST = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ACCOUNTPWD_INVALID", Value=3)]
      EGEC_ACCOUNTPWD_INVALID = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ACCOUNT_USING", Value=4)]
      EGEC_ACCOUNT_USING = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ACCOUNT_LOCKED", Value=5)]
      EGEC_ACCOUNT_LOCKED = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ACCOUNT_SUCCESS", Value=6)]
      EGEC_ACCOUNT_SUCCESS = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_VERIFY_KEY_SUCCESS", Value=7)]
      EGEC_VERIFY_KEY_SUCCESS = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_VERIFY_KEY_FAIL", Value=8)]
      EGEC_VERIFY_KEY_FAIL = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_SELECTSERVER_SUCCESS", Value=9)]
      EGEC_SELECTSERVER_SUCCESS = 9,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_SELECTSERVER_FAIL", Value=10)]
      EGEC_SELECTSERVER_FAIL = 10,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_EXIST", Value=110)]
      EGEC_CHARACTER_EXIST = 110,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_SVRZONEID_INVALID", Value=111)]
      EGEC_SVRZONEID_INVALID = 111,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_NUMOUT", Value=112)]
      EGEC_CHARACTER_NUMOUT = 112,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_INVALID", Value=113)]
      EGEC_CHARACTER_INVALID = 113,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_NOTEXIST", Value=114)]
      EGEC_CHARACTER_NOTEXIST = 114,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_USING", Value=115)]
      EGEC_CHARACTER_USING = 115,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_CHARACTER_LOCKED", Value=116)]
      EGEC_CHARACTER_LOCKED = 116,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ZONE_OVERLOAD", Value=117)]
      EGEC_ZONE_OVERLOAD = 117,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_NOT_ONLINE", Value=118)]
      EGEC_NOT_ONLINE = 118,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ALREAY_IN_TEAM", Value=119)]
      EGEC_ALREAY_IN_TEAM = 119,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_TEAM_INVITE", Value=120)]
      EGEC_INVALID_TEAM_INVITE = 120,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_TEAM_FULL", Value=121)]
      EGEC_TEAM_FULL = 121,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAIL_UPDATE_ERROR", Value=122)]
      EGEC_MAIL_UPDATE_ERROR = 122,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAIL_TONAME_INVALID", Value=123)]
      EGEC_MAIL_TONAME_INVALID = 123,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAIL_DELETE_ERROR", Value=124)]
      EGEC_MAIL_DELETE_ERROR = 124,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAIL_TOWNAME_EQUAL", Value=125)]
      EGEC_MAIL_TOWNAME_EQUAL = 125,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_LACK_MONEY", Value=126)]
      EGEC_LACK_MONEY = 126,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_FULL_PACKAGE", Value=127)]
      EGEC_FULL_PACKAGE = 127,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_ITEM", Value=128)]
      EGEC_INVALID_ITEM = 128,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_CONSIGNMENT", Value=129)]
      EGEC_INVALID_CONSIGNMENT = 129,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_NOT_ENOUGH_VIP", Value=130)]
      EGEC_NOT_ENOUGH_VIP = 130,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_NEED_LEVEL", Value=131)]
      EGEC_NEED_LEVEL = 131,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAX_FRIEND_LIST", Value=132)]
      EGEC_MAX_FRIEND_LIST = 132,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAX_ENEMY_LIST", Value=133)]
      EGEC_MAX_ENEMY_LIST = 133,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MAX_GANGS_LSIT", Value=134)]
      EGEC_MAX_GANGS_LSIT = 134,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_LIMITS_TO_RIGHTS", Value=135)]
      EGEC_LIMITS_TO_RIGHTS = 135,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_GANGS_NAME", Value=136)]
      EGEC_INVALID_GANGS_NAME = 136,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_REPEAT_ENTER_GANGS", Value=137)]
      EGEC_REPEAT_ENTER_GANGS = 137,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_GANGSMEMBER", Value=138)]
      EGEC_INVALID_GANGSMEMBER = 138,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_MASTER_LEAVE_GANGS", Value=139)]
      EGEC_MASTER_LEAVE_GANGS = 139,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_GANGS_TITLE_OUTOF", Value=140)]
      EGEC_GANGS_TITLE_OUTOF = 140,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_ENTER_GATEWAY_FAILD", Value=141)]
      EGEC_ENTER_GATEWAY_FAILD = 141,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_NO_SERVER4ZONE", Value=142)]
      EGEC_NO_SERVER4ZONE = 142,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGEC_INVALID_SKILL", Value=143)]
      EGEC_INVALID_SKILL = 143
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"EGameMsgID")]
    public enum EGameMsgID
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_UNKNOW", Value=0)]
      EGMI_UNKNOW = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_EVENT_RESULT", Value=1)]
      EGMI_EVENT_RESULT = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_EVENT_TRANSPOND", Value=2)]
      EGMI_EVENT_TRANSPOND = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_CLOSE_SOCKET", Value=3)]
      EGMI_CLOSE_SOCKET = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_MTL_WORLD_REGISTERED", Value=10)]
      EGMI_MTL_WORLD_REGISTERED = 10,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_MTL_WORLD_UNREGISTERED", Value=11)]
      EGMI_MTL_WORLD_UNREGISTERED = 11,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_MTL_WORLD_REFRESH", Value=12)]
      EGMI_MTL_WORLD_REFRESH = 12,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_LTM_LOGIN_REGISTERED", Value=20)]
      EGMI_LTM_LOGIN_REGISTERED = 20,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_LTM_LOGIN_UNREGISTERED", Value=21)]
      EGMI_LTM_LOGIN_UNREGISTERED = 21,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_LTM_LOGIN_REFRESH", Value=22)]
      EGMI_LTM_LOGIN_REFRESH = 22,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_PTWG_PROXY_REGISTERED", Value=30)]
      EGMI_PTWG_PROXY_REGISTERED = 30,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_PTWG_PROXY_UNREGISTERED", Value=31)]
      EGMI_PTWG_PROXY_UNREGISTERED = 31,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_PTWG_PROXY_REFRESH", Value=32)]
      EGMI_PTWG_PROXY_REFRESH = 32,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_GTW_GAME_REGISTERED", Value=40)]
      EGMI_GTW_GAME_REGISTERED = 40,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_GTW_GAME_UNREGISTERED", Value=41)]
      EGMI_GTW_GAME_UNREGISTERED = 41,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_GTW_GAME_REFRESH", Value=42)]
      EGMI_GTW_GAME_REFRESH = 42,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_STS_NET_INFO", Value=50)]
      EGMI_STS_NET_INFO = 50,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_LOGIN", Value=101)]
      EGMI_REQ_LOGIN = 101,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_LOGIN", Value=102)]
      EGMI_ACK_LOGIN = 102,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_LOGOUT", Value=103)]
      EGMI_REQ_LOGOUT = 103,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_WORLD_LIST", Value=110)]
      EGMI_REQ_WORLD_LIST = 110,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_WORLS_LIST", Value=111)]
      EGMI_ACK_WORLS_LIST = 111,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_CONNECT_WORLD", Value=112)]
      EGMI_REQ_CONNECT_WORLD = 112,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_CONNECT_WORLD", Value=113)]
      EGMI_ACK_CONNECT_WORLD = 113,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_KICK_CLIENT_INWORLD", Value=114)]
      EGMI_REQ_KICK_CLIENT_INWORLD = 114,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_CONNECT_KEY", Value=120)]
      EGMI_REQ_CONNECT_KEY = 120,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_CONNECT_KEY", Value=122)]
      EGMI_ACK_CONNECT_KEY = 122,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SELECT_SERVER", Value=130)]
      EGMI_REQ_SELECT_SERVER = 130,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_SELECT_SERVER", Value=131)]
      EGMI_ACK_SELECT_SERVER = 131,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_ROLE_LIST", Value=132)]
      EGMI_REQ_ROLE_LIST = 132,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_ROLE_LIST", Value=133)]
      EGMI_ACK_ROLE_LIST = 133,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_CREATE_ROLE", Value=134)]
      EGMI_REQ_CREATE_ROLE = 134,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_DELETE_ROLE", Value=135)]
      EGMI_REQ_DELETE_ROLE = 135,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_RECOVER_ROLE", Value=136)]
      EGMI_REQ_RECOVER_ROLE = 136,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_COMMAND", Value=140)]
      EGMI_REQ_COMMAND = 140,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_ENTER_GAME", Value=150)]
      EGMI_REQ_ENTER_GAME = 150,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_LEAVE_GAME", Value=151)]
      EGMI_REQ_LEAVE_GAME = 151,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SWAP_GAME", Value=152)]
      EGMI_REQ_SWAP_GAME = 152,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SWAP_SCENE", Value=153)]
      EGMI_REQ_SWAP_SCENE = 153,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_OBJECT_ENTRY", Value=200)]
      EGMI_ACK_OBJECT_ENTRY = 200,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_OBJECT_LEAVE", Value=201)]
      EGMI_ACK_OBJECT_LEAVE = 201,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_PROPERTY_INT", Value=210)]
      EGMI_ACK_PROPERTY_INT = 210,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_PROPERTY_FLOAT", Value=211)]
      EGMI_ACK_PROPERTY_FLOAT = 211,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_PROPERTY_STRING", Value=212)]
      EGMI_ACK_PROPERTY_STRING = 212,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_PROPERTY_DOUBLE", Value=213)]
      EGMI_ACK_PROPERTY_DOUBLE = 213,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_PROPERTY_OBJECT", Value=214)]
      EGMI_ACK_PROPERTY_OBJECT = 214,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_ADD_ROW", Value=220)]
      EGMI_ACK_ADD_ROW = 220,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_REMOVE_ROW", Value=221)]
      EGMI_ACK_REMOVE_ROW = 221,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_SWAP_ROW", Value=222)]
      EGMI_ACK_SWAP_ROW = 222,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_RECORD_INT", Value=223)]
      EGMI_ACK_RECORD_INT = 223,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_RECORD_FLOAT", Value=224)]
      EGMI_ACK_RECORD_FLOAT = 224,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_RECORD_DOUBLE", Value=225)]
      EGMI_ACK_RECORD_DOUBLE = 225,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_RECORD_STRING", Value=226)]
      EGMI_ACK_RECORD_STRING = 226,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_RECORD_OBJECT", Value=227)]
      EGMI_ACK_RECORD_OBJECT = 227,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_MOVE", Value=230)]
      EGMI_REQ_MOVE = 230,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_MOVE", Value=231)]
      EGMI_ACK_MOVE = 231,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_MOVE_IMMUNE", Value=232)]
      EGMI_REQ_MOVE_IMMUNE = 232,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_MOVE_IMMUNE", Value=233)]
      EGMI_ACK_MOVE_IMMUNE = 233,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SKILL_OBJECTX", Value=240)]
      EGMI_REQ_SKILL_OBJECTX = 240,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_SKILL_OBJECTX", Value=241)]
      EGMI_ACK_SKILL_OBJECTX = 241,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SKILL_POS", Value=242)]
      EGMI_REQ_SKILL_POS = 242,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_SKILL_POS", Value=243)]
      EGMI_ACK_SKILL_POS = 243,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_ITEM_OBJECT", Value=244)]
      EGMI_REQ_ITEM_OBJECT = 244,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_ITEM_OBJECT", Value=245)]
      EGMI_ACK_ITEM_OBJECT = 245,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_ITEM_POS", Value=246)]
      EGMI_REQ_ITEM_POS = 246,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_ITEM_POS", Value=247)]
      EGMI_ACK_ITEM_POS = 247,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_CHAT", Value=250)]
      EGMI_REQ_CHAT = 250,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_ACK_CHAT", Value=251)]
      EGMI_ACK_CHAT = 251,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SALE_ITEM", Value=252)]
      EGMI_REQ_SALE_ITEM = 252,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_SPLIT_ITEM", Value=253)]
      EGMI_REQ_SPLIT_ITEM = 253,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_PRODUCE_ITEM", Value=254)]
      EGMI_REQ_PRODUCE_ITEM = 254,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_PICK_ITEM", Value=255)]
      EGMI_REQ_PICK_ITEM = 255,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_ACCEPT_TASK", Value=256)]
      EGMI_REQ_ACCEPT_TASK = 256,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGMI_REQ_PULLDOWNCUSTOM", Value=257)]
      EGMI_REQ_PULLDOWNCUSTOM = 257
    }
  
}