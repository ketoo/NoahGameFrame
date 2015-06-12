// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NFDefine.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "NFDefine.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace NFMsg {

namespace {

const ::google::protobuf::EnumDescriptor* EGameEventCode_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* EGameMsgID_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* EAwardType_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* EGuildPowerType_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* EBagItemList_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_NFDefine_2eproto() {
  protobuf_AddDesc_NFDefine_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "NFDefine.proto");
  GOOGLE_CHECK(file != NULL);
  EGameEventCode_descriptor_ = file->enum_type(0);
  EGameMsgID_descriptor_ = file->enum_type(1);
  EAwardType_descriptor_ = file->enum_type(2);
  EGuildPowerType_descriptor_ = file->enum_type(3);
  EBagItemList_descriptor_ = file->enum_type(4);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_NFDefine_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_NFDefine_2eproto() {
}

void protobuf_AddDesc_NFDefine_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016NFDefine.proto\022\005NFMsg*\266\n\n\016EGameEventCo"
    "de\022\025\n\021EGEC_UNKOWN_ERROR\020\000\022\020\n\014EGEC_SUCCES"
    "S\020\001\022\026\n\022EGEC_ACCOUNT_EXIST\020\002\022\033\n\027EGEC_ACCO"
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
    "RVER4ZONE\020\216\001\022\027\n\022EGEC_INVALID_SKILL\020\217\001\022\034\n"
    "\027EGEC_ENTER_GAME_SUCCESS\020\220\001\022\036\n\031EGEC_CREA"
    "TE_GUILD_SUCCESS\020\310\001\022\034\n\027EGEC_JOIN_GUILD_S"
    "UCCESS\020\311\001\022\035\n\030EGEC_LEAVE_GUILD_SUCCESS\020\312\001"
    "*\347\026\n\nEGameMsgID\022\017\n\013EGMI_UNKNOW\020\000\022\025\n\021EGMI"
    "_EVENT_RESULT\020\001\022\030\n\024EGMI_EVENT_TRANSPOND\020"
    "\002\022\025\n\021EGMI_CLOSE_SOCKET\020\003\022\035\n\031EGMI_MTL_WOR"
    "LD_REGISTERED\020\n\022\037\n\033EGMI_MTL_WORLD_UNREGI"
    "STERED\020\013\022\032\n\026EGMI_MTL_WORLD_REFRESH\020\014\022\035\n\031"
    "EGMI_LTM_LOGIN_REGISTERED\020\024\022\037\n\033EGMI_LTM_"
    "LOGIN_UNREGISTERED\020\025\022\032\n\026EGMI_LTM_LOGIN_R"
    "EFRESH\020\026\022\036\n\032EGMI_PTWG_PROXY_REGISTERED\020\036"
    "\022 \n\034EGMI_PTWG_PROXY_UNREGISTERED\020\037\022\033\n\027EG"
    "MI_PTWG_PROXY_REFRESH\020 \022\034\n\030EGMI_GTW_GAME"
    "_REGISTERED\020(\022\036\n\032EGMI_GTW_GAME_UNREGISTE"
    "RED\020)\022\031\n\025EGMI_GTW_GAME_REFRESH\020*\022\025\n\021EGMI"
    "_STS_NET_INFO\0202\022\027\n\023EGMI_STS_HEART_BEAT\020d"
    "\022\022\n\016EGMI_REQ_LOGIN\020e\022\022\n\016EGMI_ACK_LOGIN\020f"
    "\022\023\n\017EGMI_REQ_LOGOUT\020g\022\027\n\023EGMI_REQ_WORLD_"
    "LIST\020n\022\027\n\023EGMI_ACK_WORLD_LIST\020o\022\032\n\026EGMI_"
    "REQ_CONNECT_WORLD\020p\022\032\n\026EGMI_ACK_CONNECT_"
    "WORLD\020q\022 \n\034EGMI_REQ_KICK_CLIENT_INWORLD\020"
    "r\022\030\n\024EGMI_REQ_CONNECT_KEY\020x\022\030\n\024EGMI_ACK_"
    "CONNECT_KEY\020z\022\033\n\026EGMI_REQ_SELECT_SERVER\020"
    "\202\001\022\033\n\026EGMI_ACK_SELECT_SERVER\020\203\001\022\027\n\022EGMI_"
    "REQ_ROLE_LIST\020\204\001\022\027\n\022EGMI_ACK_ROLE_LIST\020\205"
    "\001\022\031\n\024EGMI_REQ_CREATE_ROLE\020\206\001\022\031\n\024EGMI_REQ"
    "_DELETE_ROLE\020\207\001\022\032\n\025EGMI_REQ_RECOVER_ROLE"
    "\020\210\001\022\030\n\023EGMI_REQ_ENTER_GAME\020\226\001\022\030\n\023EGMI_AC"
    "K_ENTER_GAME\020\227\001\022\030\n\023EGMI_REQ_LEAVE_GAME\020\230"
    "\001\022\030\n\023EGMI_ACK_LEAVE_GAME\020\231\001\022\027\n\022EGMI_REQ_"
    "SWAP_GAME\020\232\001\022\030\n\023EGMI_REQ_SWAP_SCENE\020\233\001\022\030"
    "\n\023EGMI_ACK_SWAP_SCENE\020\234\001\022\030\n\023EGMI_REQ_END"
    "_BATTLE\020\235\001\022\030\n\023EGMI_ACK_END_BATTLE\020\236\001\022\032\n\025"
    "EGMI_ACK_OBJECT_ENTRY\020\310\001\022\032\n\025EGMI_ACK_OBJ"
    "ECT_LEAVE\020\311\001\022#\n\036EGMI_ACK_OBJECT_PROPERTY"
    "_ENTRY\020\312\001\022!\n\034EGMI_ACK_OBJECT_RECORD_ENTR"
    "Y\020\313\001\022\032\n\025EGMI_ACK_PROPERTY_INT\020\322\001\022\034\n\027EGMI"
    "_ACK_PROPERTY_FLOAT\020\323\001\022\035\n\030EGMI_ACK_PROPE"
    "RTY_STRING\020\324\001\022\035\n\030EGMI_ACK_PROPERTY_DOUBL"
    "E\020\325\001\022\035\n\030EGMI_ACK_PROPERTY_OBJECT\020\326\001\022\025\n\020E"
    "GMI_ACK_ADD_ROW\020\334\001\022\030\n\023EGMI_ACK_REMOVE_RO"
    "W\020\335\001\022\026\n\021EGMI_ACK_SWAP_ROW\020\336\001\022\030\n\023EGMI_ACK"
    "_RECORD_INT\020\337\001\022\032\n\025EGMI_ACK_RECORD_FLOAT\020"
    "\340\001\022\033\n\026EGMI_ACK_RECORD_DOUBLE\020\341\001\022\033\n\026EGMI_"
    "ACK_RECORD_STRING\020\342\001\022\033\n\026EGMI_ACK_RECORD_"
    "OBJECT\020\343\001\022\032\n\025EGMI_ACK_RECORD_CLEAR\020\344\001\022\031\n"
    "\024EGMI_ACK_RECORD_SORT\020\345\001\022\022\n\rEGMI_REQ_MOV"
    "E\020\346\001\022\022\n\rEGMI_ACK_MOVE\020\347\001\022\031\n\024EGMI_REQ_MOV"
    "E_IMMUNE\020\350\001\022\031\n\024EGMI_ACK_MOVE_IMMUNE\020\351\001\022\033"
    "\n\026EGMI_REQ_SKILL_OBJECTX\020\360\001\022\033\n\026EGMI_ACK_"
    "SKILL_OBJECTX\020\361\001\022\027\n\022EGMI_REQ_SKILL_POS\020\362"
    "\001\022\027\n\022EGMI_ACK_SKILL_POS\020\363\001\022\031\n\024EGMI_REQ_I"
    "TEM_OBJECT\020\364\001\022\031\n\024EGMI_ACK_ITEM_OBJECT\020\365\001"
    "\022\026\n\021EGMI_REQ_ITEM_POS\020\366\001\022\026\n\021EGMI_ACK_ITE"
    "M_POS\020\367\001\022\022\n\rEGMI_REQ_CHAT\020\372\001\022\022\n\rEGMI_ACK"
    "_CHAT\020\373\001\022\027\n\022EGMI_REQ_SALE_ITEM\020\374\001\022\030\n\023EGM"
    "I_REQ_SPLIT_ITEM\020\375\001\022\032\n\025EGMI_REQ_PRODUCE_"
    "ITEM\020\376\001\022\027\n\022EGMI_REQ_PICK_ITEM\020\377\001\022\031\n\024EGMI"
    "_REQ_ACCEPT_TASK\020\200\002\022\034\n\027EGMI_REQ_COMPELET"
    "E_TASK\020\201\002\022\026\n\021EGMI_REQ_JOIN_PVP\020\204\002\022\026\n\021EGM"
    "I_REQ_EXIT_PVP\020\205\002\022\027\n\022EGMT_ACK_START_PVP\020"
    "\206\002\022\032\n\025EGMI_REQ_CREATE_GUILD\020\216\002\022\030\n\023EGMI_R"
    "EQ_JOIN_GUILD\020\217\002\022\031\n\024EGMI_REQ_LEAVE_GUILD"
    "\020\220\002\022\027\n\022EGMI_REQ_OPR_GUILD\020\221\002\022\032\n\025EGMI_ACK"
    "_CREATE_GUILD\020\230\002\022\036\n\031EGMI_REQ_CMD_PROPERT"
    "Y_INT\020\350\007\022\036\n\031EGMI_REQ_CMD_PROPERTY_STR\020\351\007"
    "\022!\n\034EGMI_REQ_CMD_PROPERTY_OBJECT\020\352\007\022 \n\033E"
    "GMI_REQ_CMD_PROPERTY_FLOAT\020\353\007\022\034\n\027EGMI_RE"
    "Q_CMD_RECORD_INT\020\354\007\022\034\n\027EGMI_REQ_CMD_RECO"
    "RD_STR\020\355\007\022\037\n\032EGMI_REQ_CMD_RECORD_OBJECT\020"
    "\356\007\022\036\n\031EGMI_REQ_CMD_RECORD_FLOAT\020\357\007\022\033\n\026EG"
    "MI_REQ_BUY_FORM_SHOP\020\220N\022\033\n\026EGMI_ACK_BUY_"
    "FORM_SHOP\020\221N\022\037\n\032EGMI_REQ_MOVE_BUILD_OBJE"
    "CT\020\222N\022\037\n\032EGMI_ACK_MOVE_BUILD_OBJECT\020\223N\022\032"
    "\n\025EGMI_REQ_UP_BUILD_LVL\020\365N\022\031\n\024EGMI_REQ_C"
    "REATE_ITEM\020\366N\022\033\n\026EGMI_REQ_BUILD_OPERATE\020"
    "\367N*8\n\nEAwardType\022\025\n\021AWARD_TYPE_NORMAL\020\000\022"
    "\023\n\017AWARD_TYPE_HERO\020\001*s\n\017EGuildPowerType\022"
    "\033\n\027GUILD_POWER_TYPE_NORMAL\020\000\022#\n\037GUILD_PO"
    "WER_TYPE_VICE_PRESIDENT\020\001\022\036\n\032GUILD_POWER"
    "_TYPE_PRESIDENT\020\002*\362\002\n\014EBagItemList\022\r\n\tEB"
    "ag_GUID\020\000\022\021\n\rEBag_ConfigID\020\001\022\022\n\016EBag_Ite"
    "mCount\020\002\022\016\n\nEBag_Bound\020\003\022\024\n\020EBag_Expired"
    "Type\020\004\022\022\n\016EBag_SlotCount\020\005\022\024\n\020EBag_Inlay"
    "Stone1\020\006\022\024\n\020EBag_InlayStone2\020\007\022\024\n\020EBag_I"
    "nlayStone3\020\010\022\024\n\020EBag_InlayStone4\020\t\022\024\n\020EB"
    "ag_InlayStone5\020\n\022\024\n\020EBag_InlayStone6\020\013\022\026"
    "\n\022EBag_EnhancedLevel\020\014\022\027\n\023EBag_Sagecraft"
    "Level\020\r\022\027\n\023EBag_SagecraftStone\020\016\022\025\n\021EBag"
    "_RandProperty\020\017\022\r\n\tEBag_Date\020\020", 4830);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "NFDefine.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_NFDefine_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_NFDefine_2eproto {
  StaticDescriptorInitializer_NFDefine_2eproto() {
    protobuf_AddDesc_NFDefine_2eproto();
  }
} static_descriptor_initializer_NFDefine_2eproto_;
const ::google::protobuf::EnumDescriptor* EGameEventCode_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EGameEventCode_descriptor_;
}
bool EGameEventCode_IsValid(int value) {
  switch(value) {
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
    case 144:
    case 200:
    case 201:
    case 202:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGameMsgID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EGameMsgID_descriptor_;
}
bool EGameMsgID_IsValid(int value) {
  switch(value) {
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
    case 50:
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
    case 150:
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
    case 200:
    case 201:
    case 202:
    case 203:
    case 210:
    case 211:
    case 212:
    case 213:
    case 214:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 232:
    case 233:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 245:
    case 246:
    case 247:
    case 250:
    case 251:
    case 252:
    case 253:
    case 254:
    case 255:
    case 256:
    case 257:
    case 260:
    case 261:
    case 262:
    case 270:
    case 271:
    case 272:
    case 273:
    case 280:
    case 1000:
    case 1001:
    case 1002:
    case 1003:
    case 1004:
    case 1005:
    case 1006:
    case 1007:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10101:
    case 10102:
    case 10103:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EAwardType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EAwardType_descriptor_;
}
bool EAwardType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EGuildPowerType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EGuildPowerType_descriptor_;
}
bool EGuildPowerType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* EBagItemList_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EBagItemList_descriptor_;
}
bool EBagItemList_IsValid(int value) {
  switch(value) {
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
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace NFMsg

// @@protoc_insertion_point(global_scope)
