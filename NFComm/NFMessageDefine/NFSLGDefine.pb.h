// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NFSLGDefine.proto

#ifndef PROTOBUF_NFSLGDefine_2eproto__INCLUDED
#define PROTOBUF_NFSLGDefine_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace NFMsg {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_NFSLGDefine_2eproto();
void protobuf_AssignDesc_NFSLGDefine_2eproto();
void protobuf_ShutdownFile_NFSLGDefine_2eproto();

class ReqAckBuyObjectFormShop;
class ReqAckMoveBuildObject;
class ReqUpBuildLv;
class ReqCreateItem;

enum ESLGBuildingType {
  EBT_BASE = 0,
  EBT_DEFENSE = 1,
  EBT_ARMY = 2,
  EBT_RESOURCE = 3,
  EBT_GUILD = 4,
  EBT_TEMPLE = 5,
  EBT_NUCLEAR = 6
};
bool ESLGBuildingType_IsValid(int value);
const ESLGBuildingType ESLGBuildingType_MIN = EBT_BASE;
const ESLGBuildingType ESLGBuildingType_MAX = EBT_NUCLEAR;
const int ESLGBuildingType_ARRAYSIZE = ESLGBuildingType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ESLGBuildingType_descriptor();
inline const ::std::string& ESLGBuildingType_Name(ESLGBuildingType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ESLGBuildingType_descriptor(), value);
}
inline bool ESLGBuildingType_Parse(
    const ::std::string& name, ESLGBuildingType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ESLGBuildingType>(
    ESLGBuildingType_descriptor(), name, value);
}
enum ESLGBuildingSubType {
  EBST_HALL = 0,
  EBST_MASHROOM = 70,
  EBST_BUSH = 71,
  EBST_BRANCH_1 = 72,
  EBST_BRANCH_2 = 73,
  EBST_BRANCH_3 = 74,
  EBST_TREE_1 = 75,
  EBST_TREE_2 = 76,
  EBST_TREE_3 = 77,
  EBST_STONE_1 = 78,
  EBST_STONE_2 = 79,
  EBST_STONE_3 = 80,
  EBST_TOMB = 81,
  EBST_DARK_TOMB = 82,
  EBST_PYXIS = 83,
  EBST_TORCH = 84,
  EBST__WHITE_FLAG = 85,
  EBST_CANNON = 101,
  EBST_MORTAR = 102,
  EBST_ARCHER_TOWER = 103,
  EBST_WIZARD_TOWER = 104,
  EBST_AIR_TOWER = 105,
  EBST_TESLA_TOWER = 106,
  EBST_WALL = 107,
  EBST_X_BOW = 108,
  EBST_BOMB = 109,
  EBST_AIR_BOMB = 110,
  EBST_SPRING_TRAP = 111,
  EBST_HELL_TOWER = 112,
  EBST_CAMP = 200,
  EBST_BARRACK = 201,
  EBST_TEC_FACTORY = 202,
  EBST_DARK_BARRACK = 203,
  EBST_DRUG_FACTORY = 204,
  EBST_KING_ALTAR = 205,
  EBST_QUEEN_ALTAR = 206,
  EBST_GOLD_COLLECTOR = 300,
  EBST_GOLD_STORAGE = 301,
  EBST_STONE_COLLECTOR = 302,
  EBST_STONE_STORAGE = 303,
  EBST_STEEL_COLLECTOR = 304,
  EBST_STEEL_STORAGE = 305,
  EBST_DIAMOND_COLLECTOR = 306,
  EBST_DIAMOND_STORAGE = 307,
  EBST_HUT = 308,
  EBST_GUILD_CASTLE = 400,
  EBST_GUILD_CRYPT = 401,
  EBST_ELEMENT_LAB = 500,
  EBST_NUCLEAR_FACTORY = 600
};
bool ESLGBuildingSubType_IsValid(int value);
const ESLGBuildingSubType ESLGBuildingSubType_MIN = EBST_HALL;
const ESLGBuildingSubType ESLGBuildingSubType_MAX = EBST_NUCLEAR_FACTORY;
const int ESLGBuildingSubType_ARRAYSIZE = ESLGBuildingSubType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ESLGBuildingSubType_descriptor();
inline const ::std::string& ESLGBuildingSubType_Name(ESLGBuildingSubType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ESLGBuildingSubType_descriptor(), value);
}
inline bool ESLGBuildingSubType_Parse(
    const ::std::string& name, ESLGBuildingSubType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ESLGBuildingSubType>(
    ESLGBuildingSubType_descriptor(), name, value);
}
enum ESLGArmyType {
  EAT_FOOTMAN = 0,
  EAT_ARCHER = 1,
  EAT_GOBLIN = 2,
  EAT_GIANT = 3,
  EAT_WALL_BREAKER = 4,
  EAT_BALLOON = 5,
  EAT_HEALER = 6,
  EAT_WIZARD = 7,
  EAT_DRAGON = 8,
  EAT_PEKKA = 9,
  EAT_MINION = 20,
  EAT_HOG_RIDER = 21,
  EAT_VALKYRIE = 22,
  EAT_GOLEM = 23
};
bool ESLGArmyType_IsValid(int value);
const ESLGArmyType ESLGArmyType_MIN = EAT_FOOTMAN;
const ESLGArmyType ESLGArmyType_MAX = EAT_GOLEM;
const int ESLGArmyType_ARRAYSIZE = ESLGArmyType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ESLGArmyType_descriptor();
inline const ::std::string& ESLGArmyType_Name(ESLGArmyType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ESLGArmyType_descriptor(), value);
}
inline bool ESLGArmyType_Parse(
    const ::std::string& name, ESLGArmyType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ESLGArmyType>(
    ESLGArmyType_descriptor(), name, value);
}
enum ESLGFuncType {
  EFT_INFO = 0,
  EFT_BOOOST = 1,
  EFT_LVLUP = 2,
  EFT_CREATE_SOLDER = 3,
  EFT_CREATE_SPEEL = 4,
  EFT_RESEARCH = 5,
  EFT_COLLECT_GOLD = 6,
  EFT_COLLECT_STONE = 7,
  EFT_COLLECT_STEEL = 8,
  EFT_COLLECT_DIAMOND = 9,
  EFT_SELL = 10,
  EFT_REPAIR = 11,
  EFT_CANCEL = 12,
  EFT_FINISH = 13
};
bool ESLGFuncType_IsValid(int value);
const ESLGFuncType ESLGFuncType_MIN = EFT_INFO;
const ESLGFuncType ESLGFuncType_MAX = EFT_FINISH;
const int ESLGFuncType_ARRAYSIZE = ESLGFuncType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ESLGFuncType_descriptor();
inline const ::std::string& ESLGFuncType_Name(ESLGFuncType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ESLGFuncType_descriptor(), value);
}
inline bool ESLGFuncType_Parse(
    const ::std::string& name, ESLGFuncType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ESLGFuncType>(
    ESLGFuncType_descriptor(), name, value);
}
enum ESLGBuildingState {
  EBS_IDLE = 0,
  EBS_BOOST = 1,
  EBS_UPGRADE = 2
};
bool ESLGBuildingState_IsValid(int value);
const ESLGBuildingState ESLGBuildingState_MIN = EBS_IDLE;
const ESLGBuildingState ESLGBuildingState_MAX = EBS_UPGRADE;
const int ESLGBuildingState_ARRAYSIZE = ESLGBuildingState_MAX + 1;

