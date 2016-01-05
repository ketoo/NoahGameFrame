// -------------------------------------------------------------------------
//    @FileName         :    NFPRNameDefine.hpp
//    @Author           :    NFrame Studio
//    @Date             :    2016/01/05
//    @Module           :    NFPRNameDefine
// -------------------------------------------------------------------------

#ifndef NF_PR_NAME_HPP
#define NF_PR_NAME_HPP


namespace P_BB_Build
{
	std::string Type = "Type";
	std::string SubType = "SubType";
	std::string Prefab = "Prefab";
	std::string NormalStateFunc = "NormalStateFunc";
	std::string UpStateFunc = "UpStateFunc";
	std::string Icon = "Icon";
	std::string ShowName = "ShowName";
	std::string Desc = "Desc";

};

namespace R_BB_Build
{

};

namespace P_BB_Player
{
	std::string RoleName = "RoleName";
	std::string Sex = "Sex";
	std::string Race = "Race";
	std::string Camp = "Camp";
	std::string LastContainerID = "LastContainerID";
	std::string Level = "Level";
	std::string ShowName = "ShowName";
	std::string Gold = "Gold";
	std::string Stone = "Stone";
	std::string Steel = "Steel";
	std::string Diamond = "Diamond";
	std::string LoadPropertyFinish = "LoadPropertyFinish";

};

namespace R_BB_Player
{
	std::string BuildingList = "BuildingList";
	std::string BuildingProduce = "BuildingProduce";

};

namespace P_Buff
{
	std::string EffectType = "EffectType";
	std::string EffectValueType = "EffectValueType";
	std::string EffectValueReferType = "EffectValueReferType";
	std::string EffectTimeValue = "EffectTimeValue";
	std::string EffectTimeInterval = "EffectTimeInterval";
	std::string WashGroupID = "WashGroupID";
	std::string ReverseReferType = "ReverseReferType";
	std::string EffectClearOnDead = "EffectClearOnDead";
	std::string DownSaveType = "DownSaveType";

};

namespace R_Buff
{

};

namespace P_ChatGroup
{
	std::string Name = "Name";
	std::string CreateObject = "CreateObject";

};

namespace R_ChatGroup
{
	std::string GroupMemberList = "GroupMemberList";
	std::string ChatList = "ChatList";

};

namespace P_ConsumeData
{
	std::string VIPEXP = "VIPEXP";
	std::string EXP = "EXP";
	std::string HP = "HP";
	std::string SP = "SP";
	std::string MP = "MP";
	std::string Gold = "Gold";
	std::string Money = "Money";

};

namespace R_ConsumeData
{

};

namespace P_Cost
{
	std::string CostMoney = "CostMoney";
	std::string CostDiamond = "CostDiamond";
	std::string CostVP = "CostVP";
	std::string CostHonour = "CostHonour";

};

namespace R_Cost
{

};

namespace P_DescData
{
	std::string ShowName = "ShowName";
	std::string Icon = "Icon";
	std::string Atlas = "Atlas";
	std::string PrefabPath = "PrefabPath";
	std::string PerformanceEffect = "PerformanceEffect";
	std::string PerformanceSound = "PerformanceSound";
	std::string DescText = "DescText";

};

namespace R_DescData
{

};

namespace P_EffectData
{
	std::string MAXHP = "MAXHP";
	std::string MAXMP = "MAXMP";
	std::string MAXSP = "MAXSP";
	std::string HPREGEN = "HPREGEN";
	std::string SPREGEN = "SPREGEN";
	std::string MPREGEN = "MPREGEN";
	std::string ATK_VALUE = "ATK_VALUE";
	std::string DEF_VALUE = "DEF_VALUE";
	std::string MOVE_SPEED = "MOVE_SPEED";
	std::string ATK_SPEED = "ATK_SPEED";
	std::string ATK_FIRE = "ATK_FIRE";
	std::string ATK_LIGHT = "ATK_LIGHT";
	std::string ATK_ICE = "ATK_ICE";
	std::string ATK_POISON = "ATK_POISON";
	std::string DEF_FIRE = "DEF_FIRE";
	std::string DEF_LIGHT = "DEF_LIGHT";
	std::string DEF_ICE = "DEF_ICE";
	std::string DEF_POISON = "DEF_POISON";
	std::string DIZZY_GATE = "DIZZY_GATE";
	std::string MOVE_GATE = "MOVE_GATE";
	std::string SKILL_GATE = "SKILL_GATE";
	std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	std::string MAGIC_GATE = "MAGIC_GATE";
	std::string BUFF_GATE = "BUFF_GATE";

};

