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
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Record

};

class Buff
{
public:
	//Class name
	const std::string ThisName = "Buff";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string EffectType = "EffectType"; // int
	static const std::string EffectValueType = "EffectValueType"; // int
	static const std::string EffectValueReferType = "EffectValueReferType"; // int
	static const std::string EffectTimeValue = "EffectTimeValue"; // int
	static const std::string EffectTimeInterval = "EffectTimeInterval"; // float
	static const std::string WashGroupID = "WashGroupID"; // int
	static const std::string ReverseReferType = "ReverseReferType"; // int
	static const std::string EffectClearOnDead = "EffectClearOnDead"; // int
	static const std::string DownSaveType = "DownSaveType"; // int
	// Record

};

class ConsumeData
{
public:
	//Class name
	const std::string ThisName = "ConsumeData";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string VIPEXP = "VIPEXP"; // int
	static const std::string EXP = "EXP"; // int
	static const std::string HP = "HP"; // int
	static const std::string SP = "SP"; // int
	static const std::string MP = "MP"; // int
	static const std::string Gold = "Gold"; // int
	static const std::string Money = "Money"; // int
	// Record

};

class Cost
{
public:
	//Class name
	const std::string ThisName = "Cost";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string CostMoney = "CostMoney"; // int
	static const std::string CostDiamond = "CostDiamond"; // int
	static const std::string CostVP = "CostVP"; // int
	static const std::string CostHonour = "CostHonour"; // int
	// Record

};

class DescData
{
public:
	//Class name
	const std::string ThisName = "DescData";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ShowName = "ShowName"; // string
	static const std::string Icon = "Icon"; // string
	static const std::string Atlas = "Atlas"; // string
	static const std::string PrefabPath = "PrefabPath"; // string
	static const std::string PerformanceEffect = "PerformanceEffect"; // string
	static const std::string PerformanceSound = "PerformanceSound"; // string
	static const std::string DescText = "DescText"; // string
	// Record

};

class EffectData
{
public:
	//Class name
	const std::string ThisName = "EffectData";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	static const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	static const std::string CRITICAL = "CRITICAL"; // int
	static const std::string MAXHP = "MAXHP"; // int
	static const std::string MAXMP = "MAXMP"; // int
	static const std::string MAXSP = "MAXSP"; // int
	static const std::string HP = "HP"; // int
	static const std::string MP = "MP"; // int
	static const std::string SP = "SP"; // int
	static const std::string HPREGEN = "HPREGEN"; // int
	static const std::string SPREGEN = "SPREGEN"; // int
	static const std::string MPREGEN = "MPREGEN"; // int
	static const std::string ATK_VALUE = "ATK_VALUE"; // int
	static const std::string DEF_VALUE = "DEF_VALUE"; // int
	static const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	static const std::string ATK_SPEED = "ATK_SPEED"; // int
	static const std::string ATK_FIRE = "ATK_FIRE"; // int
	static const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	static const std::string ATK_WIND = "ATK_WIND"; // int
	static const std::string ATK_ICE = "ATK_ICE"; // int
	static const std::string ATK_POISON = "ATK_POISON"; // int
	static const std::string DEF_FIRE = "DEF_FIRE"; // int
	static const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	static const std::string DEF_WIND = "DEF_WIND"; // int
	static const std::string DEF_ICE = "DEF_ICE"; // int
	static const std::string DEF_POISON = "DEF_POISON"; // int
	static const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	static const std::string MOVE_GATE = "MOVE_GATE"; // int
	static const std::string SKILL_GATE = "SKILL_GATE"; // int
	static const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	static const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	static const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

};

class Equip
{
public:
	//Class name
	const std::string ThisName = "Equip";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Sex = "Sex"; // int
	static const std::string IntensiveBuffList = "IntensiveBuffList"; // string
	static const std::string EnchantmentBuffList = "EnchantmentBuffList"; // string
	static const std::string SuitID = "SuitID"; // int
	static const std::string SuitBuffID = "SuitBuffID"; // string
	static const std::string ItemType = "ItemType"; // int
	static const std::string ItemSubType = "ItemSubType"; // int
	static const std::string Level = "Level"; // int
	static const std::string Job = "Job"; // string
	static const std::string Quality = "Quality"; // int
	static const std::string ShowName = "ShowName"; // string
	static const std::string Desc = "Desc"; // string
	static const std::string EffectData = "EffectData"; // string
	static const std::string PrefabPath = "PrefabPath"; // string
	static const std::string DropPrePath = "DropPrePath"; // string
	static const std::string BuyPrice = "BuyPrice"; // int
	static const std::string SalePrice = "SalePrice"; // int
	static const std::string Icon = "Icon"; // string
	// Record

};

