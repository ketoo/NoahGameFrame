// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.hpp
//    @Author           :    NFrame Studio
//    @Module           :    NFProtocolDefine
// -------------------------------------------------------------------------

#ifndef NF_PR_NAME_HPP
#define NF_PR_NAME_HPP

#include <string>
namespace NFrame
{
class IObject
{
public:
	//Class name
	const std::string ThisName = "IObject";
	// IObject
	// Property
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Record

};

class Buff
{
public:
	//Class name
	const std::string ThisName = "Buff";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string EffectType = "EffectType"; // int
	const std::string EffectValueType = "EffectValueType"; // int
	const std::string EffectValueReferType = "EffectValueReferType"; // int
	const std::string EffectTimeValue = "EffectTimeValue"; // int
	const std::string EffectTimeInterval = "EffectTimeInterval"; // float
	const std::string WashGroupID = "WashGroupID"; // int
	const std::string ReverseReferType = "ReverseReferType"; // int
	const std::string EffectClearOnDead = "EffectClearOnDead"; // int
	const std::string DownSaveType = "DownSaveType"; // int
	// Record

};

class ConsumeData
{
public:
	//Class name
	const std::string ThisName = "ConsumeData";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string VIPEXP = "VIPEXP"; // int
	const std::string EXP = "EXP"; // int
	const std::string HP = "HP"; // int
	const std::string SP = "SP"; // int
	const std::string MP = "MP"; // int
	const std::string Gold = "Gold"; // int
	const std::string Money = "Money"; // int
	// Record

};

class Cost
{
public:
	//Class name
	const std::string ThisName = "Cost";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string CostMoney = "CostMoney"; // int
	const std::string CostDiamond = "CostDiamond"; // int
	const std::string CostVP = "CostVP"; // int
	const std::string CostHonour = "CostHonour"; // int
	// Record

};

class DescData
{
public:
	//Class name
	const std::string ThisName = "DescData";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ShowName = "ShowName"; // string
	const std::string Icon = "Icon"; // string
	const std::string Atlas = "Atlas"; // string
	const std::string PrefabPath = "PrefabPath"; // string
	const std::string PerformanceEffect = "PerformanceEffect"; // string
	const std::string PerformanceSound = "PerformanceSound"; // string
	const std::string DescText = "DescText"; // string
	// Record

};

class EffectData
{
public:
	//Class name
	const std::string ThisName = "EffectData";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	const std::string CRITICAL = "CRITICAL"; // int
	const std::string MAXHP = "MAXHP"; // int
	const std::string MAXMP = "MAXMP"; // int
	const std::string MAXSP = "MAXSP"; // int
	const std::string HP = "HP"; // int
	const std::string MP = "MP"; // int
	const std::string SP = "SP"; // int
	const std::string HPREGEN = "HPREGEN"; // int
	const std::string SPREGEN = "SPREGEN"; // int
	const std::string MPREGEN = "MPREGEN"; // int
	const std::string ATK_VALUE = "ATK_VALUE"; // int
	const std::string DEF_VALUE = "DEF_VALUE"; // int
	const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	const std::string ATK_SPEED = "ATK_SPEED"; // int
	const std::string ATK_FIRE = "ATK_FIRE"; // int
	const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	const std::string ATK_WIND = "ATK_WIND"; // int
	const std::string ATK_ICE = "ATK_ICE"; // int
	const std::string ATK_POISON = "ATK_POISON"; // int
	const std::string DEF_FIRE = "DEF_FIRE"; // int
	const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	const std::string DEF_WIND = "DEF_WIND"; // int
	const std::string DEF_ICE = "DEF_ICE"; // int
	const std::string DEF_POISON = "DEF_POISON"; // int
	const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	const std::string MOVE_GATE = "MOVE_GATE"; // int
	const std::string SKILL_GATE = "SKILL_GATE"; // int
	const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

};

class Equip
{
public:
	//Class name
	const std::string ThisName = "Equip";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Sex = "Sex"; // int
	const std::string IntensiveBuffList = "IntensiveBuffList"; // string
	const std::string EnchantmentBuffList = "EnchantmentBuffList"; // string
	const std::string SuitID = "SuitID"; // int
	const std::string SuitBuffID = "SuitBuffID"; // string
	const std::string ItemType = "ItemType"; // int
	const std::string ItemSubType = "ItemSubType"; // int
	const std::string Level = "Level"; // int
	const std::string Job = "Job"; // string
	const std::string Quality = "Quality"; // int
	const std::string ShowName = "ShowName"; // string
	const std::string Desc = "Desc"; // string
	const std::string EffectData = "EffectData"; // string
	const std::string PrefabPath = "PrefabPath"; // string
	const std::string DropPrePath = "DropPrePath"; // string
	const std::string BuyPrice = "BuyPrice"; // int
	const std::string SalePrice = "SalePrice"; // int
	const std::string Icon = "Icon"; // string
	// Record

};

class Guild
{
public:
	//Class name
	const std::string ThisName = "Guild";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Name = "Name"; // string
	const std::string PresidentID = "PresidentID"; // object
	const std::string PresidentName = "PresidentName"; // string
	const std::string Rank = "Rank"; // int
	const std::string GuildAD = "GuildAD"; // string
	const std::string GuildDesc = "GuildDesc"; // string
	const std::string GuildMoney = "GuildMoney"; // int
	const std::string GuildLevel = "GuildLevel"; // int
	const std::string GuildContinueDay = "GuildContinueDay"; // int
	const std::string GuildID = "GuildID"; // object
	const std::string GuildIcon = "GuildIcon"; // int
	const std::string GuildMemeberCount = "GuildMemeberCount"; // int
	const std::string GuildMemeberMaxCount = "GuildMemeberMaxCount"; // int
	const std::string GuildHonor = "GuildHonor"; // int
	const std::string GuildCreateTime = "GuildCreateTime"; // int
	const std::string GuildCreateter = "GuildCreateter"; // int
	const std::string GuildExp = "GuildExp"; // int
	const std::string GuildStatus = "GuildStatus"; // int
	const std::string DismissTime = "DismissTime"; // int
	const std::string RecruitAD = "RecruitAD"; // string
	const std::string RecruitLevel = "RecruitLevel"; // int
	const std::string KingWarResource = "KingWarResource"; // int
	const std::string AutoRecruit = "AutoRecruit"; // string
	const std::string EctypServer = "EctypServer"; // int
	const std::string EctypID = "EctypID"; // int
	const std::string EctypIDGroup = "EctypIDGroup"; // int
	// Record