namespace R_EffectData
{

};

namespace P_Equip
{
	std::string Sex = "Sex";
	std::string IntensiveBuffList = "IntensiveBuffList";
	std::string EnchantmentBuffList = "EnchantmentBuffList";
	std::string SuitID = "SuitID";
	std::string SuitBuffID = "SuitBuffID";

};

namespace R_Equip
{

};

namespace P_Guild
{
	std::string Name = "Name";
	std::string PresidentID = "PresidentID";
	std::string PresidentName = "PresidentName";
	std::string Rank = "Rank";
	std::string GuildAD = "GuildAD";
	std::string GuildDesc = "GuildDesc";
	std::string GuildMoney = "GuildMoney";
	std::string GuildLevel = "GuildLevel";
	std::string GuildContinueDay = "GuildContinueDay";
	std::string GuilID = "GuilID";
	std::string GuilIDIcon = "GuilIDIcon";
	std::string GuildMemeberCount = "GuildMemeberCount";
	std::string GuildMemeberMaxCount = "GuildMemeberMaxCount";
	std::string GuildHonor = "GuildHonor";
	std::string GuildCreateTime = "GuildCreateTime";
	std::string GuildCreateter = "GuildCreateter";
	std::string GuildExp = "GuildExp";
	std::string GuildStatus = "GuildStatus";
	std::string DismissTime = "DismissTime";
	std::string RecruitAD = "RecruitAD";
	std::string RecruitLevel = "RecruitLevel";
	std::string AutoRecruit = "AutoRecruit";

};

namespace R_Guild
{
	std::string GuildBoss = "GuildBoss";
	std::string GuildMemberList = "GuildMemberList";
	std::string GuildAppyList = "GuildAppyList";
	std::string GuildEvent = "GuildEvent";
	std::string GuildHouse = "GuildHouse";
	std::string GuildSkill = "GuildSkill";

};

namespace P_GuildConfig
{
	std::string ConditionPlayerLevel = "ConditionPlayerLevel";
	std::string ConditionPlayerVIP = "ConditionPlayerVIP";
	std::string CostMoney = "CostMoney";
	std::string DismissTime = "DismissTime";
	std::string GuildLevel = "GuildLevel";
	std::string MaxMember = "MaxMember";

};

namespace R_GuildConfig
{

};

namespace P_GuildJob
{
	std::string GuildJob = "GuildJob";
	std::string JobCount = "JobCount";
	std::string Appoint = "Appoint";
	std::string Fire = "Fire";
	std::string Demise = "Demise";
	std::string ApplyDismiss = "ApplyDismiss";
	std::string StopDismiss = "StopDismiss";
	std::string AcceptApply = "AcceptApply";
	std::string DenyApply = "DenyApply";
	std::string Kickout = "Kickout";
	std::string SetRecruit = "SetRecruit";
	std::string PublishRecruit = "PublishRecruit";
	std::string EditAD = "EditAD";
	std::string Leave = "Leave";
	std::string LevelUp = "LevelUp";

};

namespace R_GuildJob
{

};

namespace P_GuildName
{
	std::string GuildID = "GuildID";

};

namespace R_GuildName
{

};

namespace P_InitProperty
{
	std::string Job = "Job";
	std::string Level = "Level";
	std::string EffectData = "EffectData";
	std::string SkillIDRef = "SkillIDRef";
	std::string ModelPtah = "ModelPtah";

};

namespace R_InitProperty
{

};

namespace P_IObject
{
	std::string ID = "ID";
	std::string ClassName = "ClassName";
	std::string SceneID = "SceneID";
	std::string GroupID = "GroupID";
	std::string ConfigID = "ConfigID";

};

namespace R_IObject
{

};