class Guild
{
public:
	//Class name
	const std::string ThisName = "Guild";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Name = "Name"; // string
	static const std::string PresidentID = "PresidentID"; // object
	static const std::string PresidentName = "PresidentName"; // string
	static const std::string Rank = "Rank"; // int
	static const std::string GuildAD = "GuildAD"; // string
	static const std::string GuildDesc = "GuildDesc"; // string
	static const std::string GuildMoney = "GuildMoney"; // int
	static const std::string GuildLevel = "GuildLevel"; // int
	static const std::string GuildContinueDay = "GuildContinueDay"; // int
	static const std::string GuildID = "GuildID"; // object
	static const std::string GuildIcon = "GuildIcon"; // int
	static const std::string GuildMemeberCount = "GuildMemeberCount"; // int
	static const std::string GuildMemeberMaxCount = "GuildMemeberMaxCount"; // int
	static const std::string GuildHonor = "GuildHonor"; // int
	static const std::string GuildCreateTime = "GuildCreateTime"; // int
	static const std::string GuildCreateter = "GuildCreateter"; // int
	static const std::string GuildExp = "GuildExp"; // int
	static const std::string GuildStatus = "GuildStatus"; // int
	static const std::string DismissTime = "DismissTime"; // int
	static const std::string RecruitAD = "RecruitAD"; // string
	static const std::string RecruitLevel = "RecruitLevel"; // int
	static const std::string KingWarResource = "KingWarResource"; // int
	static const std::string AutoRecruit = "AutoRecruit"; // string
	static const std::string EctypServer = "EctypServer"; // int
	static const std::string EctypID = "EctypID"; // int
	static const std::string EctypIDGroup = "EctypIDGroup"; // int
	// Record

	class GuildBoss
	{
	public:
		static const std::string ThisName = "GuildBoss";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Offline		= 6; // Offline -- int
		static const int Power		= 7; // Power -- int

	};

	class GuildMemberList
	{
	public:
		static const std::string ThisName = "GuildMemberList";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int Receive		= 5; // Receive -- int
		static const int VIP		= 6; // VIP -- int
		static const int Online		= 7; // Online -- int
		static const int Power		= 8; // Power -- int
		static const int Title		= 9; // Title -- int
		static const int GameID		= 10; // GameID -- int
		static const int JoinTime		= 11; // JoinTime -- int
		static const int Contribution		= 12; // Contribution -- int
		static const int AllContribution		= 13; // AllContribution -- int

	};

	class GuildAppyList
	{
	public:
		static const std::string ThisName = "GuildAppyList";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Power		= 6; // Power -- int

	};

	class GuildEvent
	{
	public:
		static const std::string ThisName = "GuildEvent";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Offline		= 6; // Offline -- int
		static const int Power		= 7; // Power -- int
		static const int EventID		= 8; // EventID -- int
		static const int EventTime		= 9; // EventTime -- int
		static const int Context		= 10; // Context -- string

	};

	class GuildHouse
	{
	public:
		static const std::string ThisName = "GuildHouse";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Offline		= 6; // Offline -- int
		static const int Power		= 7; // Power -- int

	};

	class GuildSkill
	{
	public:
		static const std::string ThisName = "GuildSkill";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Offline		= 6; // Offline -- int
		static const int Power		= 7; // Power -- int

	};

};

class GuildConfig
{
public:
	//Class name
	const std::string ThisName = "GuildConfig";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ConditionPlayerLevel = "ConditionPlayerLevel"; // int
	static const std::string ConditionPlayerVIP = "ConditionPlayerVIP"; // int
	static const std::string CostMoney = "CostMoney"; // int
	static const std::string DismissTime = "DismissTime"; // int
	static const std::string GuildLevel = "GuildLevel"; // int
	static const std::string MaxMember = "MaxMember"; // int
	// Record

};