	class GuildBoss
	{
	public:
		const std::string ThisName = "GuildBoss";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Offline		= 6; // Offline -- int
		const int Power		= 7; // Power -- int

	};

	class GuildMemberList
	{
	public:
		const std::string ThisName = "GuildMemberList";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int Receive		= 5; // Receive -- int
		const int VIP		= 6; // VIP -- int
		const int Online		= 7; // Online -- int
		const int Power		= 8; // Power -- int
		const int Title		= 9; // Title -- int
		const int GameID		= 10; // GameID -- int
		const int JoinTime		= 11; // JoinTime -- int
		const int Contribution		= 12; // Contribution -- int
		const int AllContribution		= 13; // AllContribution -- int

	};

	class GuildAppyList
	{
	public:
		const std::string ThisName = "GuildAppyList";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Power		= 6; // Power -- int

	};

	class GuildEvent
	{
	public:
		const std::string ThisName = "GuildEvent";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Offline		= 6; // Offline -- int
		const int Power		= 7; // Power -- int
		const int EventID		= 8; // EventID -- int
		const int EventTime		= 9; // EventTime -- int
		const int Context		= 10; // Context -- string

	};

	class GuildHouse
	{
	public:
		const std::string ThisName = "GuildHouse";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Offline		= 6; // Offline -- int
		const int Power		= 7; // Power -- int

	};

