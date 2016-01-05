// -------------------------------------------------------------------------
//    @FileName         :    NFPRNameDefine.hpp
//    @Author           :    NFrame Studio
//    @Date             :    2016/01/05
//    @Module           :    NFPRNameDefine
// -------------------------------------------------------------------------

#ifndef NF_PR_NAME_HPP
#define NF_PR_NAME_HPP

namespace NFrame
{

class P_BB_Build
{
public:
	const static std::string Type = "Type";
	const static std::string SubType = "SubType";
	const static std::string Prefab = "Prefab";
	const static std::string NormalStateFunc = "NormalStateFunc";
	const static std::string UpStateFunc = "UpStateFunc";
	const static std::string Icon = "Icon";
	const static std::string ShowName = "ShowName";
	const static std::string Desc = "Desc";

};

class R_BB_Build
{
public:

};

class P_BB_Player
{
public:
	const static std::string RoleName = "RoleName";
	const static std::string Sex = "Sex";
	const static std::string Race = "Race";
	const static std::string Camp = "Camp";
	const static std::string LastContainerID = "LastContainerID";
	const static std::string Level = "Level";
	const static std::string ShowName = "ShowName";
	const static std::string Gold = "Gold";
	const static std::string Stone = "Stone";
	const static std::string Steel = "Steel";
	const static std::string Diamond = "Diamond";
	const static std::string LoadPropertyFinish = "LoadPropertyFinish";

};

class R_BB_Player
{
public:
	const static std::string BuildingList = "BuildingList";
	const static std::string BuildingProduce = "BuildingProduce";

};

class P_Buff
{
public:
	const static std::string EffectType = "EffectType";
	const static std::string EffectValueType = "EffectValueType";
	const static std::string EffectValueReferType = "EffectValueReferType";
	const static std::string EffectTimeValue = "EffectTimeValue";
	const static std::string EffectTimeInterval = "EffectTimeInterval";
	const static std::string WashGroupID = "WashGroupID";
	const static std::string ReverseReferType = "ReverseReferType";
	const static std::string EffectClearOnDead = "EffectClearOnDead";
	const static std::string DownSaveType = "DownSaveType";

};

class R_Buff
{
public:

};

class P_ChatGroup
{
public:
	const static std::string Name = "Name";
	const static std::string CreateObject = "CreateObject";

};

class R_ChatGroup
{
public:
	const static std::string GroupMemberList = "GroupMemberList";
	const static std::string ChatList = "ChatList";

};

class P_ConsumeData
{
public:
	const static std::string VIPEXP = "VIPEXP";
	const static std::string EXP = "EXP";
	const static std::string HP = "HP";
	const static std::string SP = "SP";
	const static std::string MP = "MP";
	const static std::string Gold = "Gold";
	const static std::string Money = "Money";

};

class R_ConsumeData
{
public:

};

class P_Cost
{
public:
	const static std::string CostMoney = "CostMoney";
	const static std::string CostDiamond = "CostDiamond";
	const static std::string CostVP = "CostVP";
	const static std::string CostHonour = "CostHonour";

};

class R_Cost
{
public:

};

class P_DescData
{
public:
	const static std::string ShowName = "ShowName";
	const static std::string Icon = "Icon";
	const static std::string Atlas = "Atlas";
	const static std::string PrefabPath = "PrefabPath";
	const static std::string PerformanceEffect = "PerformanceEffect";
	const static std::string PerformanceSound = "PerformanceSound";
	const static std::string DescText = "DescText";

};

class R_DescData
{
public:

};

class P_EffectData
{
public:
	const static std::string MAXHP = "MAXHP";
	const static std::string MAXMP = "MAXMP";
	const static std::string MAXSP = "MAXSP";
	const static std::string HPREGEN = "HPREGEN";
	const static std::string SPREGEN = "SPREGEN";
	const static std::string MPREGEN = "MPREGEN";
	const static std::string ATK_VALUE = "ATK_VALUE";
	const static std::string DEF_VALUE = "DEF_VALUE";
	const static std::string MOVE_SPEED = "MOVE_SPEED";
	const static std::string ATK_SPEED = "ATK_SPEED";
	const static std::string ATK_FIRE = "ATK_FIRE";
	const static std::string ATK_LIGHT = "ATK_LIGHT";
	const static std::string ATK_ICE = "ATK_ICE";
	const static std::string ATK_POISON = "ATK_POISON";
	const static std::string DEF_FIRE = "DEF_FIRE";
	const static std::string DEF_LIGHT = "DEF_LIGHT";
	const static std::string DEF_ICE = "DEF_ICE";
	const static std::string DEF_POISON = "DEF_POISON";
	const static std::string DIZZY_GATE = "DIZZY_GATE";
	const static std::string MOVE_GATE = "MOVE_GATE";
	const static std::string SKILL_GATE = "SKILL_GATE";
	const static std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	const static std::string MAGIC_GATE = "MAGIC_GATE";
	const static std::string BUFF_GATE = "BUFF_GATE";

};

class R_EffectData
{
public:

};

class P_Equip
{
public:
	const static std::string Sex = "Sex";
	const static std::string IntensiveBuffList = "IntensiveBuffList";
	const static std::string EnchantmentBuffList = "EnchantmentBuffList";
	const static std::string SuitID = "SuitID";
	const static std::string SuitBuffID = "SuitBuffID";

};

class R_Equip
{
public:

};

class P_Guild
{
public:
	const static std::string Name = "Name";
	const static std::string PresidentID = "PresidentID";
	const static std::string PresidentName = "PresidentName";
	const static std::string Rank = "Rank";
	const static std::string GuildAD = "GuildAD";
	const static std::string GuildDesc = "GuildDesc";
	const static std::string GuildMoney = "GuildMoney";
	const static std::string GuildLevel = "GuildLevel";
	const static std::string GuildContinueDay = "GuildContinueDay";
	const static std::string GuilID = "GuilID";
	const static std::string GuilIDIcon = "GuilIDIcon";
	const static std::string GuildMemeberCount = "GuildMemeberCount";
	const static std::string GuildMemeberMaxCount = "GuildMemeberMaxCount";
	const static std::string GuildHonor = "GuildHonor";
	const static std::string GuildCreateTime = "GuildCreateTime";
	const static std::string GuildCreateter = "GuildCreateter";
	const static std::string GuildExp = "GuildExp";
	const static std::string GuildStatus = "GuildStatus";
	const static std::string DismissTime = "DismissTime";
	const static std::string RecruitAD = "RecruitAD";
	const static std::string RecruitLevel = "RecruitLevel";
	const static std::string AutoRecruit = "AutoRecruit";

};

class R_Guild
{
public:
	const static std::string GuildBoss = "GuildBoss";
	const static std::string GuildMemberList = "GuildMemberList";
	const static std::string GuildAppyList = "GuildAppyList";
	const static std::string GuildEvent = "GuildEvent";
	const static std::string GuildHouse = "GuildHouse";
	const static std::string GuildSkill = "GuildSkill";

};

class P_GuildConfig
{
public:
	const static std::string ConditionPlayerLevel = "ConditionPlayerLevel";
	const static std::string ConditionPlayerVIP = "ConditionPlayerVIP";
	const static std::string CostMoney = "CostMoney";
	const static std::string DismissTime = "DismissTime";
	const static std::string GuildLevel = "GuildLevel";
	const static std::string MaxMember = "MaxMember";

};

class R_GuildConfig
{
public:

};

class P_GuildJob
{
public:
	const static std::string GuildJob = "GuildJob";
	const static std::string JobCount = "JobCount";
	const static std::string Appoint = "Appoint";
	const static std::string Fire = "Fire";
	const static std::string Demise = "Demise";
	const static std::string ApplyDismiss = "ApplyDismiss";
	const static std::string StopDismiss = "StopDismiss";
	const static std::string AcceptApply = "AcceptApply";
	const static std::string DenyApply = "DenyApply";
	const static std::string Kickout = "Kickout";
	const static std::string SetRecruit = "SetRecruit";
	const static std::string PublishRecruit = "PublishRecruit";
	const static std::string EditAD = "EditAD";
	const static std::string Leave = "Leave";
	const static std::string LevelUp = "LevelUp";

};

class R_GuildJob
{
public:

};

class P_GuildName
{
public:
	const static std::string GuildID = "GuildID";

};

class R_GuildName
{
public:

};

class P_InitProperty
{
public:
	const static std::string Job = "Job";
	const static std::string Level = "Level";
	const static std::string EffectData = "EffectData";
	const static std::string SkillIDRef = "SkillIDRef";
	const static std::string ModelPtah = "ModelPtah";

};

class R_InitProperty
{
public:

};

class P_IObject
{
public:
	const static std::string ID = "ID";
	const static std::string ClassName = "ClassName";
	const static std::string SceneID = "SceneID";
	const static std::string GroupID = "GroupID";
	const static std::string ConfigID = "ConfigID";

};

class R_IObject
{
public:

};

class P_Item
{
public:
	const static std::string ItemType = "ItemType";
	const static std::string ItemSubType = "ItemSubType";
	const static std::string Level = "Level";
	const static std::string Job = "Job";
	const static std::string Quality = "Quality";
	const static std::string DescID = "DescID";
	const static std::string EffectData = "EffectData";
	const static std::string ConsumeData = "ConsumeData";
	const static std::string AwardData = "AwardData";
	const static std::string CoolDownTime = "CoolDownTime";
	const static std::string OverlayCount = "OverlayCount";
	const static std::string ExpiredType = "ExpiredType";
	const static std::string BuyPrice = "BuyPrice";
	const static std::string SalePrice = "SalePrice";
	const static std::string Script = "Script";
	const static std::string Extend = "Extend";

};

class R_Item
{
public:

};

class P_Language
{
public:
	const static std::string English = "English";
	const static std::string Chinese = "Chinese";

};

class R_Language
{
public:

};

class P_NPC
{
public:
	const static std::string SeedID = "SeedID";
	const static std::string EXP = "EXP";
	const static std::string HP = "HP";
	const static std::string SP = "SP";
	const static std::string MP = "MP";
	const static std::string Money = "Money";
	const static std::string LastAttacker = "LastAttacker";
	const static std::string X = "X";
	const static std::string Y = "Y";
	const static std::string Z = "Z";
	const static std::string TargetX = "TargetX";
	const static std::string TargetY = "TargetY";
	const static std::string TargetZ = "TargetZ";
	const static std::string MAXHP = "MAXHP";
	const static std::string MAXMP = "MAXMP";
	const static std::string MAXSP = "MAXSP";
	const static std::string HPREGEN = "HPREGEN";
	const static std::string SPREGEN = "SPREGEN";
	const static std::string MPREGEN = "MPREGEN";
	const static std::string ATK_VALUE = "ATK_VALUE";
	const static std::string DEF_VALUE = "DEF_VALUE";
	const static std::string MOVE_SPEED = "MOVE_SPEED";
	const static std::string ATK_SPEED = "ATK_SPEED";
	const static std::string ATK_FIRE = "ATK_FIRE";
	const static std::string ATK_LIGHT = "ATK_LIGHT";
	const static std::string ATK_ICE = "ATK_ICE";
	const static std::string ATK_POISON = "ATK_POISON";
	const static std::string DEF_FIRE = "DEF_FIRE";
	const static std::string DEF_LIGHT = "DEF_LIGHT";
	const static std::string DEF_ICE = "DEF_ICE";
	const static std::string DEF_POISON = "DEF_POISON";
	const static std::string DIZZY_GATE = "DIZZY_GATE";
	const static std::string MOVE_GATE = "MOVE_GATE";
	const static std::string SKILL_GATE = "SKILL_GATE";
	const static std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	const static std::string MAGIC_GATE = "MAGIC_GATE";
	const static std::string BUFF_GATE = "BUFF_GATE";
	const static std::string Prefab = "Prefab";
	const static std::string MoveType = "MoveType";
	const static std::string AtkDis = "AtkDis";
	const static std::string DropPackList = "DropPackList";
	const static std::string SkillIDRef = "SkillIDRef";
	const static std::string Height = "Height";

};

class R_NPC
{
public:
	const static std::string PosList = "PosList";

};

class P_Player
{
public:
	const static std::string Name = "Name";
	const static std::string Sex = "Sex";
	const static std::string Race = "Race";
	const static std::string Camp = "Camp";
	const static std::string LastContainerID = "LastContainerID";
	const static std::string Level = "Level";
	const static std::string ShowName = "ShowName";
	const static std::string PrefabPath = "PrefabPath";
	const static std::string FirstTarget = "FirstTarget";
	const static std::string State = "State";
	const static std::string LogicState = "LogicState";
	const static std::string CharType = "CharType";
	const static std::string Job = "Job";
	const static std::string VIPLevel = "VIPLevel";
	const static std::string VIPEXP = "VIPEXP";
	const static std::string EXP = "EXP";
	const static std::string HP = "HP";
	const static std::string SP = "SP";
	const static std::string MP = "MP";
	const static std::string Gold = "Gold";
	const static std::string Money = "Money";
	const static std::string Account = "Account";
	const static std::string ConnectKey = "ConnectKey";
	const static std::string MAXEXP = "MAXEXP";
	const static std::string DEAD_COUNT = "DEAD_COUNT";
	const static std::string RELIVE_SOUL = "RELIVE_SOUL";
	const static std::string GUILD_NAME = "GUILD_NAME";
	const static std::string SPOUSE_NAME = "SPOUSE_NAME";
	const static std::string TEAM_CARD = "TEAM_CARD";
	const static std::string PVP_ATK_CARD = "PVP_ATK_CARD";
	const static std::string PVP_DEF_CARD = "PVP_DEF_CARD";
	const static std::string LUCKLY_CARD = "LUCKLY_CARD";
	const static std::string ATK_PVP = "ATK_PVP";
	const static std::string DEF_PVP = "DEF_PVP";
	const static std::string CreditValue = "CreditValue";
	const static std::string SoulValue = "SoulValue";
	const static std::string OnlineCount = "OnlineCount";
	const static std::string TotalTime = "TotalTime";
	const static std::string Citizenship = "Citizenship";
	const static std::string LastOfflineTime = "LastOfflineTime";
	const static std::string OnlineTime = "OnlineTime";
	const static std::string TotalLineTime = "TotalLineTime";
	const static std::string GMLevel = "GMLevel";
	const static std::string LastAttacker = "LastAttacker";
	const static std::string MAXHP = "MAXHP";
	const static std::string MAXMP = "MAXMP";
	const static std::string MAXSP = "MAXSP";
	const static std::string HPREGEN = "HPREGEN";
	const static std::string SPREGEN = "SPREGEN";
	const static std::string MPREGEN = "MPREGEN";
	const static std::string ATK_VALUE = "ATK_VALUE";
	const static std::string DEF_VALUE = "DEF_VALUE";
	const static std::string MOVE_SPEED = "MOVE_SPEED";
	const static std::string ATK_SPEED = "ATK_SPEED";
	const static std::string ATK_FIRE = "ATK_FIRE";
	const static std::string ATK_LIGHT = "ATK_LIGHT";
	const static std::string ATK_ICE = "ATK_ICE";
	const static std::string ATK_POISON = "ATK_POISON";
	const static std::string DEF_FIRE = "DEF_FIRE";
	const static std::string DEF_LIGHT = "DEF_LIGHT";
	const static std::string DEF_ICE = "DEF_ICE";
	const static std::string DEF_POISON = "DEF_POISON";
	const static std::string DIZZY_GATE = "DIZZY_GATE";
	const static std::string MOVE_GATE = "MOVE_GATE";
	const static std::string SKILL_GATE = "SKILL_GATE";
	const static std::string PHYSICAL_GATE = "PHYSICAL_GATE";
	const static std::string MAGIC_GATE = "MAGIC_GATE";
	const static std::string BUFF_GATE = "BUFF_GATE";
	const static std::string X = "X";
	const static std::string Y = "Y";
	const static std::string Z = "Z";
	const static std::string TargetX = "TargetX";
	const static std::string TargetY = "TargetY";
	const static std::string TargetZ = "TargetZ";
	const static std::string PathStep = "PathStep";
	const static std::string LoadPropertyFinish = "LoadPropertyFinish";
	const static std::string GameID = "GameID";
	const static std::string GateID = "GateID";
	const static std::string GuildID = "GuildID";

};

class R_Player
{
public:
	const static std::string PlayerViewItem = "PlayerViewItem";
	const static std::string PlayerHero = "PlayerHero";
	const static std::string BagEquipList = "BagEquipList";
	const static std::string BagItemList = "BagItemList";
	const static std::string CommPropertyValue = "CommPropertyValue";
	const static std::string EctypeList = "EctypeList";
	const static std::string DropItemList = "DropItemList";
	const static std::string SkillTable = "SkillTable";
	const static std::string TaskMonsterList = "TaskMonsterList";
	const static std::string TaskList = "TaskList";
	const static std::string PVPList = "PVPList";
	const static std::string ChatGroup = "ChatGroup";

};

class P_Scene
{
public:
	const static std::string SceneName = "SceneName";
	const static std::string SceneShowName = "SceneShowName";
	const static std::string MaxGroup = "MaxGroup";
	const static std::string MaxGroupPlayers = "MaxGroupPlayers";
	const static std::string FilePath = "FilePath";
	const static std::string RelivePos = "RelivePos";
	const static std::string Width = "Width";
	const static std::string SoundList = "SoundList";
	const static std::string Share = "Share";
	const static std::string CanClone = "CanClone";
	const static std::string ActorID = "ActorID";
	const static std::string LoadingUI = "LoadingUI";
	const static std::string CamOffestPos = "CamOffestPos";
	const static std::string CamOffestRot = "CamOffestRot";

};

class R_Scene
{
public:

};

class P_Server
{
public:
	const static std::string ServerID = "ServerID";
	const static std::string Name = "Name";
	const static std::string MaxOnline = "MaxOnline";
	const static std::string CpuCount = "CpuCount";
	const static std::string IP = "IP";
	const static std::string Port = "Port";
	const static std::string Type = "Type";

};

class R_Server
{
public:

};

class P_Shop
{
public:
	const static std::string Type = "Type";
	const static std::string ItemID = "ItemID";
	const static std::string Gold = "Gold";
	const static std::string Steel = "Steel";
	const static std::string Stone = "Stone";
	const static std::string Diamond = "Diamond";
	const static std::string Level = "Level";

};

class R_Shop
{
public:

};

class P_Skill
{
public:
	const static std::string SkillType = "SkillType";
	const static std::string AnimaState = "AnimaState";
	const static std::string ShowName = "ShowName";
	const static std::string Desc = "Desc";
	const static std::string ConsumeProperty = "ConsumeProperty";
	const static std::string ConsumeValue = "ConsumeValue";
	const static std::string ConsumeType = "ConsumeType";
	const static std::string DamageProperty = "DamageProperty";
	const static std::string DamageValue = "DamageValue";
	const static std::string DamageType = "DamageType";
	const static std::string GetBuffList = "GetBuffList";
	const static std::string SendBuffList = "SendBuffList";
	const static std::string CoolDownTime = "CoolDownTime";
	const static std::string RequireDistance = "RequireDistance";
	const static std::string DamageDistance = "DamageDistance";
	const static std::string TargetType = "TargetType";
	const static std::string NewObject = "NewObject";
	const static std::string Icon = "Icon";
	const static std::string Atlas = "Atlas";
	const static std::string UpLevel = "UpLevel";
	const static std::string AfterUpSkillID = "AfterUpSkillID";
	const static std::string PlayerSkill = "PlayerSkill";
	const static std::string AtkDis = "AtkDis";
	const static std::string NeedTar = "NeedTar";
	const static std::string DefaultHitTime = "DefaultHitTime";

};

class R_Skill
{
public:

};

class P_SkillRef
{
public:
	const static std::string NORMALATTACK1 = "NORMALATTACK1";
	const static std::string NORMALATTACK2 = "NORMALATTACK2";
	const static std::string NORMALATTACK3 = "NORMALATTACK3";
	const static std::string NORMALTHUMP = "NORMALTHUMP";
	const static std::string SKILL1 = "SKILL1";
	const static std::string SKILL2 = "SKILL2";
	const static std::string SKILL3 = "SKILL3";
	const static std::string SKILL4 = "SKILL4";
	const static std::string SKILL5 = "SKILL5";
	const static std::string SKILL6 = "SKILL6";
	const static std::string SKILL7 = "SKILL7";
	const static std::string SKILL8 = "SKILL8";
	const static std::string SKILL9 = "SKILL9";
	const static std::string SKILL10 = "SKILL10";

};

class R_SkillRef
{
public:

};

class P_SqlServer
{
public:
	const static std::string ServerID = "ServerID";
	const static std::string IP = "IP";
	const static std::string Port = "Port";
	const static std::string SqlIP = "SqlIP";
	const static std::string SqlPort = "SqlPort";
	const static std::string SqlUser = "SqlUser";
	const static std::string SqlPwd = "SqlPwd";
	const static std::string SqlName = "SqlName";

};

class R_SqlServer
{
public:

};

class P_StateFuncResources
{
public:
	const static std::string Atlas_ResID = "Atlas_ResID";

};

class R_StateFuncResources
{
public:

};

class P_StateFunction
{
public:
	const static std::string EFT_INFO = "EFT_INFO";
	const static std::string EFT_BOOOST = "EFT_BOOOST";
	const static std::string EFT_LVLUP = "EFT_LVLUP";
	const static std::string EFT_CREATE_SOLDER = "EFT_CREATE_SOLDER";
	const static std::string EFT_CREATE_SPEEL = "EFT_CREATE_SPEEL";
	const static std::string EFT_RESEARCH = "EFT_RESEARCH";
	const static std::string EFT_COLLECT_GOLD = "EFT_COLLECT_GOLD";
	const static std::string EFT_COLLECT_STONE = "EFT_COLLECT_STONE";
	const static std::string EFT_COLLECT_STEEL = "EFT_COLLECT_STEEL";
	const static std::string EFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND";
	const static std::string EFT_SELL = "EFT_SELL";
	const static std::string EFT_REPAIR = "EFT_REPAIR";
	const static std::string EFT_CANCEL = "EFT_CANCEL";
	const static std::string EFT_FINISH = "EFT_FINISH";

};

class R_StateFunction
{
public:

};

class P_Task
{
public:
	const static std::string Type = "Type";
	const static std::string NextTaskID = "NextTaskID";
	const static std::string KillMonsterName = "KillMonsterName";
	const static std::string KillCount = "KillCount";
	const static std::string LevelReq = "LevelReq";
	const static std::string AwardExp = "AwardExp";
	const static std::string AwardGold = "AwardGold";
	const static std::string AwardPack = "AwardPack";
	const static std::string Desc = "Desc";

};

class R_Task
{
public:

};

} // !@NFrame

#endif NF_PR_NAME_HPP // !NF_PR_NAME_HPP