class GuildJob
{
public:
	//Class name
	const std::string ThisName = "GuildJob";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Job = "Job"; // int
	static const std::string JobCount = "JobCount"; // object
	static const std::string Appoint = "Appoint"; // object
	static const std::string Fire = "Fire"; // object
	static const std::string Demise = "Demise"; // object
	static const std::string ApplyDismiss = "ApplyDismiss"; // object
	static const std::string StopDismiss = "StopDismiss"; // object
	static const std::string AcceptApply = "AcceptApply"; // object
	static const std::string DenyApply = "DenyApply"; // object
	static const std::string Kickout = "Kickout"; // object
	static const std::string SetRecruit = "SetRecruit"; // object
	static const std::string PublishRecruit = "PublishRecruit"; // object
	static const std::string EditAD = "EditAD"; // object
	static const std::string Leave = "Leave"; // object
	static const std::string LevelUp = "LevelUp"; // object
	// Record

};

class HttpServer
{
public:
	//Class name
	const std::string ThisName = "HttpServer";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ServerID = "ServerID"; // int
	static const std::string WebPort = "WebPort"; // int
	static const std::string WebRootPath = "WebRootPath"; // string
	// Record

};

class InitProperty
{
public:
	//Class name
	const std::string ThisName = "InitProperty";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Job = "Job"; // int
	static const std::string Level = "Level"; // int
	static const std::string EffectData = "EffectData"; // string
	static const std::string SkillIDRef = "SkillIDRef"; // string
	static const std::string ModelPtah = "ModelPtah"; // string
	// Record

};

class Item
{
public:
	//Class name
	const std::string ThisName = "Item";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ItemType = "ItemType"; // int
	static const std::string ItemSubType = "ItemSubType"; // int
	static const std::string Level = "Level"; // int
	static const std::string Job = "Job"; // string
	static const std::string Quality = "Quality"; // int
	static const std::string DescID = "DescID"; // string
	static const std::string EffectData = "EffectData"; // string
	static const std::string ConsumeData = "ConsumeData"; // string
	static const std::string AwardData = "AwardData"; // string
	static const std::string AwardProperty = "AwardProperty"; // int
	static const std::string CoolDownTime = "CoolDownTime"; // float
	static const std::string OverlayCount = "OverlayCount"; // int
	static const std::string ExpiredType = "ExpiredType"; // int
	static const std::string BuyPrice = "BuyPrice"; // int
	static const std::string SalePrice = "SalePrice"; // int
	static const std::string Script = "Script"; // string
	static const std::string Extend = "Extend"; // string
	static const std::string SpriteFile = "SpriteFile"; // string
	static const std::string Icon = "Icon"; // string
	static const std::string ShowName = "ShowName"; // string
	static const std::string HeroTye = "HeroTye"; // int
	// Record

};

class Language
{
public:
	//Class name
	const std::string ThisName = "Language";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string English = "English"; // string
	static const std::string Chinese = "Chinese"; // string
	// Record

};

class Map
{
public:
	//Class name
	const std::string ThisName = "Map";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string MaxCount = "MaxCount"; // int
	static const std::string InComeGold = "InComeGold"; // int
	static const std::string InComeDiamond = "InComeDiamond"; // int
	static const std::string InComeOil = "InComeOil"; // int
	static const std::string MapLevel = "MapLevel"; // int
	// Record

	class Station
	{
	public:
		static const std::string ThisName = "Station";
		static const int GUID		= 0; // GUID -- object
		static const int GuildID		= 1; // GuildID -- object
		static const int GuildName		= 2; // GuildName -- string
		static const int Level		= 3; // Level -- int
		static const int Title		= 4; // Title -- string
		static const int Slogan		= 5; // Slogan -- string
		static const int State		= 6; // State -- int
		static const int CurMemberCount		= 7; // CurMemberCount -- int
		static const int WinCount		= 8; // WinCount -- int

	};

};

class NoSqlServer
{
public:
	//Class name
	const std::string ThisName = "NoSqlServer";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ServerID = "ServerID"; // int
	static const std::string IP = "IP"; // string
	static const std::string Port = "Port"; // int
	static const std::string Auth = "Auth"; // string
	// Record

};