const ::google::protobuf::EnumDescriptor* ESLGBuildingState_descriptor();
inline const ::std::string& ESLGBuildingState_Name(ESLGBuildingState value) {
  return ::google::protobuf::internal::NameOfEnum(
    ESLGBuildingState_descriptor(), value);
}
inline bool ESLGBuildingState_Parse(
    const ::std::string& name, ESLGBuildingState* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ESLGBuildingState>(
    ESLGBuildingState_descriptor(), name, value);
}
// ===================================================================

class ReqAckBuyObjectFormShop : public ::google::protobuf::Message {
 public:
  ReqAckBuyObjectFormShop();
  virtual ~ReqAckBuyObjectFormShop();

  ReqAckBuyObjectFormShop(const ReqAckBuyObjectFormShop& from);

  inline ReqAckBuyObjectFormShop& operator=(const ReqAckBuyObjectFormShop& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqAckBuyObjectFormShop& default_instance();

  void Swap(ReqAckBuyObjectFormShop* other);

  // implements Message ----------------------------------------------

  ReqAckBuyObjectFormShop* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqAckBuyObjectFormShop& from);
  void MergeFrom(const ReqAckBuyObjectFormShop& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string config_id = 1;
  inline bool has_config_id() const;
  inline void clear_config_id();
  static const int kConfigIdFieldNumber = 1;
  inline const ::std::string& config_id() const;
  inline void set_config_id(const ::std::string& value);
  inline void set_config_id(const char* value);
  inline void set_config_id(const char* value, size_t size);
  inline ::std::string* mutable_config_id();
  inline ::std::string* release_config_id();
  inline void set_allocated_config_id(::std::string* config_id);

  // required float x = 2;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 2;
  inline float x() const;
  inline void set_x(float value);

  // required float y = 3;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 3;
  inline float y() const;
  inline void set_y(float value);

  // required float z = 4;
  inline bool has_z() const;
  inline void clear_z();
  static const int kZFieldNumber = 4;
  inline float z() const;
  inline void set_z(float value);

  // @@protoc_insertion_point(class_scope:NFMsg.ReqAckBuyObjectFormShop)
 private:
  inline void set_has_config_id();
  inline void clear_has_config_id();
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_z();
  inline void clear_has_z();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* config_id_;
  float x_;
  float y_;
  float z_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_NFSLGDefine_2eproto();
  friend void protobuf_AssignDesc_NFSLGDefine_2eproto();
  friend void protobuf_ShutdownFile_NFSLGDefine_2eproto();

  void InitAsDefaultInstance();
  static ReqAckBuyObjectFormShop* default_instance_;
};
// -------------------------------------------------------------------

class ReqAckMoveBuildObject : public ::google::protobuf::Message {
 public:
  ReqAckMoveBuildObject();
  virtual ~ReqAckMoveBuildObject();