	class GuildSkill
	{
	public:
		const std::string ThisName = "GuildSkill";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Offline		= 6; // Offline -- int
		const int Power		= 7; // Power -- int

	};

};

class GuildConfig
{
public:
	//Class name
	const std::string ThisName = "GuildConfig";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ConditionPlayerLevel = "ConditionPlayerLevel"; // int
	const std::string ConditionPlayerVIP = "ConditionPlayerVIP"; // int
	const std::string CostMoney = "CostMoney"; // int
	const std::string DismissTime = "DismissTime"; // int
	const std::string GuildLevel = "GuildLevel"; // int
	const std::string MaxMember = "MaxMember"; // int
	// Record

};

class GuildJob
{
public:
	//Class name
	const std::string ThisName = "GuildJob";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Job = "Job"; // int
	const std::string JobCount = "JobCount"; // object
	const std::string Appoint = "Appoint"; // object
	const std::string Fire = "Fire"; // object
	const std::string Demise = "Demise"; // object
	const std::string ApplyDismiss = "ApplyDismiss"; // object
	const std::string StopDismiss = "StopDismiss"; // object
	const std::string AcceptApply = "AcceptApply"; // object
	const std::string DenyApply = "DenyApply"; // object
	const std::string Kickout = "Kickout"; // object
	const std::string SetRecruit = "SetRecruit"; // object
	const std::string PublishRecruit = "PublishRecruit"; // object
	const std::string EditAD = "EditAD"; // object
	const std::string Leave = "Leave"; // object
	const std::string LevelUp = "LevelUp"; // object
	// Record

};

class HttpServer
{
public:
	//Class name
	const std::string ThisName = "HttpServer";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ServerID = "ServerID"; // int
	const std::string WebPort = "WebPort"; // int
	const std::string WebRootPath = "WebRootPath"; // string
	// Record

};

class InitProperty
{
public:
	//Class name
	const std::string ThisName = "InitProperty";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Job = "Job"; // int
	const std::string Level = "Level"; // int
	const std::string EffectData = "EffectData"; // string
	const std::string SkillIDRef = "SkillIDRef"; // string
	const std::string ModelPtah = "ModelPtah"; // string
	// Record

};

class Item
{
public:
	//Class name
	const std::string ThisName = "Item";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ItemType = "ItemType"; // int
	const std::string ItemSubType = "ItemSubType"; // int
	const std::string Level = "Level"; // int
	const std::string Job = "Job"; // string
	const std::string Quality = "Quality"; // int
	const std::string DescID = "DescID"; // string
	const std::string EffectData = "EffectData"; // string
	const std::string ConsumeData = "ConsumeData"; // string
	const std::string AwardData = "AwardData"; // string
	const std::string AwardProperty = "AwardProperty"; // int
	const std::string CoolDownTime = "CoolDownTime"; // float
	const std::string OverlayCount = "OverlayCount"; // int
	const std::string ExpiredType = "ExpiredType"; // int
	const std::string BuyPrice = "BuyPrice"; // int
	const std::string SalePrice = "SalePrice"; // int
	const std::string Script = "Script"; // string
	const std::string Extend = "Extend"; // string
	const std::string SpriteFile = "SpriteFile"; // string
	const std::string Icon = "Icon"; // string
	const std::string ShowName = "ShowName"; // string
	const std::string HeroTye = "HeroTye"; // int
	// Record

};

class Language
{
public:
	//Class name
	const std::string ThisName = "Language";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string English = "English"; // string
	const std::string Chinese = "Chinese"; // string
	// Record

};

class Map
{
public:
	//Class name
	const std::string ThisName = "Map";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string MaxCount = "MaxCount"; // int
	const std::string InComeGold = "InComeGold"; // int
	const std::string InComeDiamond = "InComeDiamond"; // int
	const std::string InComeOil = "InComeOil"; // int
	const std::string MapLevel = "MapLevel"; // int
	// Record