class NPC
{
public:
	//Class name
	const std::string ThisName = "NPC";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string SeedID = "SeedID"; // string
	static const std::string VIPEXP = "VIPEXP"; // int
	static const std::string EXP = "EXP"; // int
	static const std::string HP = "HP"; // int
	static const std::string SP = "SP"; // int
	static const std::string MP = "MP"; // int
	static const std::string Gold = "Gold"; // int
	static const std::string Money = "Money"; // int
	static const std::string TargetX = "TargetX"; // float
	static const std::string TargetY = "TargetY"; // float
	static const std::string Prefab = "Prefab"; // string
	static const std::string Climb = "Climb"; // int
	static const std::string AtkDis = "AtkDis"; // float
	static const std::string DropPackList = "DropPackList"; // string
	static const std::string Idle = "Idle"; // string
	static const std::string SkillNormal = "SkillNormal"; // string
	static const std::string SkillAttack = "SkillAttack"; // string
	static const std::string SkillTHUMP = "SkillTHUMP"; // string
	static const std::string Height = "Height"; // float
	static const std::string EffectData = "EffectData"; // string
	static const std::string ConsumeData = "ConsumeData"; // string
	static const std::string LastAttacker = "LastAttacker"; // object
	static const std::string ShowName = "ShowName"; // string
	static const std::string EquipIDRef = "EquipIDRef"; // string
	static const std::string SpriteFile = "SpriteFile"; // string
	static const std::string Icon = "Icon"; // string
	static const std::string ShowCard = "ShowCard"; // string
	static const std::string HeroType = "HeroType"; // int
	static const std::string Camp = "Camp"; // int
	static const std::string MasterID = "MasterID"; // object
	static const std::string NPCType = "NPCType"; // int
	static const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	static const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	static const std::string CRITICAL = "CRITICAL"; // int
	static const std::string MAXHP = "MAXHP"; // int
	static const std::string MAXMP = "MAXMP"; // int
	static const std::string MAXSP = "MAXSP"; // int
	static const std::string HPREGEN = "HPREGEN"; // int
	static const std::string SPREGEN = "SPREGEN"; // int
	static const std::string MPREGEN = "MPREGEN"; // int
	static const std::string ATK_VALUE = "ATK_VALUE"; // int
	static const std::string DEF_VALUE = "DEF_VALUE"; // int
	static const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	static const std::string ATK_SPEED = "ATK_SPEED"; // int
	static const std::string ATK_FIRE = "ATK_FIRE"; // int
	static const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	static const std::string ATK_WIND = "ATK_WIND"; // int
	static const std::string ATK_ICE = "ATK_ICE"; // int
	static const std::string ATK_POISON = "ATK_POISON"; // int
	static const std::string DEF_FIRE = "DEF_FIRE"; // int
	static const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	static const std::string DEF_WIND = "DEF_WIND"; // int
	static const std::string DEF_ICE = "DEF_ICE"; // int
	static const std::string DEF_POISON = "DEF_POISON"; // int
	static const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	static const std::string MOVE_GATE = "MOVE_GATE"; // int
	static const std::string SKILL_GATE = "SKILL_GATE"; // int
	static const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	static const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	static const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

};