  ReqAckMoveBuildObject(const ReqAckMoveBuildObject& from);

  inline ReqAckMoveBuildObject& operator=(const ReqAckMoveBuildObject& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqAckMoveBuildObject& default_instance();

  void Swap(ReqAckMoveBuildObject* other);

  // implements Message ----------------------------------------------

  ReqAckMoveBuildObject* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqAckMoveBuildObject& from);
  void MergeFrom(const ReqAckMoveBuildObject& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 row = 1;
  inline bool has_row() const;
  inline void clear_row();
  static const int kRowFieldNumber = 1;
  inline ::google::protobuf::int32 row() const;
  inline void set_row(::google::protobuf::int32 value);

  // required int64 object_guid = 2;
  inline bool has_object_guid() const;
  inline void clear_object_guid();
  static const int kObjectGuidFieldNumber = 2;
  inline ::google::protobuf::int64 object_guid() const;
  inline void set_object_guid(::google::protobuf::int64 value);

  // required float x = 3;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 3;
  inline float x() const;
  inline void set_x(float value);

  // required float y = 4;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 4;
  inline float y() const;
  inline void set_y(float value);

  // required float z = 5;
  inline bool has_z() const;
  inline void clear_z();
  static const int kZFieldNumber = 5;
  inline float z() const;
  inline void set_z(float value);

  // @@protoc_insertion_point(class_scope:NFMsg.ReqAckMoveBuildObject)
 private:
  inline void set_has_row();
  inline void clear_has_row();
  inline void set_has_object_guid();
  inline void clear_has_object_guid();
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_z();
  inline void clear_has_z();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 object_guid_;
  ::google::protobuf::int32 row_;
  float x_;
  float y_;
  float z_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_NFSLGDefine_2eproto();
  friend void protobuf_AssignDesc_NFSLGDefine_2eproto();
  friend void protobuf_ShutdownFile_NFSLGDefine_2eproto();

  void InitAsDefaultInstance();
  static ReqAckMoveBuildObject* default_instance_;
};
// -------------------------------------------------------------------

class ReqUpBuildLv : public ::google::protobuf::Message {
 public:
  ReqUpBuildLv();
  virtual ~ReqUpBuildLv();

  ReqUpBuildLv(const ReqUpBuildLv& from);