namespace P_Item
{
	std::string ItemType = "ItemType";
	std::string ItemSubType = "ItemSubType";
	std::string Level = "Level";
	std::string Job = "Job";
	std::string Quality = "Quality";
	std::string DescID = "DescID";
	std::string EffectData = "EffectData";
	std::string ConsumeData = "ConsumeData";
	std::string AwardData = "AwardData";
	std::string CoolDownTime = "CoolDownTime";
	std::string OverlayCount = "OverlayCount";
	std::string ExpiredType = "ExpiredType";
	std::string BuyPrice = "BuyPrice";
	std::string SalePrice = "SalePrice";
	std::string Script = "Script";
	std::string Extend = "Extend";

};

namespace R_Item
{

};

namespace P_Language
{
	std::string English = "English";
	std::string Chinese = "Chinese";

};

namespace R_Language
{

};

namespace P_NPC
{
	std::string SeedID = "SeedID";
	std::string EXP = "EXP";
	std::string HP = "HP";
	std::string SP = "SP";
	std::string MP = "MP";
	std::string Money = "Money";
	std::string LastAttacker = "LastAttacker";
	std::string X = "X";
	std::string Y = "Y";
	std::string Z = "Z";
	std::string TargetX = "TargetX";
	std::string TargetY = "TargetY";
	std::string TargetZ = "TargetZ";
	std::string MAXHP = "MAXHP";
	std::string MAXMP = "MAXMP";
	std::string MAXSP = "MAXSP";
	std::string HPREGEN = "HPREGEN";
	std::string SPREGEN = "SPREGEN";
	std::string MPREGEN = "MPREGEN";
	std::string ATK_VALUE = "ATK_VALUE";
	std::string DEF_VALUE = "DEF_VALUE";
	std::string MOVE_SPEED = "MOVE_SPEED";
	std::string ATK_SPEED = "ATK_SPEED";
	std::string ATK_FIRE = "ATK_FIRE";
	std::string ATK_LIGHT = "ATK_LIGHT";
	std::string ATK_ICE = "ATK_ICE";
	std::string ATK_POISON = "ATK_POISON";
	std::string DEF_FIRE = "DEF_FIRE";
	std::string DEF_LIGHT = "DEF_LIGHT";
	std::string DEF_ICE = "DEF_ICE";
	std::string DEF_POISON = "DEF_POISON";
	std::string DIZZY_GATE = "DIZZY_GATE";
	std::string MOVE_GATE = "MOVE_GATE";
	std::string SKILL_GATE = "SKILL_GATE";
	std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	std::string MAGIC_GATE = "MAGIC_GATE";
	std::string BUFF_GATE = "BUFF_GATE";
	std::string Prefab = "Prefab";
	std::string MoveType = "MoveType";
	std::string AtkDis = "AtkDis";
	std::string DropPackList = "DropPackList";
	std::string SkillIDRef = "SkillIDRef";
	std::string Height = "Height";

};

namespace R_NPC
{
	std::string PosList = "PosList";

};

