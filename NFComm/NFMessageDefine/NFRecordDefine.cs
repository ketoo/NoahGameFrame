//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: NFRecordDefine.proto
namespace NFMsg
{
    [global::ProtoBuf.ProtoContract(Name=@"BuildingList_RecordColType")]
    public enum BuildingList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_BuildingID", Value=0)]
      BuildingList_BuildingID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_BuildingGUID", Value=1)]
      BuildingList_BuildingGUID = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_State", Value=2)]
      BuildingList_State = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_PosX", Value=3)]
      BuildingList_PosX = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_PosY", Value=4)]
      BuildingList_PosY = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_PosZ", Value=5)]
      BuildingList_PosZ = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_StateStartTime", Value=6)]
      BuildingList_StateStartTime = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingList_StateEndTime", Value=7)]
      BuildingList_StateEndTime = 7
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"BuildingProduce_RecordColType")]
    public enum BuildingProduce_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingProduce_BuildingGUID", Value=0)]
      BuildingProduce_BuildingGUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingProduce_ItemID", Value=1)]
      BuildingProduce_ItemID = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingProduce_LeftCount", Value=2)]
      BuildingProduce_LeftCount = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingProduce_OnceTime", Value=3)]
      BuildingProduce_OnceTime = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BuildingProduce_OnceStartTime", Value=4)]
      BuildingProduce_OnceStartTime = 4
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GroupMemberList_RecordColType")]
    public enum GroupMemberList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GroupMemberList_GUID", Value=0)]
      GroupMemberList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GroupMemberList_Online", Value=1)]
      GroupMemberList_Online = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GroupMemberList_GameID", Value=2)]
      GroupMemberList_GameID = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"ChatList_RecordColType")]
    public enum ChatList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"ChatList_GUID", Value=0)]
      ChatList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"ChatList_msg", Value=1)]
      ChatList_msg = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"ChatList_time", Value=2)]
      ChatList_time = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildBoss_RecordColType")]
    public enum GuildBoss_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_GUID", Value=0)]
      GuildBoss_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Name", Value=1)]
      GuildBoss_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Level", Value=2)]
      GuildBoss_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Job", Value=3)]
      GuildBoss_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Donation", Value=4)]
      GuildBoss_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_VIP", Value=5)]
      GuildBoss_VIP = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Offline", Value=6)]
      GuildBoss_Offline = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildBoss_Power", Value=7)]
      GuildBoss_Power = 7
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildMemberList_RecordColType")]
    public enum GuildMemberList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_GUID", Value=0)]
      GuildMemberList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Name", Value=1)]
      GuildMemberList_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Level", Value=2)]
      GuildMemberList_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Job", Value=3)]
      GuildMemberList_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Donation", Value=4)]
      GuildMemberList_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Receive", Value=5)]
      GuildMemberList_Receive = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_VIP", Value=6)]
      GuildMemberList_VIP = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Online", Value=7)]
      GuildMemberList_Online = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Power", Value=8)]
      GuildMemberList_Power = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Title", Value=9)]
      GuildMemberList_Title = 9,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_GameID", Value=10)]
      GuildMemberList_GameID = 10,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_JoinTime", Value=11)]
      GuildMemberList_JoinTime = 11,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_Contribution", Value=12)]
      GuildMemberList_Contribution = 12,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildMemberList_AllContribution", Value=13)]
      GuildMemberList_AllContribution = 13
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildAppyList_RecordColType")]
    public enum GuildAppyList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_GUID", Value=0)]
      GuildAppyList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_Name", Value=1)]
      GuildAppyList_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_Level", Value=2)]
      GuildAppyList_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_Job", Value=3)]
      GuildAppyList_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_Donation", Value=4)]
      GuildAppyList_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_VIP", Value=5)]
      GuildAppyList_VIP = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildAppyList_Power", Value=6)]
      GuildAppyList_Power = 6
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildEvent_RecordColType")]
    public enum GuildEvent_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_GUID", Value=0)]
      GuildEvent_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Name", Value=1)]
      GuildEvent_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Level", Value=2)]
      GuildEvent_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Job", Value=3)]
      GuildEvent_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Donation", Value=4)]
      GuildEvent_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_VIP", Value=5)]
      GuildEvent_VIP = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Offline", Value=6)]
      GuildEvent_Offline = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Power", Value=7)]
      GuildEvent_Power = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_EventID", Value=8)]
      GuildEvent_EventID = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_EventTime", Value=9)]
      GuildEvent_EventTime = 9,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildEvent_Context", Value=10)]
      GuildEvent_Context = 10
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildHouse_RecordColType")]
    public enum GuildHouse_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_GUID", Value=0)]
      GuildHouse_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Name", Value=1)]
      GuildHouse_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Level", Value=2)]
      GuildHouse_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Job", Value=3)]
      GuildHouse_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Donation", Value=4)]
      GuildHouse_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_VIP", Value=5)]
      GuildHouse_VIP = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Offline", Value=6)]
      GuildHouse_Offline = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildHouse_Power", Value=7)]
      GuildHouse_Power = 7
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"GuildSkill_RecordColType")]
    public enum GuildSkill_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_GUID", Value=0)]
      GuildSkill_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Name", Value=1)]
      GuildSkill_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Level", Value=2)]
      GuildSkill_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Job", Value=3)]
      GuildSkill_Job = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Donation", Value=4)]
      GuildSkill_Donation = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_VIP", Value=5)]
      GuildSkill_VIP = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Offline", Value=6)]
      GuildSkill_Offline = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuildSkill_Power", Value=7)]
      GuildSkill_Power = 7
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"PosList_RecordColType")]
    public enum PosList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"PosList_X", Value=0)]
      PosList_X = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PosList_Y", Value=1)]
      PosList_Y = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PosList_Z", Value=2)]
      PosList_Z = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PosList_StayTime", Value=3)]
      PosList_StayTime = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"PlayerViewItem_RecordColType")]
    public enum PlayerViewItem_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"PlayerViewItem_ConfigID", Value=0)]
      PlayerViewItem_ConfigID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PlayerViewItem_EnhanceLevel", Value=1)]
      PlayerViewItem_EnhanceLevel = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PlayerViewItem_InlayStone", Value=2)]
      PlayerViewItem_InlayStone = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PlayerViewItem_SagecraftLevel", Value=3)]
      PlayerViewItem_SagecraftLevel = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"Record_PlayerHero_RecordColType")]
    public enum Record_PlayerHero_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"Record_PlayerHero_GUID", Value=0)]
      Record_PlayerHero_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Record_PlayerHero_ConfigID", Value=1)]
      Record_PlayerHero_ConfigID = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Record_PlayerHero_Level", Value=2)]
      Record_PlayerHero_Level = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Record_PlayerHero_Exp", Value=3)]
      Record_PlayerHero_Exp = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"HeroEquipList_RecordColType")]
    public enum HeroEquipList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"HeroEquipList_GUID", Value=0)]
      HeroEquipList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"HeroEquipList_HeroID", Value=1)]
      HeroEquipList_HeroID = 1
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"BagEquipList_RecordColType")]
    public enum BagEquipList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_GUID", Value=0)]
      BagEquipList_GUID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_WearGUID", Value=1)]
      BagEquipList_WearGUID = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ConfigID", Value=2)]
      BagEquipList_ConfigID = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_Bound", Value=3)]
      BagEquipList_Bound = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ExpiredType", Value=4)]
      BagEquipList_ExpiredType = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_RandPropertyID", Value=5)]
      BagEquipList_RandPropertyID = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_Date", Value=6)]
      BagEquipList_Date = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_SlotCount", Value=7)]
      BagEquipList_SlotCount = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone1", Value=8)]
      BagEquipList_InlayStone1 = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone2", Value=9)]
      BagEquipList_InlayStone2 = 9,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone3", Value=10)]
      BagEquipList_InlayStone3 = 10,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone4", Value=11)]
      BagEquipList_InlayStone4 = 11,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone5", Value=12)]
      BagEquipList_InlayStone5 = 12,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone6", Value=13)]
      BagEquipList_InlayStone6 = 13,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone7", Value=14)]
      BagEquipList_InlayStone7 = 14,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone8", Value=15)]
      BagEquipList_InlayStone8 = 15,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone9", Value=16)]
      BagEquipList_InlayStone9 = 16,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_InlayStone10", Value=17)]
      BagEquipList_InlayStone10 = 17,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_IntensifyLevel", Value=18)]
      BagEquipList_IntensifyLevel = 18,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel1", Value=19)]
      BagEquipList_ElementLevel1 = 19,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel2", Value=20)]
      BagEquipList_ElementLevel2 = 20,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel3", Value=21)]
      BagEquipList_ElementLevel3 = 21,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel4", Value=22)]
      BagEquipList_ElementLevel4 = 22,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel5", Value=23)]
      BagEquipList_ElementLevel5 = 23,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel6", Value=24)]
      BagEquipList_ElementLevel6 = 24,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel7", Value=25)]
      BagEquipList_ElementLevel7 = 25,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel8", Value=26)]
      BagEquipList_ElementLevel8 = 26,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel9", Value=27)]
      BagEquipList_ElementLevel9 = 27,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagEquipList_ElementLevel10", Value=28)]
      BagEquipList_ElementLevel10 = 28
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"BagItemList_RecordColType")]
    public enum BagItemList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagItemList_ConfigID", Value=0)]
      BagItemList_ConfigID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagItemList_ItemCount", Value=1)]
      BagItemList_ItemCount = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagItemList_Bound", Value=2)]
      BagItemList_Bound = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagItemList_ExpiredType", Value=3)]
      BagItemList_ExpiredType = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"BagItemList_Date", Value=4)]
      BagItemList_Date = 4
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"CommPropertyValue_RecordColType")]
    public enum CommPropertyValue_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MAXHP", Value=0)]
      CommPropertyValue_MAXHP = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MAXMP", Value=1)]
      CommPropertyValue_MAXMP = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MAXSP", Value=2)]
      CommPropertyValue_MAXSP = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_HPREGEN", Value=3)]
      CommPropertyValue_HPREGEN = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_SPREGEN", Value=4)]
      CommPropertyValue_SPREGEN = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MPREGEN", Value=5)]
      CommPropertyValue_MPREGEN = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_VALUE", Value=6)]
      CommPropertyValue_ATK_VALUE = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DEF_VALUE", Value=7)]
      CommPropertyValue_DEF_VALUE = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MOVE_SPEED", Value=8)]
      CommPropertyValue_MOVE_SPEED = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_SPEED", Value=9)]
      CommPropertyValue_ATK_SPEED = 9,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_FIRE", Value=10)]
      CommPropertyValue_ATK_FIRE = 10,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_LIGHT", Value=11)]
      CommPropertyValue_ATK_LIGHT = 11,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_ICE", Value=12)]
      CommPropertyValue_ATK_ICE = 12,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_ATK_POISON", Value=13)]
      CommPropertyValue_ATK_POISON = 13,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DEF_FIRE", Value=14)]
      CommPropertyValue_DEF_FIRE = 14,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DEF_LIGHT", Value=15)]
      CommPropertyValue_DEF_LIGHT = 15,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DEF_ICE", Value=16)]
      CommPropertyValue_DEF_ICE = 16,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DEF_POISON", Value=17)]
      CommPropertyValue_DEF_POISON = 17,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_DIZZY_GATE", Value=18)]
      CommPropertyValue_DIZZY_GATE = 18,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MOVE_GATE", Value=19)]
      CommPropertyValue_MOVE_GATE = 19,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_SKILL_GATE", Value=20)]
      CommPropertyValue_SKILL_GATE = 20,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_PHYSICAL_GATE", Value=21)]
      CommPropertyValue_PHYSICAL_GATE = 21,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_MAGIC_GATE", Value=22)]
      CommPropertyValue_MAGIC_GATE = 22,
            
      [global::ProtoBuf.ProtoEnum(Name=@"CommPropertyValue_BUFF_GATE", Value=23)]
      CommPropertyValue_BUFF_GATE = 23
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"EctypeList_RecordColType")]
    public enum EctypeList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EctypeList_EctypeID", Value=0)]
      EctypeList_EctypeID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EctypeList_IsPass", Value=1)]
      EctypeList_IsPass = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EctypeList_Star", Value=2)]
      EctypeList_Star = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"DropItemList_RecordColType")]
    public enum DropItemList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"DropItemList_MonsterID", Value=0)]
      DropItemList_MonsterID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"DropItemList_ItemID", Value=1)]
      DropItemList_ItemID = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"DropItemList_ItemCount", Value=2)]
      DropItemList_ItemCount = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"DropItemList_DrawState", Value=3)]
      DropItemList_DrawState = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"SkillTable_RecordColType")]
    public enum SkillTable_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"SkillTable_SkillID", Value=0)]
      SkillTable_SkillID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"SkillTable_SkillLevel", Value=1)]
      SkillTable_SkillLevel = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"SkillTable_SkillStone", Value=2)]
      SkillTable_SkillStone = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"SkillTable_SkillSoulLevel", Value=3)]
      SkillTable_SkillSoulLevel = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"TaskMonsterList_RecordColType")]
    public enum TaskMonsterList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskMonsterList_MonsterID", Value=0)]
      TaskMonsterList_MonsterID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskMonsterList_CurrentKillCount", Value=1)]
      TaskMonsterList_CurrentKillCount = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskMonsterList_RequireKillCount", Value=2)]
      TaskMonsterList_RequireKillCount = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskMonsterList_TaskID", Value=3)]
      TaskMonsterList_TaskID = 3
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"TaskList_RecordColType")]
    public enum TaskList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskList_TaskID", Value=0)]
      TaskList_TaskID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskList_TaskStatus", Value=1)]
      TaskList_TaskStatus = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"TaskList_Process", Value=2)]
      TaskList_Process = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"PVPList_RecordColType")]
    public enum PVPList_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"PVPList_ID", Value=0)]
      PVPList_ID = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PVPList_Name", Value=1)]
      PVPList_Name = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PVPList_Level", Value=2)]
      PVPList_Level = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"ChatGroup_RecordColType")]
    public enum ChatGroup_RecordColType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"ChatGroup_ChatType", Value=0)]
      ChatGroup_ChatType = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"ChatGroup_GroupGUID", Value=1)]
      ChatGroup_GroupGUID = 1
    }
  
}