  inline ReqUpBuildLv& operator=(const ReqUpBuildLv& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqUpBuildLv& default_instance();

  void Swap(ReqUpBuildLv* other);

  // implements Message ----------------------------------------------

  ReqUpBuildLv* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqUpBuildLv& from);
  void MergeFrom(const ReqUpBuildLv& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 row = 1;
  inline bool has_row() const;
  inline void clear_row();
  static const int kRowFieldNumber = 1;
  inline ::google::protobuf::int32 row() const;
  inline void set_row(::google::protobuf::int32 value);

  // required int64 object_guid = 2;
  inline bool has_object_guid() const;
  inline void clear_object_guid();
  static const int kObjectGuidFieldNumber = 2;
  inline ::google::protobuf::int64 object_guid() const;
  inline void set_object_guid(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:NFMsg.ReqUpBuildLv)
 private:
  inline void set_has_row();
  inline void clear_has_row();
  inline void set_has_object_guid();
  inline void clear_has_object_guid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 object_guid_;
  ::google::protobuf::int32 row_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_NFSLGDefine_2eproto();
  friend void protobuf_AssignDesc_NFSLGDefine_2eproto();
  friend void protobuf_ShutdownFile_NFSLGDefine_2eproto();

  void InitAsDefaultInstance();
  static ReqUpBuildLv* default_instance_;
};
// -------------------------------------------------------------------

class ReqCreateItem : public ::google::protobuf::Message {
 public:
  ReqCreateItem();
  virtual ~ReqCreateItem();

  ReqCreateItem(const ReqCreateItem& from);