namespace P_Player
{
	std::string Name = "Name";
	std::string Sex = "Sex";
	std::string Race = "Race";
	std::string Camp = "Camp";
	std::string LastContainerID = "LastContainerID";
	std::string Level = "Level";
	std::string ShowName = "ShowName";
	std::string PrefabPath = "PrefabPath";
	std::string FirstTarget = "FirstTarget";
	std::string State = "State";
	std::string LogicState = "LogicState";
	std::string CharType = "CharType";
	std::string Job = "Job";
	std::string VIPLevel = "VIPLevel";
	std::string VIPEXP = "VIPEXP";
	std::string EXP = "EXP";
	std::string HP = "HP";
	std::string SP = "SP";
	std::string MP = "MP";
	std::string Gold = "Gold";
	std::string Money = "Money";
	std::string Account = "Account";
	std::string ConnectKey = "ConnectKey";
	std::string MAXEXP = "MAXEXP";
	std::string DEAD_COUNT = "DEAD_COUNT";
	std::string RELIVE_SOUL = "RELIVE_SOUL";
	std::string GUILD_NAME = "GUILD_NAME";
	std::string SPOUSE_NAME = "SPOUSE_NAME";
	std::string TEAM_CARD = "TEAM_CARD";
	std::string PVP_ATK_CARD = "PVP_ATK_CARD";
	std::string PVP_DEF_CARD = "PVP_DEF_CARD";
	std::string LUCKLY_CARD = "LUCKLY_CARD";
	std::string ATK_PVP = "ATK_PVP";
	std::string DEF_PVP = "DEF_PVP";
	std::string CreditValue = "CreditValue";
	std::string SoulValue = "SoulValue";
	std::string OnlineCount = "OnlineCount";
	std::string TotalTime = "TotalTime";
	std::string Citizenship = "Citizenship";
	std::string LastOfflineTime = "LastOfflineTime";
	std::string OnlineTime = "OnlineTime";
	std::string TotalLineTime = "TotalLineTime";
	std::string GMLevel = "GMLevel";
	std::string LastAttacker = "LastAttacker";
	std::string MAXHP = "MAXHP";
	std::string MAXMP = "MAXMP";
	std::string MAXSP = "MAXSP";
	std::string HPREGEN = "HPREGEN";
	std::string SPREGEN = "SPREGEN";
	std::string MPREGEN = "MPREGEN";
	std::string ATK_VALUE = "ATK_VALUE";
	std::string DEF_VALUE = "DEF_VALUE";
	std::string MOVE_SPEED = "MOVE_SPEED";
	std::string ATK_SPEED = "ATK_SPEED";
	std::string ATK_FIRE = "ATK_FIRE";
	std::string ATK_LIGHT = "ATK_LIGHT";
	std::string ATK_ICE = "ATK_ICE";
	std::string ATK_POISON = "ATK_POISON";
	std::string DEF_FIRE = "DEF_FIRE";
	std::string DEF_LIGHT = "DEF_LIGHT";
	std::string DEF_ICE = "DEF_ICE";
	std::string DEF_POISON = "DEF_POISON";
	std::string DIZZY_GATE = "DIZZY_GATE";
	std::string MOVE_GATE = "MOVE_GATE";
	std::string SKILL_GATE = "SKILL_GATE";
	std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	std::string MAGIC_GATE = "MAGIC_GATE";
	std::string BUFF_GATE = "BUFF_GATE";
	std::string X = "X";
	std::string Y = "Y";
	std::string Z = "Z";
	std::string TargetX = "TargetX";
	std::string TargetY = "TargetY";
	std::string TargetZ = "TargetZ";
	std::string PathStep = "PathStep";
	std::string LoadPropertyFinish = "LoadPropertyFinish";
	std::string GameID = "GameID";
	std::string GateID = "GateID";
	std::string GuildID = "GuildID";

};

namespace R_Player
{
	std::string PlayerViewItem = "PlayerViewItem";
	std::string PlayerHero = "PlayerHero";
	std::string BagEquipList = "BagEquipList";
	std::string BagItemList = "BagItemList";
	std::string CommPropertyValue = "CommPropertyValue";
	std::string EctypeList = "EctypeList";
	std::string DropItemList = "DropItemList";
	std::string SkillTable = "SkillTable";
	std::string TaskMonsterList = "TaskMonsterList";
	std::string TaskList = "TaskList";
	std::string PVPList = "PVPList";
	std::string ChatGroup = "ChatGroup";

};

namespace P_Scene
{
	std::string SceneName = "SceneName";
	std::string SceneShowName = "SceneShowName";
	std::string MaxGroup = "MaxGroup";
	std::string MaxGroupPlayers = "MaxGroupPlayers";
	std::string FilePath = "FilePath";
	std::string RelivePos = "RelivePos";
	std::string Width = "Width";
	std::string SoundList = "SoundList";
	std::string Share = "Share";
	std::string CanClone = "CanClone";
	std::string ActorID = "ActorID";
	std::string LoadingUI = "LoadingUI";
	std::string CamOffestPos = "CamOffestPos";
	std::string CamOffestRot = "CamOffestRot";

};

namespace R_Scene
{

};

namespace P_Server
{
	std::string ServerID = "ServerID";
	std::string Name = "Name";
	std::string MaxOnline = "MaxOnline";
	std::string CpuCount = "CpuCount";
	std::string IP = "IP";
	std::string Port = "Port";
	std::string Type = "Type";

};

namespace R_Server
{

};

namespace P_Shop
{
	std::string Type = "Type";
	std::string ItemID = "ItemID";
	std::string Gold = "Gold";
	std::string Steel = "Steel";
	std::string Stone = "Stone";
	std::string Diamond = "Diamond";
	std::string Level = "Level";

};