	class Station
	{
	public:
		const std::string ThisName = "Station";
		const int GUID		= 0; // GUID -- object
		const int GuildID		= 1; // GuildID -- object
		const int GuildName		= 2; // GuildName -- string
		const int Level		= 3; // Level -- int
		const int Title		= 4; // Title -- string
		const int Slogan		= 5; // Slogan -- string
		const int State		= 6; // State -- int
		const int CurMemberCount		= 7; // CurMemberCount -- int
		const int WinCount		= 8; // WinCount -- int

	};

};

class NoSqlServer
{
public:
	//Class name
	const std::string ThisName = "NoSqlServer";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ServerID = "ServerID"; // int
	const std::string IP = "IP"; // string
	const std::string Port = "Port"; // int
	const std::string Auth = "Auth"; // string
	// Record

};

class NPC
{
public:
	//Class name
	const std::string ThisName = "NPC";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string SeedID = "SeedID"; // string
	const std::string VIPEXP = "VIPEXP"; // int
	const std::string EXP = "EXP"; // int
	const std::string HP = "HP"; // int
	const std::string SP = "SP"; // int
	const std::string MP = "MP"; // int
	const std::string Gold = "Gold"; // int
	const std::string Money = "Money"; // int
	const std::string TargetX = "TargetX"; // float
	const std::string TargetY = "TargetY"; // float
	const std::string Prefab = "Prefab"; // string
	const std::string Climb = "Climb"; // int
	const std::string AtkDis = "AtkDis"; // float
	const std::string DropPackList = "DropPackList"; // string
	const std::string Idle = "Idle"; // string
	const std::string SkillNormal = "SkillNormal"; // string
	const std::string SkillAttack = "SkillAttack"; // string
	const std::string SkillTHUMP = "SkillTHUMP"; // string
	const std::string Height = "Height"; // float
	const std::string EffectData = "EffectData"; // string
	const std::string ConsumeData = "ConsumeData"; // string
	const std::string LastAttacker = "LastAttacker"; // object
	const std::string ShowName = "ShowName"; // string
	const std::string EquipIDRef = "EquipIDRef"; // string
	const std::string SpriteFile = "SpriteFile"; // string
	const std::string Icon = "Icon"; // string
	const std::string ShowCard = "ShowCard"; // string
	const std::string HeroType = "HeroType"; // int
	const std::string Camp = "Camp"; // int
	const std::string MasterID = "MasterID"; // object
	const std::string NPCType = "NPCType"; // int
	const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	const std::string CRITICAL = "CRITICAL"; // int
	const std::string MAXHP = "MAXHP"; // int
	const std::string MAXMP = "MAXMP"; // int
	const std::string MAXSP = "MAXSP"; // int
	const std::string HPREGEN = "HPREGEN"; // int
	const std::string SPREGEN = "SPREGEN"; // int
	const std::string MPREGEN = "MPREGEN"; // int
	const std::string ATK_VALUE = "ATK_VALUE"; // int
	const std::string DEF_VALUE = "DEF_VALUE"; // int
	const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	const std::string ATK_SPEED = "ATK_SPEED"; // int
	const std::string ATK_FIRE = "ATK_FIRE"; // int
	const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	const std::string ATK_WIND = "ATK_WIND"; // int
	const std::string ATK_ICE = "ATK_ICE"; // int
	const std::string ATK_POISON = "ATK_POISON"; // int
	const std::string DEF_FIRE = "DEF_FIRE"; // int
	const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	const std::string DEF_WIND = "DEF_WIND"; // int
	const std::string DEF_ICE = "DEF_ICE"; // int
	const std::string DEF_POISON = "DEF_POISON"; // int
	const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	const std::string MOVE_GATE = "MOVE_GATE"; // int
	const std::string SKILL_GATE = "SKILL_GATE"; // int
	const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

};

class Player
{
public:
	//Class name
	const std::string ThisName = "Player";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Name = "Name"; // string
	const std::string Sex = "Sex"; // int
	const std::string Job = "Job"; // int
	const std::string Race = "Race"; // int
	const std::string Camp = "Camp"; // int
	const std::string Level = "Level"; // int
	const std::string ShowName = "ShowName"; // string
	const std::string PrefabPath = "PrefabPath"; // string
	const std::string FirstTarget = "FirstTarget"; // object
	const std::string FightHero = "FightHero"; // object
	const std::string HeroPos1 = "HeroPos1"; // object
	const std::string HeroPos2 = "HeroPos2"; // object
	const std::string HeroPos3 = "HeroPos3"; // object
	const std::string Skill1 = "Skill1"; // string
	const std::string Skill2 = "Skill2"; // string
	const std::string Skill3 = "Skill3"; // string
	const std::string VIPLevel = "VIPLevel"; // int
	const std::string VIPEXP = "VIPEXP"; // int
	const std::string EXP = "EXP"; // int
	const std::string HP = "HP"; // int
	const std::string SP = "SP"; // int
	const std::string MP = "MP"; // int
	const std::string Gold = "Gold"; // int
	const std::string Money = "Money"; // int
	const std::string Account = "Account"; // string
	const std::string ConnectKey = "ConnectKey"; // string
	const std::string MAXEXP = "MAXEXP"; // int
	const std::string RELIVE_SOUL = "RELIVE_SOUL"; // int
	const std::string ATK_PVP = "ATK_PVP"; // int
	const std::string DEF_PVP = "DEF_PVP"; // int
	const std::string OnlineCount = "OnlineCount"; // int
	const std::string TotalTime = "TotalTime"; // int
	const std::string LastOfflineTime = "LastOfflineTime"; // object
	const std::string OnlineTime = "OnlineTime"; // object
	const std::string TotalLineTime = "TotalLineTime"; // object
	const std::string GMLevel = "GMLevel"; // int
	const std::string GameID = "GameID"; // int
	const std::string GateID = "GateID"; // int
	const std::string GuildID = "GuildID"; // object
	const std::string TeamID = "TeamID"; // object
	const std::string HomeSceneID = "HomeSceneID"; // int
	const std::string ViewOppnent = "ViewOppnent"; // object
	const std::string FightOppnent = "FightOppnent"; // object
	const std::string GambleGold = "GambleGold"; // int
	const std::string GambleDiamond = "GambleDiamond"; // int
	const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	const std::string CRITICAL = "CRITICAL"; // int
	const std::string MAXHP = "MAXHP"; // int
	const std::string MAXMP = "MAXMP"; // int
	const std::string MAXSP = "MAXSP"; // int
	const std::string HPREGEN = "HPREGEN"; // int
	const std::string SPREGEN = "SPREGEN"; // int
	const std::string MPREGEN = "MPREGEN"; // int
	const std::string ATK_VALUE = "ATK_VALUE"; // int
	const std::string DEF_VALUE = "DEF_VALUE"; // int
	const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	const std::string ATK_SPEED = "ATK_SPEED"; // int
	const std::string ATK_FIRE = "ATK_FIRE"; // int
	const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	const std::string ATK_WIND = "ATK_WIND"; // int
	const std::string ATK_ICE = "ATK_ICE"; // int
	const std::string ATK_POISON = "ATK_POISON"; // int
	const std::string DEF_FIRE = "DEF_FIRE"; // int
	const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	const std::string DEF_WIND = "DEF_WIND"; // int
	const std::string DEF_ICE = "DEF_ICE"; // int
	const std::string DEF_POISON = "DEF_POISON"; // int
	const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	const std::string MOVE_GATE = "MOVE_GATE"; // int
	const std::string SKILL_GATE = "SKILL_GATE"; // int
	const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

