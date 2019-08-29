// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NFDefine.proto

#include "NFDefine.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace NFMsg {
}  // namespace NFMsg
namespace protobuf_NFDefine_2eproto {
void InitDefaults() {
}

const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[25];
const ::google::protobuf::uint32 TableStruct::offsets[1] = {};
static const ::google::protobuf::internal::MigrationSchema* schemas = NULL;
static const ::google::protobuf::Message* const* file_default_instances = NULL;

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "NFDefine.proto", schemas, file_default_instances, TableStruct::offsets,
      NULL, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\016NFDefine.proto\022\005NFMsg*\225\n\n\016EGameEventCo"
      "de\022\020\n\014EGEC_SUCCESS\020\000\022\025\n\021EGEC_UNKOWN_ERRO"
      "R\020\001\022\026\n\022EGEC_ACCOUNT_EXIST\020\002\022\033\n\027EGEC_ACCO"
      "UNTPWD_INVALID\020\003\022\026\n\022EGEC_ACCOUNT_USING\020\004"
      "\022\027\n\023EGEC_ACCOUNT_LOCKED\020\005\022\030\n\024EGEC_ACCOUN"
      "T_SUCCESS\020\006\022\033\n\027EGEC_VERIFY_KEY_SUCCESS\020\007"
      "\022\030\n\024EGEC_VERIFY_KEY_FAIL\020\010\022\035\n\031EGEC_SELEC"
      "TSERVER_SUCCESS\020\t\022\032\n\026EGEC_SELECTSERVER_F"
      "AIL\020\n\022\030\n\024EGEC_CHARACTER_EXIST\020n\022\032\n\026EGEC_"
      "SVRZONEID_INVALID\020o\022\031\n\025EGEC_CHARACTER_NU"
      "MOUT\020p\022\032\n\026EGEC_CHARACTER_INVALID\020q\022\033\n\027EG"
      "EC_CHARACTER_NOTEXIST\020r\022\030\n\024EGEC_CHARACTE"
      "R_USING\020s\022\031\n\025EGEC_CHARACTER_LOCKED\020t\022\026\n\022"
      "EGEC_ZONE_OVERLOAD\020u\022\023\n\017EGEC_NOT_ONLINE\020"
      "v\022\027\n\023EGEC_ALREAY_IN_TEAM\020w\022\034\n\030EGEC_INVAL"
      "ID_TEAM_INVITE\020x\022\022\n\016EGEC_TEAM_FULL\020y\022\032\n\026"
      "EGEC_MAIL_UPDATE_ERROR\020z\022\034\n\030EGEC_MAIL_TO"
      "NAME_INVALID\020{\022\032\n\026EGEC_MAIL_DELETE_ERROR"
      "\020|\022\033\n\027EGEC_MAIL_TOWNAME_EQUAL\020}\022\023\n\017EGEC_"
      "LACK_MONEY\020~\022\025\n\021EGEC_FULL_PACKAGE\020\177\022\026\n\021E"
      "GEC_INVALID_ITEM\020\200\001\022\035\n\030EGEC_INVALID_CONS"
      "IGNMENT\020\201\001\022\030\n\023EGEC_NOT_ENOUGH_VIP\020\202\001\022\024\n\017"
      "EGEC_NEED_LEVEL\020\203\001\022\031\n\024EGEC_MAX_FRIEND_LI"
      "ST\020\204\001\022\030\n\023EGEC_MAX_ENEMY_LIST\020\205\001\022\030\n\023EGEC_"
      "MAX_GANGS_LSIT\020\206\001\022\032\n\025EGEC_LIMITS_TO_RIGH"
      "TS\020\207\001\022\034\n\027EGEC_INVALID_GANGS_NAME\020\210\001\022\034\n\027E"
      "GEC_REPEAT_ENTER_GANGS\020\211\001\022\035\n\030EGEC_INVALI"
      "D_GANGSMEMBER\020\212\001\022\034\n\027EGEC_MASTER_LEAVE_GA"
      "NGS\020\213\001\022\033\n\026EGEC_GANGS_TITLE_OUTOF\020\214\001\022\035\n\030E"
      "GEC_ENTER_GATEWAY_FAILD\020\215\001\022\030\n\023EGEC_NO_SE"
      "RVER4ZONE\020\216\001\022\027\n\022EGEC_INVALID_SKILL\020\217\001\022\035\n"
      "\030EGEC_CREATE_CLAN_SUCCESS\020\310\001\022\033\n\026EGEC_JOI"
      "N_CLAN_SUCCESS\020\311\001\022\034\n\027EGEC_LEAVE_CLAN_SUC"
      "CESS\020\312\001*\034\n\tESynMsgID\022\017\n\013ESMI_UNKNOW\020\000*\3671"
      "\n\nEGameMsgID\022\017\n\013EGMI_UNKNOW\020\000\022\025\n\021EGMI_EV"
      "ENT_RESULT\020\001\022\030\n\024EGMI_EVENT_TRANSPOND\020\002\022\025"
      "\n\021EGMI_CLOSE_SOCKET\020\003\022\035\n\031EGMI_WTM_WORLD_"
      "REGISTERED\020\n\022\037\n\033EGMI_WTM_WORLD_UNREGISTE"
      "RED\020\013\022\032\n\026EGMI_WTM_WORLD_REFRESH\020\014\022\035\n\031EGM"
      "I_LTM_LOGIN_REGISTERED\020\024\022\037\n\033EGMI_LTM_LOG"
      "IN_UNREGISTERED\020\025\022\032\n\026EGMI_LTM_LOGIN_REFR"
      "ESH\020\026\022\036\n\032EGMI_PTWG_PROXY_REGISTERED\020\036\022 \n"
      "\034EGMI_PTWG_PROXY_UNREGISTERED\020\037\022\033\n\027EGMI_"
      "PTWG_PROXY_REFRESH\020 \022\034\n\030EGMI_GTW_GAME_RE"
      "GISTERED\020(\022\036\n\032EGMI_GTW_GAME_UNREGISTERED"
      "\020)\022\031\n\025EGMI_GTW_GAME_REFRESH\020*\022\032\n\026EGMI_DT"
      "W_DB_REGISTERED\020<\022\034\n\030EGMI_DTW_DB_UNREGIS"
      "TERED\020=\022\027\n\023EGMI_DTW_DB_REFRESH\020>\022\025\n\021EGMI"
      "_STS_NET_INFO\020F\022\025\n\021EGEC_REQ_LAG_TEST\020P\022\032"
      "\n\026EGEC_ACK_GATE_LAG_TEST\020Q\022\032\n\026EGEC_ACK_G"
      "AME_LAG_TEST\020R\022\032\n\026EGMI_STS_SERVER_REPORT"
      "\020Z\022\027\n\023EGMI_STS_HEART_BEAT\020d\022\022\n\016EGMI_REQ_"
      "LOGIN\020e\022\022\n\016EGMI_ACK_LOGIN\020f\022\023\n\017EGMI_REQ_"
      "LOGOUT\020g\022\027\n\023EGMI_REQ_WORLD_LIST\020n\022\027\n\023EGM"
      "I_ACK_WORLD_LIST\020o\022\032\n\026EGMI_REQ_CONNECT_W"
      "ORLD\020p\022\032\n\026EGMI_ACK_CONNECT_WORLD\020q\022 \n\034EG"
      "MI_REQ_KICK_CLIENT_INWORLD\020r\022\030\n\024EGMI_REQ"
      "_CONNECT_KEY\020x\022\030\n\024EGMI_ACK_CONNECT_KEY\020z"
      "\022\033\n\026EGMI_REQ_SELECT_SERVER\020\202\001\022\033\n\026EGMI_AC"
      "K_SELECT_SERVER\020\203\001\022\027\n\022EGMI_REQ_ROLE_LIST"
      "\020\204\001\022\027\n\022EGMI_ACK_ROLE_LIST\020\205\001\022\031\n\024EGMI_REQ"
      "_CREATE_ROLE\020\206\001\022\031\n\024EGMI_REQ_DELETE_ROLE\020"
      "\207\001\022\032\n\025EGMI_REQ_RECOVER_ROLE\020\210\001\022\034\n\027EGMI_R"
      "EQ_LOAD_ROLE_DATA\020\214\001\022\034\n\027EGMI_ACK_LOAD_RO"
      "LE_DATA\020\215\001\022\034\n\027EGMI_REQ_SAVE_ROLE_DATA\020\216\001"
      "\022\034\n\027EGMI_ACK_SAVE_ROLE_DATA\020\217\001\022\030\n\023EGMI_R"
      "EQ_ENTER_GAME\020\226\001\022\030\n\023EGMI_ACK_ENTER_GAME\020"
      "\227\001\022\030\n\023EGMI_REQ_LEAVE_GAME\020\230\001\022\030\n\023EGMI_ACK"
      "_LEAVE_GAME\020\231\001\022\027\n\022EGMI_REQ_SWAP_GAME\020\232\001\022"
      "\030\n\023EGMI_REQ_SWAP_SCENE\020\233\001\022\030\n\023EGMI_ACK_SW"
      "AP_SCENE\020\234\001\022\035\n\030EGMI_REQ_SWAP_HOME_SCENE\020"
      "\235\001\022\035\n\030EGMI_ACK_SWAP_HOME_SCENE\020\236\001\022\037\n\032EGM"
      "I_REQ_ENTER_GAME_FINISH\020\237\001\022\037\n\032EGMI_ACK_E"
      "NTER_GAME_FINISH\020\240\001\022\032\n\025EGMI_ACK_OBJECT_E"
      "NTRY\020\310\001\022\032\n\025EGMI_ACK_OBJECT_LEAVE\020\311\001\022#\n\036E"
      "GMI_ACK_OBJECT_PROPERTY_ENTRY\020\312\001\022!\n\034EGMI"
      "_ACK_OBJECT_RECORD_ENTRY\020\313\001\022\032\n\025EGMI_ACK_"
      "PROPERTY_INT\020\322\001\022\034\n\027EGMI_ACK_PROPERTY_FLO"
      "AT\020\323\001\022\035\n\030EGMI_ACK_PROPERTY_STRING\020\324\001\022\035\n\030"
      "EGMI_ACK_PROPERTY_OBJECT\020\326\001\022\036\n\031EGMI_ACK_"
      "PROPERTY_VECTOR2\020\327\001\022\036\n\031EGMI_ACK_PROPERTY"
      "_VECTOR3\020\330\001\022\034\n\027EGMI_ACK_PROPERTY_CLEAR\020\331"
      "\001\022\025\n\020EGMI_ACK_ADD_ROW\020\334\001\022\030\n\023EGMI_ACK_REM"
      "OVE_ROW\020\335\001\022\026\n\021EGMI_ACK_SWAP_ROW\020\336\001\022\030\n\023EG"
      "MI_ACK_RECORD_INT\020\337\001\022\032\n\025EGMI_ACK_RECORD_"
      "FLOAT\020\340\001\022\033\n\026EGMI_ACK_RECORD_STRING\020\342\001\022\033\n"
      "\026EGMI_ACK_RECORD_OBJECT\020\343\001\022\034\n\027EGMI_ACK_R"
      "ECORD_VECTOR2\020\344\001\022\034\n\027EGMI_ACK_RECORD_VECT"
      "OR3\020\345\001\022\032\n\025EGMI_ACK_RECORD_CLEAR\020\372\001\022\031\n\024EG"
      "MI_ACK_RECORD_SORT\020\373\001\022\033\n\026EGMI_ACK_DATA_F"
      "INISHED\020\204\002\022\026\n\021EGMI_REQ_AI_ONWER\020\254\002\022\034\n\027EG"
      "MI_REQ_SEARCH_OPPNENT\020\304\t\022\034\n\027EGMI_ACK_SEA"
      "RCH_OPPNENT\020\305\t\022\031\n\024EGMI_REQ_END_OPPNENT\020\306"
      "\t\022\031\n\024EGMI_ACK_END_OPPNENT\020\307\t\022\033\n\026EGMI_REQ"
      "_CANCEL_SEARCH\020\310\t\022\033\n\026EGMI_ACK_CANCEL_SEA"
      "RCH\020\311\t\022\022\n\rEGMI_REQ_MOVE\020\316\t\022\022\n\rEGMI_ACK_M"
      "OVE\020\317\t\022\031\n\024EGMI_REQ_MOVE_IMMUNE\020\320\t\022\031\n\024EGM"
      "I_ACK_MOVE_IMMUNE\020\321\t\022\030\n\023EGMI_REQ_STATE_S"
      "YNC\020\322\t\022\030\n\023EGMI_ACK_STATE_SYNC\020\323\t\022\026\n\021EGMI"
      "_REQ_POS_SYNC\020\324\t\022\026\n\021EGMI_ACK_POS_SYNC\020\325\t"
      "\022\033\n\026EGMI_REQ_SKILL_OBJECTX\020\330\t\022\033\n\026EGMI_AC"
      "K_SKILL_OBJECTX\020\331\t\022\027\n\022EGMI_REQ_SKILL_POS"
      "\020\332\t\022\027\n\022EGMI_ACK_SKILL_POS\020\333\t\022\031\n\024EGMI_REQ"
      "_ITEM_OBJECT\020\334\t\022\031\n\024EGMI_ACK_ITEM_OBJECT\020"
      "\335\t\022\026\n\021EGMI_REQ_ITEM_POS\020\336\t\022\026\n\021EGMI_ACK_I"
      "TEM_POS\020\337\t\022\022\n\rEGMI_REQ_CHAT\020\342\t\022\022\n\rEGMI_A"
      "CK_CHAT\020\343\t\022\027\n\022EGMI_REQ_SALE_ITEM\020\344\t\022\030\n\023E"
      "GMI_REQ_SPLIT_ITEM\020\345\t\022\032\n\025EGMI_REQ_PRODUC"
      "E_ITEM\020\346\t\022\027\n\022EGMI_REQ_PICK_ITEM\020\347\t\022\031\n\024EG"
      "MI_REQ_ACCEPT_TASK\020\350\t\022\034\n\027EGMI_REQ_COMPEL"
      "ETE_TASK\020\351\t\022\033\n\026EGMI_ACK_ONLINE_NOTIFY\020\212\n"
      "\022\034\n\027EGMI_ACK_OFFLINE_NOTIFY\020\213\n\022\031\n\024EGMI_R"
      "EQ_CREATE_CLAN\020\224\n\022\031\n\024EGMI_ACK_CREATE_CLA"
      "N\020\225\n\022\027\n\022EGMI_REQ_JOIN_CLAN\020\226\n\022\027\n\022EGMI_AC"
      "K_JOIN_CLAN\020\227\n\022\030\n\023EGMI_REQ_LEAVE_CLAN\020\230\n"
      "\022\030\n\023EGMI_ACK_LEAVE_CLAN\020\231\n\022\026\n\021EGMI_REQ_O"
      "PR_CLAN\020\232\n\022\026\n\021EGMI_ACK_OPR_CLAN\020\233\n\022\031\n\024EG"
      "MI_REQ_SEARCH_CLAN\020\234\n\022\031\n\024EGMI_ACK_SEARCH"
      "_CLAN\020\235\n\022 \n\033EGMI_REQ_CANCEL_SEARCH_CLAN\020"
      "\236\n\022\036\n\031EGEC_REQ_CREATE_CHATGROUP\020\370\n\022\036\n\031EG"
      "EC_ACK_CREATE_CHATGROUP\020\371\n\022\034\n\027EGEC_REQ_J"
      "OIN_CHATGROUP\020\372\n\022\034\n\027EGEC_ACK_JOIN_CHATGR"
      "OUP\020\373\n\022\035\n\030EGEC_REQ_LEAVE_CHATGROUP\020\374\n\022\035\n"
      "\030EGEC_ACK_LEAVE_CHATGROUP\020\375\n\022$\n\037EGEC_REQ"
      "_SUBSCRIPTION_CHATGROUP\020\376\n\022*\n%EGEC_REQ_C"
      "ANCELSUBSCRIPTION_CHATGROUP\020\377\n\022\036\n\031EGEC_R"
      "EQ_INVITE_CHATGROUP\020\200\013\022\036\n\031EGEC_ACK_INVIT"
      "E_CHATGROUP\020\201\013\022\034\n\027EGEC_REQ_KICK_CHATGROU"
      "P\020\202\013\022\034\n\027EGEC_ACK_KICK_CHATGROUP\020\203\013\022\037\n\032EG"
      "EC_REQ_CHATGROUP_TO_TEAM\020\252\013\022\037\n\032EGEC_ACK_"
      "CHATGROUP_TO_TEAM\020\253\013\022%\n EGEC_REQ_INTENSI"
      "FYLEVEL_TO_EQUIP\020\334\013\022%\n EGEC_ACK_INTENSIF"
      "YLEVEL_TO_EQUIP\020\335\013\022\033\n\026EGEC_REQ_HOLE_TO_E"
      "QUIP\020\336\013\022\033\n\026EGEC_ACK_HOLE_TO_EQUIP\020\337\013\022!\n\034"
      "EGEC_REQ_INLAYSTONE_TO_EQUIP\020\340\013\022!\n\034EGEC_"
      "ACK_INLAYSTONE_TO_EQUIP\020\341\013\022#\n\036EGEC_REQ_E"
      "LEMENTLEVEL_TO_EQUIP\020\342\013\022#\n\036EGEC_ACK_ELEM"
      "ENTLEVEL_TO_EQUIP\020\343\013\022\024\n\017EGEC_WEAR_EQUIP\020"
      "\344\013\022\027\n\022EGEC_TAKEOFF_EQUIP\020\345\013\022\034\n\027EGEC_REQ_"
      "SET_FIGHT_HERO\020\360\013\022\037\n\032EGEC_REQ_SWITCH_FIG"
      "HT_HERO\020\361\013\022\031\n\024EGEC_REQ_RELIVE_HERO\020\362\013\022\027\n"
      "\022EGEC_REQ_SEND_MAIL\020\244\r\022\027\n\022EGEC_REQ_DRAW_"
      "MAIL\020\245\r\022\031\n\024EGEC_REQ_DELETE_MAIL\020\246\r\022 \n\033EG"
      "MI_ACK_CLONE_OBJECT_ENTRY\020\210\016\022 \n\033EGMI_ACK"
      "_CLONE_OBJECT_LEAVE\020\211\016\022)\n$EGMI_ACK_CLONE"
      "_OBJECT_PROPERTY_ENTRY\020\212\016\022\'\n\"EGMI_ACK_CL"
      "ONE_OBJECT_RECORD_ENTRY\020\213\016\022 \n\033EGMI_ACK_C"
      "LONE_PROPERTY_INT\020\222\016\022\"\n\035EGMI_ACK_CLONE_P"
      "ROPERTY_FLOAT\020\223\016\022#\n\036EGMI_ACK_CLONE_PROPE"
      "RTY_STRING\020\224\016\022#\n\036EGMI_ACK_CLONE_PROPERTY"
      "_OBJECT\020\226\016\022\030\n\023EGMI_REQSWICHSERVER\020\260\016\022\030\n\023"
      "EGMI_ACKSWICHSERVER\020\261\016\022\031\n\024EGMI_REQ_CREAT"
      "E_TEAM\020\304\016\022\031\n\024EGMI_ACK_CREATE_TEAM\020\305\016\022\027\n\022"
      "EGMI_REQ_JOIN_TEAM\020\306\016\022\027\n\022EGMI_ACK_JOIN_T"
      "EAM\020\307\016\022\030\n\023EGMI_REQ_LEAVE_TEAM\020\310\016\022\030\n\023EGMI"
      "_ACK_LEAVE_TEAM\020\311\016\022\031\n\024EGMI_REQ_INVITE_TE"
      "AM\020\312\016\022\034\n\027EGMI_REQ_OPRMEMBER_TEAM\020\313\016\022\034\n\027E"
      "GMI_ACK_OPRMEMBER_TEAM\020\314\016\022\037\n\032EGMI_REQ_TE"
      "AM_ENTER_ECTYPE\020\315\016\022\037\n\032EGMI_ACK_TEAM_ENTE"
      "R_ECTYPE\020\316\016\022\033\n\026EGMI_REQ_MAP_GRID_INFO\020\354\016"
      "\022\033\n\026EGMI_ACK_MAP_GRID_INFO\020\355\016\022\032\n\025EGMI_RE"
      "Q_BIG_MAP_INFO\020\356\016\022\032\n\025EGMI_ACK_BIG_MAP_IN"
      "FO\020\357\016\022\033\n\026EGMI_REQ_HOLD_MAP_GRID\020\366\016\022\033\n\026EG"
      "MI_ACK_HOLD_MAP_GRID\020\367\016\022 \n\033EGMI_REQ_LEAV"
      "E_MSG_MAP_GRID\020\370\016\022 \n\033EGMI_ACK_LEAVE_MSG_"
      "MAP_GRID\020\371\016\022 \n\033EGMI_REQ_GET_MAP_GRID_AWA"
      "RD\020\372\016\022 \n\033EGMI_ACK_GET_MAP_GRID_AWARD\020\373\016\022"
      "\036\n\031EGMI_REQ_MAP_GRID_HUNTING\020\374\016\022\036\n\031EGMI_"
      "ACK_MAP_GRID_HUNTING\020\375\016\022\037\n\032EGMI_REQ_MAP_"
      "GRID_KING_WAR\020\376\016\022\037\n\032EGMI_ACK_MAP_GRID_KI"
      "NG_WAR\020\377\016\022\032\n\025EGMI_REQ_ADD_BUILDING\020\210\'\022\037\n"
      "\032EGMI_REQ_ACK_BUILDING_LIST\020\211\'\022!\n\034EGMI_R"
      "EQ_STORE_BUILDING_LIST\020\222\'\022\035\n\030EGMI_REQ_AC"
      "K_SEND_INVITE\020\360.\022\037\n\032EGMI_REQ_ACK_ACCEPT_"
      "INVITE\020\361.\022\037\n\032EGMI_REQ_ACK_REJECT_INVITE\020"
      "\362.\022\037\n\032EGMI_REQ_ACK_IGNORE_INVITE\020\363.\022\036\n\031E"
      "GMI_REQ_ACK_BLOCK_PLAYER\020\372.\022 \n\033EGMI_REQ_"
      "ACK_UNBLOCK_PLAYER\020\373.\022\036\n\031EGMI_REQ_CMD_PR"
      "OPERTY_INT\020\220N\022\036\n\031EGMI_REQ_CMD_PROPERTY_S"
      "TR\020\221N\022!\n\034EGMI_REQ_CMD_PROPERTY_OBJECT\020\222N"
      "\022 \n\033EGMI_REQ_CMD_PROPERTY_FLOAT\020\223N\022\034\n\027EG"
      "MI_REQ_CMD_RECORD_INT\020\224N\022\034\n\027EGMI_REQ_CMD"
      "_RECORD_STR\020\225N\022\037\n\032EGMI_REQ_CMD_RECORD_OB"
      "JECT\020\226N\022\036\n\031EGMI_REQ_CMD_RECORD_FLOAT\020\227N\022"
      "\030\n\023EGMI_REQ_CMD_NORMAL\020\230N\022!\n\034EGMI_REQ_QU"
      "ERY_SERVER_STATUS\020\330O\022\034\n\026EGMI_REQ_BUY_FOR"
      "M_SHOP\020\240\234\001\022\034\n\026EGMI_ACK_BUY_FORM_SHOP\020\241\234\001"
      "\022 \n\032EGMI_REQ_MOVE_BUILD_OBJECT\020\242\234\001\022 \n\032EG"
      "MI_ACK_MOVE_BUILD_OBJECT\020\243\234\001\022\033\n\025EGMI_REQ"
      "_UP_BUILD_LVL\020\205\235\001\022\032\n\024EGMI_REQ_CREATE_ITE"
      "M\020\206\235\001\022\034\n\026EGMI_REQ_BUILD_OPERATE\020\207\235\001\022\036\n\030E"
      "GMI_REQ_ACK_FRIEND_LIST\020\261\352\001\022\034\n\026EGMI_ACK_"
      "FRIEND_DELETE\020\262\352\001\022\031\n\023EGMI_ACK_FRIEND_ADD"
      "\020\263\352\001\022\031\n\023EGMI_ACK_INVITE_ADD\020\264\352\001*\233\001\n\nESki"
      "llType\022\024\n\020EST_BRIEF_SINGLE\020\000\022\023\n\017EST_BRIE"
      "F_GROUP\020\001\022\025\n\021EST_BULLET_SINGLE\020\002\022\026\n\022EST_"
      "BULLET_REBOUND\020\003\022\032\n\026EST_BULLET_TARGET_BO"
      "MB\020\004\022\027\n\023EST_BULLET_POS_BOMB\020\005*G\n\tEItemTy"
      "pe\022\r\n\tEIT_EQUIP\020\000\022\013\n\007EIT_GEM\020\001\022\016\n\nEIT_SU"
      "PPLY\020\002\022\016\n\nEIT_SCROLL\020\003*\220\001\n\021EGameEquipSub"
      "Type\022\024\n\020EQUIPTYPE_WEAPON\020\000\022\024\n\020EQUIPTYPE_"
      "HELMET\020\001\022\023\n\017EQUIPTYPE_ARMOR\020\002\022\022\n\016EQUIPTY"
      "PE_HAND\020\003\022\022\n\016EQUIPTYPE_RING\020\004\022\022\n\016EQUIPTY"
      "PE_BOOT\020\005*\311\001\n\017EGameGemSubType\022\024\n\020EGGSTYP"
      "E_GEM_ATK\020\000\022\024\n\020EGGSTYPE_GEM_DEF\020\001\022\025\n\021EGG"
      "STYPE_GEM_FIRE\020\003\022\026\n\022EGGSTYPE_GEM_LIGHT\020\004"
      "\022\025\n\021EGGSTYPE_GEM_WIND\020\005\022\024\n\020EGGSTYPE_GEM_"
      "ICE\020\006\022\027\n\023EGGSTYPE_GEM_POISON\020\007\022\025\n\021EGGSTY"
      "PE_GEM_DRAK\020\010*\317\001\n\022EGameSupplySubType\022\023\n\017"
      "EGIT_ITEM_WATER\020\000\022\025\n\021EGIT_ITEM_DIAMOND\020\001"
      "\022\026\n\022EGIT_ITEM_CURRENCY\020\002\022\021\n\rEGIT_ITEM_EX"
      "P\020\003\022\020\n\014EGIT_ITEM_HP\020\004\022\020\n\014EGIT_ITEM_MP\020\005\022"
      "\020\n\014EGIT_ITEM_SP\020\006\022\022\n\016EGIT_ITEM_PACK\020\007\022\030\n"
      "\024EGIT_ITEM_MEMORY_POS\020\010*\222\001\n\022EGameScrollS"
      "ubType\022\031\n\025EGTST_TOKEN_HERO_CARD\020\000\022\030\n\024EGT"
      "ST_TOKEN_POSITION\020\001\022\026\n\022EGTST_TOKEN_BOUNT"
      "Y\020\002\022\030\n\024EGTST_TOKEN_ACTIVITY\020\003\022\025\n\021EGTST_T"
      "OKEN_BUILD\020\004*e\n\024EGameItemExpiredType\022\016\n\n"
      "EGIET_NONE\020\000\022\023\n\017EGIET_USE_TIMES\020\001\022\023\n\017EGI"
      "ET_REAL_TIME\020\002\022\023\n\017EGIET_END_INDEX\020\003*Y\n\022E"
      "DrawDropItemState\022\025\n\021E_DRAW_STATE_NONE\020\000"
      "\022\025\n\021E_DRAW_STATE_GAIN\020\001\022\025\n\021E_DRAW_STATE_"
      "RECV\020\002*p\n\017EGuildPowerType\022\032\n\026CLAN_POWER_"
      "TYPE_NORMAL\020\000\022\"\n\036CLAN_POWER_TYPE_VICE_PR"
      "ESIDENT\020\001\022\035\n\031CLAN_POWER_TYPE_PRESIDENT\020\002"
      "*V\n\nETaskState\022\023\n\017TASK_IN_PROCESS\020\000\022\r\n\tT"
      "ASK_DONE\020\001\022\023\n\017TASK_DRAW_AWARD\020\002\022\017\n\013TASK_"
      "FINISH\020\003*\251\002\n\tETaskType\022 \n\034TASK_LEVEL_UPD"
      "ATE_SOME_LEVEL\020\000\022\024\n\020TASK_PASS_ECTYPE\020\001\022\032"
      "\n\026TASK_PASS_ECTYPE_COUNT\020\002\022$\n TASK_PASS_"
      "SOME_TYPE_ECTYPE_COUNT\020\003\022\033\n\027TASK_UPDATE_"
      "SKILL_COUNT\020\004\022\026\n\022TASK_CONSUME_MONEY\020\005\022\030\n"
      "\024TASK_CONSUME_DIAMOND\020\006\022\024\n\020TASK_ARENA_CO"
      "UNT\020\007\022\033\n\027TASK_KILL_MONSTER_COUNT\020\010\022 \n\034TA"
      "SK_KILL_SOME_MONSTER_COUNT\020\t*m\n\nESceneTy"
      "pe\022\020\n\014SCENE_NORMAL\020\000\022\026\n\022SCENE_SINGLE_CLO"
      "NE\020\001\022\025\n\021SCENE_MULTI_CLONE\020\002\022\016\n\nSCENE_HOM"
      "E\020\003\022\016\n\nSCENE_CLAN\020\004*t\n\010EPVPType\022\014\n\010PVP_H"
      "OME\020\000\022\022\n\016PVP_INDIVIDUAL\020\001\022\014\n\010PVP_DOTA\020\002\022"
      "\020\n\014PVP_SURVIVAL\020\003\022\020\n\014PVP_CLAN_WAR\020\004\022\024\n\020P"
      "VP_CLAN_DUNGEON\020\005*s\n\tEShopType\022\016\n\nEST_UN"
      "KNOW\020\000\022\020\n\014EST_BUILDING\020\001\022\014\n\010EST_GOLD\020\002\022\014"
      "\n\010EST_ITEM\020\003\022\r\n\tEST_EQUIP\020\005\022\013\n\007EST_GEM\020\006"
      "\022\014\n\010EST_CARD\020\007*\211\001\n\tERankType\022\016\n\nERT_UNKN"
      "OW\020\000\022\024\n\020ERT_PLAYER_LEVEL\020\001\022\023\n\017ERT_FIGHT_"
      "VALUE\020\002\022\024\n\020ERT_PLAYER_MONEY\020\003\022\022\n\016ERT_CLA"
      "N_LEVEL\020\004\022\027\n\023ERT_CLAN_KINGSOURCE\020\005*G\n\010EN"
      "PCType\022\023\n\017ENPCTYPE_NORMAL\020\000\022\021\n\rENPCTYPE_"
      "HERO\020\001\022\023\n\017ENPCTYPE_TURRET\020\002*\216\001\n\022ESubNPCT"
      "ype_TURRET\022\030\n\024ENPCTYPE_SUB_STORAGE\020\000\022\030\n\024"
      "ENPCTYPE_SUB_WEAPONE\020\001\022\026\n\022ENPCTYPE_SUB_G"
      "UARD\020\002\022\025\n\021ENPCTYPE_SUB_BOMB\020\003\022\025\n\021ENPCTYP"
      "E_SUB_TRAP\020\004*E\n\nEMeleeType\022\017\n\013ETYPE_MELE"
      "E\020\000\022\024\n\020ETYPE_MELEE_DASH\020\001\022\020\n\014ETYPE_RANGE"
      "R\020\002*U\n\016EEffectObjType\022\025\n\021ETARGETTYPE_ENE"
      "MY\020\000\022\024\n\020ETARGETTYPE_SELF\020\001\022\026\n\022ETARGETTYP"
      "E_FRIEND\020\002*X\n\rEPropertyType\022\016\n\nEPT_UNKNO"
      "W\020\000\022\020\n\014EPT_STRENGTH\020\001\022\017\n\013EPT_AGILITY\020\002\022\024"
      "\n\020EPT_INTELLIGENCE\020\003*\236\001\n\020EGameElementTyp"
      "e\022\025\n\021EELEMENTTYPE_FIRE\020\000\022\026\n\022EELEMENTTYPE"
      "_LIGHT\020\001\022\025\n\021EELEMENTTYPE_WIND\020\002\022\024\n\020EELEM"
      "ENTTYPE_ICE\020\003\022\027\n\023EELEMENTTYPE_POISON\020\004\022\025"
      "\n\021EELEMENTTYPE_DARK\020\005*\244\001\n\013EBattleType\022\023\n"
      "\017EBT_SINGLE_MODE\020\000\022\022\n\016EBT_MULTI_MODE\020\001\022\021"
      "\n\rEBT_CLAN_MODE\020\002\022\026\n\022EBT_CLAN_BOSS_MODE\020"
      "\003\022\027\n\023EBT_WORLD_BOSS_MODE\020\004\022\021\n\rEBT_MOBA_M"
      "ODE\020\005\022\025\n\021EBT_SURVIVAL_MODE\020\006b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 10676);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "NFDefine.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_NFDefine_2eproto
namespace NFMsg {
const ::google::protobuf::EnumDescriptor* EGameEventCode_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[0];
}
bool EGameEventCode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 119:
    case 120:
    case 121:
    case 122:
    case 123:
    case 124:
    case 125:
    case 126:
    case 127:
    case 128:
    case 129:
    case 130:
    case 131:
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 137:
    case 138:
    case 139:
    case 140:
    case 141:
    case 142:
    case 143:
    case 200:
    case 201:
    case 202:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ESynMsgID_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[1];
}
bool ESynMsgID_IsValid(int value) {
  switch (value) {
    case 0:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameMsgID_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[2];
}
bool EGameMsgID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 10:
    case 11:
    case 12:
    case 20:
    case 21:
    case 22:
    case 30:
    case 31:
    case 32:
    case 40:
    case 41:
    case 42:
    case 60:
    case 61:
    case 62:
    case 70:
    case 80:
    case 81:
    case 82:
    case 90:
    case 100:
    case 101:
    case 102:
    case 103:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 120:
    case 122:
    case 130:
    case 131:
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 140:
    case 141:
    case 142:
    case 143:
    case 150:
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 200:
    case 201:
    case 202:
    case 203:
    case 210:
    case 211:
    case 212:
    case 214:
    case 215:
    case 216:
    case 217:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 226:
    case 227:
    case 228:
    case 229:
    case 250:
    case 251:
    case 260:
    case 300:
    case 1220:
    case 1221:
    case 1222:
    case 1223:
    case 1224:
    case 1225:
    case 1230:
    case 1231:
    case 1232:
    case 1233:
    case 1234:
    case 1235:
    case 1236:
    case 1237:
    case 1240:
    case 1241:
    case 1242:
    case 1243:
    case 1244:
    case 1245:
    case 1246:
    case 1247:
    case 1250:
    case 1251:
    case 1252:
    case 1253:
    case 1254:
    case 1255:
    case 1256:
    case 1257:
    case 1290:
    case 1291:
    case 1300:
    case 1301:
    case 1302:
    case 1303:
    case 1304:
    case 1305:
    case 1306:
    case 1307:
    case 1308:
    case 1309:
    case 1310:
    case 1400:
    case 1401:
    case 1402:
    case 1403:
    case 1404:
    case 1405:
    case 1406:
    case 1407:
    case 1408:
    case 1409:
    case 1410:
    case 1411:
    case 1450:
    case 1451:
    case 1500:
    case 1501:
    case 1502:
    case 1503:
    case 1504:
    case 1505:
    case 1506:
    case 1507:
    case 1508:
    case 1509:
    case 1520:
    case 1521:
    case 1522:
    case 1700:
    case 1701:
    case 1702:
    case 1800:
    case 1801:
    case 1802:
    case 1803:
    case 1810:
    case 1811:
    case 1812:
    case 1814:
    case 1840:
    case 1841:
    case 1860:
    case 1861:
    case 1862:
    case 1863:
    case 1864:
    case 1865:
    case 1866:
    case 1867:
    case 1868:
    case 1869:
    case 1870:
    case 1900:
    case 1901:
    case 1902:
    case 1903:
    case 1910:
    case 1911:
    case 1912:
    case 1913:
    case 1914:
    case 1915:
    case 1916:
    case 1917:
    case 1918:
    case 1919:
    case 5000:
    case 5001:
    case 5010:
    case 6000:
    case 6001:
    case 6002:
    case 6003:
    case 6010:
    case 6011:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10004:
    case 10005:
    case 10006:
    case 10007:
    case 10008:
    case 10200:
    case 20000:
    case 20001:
    case 20002:
    case 20003:
    case 20101:
    case 20102:
    case 20103:
    case 30001:
    case 30002:
    case 30003:
    case 30004:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ESkillType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[3];
}
bool ESkillType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EItemType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[4];
}
bool EItemType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameEquipSubType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[5];
}
bool EGameEquipSubType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameGemSubType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[6];
}
bool EGameGemSubType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameSupplySubType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[7];
}
bool EGameSupplySubType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameScrollSubType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[8];
}
bool EGameScrollSubType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameItemExpiredType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[9];
}
bool EGameItemExpiredType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EDrawDropItemState_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[10];
}
bool EDrawDropItemState_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGuildPowerType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[11];
}
bool EGuildPowerType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ETaskState_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[12];
}
bool ETaskState_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ETaskType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[13];
}
bool ETaskType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ESceneType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[14];
}
bool ESceneType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EPVPType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[15];
}
bool EPVPType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EShopType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[16];
}
bool EShopType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ERankType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[17];
}
bool ERankType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ENPCType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[18];
}
bool ENPCType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ESubNPCType_TURRET_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[19];
}
bool ESubNPCType_TURRET_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EMeleeType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[20];
}
bool EMeleeType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EEffectObjType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[21];
}
bool EEffectObjType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EPropertyType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[22];
}
bool EPropertyType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameElementType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[23];
}
bool EGameElementType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EBattleType_descriptor() {
  protobuf_NFDefine_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_NFDefine_2eproto::file_level_enum_descriptors[24];
}
bool EBattleType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace NFMsg
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