class Player
{
public:
	//Class name
	const std::string ThisName = "Player";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Name = "Name"; // string
	static const std::string Sex = "Sex"; // int
	static const std::string Job = "Job"; // int
	static const std::string Race = "Race"; // int
	static const std::string Camp = "Camp"; // int
	static const std::string Level = "Level"; // int
	static const std::string ShowName = "ShowName"; // string
	static const std::string PrefabPath = "PrefabPath"; // string
	static const std::string FirstTarget = "FirstTarget"; // object
	static const std::string FightHero = "FightHero"; // object
	static const std::string HeroPos1 = "HeroPos1"; // object
	static const std::string HeroPos2 = "HeroPos2"; // object
	static const std::string HeroPos3 = "HeroPos3"; // object
	static const std::string Skill1 = "Skill1"; // string
	static const std::string Skill2 = "Skill2"; // string
	static const std::string Skill3 = "Skill3"; // string
	static const std::string VIPLevel = "VIPLevel"; // int
	static const std::string VIPEXP = "VIPEXP"; // int
	static const std::string EXP = "EXP"; // int
	static const std::string HP = "HP"; // int
	static const std::string SP = "SP"; // int
	static const std::string MP = "MP"; // int
	static const std::string Gold = "Gold"; // int
	static const std::string Money = "Money"; // int
	static const std::string Account = "Account"; // string
	static const std::string ConnectKey = "ConnectKey"; // string
	static const std::string MAXEXP = "MAXEXP"; // int
	static const std::string RELIVE_SOUL = "RELIVE_SOUL"; // int
	static const std::string ATK_PVP = "ATK_PVP"; // int
	static const std::string DEF_PVP = "DEF_PVP"; // int
	static const std::string OnlineCount = "OnlineCount"; // int
	static const std::string TotalTime = "TotalTime"; // int
	static const std::string LastOfflineTime = "LastOfflineTime"; // object
	static const std::string OnlineTime = "OnlineTime"; // object
	static const std::string TotalLineTime = "TotalLineTime"; // object
	static const std::string GMLevel = "GMLevel"; // int
	static const std::string GameID = "GameID"; // int
	static const std::string GateID = "GateID"; // int
	static const std::string GuildID = "GuildID"; // object
	static const std::string TeamID = "TeamID"; // object
	static const std::string HomeSceneID = "HomeSceneID"; // int
	static const std::string ViewOppnent = "ViewOppnent"; // object
	static const std::string FightOppnent = "FightOppnent"; // object
	static const std::string GambleGold = "GambleGold"; // int
	static const std::string GambleDiamond = "GambleDiamond"; // int
	static const std::string SUCKBLOOD = "SUCKBLOOD"; // int
	static const std::string REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	static const std::string CRITICAL = "CRITICAL"; // int
	static const std::string MAXHP = "MAXHP"; // int
	static const std::string MAXMP = "MAXMP"; // int
	static const std::string MAXSP = "MAXSP"; // int
	static const std::string HPREGEN = "HPREGEN"; // int
	static const std::string SPREGEN = "SPREGEN"; // int
	static const std::string MPREGEN = "MPREGEN"; // int
	static const std::string ATK_VALUE = "ATK_VALUE"; // int
	static const std::string DEF_VALUE = "DEF_VALUE"; // int
	static const std::string MOVE_SPEED = "MOVE_SPEED"; // int
	static const std::string ATK_SPEED = "ATK_SPEED"; // int
	static const std::string ATK_FIRE = "ATK_FIRE"; // int
	static const std::string ATK_LIGHT = "ATK_LIGHT"; // int
	static const std::string ATK_WIND = "ATK_WIND"; // int
	static const std::string ATK_ICE = "ATK_ICE"; // int
	static const std::string ATK_POISON = "ATK_POISON"; // int
	static const std::string DEF_FIRE = "DEF_FIRE"; // int
	static const std::string DEF_LIGHT = "DEF_LIGHT"; // int
	static const std::string DEF_WIND = "DEF_WIND"; // int
	static const std::string DEF_ICE = "DEF_ICE"; // int
	static const std::string DEF_POISON = "DEF_POISON"; // int
	static const std::string DIZZY_GATE = "DIZZY_GATE"; // int
	static const std::string MOVE_GATE = "MOVE_GATE"; // int
	static const std::string SKILL_GATE = "SKILL_GATE"; // int
	static const std::string PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	static const std::string MAGIC_GATE = "MAGIC_GATE"; // int
	static const std::string BUFF_GATE = "BUFF_GATE"; // int
	// Record

	class CommValue
	{
	public:
		static const std::string ThisName = "CommValue";
		static const int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		static const int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		static const int CRITICAL		= 2; // CRITICAL -- int
		static const int MAXHP		= 3; // MAXHP -- int
		static const int MAXMP		= 4; // MAXMP -- int
		static const int MAXSP		= 5; // MAXSP -- int
		static const int HPREGEN		= 6; // HPREGEN -- int
		static const int SPREGEN		= 7; // SPREGEN -- int
		static const int MPREGEN		= 8; // MPREGEN -- int
		static const int ATK_VALUE		= 9; // ATK_VALUE -- int
		static const int DEF_VALUE		= 10; // DEF_VALUE -- int
		static const int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		static const int ATK_SPEED		= 12; // ATK_SPEED -- int
		static const int ATK_FIRE		= 13; // ATK_FIRE -- int
		static const int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		static const int ATK_WIND		= 15; // ATK_WIND -- int
		static const int ATK_ICE		= 16; // ATK_ICE -- int
		static const int ATK_POISON		= 17; // ATK_POISON -- int
		static const int DEF_FIRE		= 18; // DEF_FIRE -- int
		static const int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		static const int DEF_WIND		= 20; // DEF_WIND -- int
		static const int DEF_ICE		= 21; // DEF_ICE -- int
		static const int DEF_POISON		= 22; // DEF_POISON -- int
		static const int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		static const int MOVE_GATE		= 24; // MOVE_GATE -- int
		static const int SKILL_GATE		= 25; // SKILL_GATE -- int
		static const int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		static const int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		static const int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	class PlayerHero
	{
	public:
		static const std::string ThisName = "PlayerHero";
		static const int GUID		= 0; // GUID -- object
		static const int ConfigID		= 1; // ConfigID -- string
		static const int Activated		= 2; // Activated -- int
		static const int Level		= 3; // Level -- int
		static const int Exp		= 4; // Exp -- int
		static const int Star		= 5; // Star -- int
		static const int Equip1		= 6; // Equip1 -- object
		static const int Equip2		= 7; // Equip2 -- object
		static const int Equip3		= 8; // Equip3 -- object
		static const int Equip4		= 9; // Equip4 -- object
		static const int Equip5		= 10; // Equip5 -- object
		static const int Equip6		= 11; // Equip6 -- object
		static const int Talent1		= 12; // Talent1 -- string
		static const int Talent2		= 13; // Talent2 -- string
		static const int Talent3		= 14; // Talent3 -- string
		static const int Talent4		= 15; // Talent4 -- string
		static const int Talent5		= 16; // Talent5 -- string
		static const int Skill1		= 17; // Skill1 -- string
		static const int Skill2		= 18; // Skill2 -- string
		static const int Skill3		= 19; // Skill3 -- string
		static const int Skill4		= 20; // Skill4 -- string
		static const int Skill5		= 21; // Skill5 -- string
		static const int FightSkill		= 22; // FightSkill -- string

	};