	class CommValue
	{
	public:
		const std::string ThisName = "CommValue";
		const int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		const int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		const int CRITICAL		= 2; // CRITICAL -- int
		const int MAXHP		= 3; // MAXHP -- int
		const int MAXMP		= 4; // MAXMP -- int
		const int MAXSP		= 5; // MAXSP -- int
		const int HPREGEN		= 6; // HPREGEN -- int
		const int SPREGEN		= 7; // SPREGEN -- int
		const int MPREGEN		= 8; // MPREGEN -- int
		const int ATK_VALUE		= 9; // ATK_VALUE -- int
		const int DEF_VALUE		= 10; // DEF_VALUE -- int
		const int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		const int ATK_SPEED		= 12; // ATK_SPEED -- int
		const int ATK_FIRE		= 13; // ATK_FIRE -- int
		const int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		const int ATK_WIND		= 15; // ATK_WIND -- int
		const int ATK_ICE		= 16; // ATK_ICE -- int
		const int ATK_POISON		= 17; // ATK_POISON -- int
		const int DEF_FIRE		= 18; // DEF_FIRE -- int
		const int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		const int DEF_WIND		= 20; // DEF_WIND -- int
		const int DEF_ICE		= 21; // DEF_ICE -- int
		const int DEF_POISON		= 22; // DEF_POISON -- int
		const int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		const int MOVE_GATE		= 24; // MOVE_GATE -- int
		const int SKILL_GATE		= 25; // SKILL_GATE -- int
		const int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		const int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		const int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	class PlayerHero
	{
	public:
		const std::string ThisName = "PlayerHero";
		const int GUID		= 0; // GUID -- object
		const int ConfigID		= 1; // ConfigID -- string
		const int Activated		= 2; // Activated -- int
		const int Level		= 3; // Level -- int
		const int Exp		= 4; // Exp -- int
		const int Star		= 5; // Star -- int
		const int Equip1		= 6; // Equip1 -- object
		const int Equip2		= 7; // Equip2 -- object
		const int Equip3		= 8; // Equip3 -- object
		const int Equip4		= 9; // Equip4 -- object
		const int Equip5		= 10; // Equip5 -- object
		const int Equip6		= 11; // Equip6 -- object
		const int Talent1		= 12; // Talent1 -- string
		const int Talent2		= 13; // Talent2 -- string
		const int Talent3		= 14; // Talent3 -- string
		const int Talent4		= 15; // Talent4 -- string
		const int Talent5		= 16; // Talent5 -- string
		const int Skill1		= 17; // Skill1 -- string
		const int Skill2		= 18; // Skill2 -- string
		const int Skill3		= 19; // Skill3 -- string
		const int Skill4		= 20; // Skill4 -- string
		const int Skill5		= 21; // Skill5 -- string
		const int FightSkill		= 22; // FightSkill -- string

	};