  inline ReqCreateItem& operator=(const ReqCreateItem& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqCreateItem& default_instance();

  void Swap(ReqCreateItem* other);

  // implements Message ----------------------------------------------

  ReqCreateItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqCreateItem& from);
  void MergeFrom(const ReqCreateItem& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 row = 1;
  inline bool has_row() const;
  inline void clear_row();
  static const int kRowFieldNumber = 1;
  inline ::google::protobuf::int32 row() const;
  inline void set_row(::google::protobuf::int32 value);

  // required int64 object_guid = 2;
  inline bool has_object_guid() const;
  inline void clear_object_guid();
  static const int kObjectGuidFieldNumber = 2;
  inline ::google::protobuf::int64 object_guid() const;
  inline void set_object_guid(::google::protobuf::int64 value);

  // required string config_id = 3;
  inline bool has_config_id() const;
  inline void clear_config_id();
  static const int kConfigIdFieldNumber = 3;
  inline const ::std::string& config_id() const;
  inline void set_config_id(const ::std::string& value);
  inline void set_config_id(const char* value);
  inline void set_config_id(const char* value, size_t size);
  inline ::std::string* mutable_config_id();
  inline ::std::string* release_config_id();
  inline void set_allocated_config_id(::std::string* config_id);

  // required int32 count = 4;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 4;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:NFMsg.ReqCreateItem)
 private:
  inline void set_has_row();
  inline void clear_has_row();
  inline void set_has_object_guid();
  inline void clear_has_object_guid();
  inline void set_has_config_id();
  inline void clear_has_config_id();
  inline void set_has_count();
  inline void clear_has_count();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 object_guid_;
  ::google::protobuf::int32 row_;
  ::google::protobuf::int32 count_;
  ::std::string* config_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_NFSLGDefine_2eproto();
  friend void protobuf_AssignDesc_NFSLGDefine_2eproto();
  friend void protobuf_ShutdownFile_NFSLGDefine_2eproto();

  void InitAsDefaultInstance();
  static ReqCreateItem* default_instance_;
};
// ===================================================================


// ===================================================================

// ReqAckBuyObjectFormShop

// required string config_id = 1;
inline bool ReqAckBuyObjectFormShop::has_config_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqAckBuyObjectFormShop::set_has_config_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqAckBuyObjectFormShop::clear_has_config_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqAckBuyObjectFormShop::clear_config_id() {
  if (config_id_ != &::google::protobuf::internal::kEmptyString) {
    config_id_->clear();
  }
  clear_has_config_id();
}
inline const ::std::string& ReqAckBuyObjectFormShop::config_id() const {
  return *config_id_;
}
inline void ReqAckBuyObjectFormShop::set_config_id(const ::std::string& value) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(value);
}
inline void ReqAckBuyObjectFormShop::set_config_id(const char* value) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(value);
}
inline void ReqAckBuyObjectFormShop::set_config_id(const char* value, size_t size) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqAckBuyObjectFormShop::mutable_config_id() {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  return config_id_;
}
inline ::std::string* ReqAckBuyObjectFormShop::release_config_id() {
  clear_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = config_id_;
    config_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ReqAckBuyObjectFormShop::set_allocated_config_id(::std::string* config_id) {
  if (config_id_ != &::google::protobuf::internal::kEmptyString) {
    delete config_id_;
  }
  if (config_id) {
    set_has_config_id();
    config_id_ = config_id;
  } else {
    clear_has_config_id();
    config_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required float x = 2;
inline bool ReqAckBuyObjectFormShop::has_x() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqAckBuyObjectFormShop::set_has_x() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqAckBuyObjectFormShop::clear_has_x() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqAckBuyObjectFormShop::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline float ReqAckBuyObjectFormShop::x() const {
  return x_;
}
inline void ReqAckBuyObjectFormShop::set_x(float value) {
  set_has_x();
  x_ = value;
}

// required float y = 3;
inline bool ReqAckBuyObjectFormShop::has_y() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqAckBuyObjectFormShop::set_has_y() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqAckBuyObjectFormShop::clear_has_y() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqAckBuyObjectFormShop::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float ReqAckBuyObjectFormShop::y() const {
  return y_;
}
inline void ReqAckBuyObjectFormShop::set_y(float value) {
  set_has_y();
  y_ = value;
}

// required float z = 4;
inline bool ReqAckBuyObjectFormShop::has_z() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqAckBuyObjectFormShop::set_has_z() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqAckBuyObjectFormShop::clear_has_z() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqAckBuyObjectFormShop::clear_z() {
  z_ = 0;
  clear_has_z();
}
inline float ReqAckBuyObjectFormShop::z() const {
  return z_;
}
inline void ReqAckBuyObjectFormShop::set_z(float value) {
  set_has_z();
  z_ = value;
}

// -------------------------------------------------------------------

// ReqAckMoveBuildObject

// optional int32 row = 1;
inline bool ReqAckMoveBuildObject::has_row() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqAckMoveBuildObject::set_has_row() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqAckMoveBuildObject::clear_has_row() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqAckMoveBuildObject::clear_row() {
  row_ = 0;
  clear_has_row();
}
inline ::google::protobuf::int32 ReqAckMoveBuildObject::row() const {
  return row_;
}
inline void ReqAckMoveBuildObject::set_row(::google::protobuf::int32 value) {
  set_has_row();
  row_ = value;
}

// required int64 object_guid = 2;
inline bool ReqAckMoveBuildObject::has_object_guid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqAckMoveBuildObject::set_has_object_guid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqAckMoveBuildObject::clear_has_object_guid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqAckMoveBuildObject::clear_object_guid() {
  object_guid_ = GOOGLE_LONGLONG(0);
  clear_has_object_guid();
}
inline ::google::protobuf::int64 ReqAckMoveBuildObject::object_guid() const {
  return object_guid_;
}
inline void ReqAckMoveBuildObject::set_object_guid(::google::protobuf::int64 value) {
  set_has_object_guid();
  object_guid_ = value;
}

// required float x = 3;
inline bool ReqAckMoveBuildObject::has_x() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqAckMoveBuildObject::set_has_x() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqAckMoveBuildObject::clear_has_x() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqAckMoveBuildObject::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline float ReqAckMoveBuildObject::x() const {
  return x_;
}
inline void ReqAckMoveBuildObject::set_x(float value) {
  set_has_x();
  x_ = value;
}

// required float y = 4;
inline bool ReqAckMoveBuildObject::has_y() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqAckMoveBuildObject::set_has_y() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqAckMoveBuildObject::clear_has_y() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqAckMoveBuildObject::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float ReqAckMoveBuildObject::y() const {
  return y_;
}
inline void ReqAckMoveBuildObject::set_y(float value) {
  set_has_y();
  y_ = value;
}

// required float z = 5;
inline bool ReqAckMoveBuildObject::has_z() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ReqAckMoveBuildObject::set_has_z() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ReqAckMoveBuildObject::clear_has_z() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ReqAckMoveBuildObject::clear_z() {
  z_ = 0;
  clear_has_z();
}
inline float ReqAckMoveBuildObject::z() const {
  return z_;
}
inline void ReqAckMoveBuildObject::set_z(float value) {
  set_has_z();
  z_ = value;
}

// -------------------------------------------------------------------

// ReqUpBuildLv

// optional int32 row = 1;
inline bool ReqUpBuildLv::has_row() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqUpBuildLv::set_has_row() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqUpBuildLv::clear_has_row() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqUpBuildLv::clear_row() {
  row_ = 0;
  clear_has_row();
}
inline ::google::protobuf::int32 ReqUpBuildLv::row() const {
  return row_;
}
inline void ReqUpBuildLv::set_row(::google::protobuf::int32 value) {
  set_has_row();
  row_ = value;
}

// required int64 object_guid = 2;
inline bool ReqUpBuildLv::has_object_guid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqUpBuildLv::set_has_object_guid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqUpBuildLv::clear_has_object_guid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqUpBuildLv::clear_object_guid() {
  object_guid_ = GOOGLE_LONGLONG(0);
  clear_has_object_guid();
}
inline ::google::protobuf::int64 ReqUpBuildLv::object_guid() const {
  return object_guid_;
}
inline void ReqUpBuildLv::set_object_guid(::google::protobuf::int64 value) {
  set_has_object_guid();
  object_guid_ = value;
}

// -------------------------------------------------------------------

// ReqCreateItem

// optional int32 row = 1;
inline bool ReqCreateItem::has_row() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqCreateItem::set_has_row() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqCreateItem::clear_has_row() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqCreateItem::clear_row() {
  row_ = 0;
  clear_has_row();
}
inline ::google::protobuf::int32 ReqCreateItem::row() const {
  return row_;
}
inline void ReqCreateItem::set_row(::google::protobuf::int32 value) {
  set_has_row();
  row_ = value;
}

// required int64 object_guid = 2;
inline bool ReqCreateItem::has_object_guid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqCreateItem::set_has_object_guid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqCreateItem::clear_has_object_guid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqCreateItem::clear_object_guid() {
  object_guid_ = GOOGLE_LONGLONG(0);
  clear_has_object_guid();
}
inline ::google::protobuf::int64 ReqCreateItem::object_guid() const {
  return object_guid_;
}
inline void ReqCreateItem::set_object_guid(::google::protobuf::int64 value) {
  set_has_object_guid();
  object_guid_ = value;
}

// required string config_id = 3;
inline bool ReqCreateItem::has_config_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqCreateItem::set_has_config_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqCreateItem::clear_has_config_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqCreateItem::clear_config_id() {
  if (config_id_ != &::google::protobuf::internal::kEmptyString) {
    config_id_->clear();
  }
  clear_has_config_id();
}
inline const ::std::string& ReqCreateItem::config_id() const {
  return *config_id_;
}
inline void ReqCreateItem::set_config_id(const ::std::string& value) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(value);
}
inline void ReqCreateItem::set_config_id(const char* value) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(value);
}
inline void ReqCreateItem::set_config_id(const char* value, size_t size) {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  config_id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqCreateItem::mutable_config_id() {
  set_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    config_id_ = new ::std::string;
  }
  return config_id_;
}
inline ::std::string* ReqCreateItem::release_config_id() {
  clear_has_config_id();
  if (config_id_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = config_id_;
    config_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ReqCreateItem::set_allocated_config_id(::std::string* config_id) {
  if (config_id_ != &::google::protobuf::internal::kEmptyString) {
    delete config_id_;
  }
  if (config_id) {
    set_has_config_id();
    config_id_ = config_id;
  } else {
    clear_has_config_id();
    config_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 count = 4;
inline bool ReqCreateItem::has_count() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqCreateItem::set_has_count() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqCreateItem::clear_has_count() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqCreateItem::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 ReqCreateItem::count() const {
  return count_;
}
inline void ReqCreateItem::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace NFMsg

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::ESLGBuildingType>() {
  return ::NFMsg::ESLGBuildingType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::ESLGBuildingSubType>() {
  return ::NFMsg::ESLGBuildingSubType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::ESLGArmyType>() {
  return ::NFMsg::ESLGArmyType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::ESLGFuncType>() {
  return ::NFMsg::ESLGFuncType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::ESLGBuildingState>() {
  return ::NFMsg::ESLGBuildingState_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_NFSLGDefine_2eproto__INCLUDED