	class HeroPropertyValue
	{
	public:
		static const std::string ThisName = "HeroPropertyValue";
		static const int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		static const int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		static const int CRITICAL		= 2; // CRITICAL -- int
		static const int MAXHP		= 3; // MAXHP -- int
		static const int MAXMP		= 4; // MAXMP -- int
		static const int MAXSP		= 5; // MAXSP -- int
		static const int HPREGEN		= 6; // HPREGEN -- int
		static const int SPREGEN		= 7; // SPREGEN -- int
		static const int MPREGEN		= 8; // MPREGEN -- int
		static const int ATK_VALUE		= 9; // ATK_VALUE -- int
		static const int DEF_VALUE		= 10; // DEF_VALUE -- int
		static const int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		static const int ATK_SPEED		= 12; // ATK_SPEED -- int
		static const int ATK_FIRE		= 13; // ATK_FIRE -- int
		static const int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		static const int ATK_WIND		= 15; // ATK_WIND -- int
		static const int ATK_ICE		= 16; // ATK_ICE -- int
		static const int ATK_POISON		= 17; // ATK_POISON -- int
		static const int DEF_FIRE		= 18; // DEF_FIRE -- int
		static const int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		static const int DEF_WIND		= 20; // DEF_WIND -- int
		static const int DEF_ICE		= 21; // DEF_ICE -- int
		static const int DEF_POISON		= 22; // DEF_POISON -- int
		static const int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		static const int MOVE_GATE		= 24; // MOVE_GATE -- int
		static const int SKILL_GATE		= 25; // SKILL_GATE -- int
		static const int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		static const int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		static const int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	class BagEquipList
	{
	public:
		static const std::string ThisName = "BagEquipList";
		static const int GUID		= 0; // GUID -- object
		static const int WearGUID		= 1; // WearGUID -- object
		static const int ConfigID		= 2; // ConfigID -- string
		static const int ExpiredType		= 3; // ExpiredType -- int
		static const int Date		= 4; // Date -- int
		static const int RandPropertyID		= 5; // RandPropertyID -- string
		static const int SlotCount		= 6; // SlotCount -- int
		static const int InlayStone1		= 7; // InlayStone1 -- string
		static const int InlayStone2		= 8; // InlayStone2 -- string
		static const int InlayStone3		= 9; // InlayStone3 -- string
		static const int InlayStone4		= 10; // InlayStone4 -- string
		static const int InlayStone5		= 11; // InlayStone5 -- string
		static const int InlayStone6		= 12; // InlayStone6 -- string
		static const int InlayStone7		= 13; // InlayStone7 -- string
		static const int InlayStone8		= 14; // InlayStone8 -- string
		static const int InlayStone9		= 15; // InlayStone9 -- string
		static const int InlayStone10		= 16; // InlayStone10 -- string
		static const int IntensifyLevel		= 17; // IntensifyLevel -- string
		static const int ElementLevel1_FIRE		= 18; // ElementLevel1_FIRE -- int
		static const int ElementLevel2_LIGHT		= 19; // ElementLevel2_LIGHT -- int
		static const int ElementLevel3_Wind		= 20; // ElementLevel3_Wind -- int
		static const int ElementLevel4_ICE		= 21; // ElementLevel4_ICE -- int
		static const int ElementLevel5_POISON		= 22; // ElementLevel5_POISON -- int

	};