	class HeroPropertyValue
	{
	public:
		const std::string ThisName = "HeroPropertyValue";
		const int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		const int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		const int CRITICAL		= 2; // CRITICAL -- int
		const int MAXHP		= 3; // MAXHP -- int
		const int MAXMP		= 4; // MAXMP -- int
		const int MAXSP		= 5; // MAXSP -- int
		const int HPREGEN		= 6; // HPREGEN -- int
		const int SPREGEN		= 7; // SPREGEN -- int
		const int MPREGEN		= 8; // MPREGEN -- int
		const int ATK_VALUE		= 9; // ATK_VALUE -- int
		const int DEF_VALUE		= 10; // DEF_VALUE -- int
		const int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		const int ATK_SPEED		= 12; // ATK_SPEED -- int
		const int ATK_FIRE		= 13; // ATK_FIRE -- int
		const int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		const int ATK_WIND		= 15; // ATK_WIND -- int
		const int ATK_ICE		= 16; // ATK_ICE -- int
		const int ATK_POISON		= 17; // ATK_POISON -- int
		const int DEF_FIRE		= 18; // DEF_FIRE -- int
		const int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		const int DEF_WIND		= 20; // DEF_WIND -- int
		const int DEF_ICE		= 21; // DEF_ICE -- int
		const int DEF_POISON		= 22; // DEF_POISON -- int
		const int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		const int MOVE_GATE		= 24; // MOVE_GATE -- int
		const int SKILL_GATE		= 25; // SKILL_GATE -- int
		const int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		const int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		const int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	class BagEquipList
	{
	public:
		const std::string ThisName = "BagEquipList";
		const int GUID		= 0; // GUID -- object
		const int WearGUID		= 1; // WearGUID -- object
		const int ConfigID		= 2; // ConfigID -- string
		const int ExpiredType		= 3; // ExpiredType -- int
		const int Date		= 4; // Date -- int
		const int RandPropertyID		= 5; // RandPropertyID -- string
		const int SlotCount		= 6; // SlotCount -- int
		const int InlayStone1		= 7; // InlayStone1 -- string
		const int InlayStone2		= 8; // InlayStone2 -- string
		const int InlayStone3		= 9; // InlayStone3 -- string
		const int InlayStone4		= 10; // InlayStone4 -- string
		const int InlayStone5		= 11; // InlayStone5 -- string
		const int InlayStone6		= 12; // InlayStone6 -- string
		const int InlayStone7		= 13; // InlayStone7 -- string
		const int InlayStone8		= 14; // InlayStone8 -- string
		const int InlayStone9		= 15; // InlayStone9 -- string
		const int InlayStone10		= 16; // InlayStone10 -- string
		const int IntensifyLevel		= 17; // IntensifyLevel -- string
		const int ElementLevel1_FIRE		= 18; // ElementLevel1_FIRE -- int
		const int ElementLevel2_LIGHT		= 19; // ElementLevel2_LIGHT -- int
		const int ElementLevel3_Wind		= 20; // ElementLevel3_Wind -- int
		const int ElementLevel4_ICE		= 21; // ElementLevel4_ICE -- int
		const int ElementLevel5_POISON		= 22; // ElementLevel5_POISON -- int

	};