namespace R_Shop
{

};

namespace P_Skill
{
	std::string SkillType = "SkillType";
	std::string AnimaState = "AnimaState";
	std::string ShowName = "ShowName";
	std::string Desc = "Desc";
	std::string ConsumeProperty = "ConsumeProperty";
	std::string ConsumeValue = "ConsumeValue";
	std::string ConsumeType = "ConsumeType";
	std::string DamageProperty = "DamageProperty";
	std::string DamageValue = "DamageValue";
	std::string DamageType = "DamageType";
	std::string GetBuffList = "GetBuffList";
	std::string SendBuffList = "SendBuffList";
	std::string CoolDownTime = "CoolDownTime";
	std::string RequireDistance = "RequireDistance";
	std::string DamageDistance = "DamageDistance";
	std::string TargetType = "TargetType";
	std::string NewObject = "NewObject";
	std::string Icon = "Icon";
	std::string Atlas = "Atlas";
	std::string UpLevel = "UpLevel";
	std::string AfterUpSkillID = "AfterUpSkillID";
	std::string PlayerSkill = "PlayerSkill";
	std::string AtkDis = "AtkDis";
	std::string NeedTar = "NeedTar";
	std::string DefaultHitTime = "DefaultHitTime";

};

namespace R_Skill
{

};

namespace P_SkillRef
{
	std::string NORMALATTACK1 = "NORMALATTACK1";
	std::string NORMALATTACK2 = "NORMALATTACK2";
	std::string NORMALATTACK3 = "NORMALATTACK3";
	std::string NORMALTHUMP = "NORMALTHUMP";
	std::string SKILL1 = "SKILL1";
	std::string SKILL2 = "SKILL2";
	std::string SKILL3 = "SKILL3";
	std::string SKILL4 = "SKILL4";
	std::string SKILL5 = "SKILL5";
	std::string SKILL6 = "SKILL6";
	std::string SKILL7 = "SKILL7";
	std::string SKILL8 = "SKILL8";
	std::string SKILL9 = "SKILL9";
	std::string SKILL10 = "SKILL10";

};

namespace R_SkillRef
{

};

namespace P_SqlServer
{
	std::string ServerID = "ServerID";
	std::string IP = "IP";
	std::string Port = "Port";
	std::string SqlIP = "SqlIP";
	std::string SqlPort = "SqlPort";
	std::string SqlUser = "SqlUser";
	std::string SqlPwd = "SqlPwd";
	std::string SqlName = "SqlName";

};

namespace R_SqlServer
{

};

namespace P_StateFuncResources
{
	std::string Atlas_ResID = "Atlas_ResID";

};

namespace R_StateFuncResources
{

};

namespace P_StateFunction
{
	std::string EFT_INFO = "EFT_INFO";
	std::string EFT_BOOOST = "EFT_BOOOST";
	std::string EFT_LVLUP = "EFT_LVLUP";
	std::string EFT_CREATE_SOLDER = "EFT_CREATE_SOLDER";
	std::string EFT_CREATE_SPEEL = "EFT_CREATE_SPEEL";
	std::string EFT_RESEARCH = "EFT_RESEARCH";
	std::string EFT_COLLECT_GOLD = "EFT_COLLECT_GOLD";
	std::string EFT_COLLECT_STONE = "EFT_COLLECT_STONE";
	std::string EFT_COLLECT_STEEL = "EFT_COLLECT_STEEL";
	std::string EFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND";
	std::string EFT_SELL = "EFT_SELL";
	std::string EFT_REPAIR = "EFT_REPAIR";
	std::string EFT_CANCEL = "EFT_CANCEL";
	std::string EFT_FINISH = "EFT_FINISH";

};

namespace R_StateFunction
{

};

namespace P_Task
{
	std::string Type = "Type";
	std::string NextTaskID = "NextTaskID";
	std::string KillMonsterName = "KillMonsterName";
	std::string KillCount = "KillCount";
	std::string LevelReq = "LevelReq";
	std::string AwardExp = "AwardExp";
	std::string AwardGold = "AwardGold";
	std::string AwardPack = "AwardPack";
	std::string Desc = "Desc";

};

namespace R_Task
{

};


#endif NF_PR_NAME_HPP