	class BagItemList
	{
	public:
		static const std::string ThisName = "BagItemList";
		static const int ConfigID		= 0; // ConfigID -- string
		static const int ItemCount		= 1; // ItemCount -- int
		static const int Bound		= 2; // Bound -- int
		static const int ExpiredType		= 3; // ExpiredType -- int
		static const int Date		= 4; // Date -- int

	};

	class TaskMonsterList
	{
	public:
		static const std::string ThisName = "TaskMonsterList";
		static const int MonsterID		= 0; // MonsterID -- string
		static const int CurrentKillCount		= 1; // CurrentKillCount -- int
		static const int RequireKillCount		= 2; // RequireKillCount -- int
		static const int TaskID		= 3; // TaskID -- string

	};

	class TaskList
	{
	public:
		static const std::string ThisName = "TaskList";
		static const int TaskID		= 0; // TaskID -- string
		static const int TaskStatus		= 1; // TaskStatus -- int
		static const int Process		= 2; // Process -- int

	};

	class BuildingList
	{
	public:
		static const std::string ThisName = "BuildingList";
		static const int BuildingID		= 0; // BuildingID -- string
		static const int BuildingGUID		= 1; // BuildingGUID -- object
		static const int State		= 2; // State -- int
		static const int PosX		= 3; // PosX -- float
		static const int Pos		= 4; // Pos -- vector3
		static const int StateEndTime		= 5; // StateEndTime -- int

	};