	class BagItemList
	{
	public:
		const std::string ThisName = "BagItemList";
		const int ConfigID		= 0; // ConfigID -- string
		const int ItemCount		= 1; // ItemCount -- int
		const int Bound		= 2; // Bound -- int
		const int ExpiredType		= 3; // ExpiredType -- int
		const int Date		= 4; // Date -- int

	};

	class TaskMonsterList
	{
	public:
		const std::string ThisName = "TaskMonsterList";
		const int MonsterID		= 0; // MonsterID -- string
		const int CurrentKillCount		= 1; // CurrentKillCount -- int
		const int RequireKillCount		= 2; // RequireKillCount -- int
		const int TaskID		= 3; // TaskID -- string

	};

	class TaskList
	{
	public:
		const std::string ThisName = "TaskList";
		const int TaskID		= 0; // TaskID -- string
		const int TaskStatus		= 1; // TaskStatus -- int
		const int Process		= 2; // Process -- int

	};

	class BuildingList
	{
	public:
		const std::string ThisName = "BuildingList";
		const int BuildingID		= 0; // BuildingID -- string
		const int BuildingGUID		= 1; // BuildingGUID -- object
		const int State		= 2; // State -- int
		const int PosX		= 3; // PosX -- float
		const int Pos		= 4; // Pos -- vector3
		const int StateEndTime		= 5; // StateEndTime -- int

	};