	class BuildingListProduce
	{
	public:
		static const std::string ThisName = "BuildingListProduce";
		static const int BuildingID		= 0; // BuildingID -- string
		static const int BuildingGUID		= 1; // BuildingGUID -- object
		static const int State		= 2; // State -- int
		static const int PosX		= 3; // PosX -- float
		static const int PosY		= 4; // PosY -- float
		static const int StateEndTime		= 5; // StateEndTime -- int

	};

};

class Scene
{
public:
	//Class name
	const std::string ThisName = "Scene";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string SceneName = "SceneName"; // string
	static const std::string SceneShowName = "SceneShowName"; // string
	static const std::string MaxGroup = "MaxGroup"; // int
	static const std::string MaxGroupPlayers = "MaxGroupPlayers"; // int
	static const std::string FilePath = "FilePath"; // string
	static const std::string RelivePos = "RelivePos"; // string
	static const std::string Width = "Width"; // int
	static const std::string SoundList = "SoundList"; // string
	static const std::string Tile = "Tile"; // int
	static const std::string Share = "Share"; // int
	static const std::string CanClone = "CanClone"; // int
	static const std::string ActorID = "ActorID"; // int
	static const std::string LoadingUI = "LoadingUI"; // string
	static const std::string CamOffestPos = "CamOffestPos"; // string
	static const std::string CamOffestRot = "CamOffestRot"; // string
	static const std::string SyncObject = "SyncObject"; // int
	static const std::string NavigationResPath = "NavigationResPath"; // string
	// Record

};

class Security
{
public:
	//Class name
	const std::string ThisName = "Security";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string SecurityData = "SecurityData"; // string
	// Record

};

class Server
{
public:
	//Class name
	const std::string ThisName = "Server";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ServerID = "ServerID"; // int
	static const std::string Name = "Name"; // string
	static const std::string MaxOnline = "MaxOnline"; // int
	static const std::string CpuCount = "CpuCount"; // int
	static const std::string IP = "IP"; // string
	static const std::string Port = "Port"; // int
	static const std::string Type = "Type"; // int
	static const std::string Area = "Area"; // int
	// Record

};

class Shop
{
public:
	//Class name
	const std::string ThisName = "Shop";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Type = "Type"; // int
	static const std::string ItemID = "ItemID"; // string
	static const std::string Gold = "Gold"; // int
	static const std::string Steel = "Steel"; // int
	static const std::string Stone = "Stone"; // int
	static const std::string Diamond = "Diamond"; // int
	static const std::string Level = "Level"; // int
	static const std::string Count = "Count"; // int
	// Record

};

class Skill
{
public:
	//Class name
	const std::string ThisName = "Skill";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string SkillType = "SkillType"; // int
	static const std::string AnimaState = "AnimaState"; // string
	static const std::string ShowName = "ShowName"; // string
	static const std::string Desc = "Desc"; // string
	static const std::string ConsumeProperty = "ConsumeProperty"; // string
	static const std::string ConsumeValue = "ConsumeValue"; // string
	static const std::string ConsumeType = "ConsumeType"; // int
	static const std::string DamageProperty = "DamageProperty"; // string
	static const std::string DamageValue = "DamageValue"; // string
	static const std::string DamageType = "DamageType"; // int
	static const std::string GetBuffList = "GetBuffList"; // string
	static const std::string SendBuffList = "SendBuffList"; // string
	static const std::string CoolDownTime = "CoolDownTime"; // float
	static const std::string RequireDistance = "RequireDistance"; // float
	static const std::string DamageDistance = "DamageDistance"; // float
	static const std::string TargetType = "TargetType"; // int
	static const std::string NewObject = "NewObject"; // string
	static const std::string Icon = "Icon"; // string
	static const std::string SpriteFile = "SpriteFile"; // string
	static const std::string PlayerSkill = "PlayerSkill"; // int
	static const std::string AtkDis = "AtkDis"; // float
	static const std::string DefaultHitTime = "DefaultHitTime"; // float
	static const std::string NextID = "NextID"; // string
	// Record

};

class SqlServer
{
public:
	//Class name
	const std::string ThisName = "SqlServer";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ServerID = "ServerID"; // int
	static const std::string IP = "IP"; // string
	static const std::string Port = "Port"; // int
	static const std::string SqlIP = "SqlIP"; // string
	static const std::string SqlPort = "SqlPort"; // int
	static const std::string SqlUser = "SqlUser"; // string
	static const std::string SqlPwd = "SqlPwd"; // string
	static const std::string SqlName = "SqlName"; // string
	// Record

};

class Talent
{
public:
	//Class name
	const std::string ThisName = "Talent";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string ShowName = "ShowName"; // string
	static const std::string Desc = "Desc"; // string
	static const std::string EffectData = "EffectData"; // string
	static const std::string Icon = "Icon"; // string
	static const std::string SpriteFile = "SpriteFile"; // string
	static const std::string NextID = "NextID"; // string
	// Record

};

class Task
{
public:
	//Class name
	const std::string ThisName = "Task";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Type = "Type"; // int
	static const std::string NextTaskID = "NextTaskID"; // string
	static const std::string KillMonsterName = "KillMonsterName"; // string
	static const std::string KillCount = "KillCount"; // string
	static const std::string LevelReq = "LevelReq"; // string
	static const std::string AwardExp = "AwardExp"; // int
	static const std::string AwardGold = "AwardGold"; // int
	static const std::string AwardPack = "AwardPack"; // string
	static const std::string Desc = "Desc"; // string
	// Record

};

class Team
{
public:
	//Class name
	const std::string ThisName = "Team";
	// IObject
	static const std::string ID = "ID"; // string
	static const std::string ClassName = "ClassName"; // string
	static const std::string SceneID = "SceneID"; // int
	static const std::string GroupID = "GroupID"; // int
	static const std::string ConfigID = "ConfigID"; // string
	static const std::string Position = "Position"; // vector3
	// Property
	static const std::string Captain = "Captain"; // object
	static const std::string PresidentName = "PresidentName"; // string
	// Record

	class MemberList
	{
	public:
		static const std::string ThisName = "MemberList";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int Receive		= 5; // Receive -- int
		static const int VIP		= 6; // VIP -- int
		static const int Online		= 7; // Online -- int
		static const int Title		= 8; // Title -- int
		static const int GameID		= 9; // GameID -- int

	};

	class ApplyList
	{
	public:
		static const std::string ThisName = "ApplyList";
		static const int GUID		= 0; // GUID -- object
		static const int Name		= 1; // Name -- string
		static const int Level		= 2; // Level -- int
		static const int Job		= 3; // Job -- int
		static const int Donation		= 4; // Donation -- int
		static const int VIP		= 5; // VIP -- int
		static const int Power		= 6; // Power -- int

	};

};

} // !@NFrame

#endif // !NF_PR_NAME_HPP