	class BuildingListProduce
	{
	public:
		const std::string ThisName = "BuildingListProduce";
		const int BuildingID		= 0; // BuildingID -- string
		const int BuildingGUID		= 1; // BuildingGUID -- object
		const int State		= 2; // State -- int
		const int PosX		= 3; // PosX -- float
		const int PosY		= 4; // PosY -- float
		const int StateEndTime		= 5; // StateEndTime -- int

	};

};

class Scene
{
public:
	//Class name
	const std::string ThisName = "Scene";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string SceneName = "SceneName"; // string
	const std::string SceneShowName = "SceneShowName"; // string
	const std::string MaxGroup = "MaxGroup"; // int
	const std::string MaxGroupPlayers = "MaxGroupPlayers"; // int
	const std::string FilePath = "FilePath"; // string
	const std::string RelivePos = "RelivePos"; // string
	const std::string Width = "Width"; // int
	const std::string SoundList = "SoundList"; // string
	const std::string Tile = "Tile"; // int
	const std::string Share = "Share"; // int
	const std::string CanClone = "CanClone"; // int
	const std::string ActorID = "ActorID"; // int
	const std::string LoadingUI = "LoadingUI"; // string
	const std::string CamOffestPos = "CamOffestPos"; // string
	const std::string CamOffestRot = "CamOffestRot"; // string
	const std::string SyncObject = "SyncObject"; // int
	const std::string NavigationResPath = "NavigationResPath"; // string
	// Record

};

class Security
{
public:
	//Class name
	const std::string ThisName = "Security";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string SecurityData = "SecurityData"; // string
	// Record

};

class Server
{
public:
	//Class name
	const std::string ThisName = "Server";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ServerID = "ServerID"; // int
	const std::string Name = "Name"; // string
	const std::string MaxOnline = "MaxOnline"; // int
	const std::string CpuCount = "CpuCount"; // int
	const std::string IP = "IP"; // string
	const std::string Port = "Port"; // int
	const std::string Type = "Type"; // int
	const std::string Area = "Area"; // int
	// Record

};

class Shop
{
public:
	//Class name
	const std::string ThisName = "Shop";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Type = "Type"; // int
	const std::string ItemID = "ItemID"; // string
	const std::string Gold = "Gold"; // int
	const std::string Steel = "Steel"; // int
	const std::string Stone = "Stone"; // int
	const std::string Diamond = "Diamond"; // int
	const std::string Level = "Level"; // int
	const std::string Count = "Count"; // int
	// Record

};

class Skill
{
public:
	//Class name
	const std::string ThisName = "Skill";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string SkillType = "SkillType"; // int
	const std::string AnimaState = "AnimaState"; // string
	const std::string ShowName = "ShowName"; // string
	const std::string Desc = "Desc"; // string
	const std::string ConsumeProperty = "ConsumeProperty"; // string
	const std::string ConsumeValue = "ConsumeValue"; // string
	const std::string ConsumeType = "ConsumeType"; // int
	const std::string DamageProperty = "DamageProperty"; // string
	const std::string DamageValue = "DamageValue"; // string
	const std::string DamageType = "DamageType"; // int
	const std::string GetBuffList = "GetBuffList"; // string
	const std::string SendBuffList = "SendBuffList"; // string
	const std::string CoolDownTime = "CoolDownTime"; // float
	const std::string RequireDistance = "RequireDistance"; // float
	const std::string DamageDistance = "DamageDistance"; // float
	const std::string TargetType = "TargetType"; // int
	const std::string NewObject = "NewObject"; // string
	const std::string Icon = "Icon"; // string
	const std::string SpriteFile = "SpriteFile"; // string
	const std::string PlayerSkill = "PlayerSkill"; // int
	const std::string AtkDis = "AtkDis"; // float
	const std::string DefaultHitTime = "DefaultHitTime"; // float
	const std::string NextID = "NextID"; // string
	// Record

};

class SqlServer
{
public:
	//Class name
	const std::string ThisName = "SqlServer";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ServerID = "ServerID"; // int
	const std::string IP = "IP"; // string
	const std::string Port = "Port"; // int
	const std::string SqlIP = "SqlIP"; // string
	const std::string SqlPort = "SqlPort"; // int
	const std::string SqlUser = "SqlUser"; // string
	const std::string SqlPwd = "SqlPwd"; // string
	const std::string SqlName = "SqlName"; // string
	// Record

};

class Talent
{
public:
	//Class name
	const std::string ThisName = "Talent";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string ShowName = "ShowName"; // string
	const std::string Desc = "Desc"; // string
	const std::string EffectData = "EffectData"; // string
	const std::string Icon = "Icon"; // string
	const std::string SpriteFile = "SpriteFile"; // string
	const std::string NextID = "NextID"; // string
	// Record

};

class Task
{
public:
	//Class name
	const std::string ThisName = "Task";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Type = "Type"; // int
	const std::string NextTaskID = "NextTaskID"; // string
	const std::string KillMonsterName = "KillMonsterName"; // string
	const std::string KillCount = "KillCount"; // string
	const std::string LevelReq = "LevelReq"; // string
	const std::string AwardExp = "AwardExp"; // int
	const std::string AwardGold = "AwardGold"; // int
	const std::string AwardPack = "AwardPack"; // string
	const std::string Desc = "Desc"; // string
	// Record

};

class Team
{
public:
	//Class name
	const std::string ThisName = "Team";
	// IObject
	const std::string ID = "ID"; // string
	const std::string ClassName = "ClassName"; // string
	const std::string SceneID = "SceneID"; // int
	const std::string GroupID = "GroupID"; // int
	const std::string ConfigID = "ConfigID"; // string
	const std::string Position = "Position"; // vector3
	// Property
	const std::string Captain = "Captain"; // object
	const std::string PresidentName = "PresidentName"; // string
	// Record

	class MemberList
	{
	public:
		const std::string ThisName = "MemberList";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int Receive		= 5; // Receive -- int
		const int VIP		= 6; // VIP -- int
		const int Online		= 7; // Online -- int
		const int Title		= 8; // Title -- int
		const int GameID		= 9; // GameID -- int

	};

	class ApplyList
	{
	public:
		const std::string ThisName = "ApplyList";
		const int GUID		= 0; // GUID -- object
		const int Name		= 1; // Name -- string
		const int Level		= 2; // Level -- int
		const int Job		= 3; // Job -- int
		const int Donation		= 4; // Donation -- int
		const int VIP		= 5; // VIP -- int
		const int Power		= 6; // Power -- int

	};

};

} // !@NFrame

#endif // !NF_PR_NAME_HPP