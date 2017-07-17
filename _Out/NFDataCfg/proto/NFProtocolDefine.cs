// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.cs
//    @Author           :    NFrame Studio
//    @Module           :    NFProtocolDefine
// -------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
namespace NFrame
{
public class IObject
{
	//Class name
	public static readonly string ThisName = "IObject";
	// IObject
	// Property
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Record

}

public class Buff
{
	//Class name
	public static readonly string ThisName = "Buff";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String EffectType = "EffectType"; // int
	public static readonly String EffectValueType = "EffectValueType"; // int
	public static readonly String EffectValueReferType = "EffectValueReferType"; // int
	public static readonly String EffectTimeValue = "EffectTimeValue"; // int
	public static readonly String EffectTimeInterval = "EffectTimeInterval"; // float
	public static readonly String WashGroupID = "WashGroupID"; // int
	public static readonly String ReverseReferType = "ReverseReferType"; // int
	public static readonly String EffectClearOnDead = "EffectClearOnDead"; // int
	public static readonly String DownSaveType = "DownSaveType"; // int
	// Record

}

public class ConsumeData
{
	//Class name
	public static readonly string ThisName = "ConsumeData";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String VIPEXP = "VIPEXP"; // int
	public static readonly String EXP = "EXP"; // int
	public static readonly String HP = "HP"; // int
	public static readonly String SP = "SP"; // int
	public static readonly String MP = "MP"; // int
	public static readonly String Gold = "Gold"; // int
	public static readonly String Money = "Money"; // int
	// Record

}

public class Cost
{
	//Class name
	public static readonly string ThisName = "Cost";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String CostMoney = "CostMoney"; // int
	public static readonly String CostDiamond = "CostDiamond"; // int
	public static readonly String CostVP = "CostVP"; // int
	public static readonly String CostHonour = "CostHonour"; // int
	// Record

}

public class DescData
{
	//Class name
	public static readonly string ThisName = "DescData";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String Atlas = "Atlas"; // string
	public static readonly String PrefabPath = "PrefabPath"; // string
	public static readonly String PerformanceEffect = "PerformanceEffect"; // string
	public static readonly String PerformanceSound = "PerformanceSound"; // string
	public static readonly String DescText = "DescText"; // string
	// Record

}

public class EffectData
{
	//Class name
	public static readonly string ThisName = "EffectData";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String SUCKBLOOD = "SUCKBLOOD"; // int
	public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static readonly String CRITICAL = "CRITICAL"; // int
	public static readonly String MAXHP = "MAXHP"; // int
	public static readonly String MAXMP = "MAXMP"; // int
	public static readonly String MAXSP = "MAXSP"; // int
	public static readonly String HP = "HP"; // int
	public static readonly String MP = "MP"; // int
	public static readonly String SP = "SP"; // int
	public static readonly String HPREGEN = "HPREGEN"; // int
	public static readonly String SPREGEN = "SPREGEN"; // int
	public static readonly String MPREGEN = "MPREGEN"; // int
	public static readonly String ATK_VALUE = "ATK_VALUE"; // int
	public static readonly String DEF_VALUE = "DEF_VALUE"; // int
	public static readonly String MOVE_SPEED = "MOVE_SPEED"; // int
	public static readonly String ATK_SPEED = "ATK_SPEED"; // int
	public static readonly String ATK_FIRE = "ATK_FIRE"; // int
	public static readonly String ATK_LIGHT = "ATK_LIGHT"; // int
	public static readonly String ATK_WIND = "ATK_WIND"; // int
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
	public static readonly String DEF_WIND = "DEF_WIND"; // int
	public static readonly String DEF_ICE = "DEF_ICE"; // int
	public static readonly String DEF_POISON = "DEF_POISON"; // int
	public static readonly String DIZZY_GATE = "DIZZY_GATE"; // int
	public static readonly String MOVE_GATE = "MOVE_GATE"; // int
	public static readonly String SKILL_GATE = "SKILL_GATE"; // int
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static readonly String MAGIC_GATE = "MAGIC_GATE"; // int
	public static readonly String BUFF_GATE = "BUFF_GATE"; // int
	// Record

}

public class Equip
{
	//Class name
	public static readonly string ThisName = "Equip";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Sex = "Sex"; // int
	public static readonly String IntensiveBuffList = "IntensiveBuffList"; // string
	public static readonly String EnchantmentBuffList = "EnchantmentBuffList"; // string
	public static readonly String SuitID = "SuitID"; // int
	public static readonly String SuitBuffID = "SuitBuffID"; // string
	public static readonly String ItemType = "ItemType"; // int
	public static readonly String ItemSubType = "ItemSubType"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String Job = "Job"; // string
	public static readonly String Quality = "Quality"; // int
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Desc = "Desc"; // string
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String PrefabPath = "PrefabPath"; // string
	public static readonly String DropPrePath = "DropPrePath"; // string
	public static readonly String BuyPrice = "BuyPrice"; // int
	public static readonly String SalePrice = "SalePrice"; // int
	public static readonly String Icon = "Icon"; // string
	// Record

}

public class Guild
{
	//Class name
	public static readonly string ThisName = "Guild";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Name = "Name"; // string
	public static readonly String PresidentID = "PresidentID"; // object
	public static readonly String PresidentName = "PresidentName"; // string
	public static readonly String Rank = "Rank"; // int
	public static readonly String GuildAD = "GuildAD"; // string
	public static readonly String GuildDesc = "GuildDesc"; // string
	public static readonly String GuildMoney = "GuildMoney"; // int
	public static readonly String GuildLevel = "GuildLevel"; // int
	public static readonly String GuildContinueDay = "GuildContinueDay"; // int
	public static readonly String GuildID = "GuildID"; // object
	public static readonly String GuildIcon = "GuildIcon"; // int
	public static readonly String GuildMemeberCount = "GuildMemeberCount"; // int
	public static readonly String GuildMemeberMaxCount = "GuildMemeberMaxCount"; // int
	public static readonly String GuildHonor = "GuildHonor"; // int
	public static readonly String GuildCreateTime = "GuildCreateTime"; // int
	public static readonly String GuildCreateter = "GuildCreateter"; // int
	public static readonly String GuildExp = "GuildExp"; // int
	public static readonly String GuildStatus = "GuildStatus"; // int
	public static readonly String DismissTime = "DismissTime"; // int
	public static readonly String RecruitAD = "RecruitAD"; // string
	public static readonly String RecruitLevel = "RecruitLevel"; // int
	public static readonly String KingWarResource = "KingWarResource"; // int
	public static readonly String AutoRecruit = "AutoRecruit"; // string
	public static readonly String EctypServer = "EctypServer"; // int
	public static readonly String EctypID = "EctypID"; // int
	public static readonly String EctypIDGroup = "EctypIDGroup"; // int
	// Record

	public class GuildBoss
	{
		public static readonly String ThisName = "GuildBoss";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Offline		= 6; // Offline -- int
		public static readonly int Power		= 7; // Power -- int

	};

	public class GuildMemberList
	{
		public static readonly String ThisName = "GuildMemberList";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int Receive		= 5; // Receive -- int
		public static readonly int VIP		= 6; // VIP -- int
		public static readonly int Online		= 7; // Online -- int
		public static readonly int Power		= 8; // Power -- int
		public static readonly int Title		= 9; // Title -- int
		public static readonly int GameID		= 10; // GameID -- int
		public static readonly int JoinTime		= 11; // JoinTime -- int
		public static readonly int Contribution		= 12; // Contribution -- int
		public static readonly int AllContribution		= 13; // AllContribution -- int

	};

	public class GuildAppyList
	{
		public static readonly String ThisName = "GuildAppyList";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Power		= 6; // Power -- int

	};

	public class GuildEvent
	{
		public static readonly String ThisName = "GuildEvent";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Offline		= 6; // Offline -- int
		public static readonly int Power		= 7; // Power -- int
		public static readonly int EventID		= 8; // EventID -- int
		public static readonly int EventTime		= 9; // EventTime -- int
		public static readonly int Context		= 10; // Context -- string

	};

	public class GuildHouse
	{
		public static readonly String ThisName = "GuildHouse";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Offline		= 6; // Offline -- int
		public static readonly int Power		= 7; // Power -- int

	};

	public class GuildSkill
	{
		public static readonly String ThisName = "GuildSkill";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Offline		= 6; // Offline -- int
		public static readonly int Power		= 7; // Power -- int

	};

}

public class GuildConfig
{
	//Class name
	public static readonly string ThisName = "GuildConfig";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ConditionPlayerLevel = "ConditionPlayerLevel"; // int
	public static readonly String ConditionPlayerVIP = "ConditionPlayerVIP"; // int
	public static readonly String CostMoney = "CostMoney"; // int
	public static readonly String DismissTime = "DismissTime"; // int
	public static readonly String GuildLevel = "GuildLevel"; // int
	public static readonly String MaxMember = "MaxMember"; // int
	// Record

}

public class GuildJob
{
	//Class name
	public static readonly string ThisName = "GuildJob";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Job = "Job"; // int
	public static readonly String JobCount = "JobCount"; // object
	public static readonly String Appoint = "Appoint"; // object
	public static readonly String Fire = "Fire"; // object
	public static readonly String Demise = "Demise"; // object
	public static readonly String ApplyDismiss = "ApplyDismiss"; // object
	public static readonly String StopDismiss = "StopDismiss"; // object
	public static readonly String AcceptApply = "AcceptApply"; // object
	public static readonly String DenyApply = "DenyApply"; // object
	public static readonly String Kickout = "Kickout"; // object
	public static readonly String SetRecruit = "SetRecruit"; // object
	public static readonly String PublishRecruit = "PublishRecruit"; // object
	public static readonly String EditAD = "EditAD"; // object
	public static readonly String Leave = "Leave"; // object
	public static readonly String LevelUp = "LevelUp"; // object
	// Record

}

public class HttpServer
{
	//Class name
	public static readonly string ThisName = "HttpServer";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ServerID = "ServerID"; // int
	public static readonly String WebPort = "WebPort"; // int
	public static readonly String WebRootPath = "WebRootPath"; // string
	// Record

}

public class InitProperty
{
	//Class name
	public static readonly string ThisName = "InitProperty";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Job = "Job"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String SkillIDRef = "SkillIDRef"; // string
	public static readonly String ModelPtah = "ModelPtah"; // string
	// Record

}

public class Item
{
	//Class name
	public static readonly string ThisName = "Item";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ItemType = "ItemType"; // int
	public static readonly String ItemSubType = "ItemSubType"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String Job = "Job"; // string
	public static readonly String Quality = "Quality"; // int
	public static readonly String DescID = "DescID"; // string
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String ConsumeData = "ConsumeData"; // string
	public static readonly String AwardData = "AwardData"; // string
	public static readonly String AwardProperty = "AwardProperty"; // int
	public static readonly String CoolDownTime = "CoolDownTime"; // float
	public static readonly String OverlayCount = "OverlayCount"; // int
	public static readonly String ExpiredType = "ExpiredType"; // int
	public static readonly String BuyPrice = "BuyPrice"; // int
	public static readonly String SalePrice = "SalePrice"; // int
	public static readonly String Script = "Script"; // string
	public static readonly String Extend = "Extend"; // string
	public static readonly String SpriteFile = "SpriteFile"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String HeroTye = "HeroTye"; // int
	// Record

}

public class Language
{
	//Class name
	public static readonly string ThisName = "Language";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String English = "English"; // string
	public static readonly String Chinese = "Chinese"; // string
	// Record

}

public class Map
{
	//Class name
	public static readonly string ThisName = "Map";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String MaxCount = "MaxCount"; // int
	public static readonly String InComeGold = "InComeGold"; // int
	public static readonly String InComeDiamond = "InComeDiamond"; // int
	public static readonly String InComeOil = "InComeOil"; // int
	public static readonly String MapLevel = "MapLevel"; // int
	// Record

	public class Station
	{
		public static readonly String ThisName = "Station";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int GuildID		= 1; // GuildID -- object
		public static readonly int GuildName		= 2; // GuildName -- string
		public static readonly int Level		= 3; // Level -- int
		public static readonly int Title		= 4; // Title -- string
		public static readonly int Slogan		= 5; // Slogan -- string
		public static readonly int State		= 6; // State -- int
		public static readonly int CurMemberCount		= 7; // CurMemberCount -- int
		public static readonly int WinCount		= 8; // WinCount -- int

	};

}

public class NoSqlServer
{
	//Class name
	public static readonly string ThisName = "NoSqlServer";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ServerID = "ServerID"; // int
	public static readonly String IP = "IP"; // string
	public static readonly String Port = "Port"; // int
	public static readonly String Auth = "Auth"; // string
	// Record

}

public class NPC
{
	//Class name
	public static readonly string ThisName = "NPC";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String SeedID = "SeedID"; // string
	public static readonly String VIPEXP = "VIPEXP"; // int
	public static readonly String EXP = "EXP"; // int
	public static readonly String HP = "HP"; // int
	public static readonly String SP = "SP"; // int
	public static readonly String MP = "MP"; // int
	public static readonly String Gold = "Gold"; // int
	public static readonly String Money = "Money"; // int
	public static readonly String TargetX = "TargetX"; // float
	public static readonly String TargetY = "TargetY"; // float
	public static readonly String Prefab = "Prefab"; // string
	public static readonly String Climb = "Climb"; // int
	public static readonly String AtkDis = "AtkDis"; // float
	public static readonly String DropPackList = "DropPackList"; // string
	public static readonly String Idle = "Idle"; // string
	public static readonly String SkillNormal = "SkillNormal"; // string
	public static readonly String SkillAttack = "SkillAttack"; // string
	public static readonly String SkillTHUMP = "SkillTHUMP"; // string
	public static readonly String Height = "Height"; // float
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String ConsumeData = "ConsumeData"; // string
	public static readonly String LastAttacker = "LastAttacker"; // object
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String EquipIDRef = "EquipIDRef"; // string
	public static readonly String SpriteFile = "SpriteFile"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String ShowCard = "ShowCard"; // string
	public static readonly String HeroType = "HeroType"; // int
	public static readonly String Camp = "Camp"; // int
	public static readonly String MasterID = "MasterID"; // object
	public static readonly String NPCType = "NPCType"; // int
	public static readonly String SUCKBLOOD = "SUCKBLOOD"; // int
	public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static readonly String CRITICAL = "CRITICAL"; // int
	public static readonly String MAXHP = "MAXHP"; // int
	public static readonly String MAXMP = "MAXMP"; // int
	public static readonly String MAXSP = "MAXSP"; // int
	public static readonly String HPREGEN = "HPREGEN"; // int
	public static readonly String SPREGEN = "SPREGEN"; // int
	public static readonly String MPREGEN = "MPREGEN"; // int
	public static readonly String ATK_VALUE = "ATK_VALUE"; // int
	public static readonly String DEF_VALUE = "DEF_VALUE"; // int
	public static readonly String MOVE_SPEED = "MOVE_SPEED"; // int
	public static readonly String ATK_SPEED = "ATK_SPEED"; // int
	public static readonly String ATK_FIRE = "ATK_FIRE"; // int
	public static readonly String ATK_LIGHT = "ATK_LIGHT"; // int
	public static readonly String ATK_WIND = "ATK_WIND"; // int
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
	public static readonly String DEF_WIND = "DEF_WIND"; // int
	public static readonly String DEF_ICE = "DEF_ICE"; // int
	public static readonly String DEF_POISON = "DEF_POISON"; // int
	public static readonly String DIZZY_GATE = "DIZZY_GATE"; // int
	public static readonly String MOVE_GATE = "MOVE_GATE"; // int
	public static readonly String SKILL_GATE = "SKILL_GATE"; // int
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static readonly String MAGIC_GATE = "MAGIC_GATE"; // int
	public static readonly String BUFF_GATE = "BUFF_GATE"; // int
	// Record

}

public class Player
{
	//Class name
	public static readonly string ThisName = "Player";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Name = "Name"; // string
	public static readonly String Sex = "Sex"; // int
	public static readonly String Job = "Job"; // int
	public static readonly String Race = "Race"; // int
	public static readonly String Camp = "Camp"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String PrefabPath = "PrefabPath"; // string
	public static readonly String FirstTarget = "FirstTarget"; // object
	public static readonly String FightHero = "FightHero"; // object
	public static readonly String HeroPos1 = "HeroPos1"; // object
	public static readonly String HeroPos2 = "HeroPos2"; // object
	public static readonly String HeroPos3 = "HeroPos3"; // object
	public static readonly String Skill1 = "Skill1"; // string
	public static readonly String Skill2 = "Skill2"; // string
	public static readonly String Skill3 = "Skill3"; // string
	public static readonly String VIPLevel = "VIPLevel"; // int
	public static readonly String VIPEXP = "VIPEXP"; // int
	public static readonly String EXP = "EXP"; // int
	public static readonly String HP = "HP"; // int
	public static readonly String SP = "SP"; // int
	public static readonly String MP = "MP"; // int
	public static readonly String Gold = "Gold"; // int
	public static readonly String Money = "Money"; // int
	public static readonly String Account = "Account"; // string
	public static readonly String ConnectKey = "ConnectKey"; // string
	public static readonly String MAXEXP = "MAXEXP"; // int
	public static readonly String RELIVE_SOUL = "RELIVE_SOUL"; // int
	public static readonly String ATK_PVP = "ATK_PVP"; // int
	public static readonly String DEF_PVP = "DEF_PVP"; // int
	public static readonly String OnlineCount = "OnlineCount"; // int
	public static readonly String TotalTime = "TotalTime"; // int
	public static readonly String LastOfflineTime = "LastOfflineTime"; // object
	public static readonly String OnlineTime = "OnlineTime"; // object
	public static readonly String TotalLineTime = "TotalLineTime"; // object
	public static readonly String GMLevel = "GMLevel"; // int
	public static readonly String GameID = "GameID"; // int
	public static readonly String GateID = "GateID"; // int
	public static readonly String GuildID = "GuildID"; // object
	public static readonly String TeamID = "TeamID"; // object
	public static readonly String HomeSceneID = "HomeSceneID"; // int
	public static readonly String ViewOppnent = "ViewOppnent"; // object
	public static readonly String FightOppnent = "FightOppnent"; // object
	public static readonly String GambleGold = "GambleGold"; // int
	public static readonly String GambleDiamond = "GambleDiamond"; // int
	public static readonly String SUCKBLOOD = "SUCKBLOOD"; // int
	public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static readonly String CRITICAL = "CRITICAL"; // int
	public static readonly String MAXHP = "MAXHP"; // int
	public static readonly String MAXMP = "MAXMP"; // int
	public static readonly String MAXSP = "MAXSP"; // int
	public static readonly String HPREGEN = "HPREGEN"; // int
	public static readonly String SPREGEN = "SPREGEN"; // int
	public static readonly String MPREGEN = "MPREGEN"; // int
	public static readonly String ATK_VALUE = "ATK_VALUE"; // int
	public static readonly String DEF_VALUE = "DEF_VALUE"; // int
	public static readonly String MOVE_SPEED = "MOVE_SPEED"; // int
	public static readonly String ATK_SPEED = "ATK_SPEED"; // int
	public static readonly String ATK_FIRE = "ATK_FIRE"; // int
	public static readonly String ATK_LIGHT = "ATK_LIGHT"; // int
	public static readonly String ATK_WIND = "ATK_WIND"; // int
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
	public static readonly String DEF_WIND = "DEF_WIND"; // int
	public static readonly String DEF_ICE = "DEF_ICE"; // int
	public static readonly String DEF_POISON = "DEF_POISON"; // int
	public static readonly String DIZZY_GATE = "DIZZY_GATE"; // int
	public static readonly String MOVE_GATE = "MOVE_GATE"; // int
	public static readonly String SKILL_GATE = "SKILL_GATE"; // int
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static readonly String MAGIC_GATE = "MAGIC_GATE"; // int
	public static readonly String BUFF_GATE = "BUFF_GATE"; // int
	// Record

	public class CommValue
	{
		public static readonly String ThisName = "CommValue";
		public static readonly int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		public static readonly int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		public static readonly int CRITICAL		= 2; // CRITICAL -- int
		public static readonly int MAXHP		= 3; // MAXHP -- int
		public static readonly int MAXMP		= 4; // MAXMP -- int
		public static readonly int MAXSP		= 5; // MAXSP -- int
		public static readonly int HPREGEN		= 6; // HPREGEN -- int
		public static readonly int SPREGEN		= 7; // SPREGEN -- int
		public static readonly int MPREGEN		= 8; // MPREGEN -- int
		public static readonly int ATK_VALUE		= 9; // ATK_VALUE -- int
		public static readonly int DEF_VALUE		= 10; // DEF_VALUE -- int
		public static readonly int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		public static readonly int ATK_SPEED		= 12; // ATK_SPEED -- int
		public static readonly int ATK_FIRE		= 13; // ATK_FIRE -- int
		public static readonly int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		public static readonly int ATK_WIND		= 15; // ATK_WIND -- int
		public static readonly int ATK_ICE		= 16; // ATK_ICE -- int
		public static readonly int ATK_POISON		= 17; // ATK_POISON -- int
		public static readonly int DEF_FIRE		= 18; // DEF_FIRE -- int
		public static readonly int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		public static readonly int DEF_WIND		= 20; // DEF_WIND -- int
		public static readonly int DEF_ICE		= 21; // DEF_ICE -- int
		public static readonly int DEF_POISON		= 22; // DEF_POISON -- int
		public static readonly int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		public static readonly int MOVE_GATE		= 24; // MOVE_GATE -- int
		public static readonly int SKILL_GATE		= 25; // SKILL_GATE -- int
		public static readonly int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		public static readonly int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		public static readonly int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	public class PlayerHero
	{
		public static readonly String ThisName = "PlayerHero";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int ConfigID		= 1; // ConfigID -- string
		public static readonly int Activated		= 2; // Activated -- int
		public static readonly int Level		= 3; // Level -- int
		public static readonly int Exp		= 4; // Exp -- int
		public static readonly int Star		= 5; // Star -- int
		public static readonly int Equip1		= 6; // Equip1 -- object
		public static readonly int Equip2		= 7; // Equip2 -- object
		public static readonly int Equip3		= 8; // Equip3 -- object
		public static readonly int Equip4		= 9; // Equip4 -- object
		public static readonly int Equip5		= 10; // Equip5 -- object
		public static readonly int Equip6		= 11; // Equip6 -- object
		public static readonly int Talent1		= 12; // Talent1 -- string
		public static readonly int Talent2		= 13; // Talent2 -- string
		public static readonly int Talent3		= 14; // Talent3 -- string
		public static readonly int Talent4		= 15; // Talent4 -- string
		public static readonly int Talent5		= 16; // Talent5 -- string
		public static readonly int Skill1		= 17; // Skill1 -- string
		public static readonly int Skill2		= 18; // Skill2 -- string
		public static readonly int Skill3		= 19; // Skill3 -- string
		public static readonly int Skill4		= 20; // Skill4 -- string
		public static readonly int Skill5		= 21; // Skill5 -- string
		public static readonly int FightSkill		= 22; // FightSkill -- string

	};

	public class HeroPropertyValue
	{
		public static readonly String ThisName = "HeroPropertyValue";
		public static readonly int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		public static readonly int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		public static readonly int CRITICAL		= 2; // CRITICAL -- int
		public static readonly int MAXHP		= 3; // MAXHP -- int
		public static readonly int MAXMP		= 4; // MAXMP -- int
		public static readonly int MAXSP		= 5; // MAXSP -- int
		public static readonly int HPREGEN		= 6; // HPREGEN -- int
		public static readonly int SPREGEN		= 7; // SPREGEN -- int
		public static readonly int MPREGEN		= 8; // MPREGEN -- int
		public static readonly int ATK_VALUE		= 9; // ATK_VALUE -- int
		public static readonly int DEF_VALUE		= 10; // DEF_VALUE -- int
		public static readonly int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		public static readonly int ATK_SPEED		= 12; // ATK_SPEED -- int
		public static readonly int ATK_FIRE		= 13; // ATK_FIRE -- int
		public static readonly int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		public static readonly int ATK_WIND		= 15; // ATK_WIND -- int
		public static readonly int ATK_ICE		= 16; // ATK_ICE -- int
		public static readonly int ATK_POISON		= 17; // ATK_POISON -- int
		public static readonly int DEF_FIRE		= 18; // DEF_FIRE -- int
		public static readonly int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		public static readonly int DEF_WIND		= 20; // DEF_WIND -- int
		public static readonly int DEF_ICE		= 21; // DEF_ICE -- int
		public static readonly int DEF_POISON		= 22; // DEF_POISON -- int
		public static readonly int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		public static readonly int MOVE_GATE		= 24; // MOVE_GATE -- int
		public static readonly int SKILL_GATE		= 25; // SKILL_GATE -- int
		public static readonly int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		public static readonly int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		public static readonly int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	public class BagEquipList
	{
		public static readonly String ThisName = "BagEquipList";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int WearGUID		= 1; // WearGUID -- object
		public static readonly int ConfigID		= 2; // ConfigID -- string
		public static readonly int ExpiredType		= 3; // ExpiredType -- int
		public static readonly int Date		= 4; // Date -- int
		public static readonly int RandPropertyID		= 5; // RandPropertyID -- string
		public static readonly int SlotCount		= 6; // SlotCount -- int
		public static readonly int InlayStone1		= 7; // InlayStone1 -- string
		public static readonly int InlayStone2		= 8; // InlayStone2 -- string
		public static readonly int InlayStone3		= 9; // InlayStone3 -- string
		public static readonly int InlayStone4		= 10; // InlayStone4 -- string
		public static readonly int InlayStone5		= 11; // InlayStone5 -- string
		public static readonly int InlayStone6		= 12; // InlayStone6 -- string
		public static readonly int InlayStone7		= 13; // InlayStone7 -- string
		public static readonly int InlayStone8		= 14; // InlayStone8 -- string
		public static readonly int InlayStone9		= 15; // InlayStone9 -- string
		public static readonly int InlayStone10		= 16; // InlayStone10 -- string
		public static readonly int IntensifyLevel		= 17; // IntensifyLevel -- string
		public static readonly int ElementLevel1_FIRE		= 18; // ElementLevel1_FIRE -- int
		public static readonly int ElementLevel2_LIGHT		= 19; // ElementLevel2_LIGHT -- int
		public static readonly int ElementLevel3_Wind		= 20; // ElementLevel3_Wind -- int
		public static readonly int ElementLevel4_ICE		= 21; // ElementLevel4_ICE -- int
		public static readonly int ElementLevel5_POISON		= 22; // ElementLevel5_POISON -- int

	};

	public class BagItemList
	{
		public static readonly String ThisName = "BagItemList";
		public static readonly int ConfigID		= 0; // ConfigID -- string
		public static readonly int ItemCount		= 1; // ItemCount -- int
		public static readonly int Bound		= 2; // Bound -- int
		public static readonly int ExpiredType		= 3; // ExpiredType -- int
		public static readonly int Date		= 4; // Date -- int

	};

	public class TaskMonsterList
	{
		public static readonly String ThisName = "TaskMonsterList";
		public static readonly int MonsterID		= 0; // MonsterID -- string
		public static readonly int CurrentKillCount		= 1; // CurrentKillCount -- int
		public static readonly int RequireKillCount		= 2; // RequireKillCount -- int
		public static readonly int TaskID		= 3; // TaskID -- string

	};

	public class TaskList
	{
		public static readonly String ThisName = "TaskList";
		public static readonly int TaskID		= 0; // TaskID -- string
		public static readonly int TaskStatus		= 1; // TaskStatus -- int
		public static readonly int Process		= 2; // Process -- int

	};

	public class BuildingList
	{
		public static readonly String ThisName = "BuildingList";
		public static readonly int BuildingID		= 0; // BuildingID -- string
		public static readonly int BuildingGUID		= 1; // BuildingGUID -- object
		public static readonly int State		= 2; // State -- int
		public static readonly int PosX		= 3; // PosX -- float
		public static readonly int Pos		= 4; // Pos -- vector3
		public static readonly int StateEndTime		= 5; // StateEndTime -- int

	};

	public class BuildingListProduce
	{
		public static readonly String ThisName = "BuildingListProduce";
		public static readonly int BuildingID		= 0; // BuildingID -- string
		public static readonly int BuildingGUID		= 1; // BuildingGUID -- object
		public static readonly int State		= 2; // State -- int
		public static readonly int PosX		= 3; // PosX -- float
		public static readonly int PosY		= 4; // PosY -- float
		public static readonly int StateEndTime		= 5; // StateEndTime -- int

	};

}

public class Scene
{
	//Class name
	public static readonly string ThisName = "Scene";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String SceneName = "SceneName"; // string
	public static readonly String SceneShowName = "SceneShowName"; // string
	public static readonly String MaxGroup = "MaxGroup"; // int
	public static readonly String MaxGroupPlayers = "MaxGroupPlayers"; // int
	public static readonly String FilePath = "FilePath"; // string
	public static readonly String RelivePos = "RelivePos"; // string
	public static readonly String Width = "Width"; // int
	public static readonly String SoundList = "SoundList"; // string
	public static readonly String Tile = "Tile"; // int
	public static readonly String Share = "Share"; // int
	public static readonly String CanClone = "CanClone"; // int
	public static readonly String ActorID = "ActorID"; // int
	public static readonly String LoadingUI = "LoadingUI"; // string
	public static readonly String CamOffestPos = "CamOffestPos"; // string
	public static readonly String CamOffestRot = "CamOffestRot"; // string
	public static readonly String SyncObject = "SyncObject"; // int
	public static readonly String NavigationResPath = "NavigationResPath"; // string
	// Record

}

public class Security
{
	//Class name
	public static readonly string ThisName = "Security";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String SecurityData = "SecurityData"; // string
	// Record

}

public class Server
{
	//Class name
	public static readonly string ThisName = "Server";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ServerID = "ServerID"; // int
	public static readonly String Name = "Name"; // string
	public static readonly String MaxOnline = "MaxOnline"; // int
	public static readonly String CpuCount = "CpuCount"; // int
	public static readonly String IP = "IP"; // string
	public static readonly String Port = "Port"; // int
	public static readonly String Type = "Type"; // int
	public static readonly String Area = "Area"; // int
	// Record

}

public class Shop
{
	//Class name
	public static readonly string ThisName = "Shop";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Type = "Type"; // int
	public static readonly String ItemID = "ItemID"; // string
	public static readonly String Gold = "Gold"; // int
	public static readonly String Steel = "Steel"; // int
	public static readonly String Stone = "Stone"; // int
	public static readonly String Diamond = "Diamond"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String Count = "Count"; // int
	// Record

}

public class Skill
{
	//Class name
	public static readonly string ThisName = "Skill";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String SkillType = "SkillType"; // int
	public static readonly String AnimaState = "AnimaState"; // string
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Desc = "Desc"; // string
	public static readonly String ConsumeProperty = "ConsumeProperty"; // string
	public static readonly String ConsumeValue = "ConsumeValue"; // string
	public static readonly String ConsumeType = "ConsumeType"; // int
	public static readonly String DamageProperty = "DamageProperty"; // string
	public static readonly String DamageValue = "DamageValue"; // string
	public static readonly String DamageType = "DamageType"; // int
	public static readonly String GetBuffList = "GetBuffList"; // string
	public static readonly String SendBuffList = "SendBuffList"; // string
	public static readonly String CoolDownTime = "CoolDownTime"; // float
	public static readonly String RequireDistance = "RequireDistance"; // float
	public static readonly String DamageDistance = "DamageDistance"; // float
	public static readonly String TargetType = "TargetType"; // int
	public static readonly String NewObject = "NewObject"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String SpriteFile = "SpriteFile"; // string
	public static readonly String PlayerSkill = "PlayerSkill"; // int
	public static readonly String AtkDis = "AtkDis"; // float
	public static readonly String DefaultHitTime = "DefaultHitTime"; // float
	public static readonly String NextID = "NextID"; // string
	// Record

}

public class SqlServer
{
	//Class name
	public static readonly string ThisName = "SqlServer";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ServerID = "ServerID"; // int
	public static readonly String IP = "IP"; // string
	public static readonly String Port = "Port"; // int
	public static readonly String SqlIP = "SqlIP"; // string
	public static readonly String SqlPort = "SqlPort"; // int
	public static readonly String SqlUser = "SqlUser"; // string
	public static readonly String SqlPwd = "SqlPwd"; // string
	public static readonly String SqlName = "SqlName"; // string
	// Record

}

public class Talent
{
	//Class name
	public static readonly string ThisName = "Talent";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Desc = "Desc"; // string
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String SpriteFile = "SpriteFile"; // string
	public static readonly String NextID = "NextID"; // string
	// Record

}

public class Task
{
	//Class name
	public static readonly string ThisName = "Task";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Type = "Type"; // int
	public static readonly String NextTaskID = "NextTaskID"; // string
	public static readonly String KillMonsterName = "KillMonsterName"; // string
	public static readonly String KillCount = "KillCount"; // string
	public static readonly String LevelReq = "LevelReq"; // string
	public static readonly String AwardExp = "AwardExp"; // int
	public static readonly String AwardGold = "AwardGold"; // int
	public static readonly String AwardPack = "AwardPack"; // string
	public static readonly String Desc = "Desc"; // string
	// Record

}

public class Team
{
	//Class name
	public static readonly string ThisName = "Team";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	public static readonly String Position = "Position"; // vector3
	// Property
	public static readonly String Captain = "Captain"; // object
	public static readonly String PresidentName = "PresidentName"; // string
	// Record

	public class MemberList
	{
		public static readonly String ThisName = "MemberList";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int Receive		= 5; // Receive -- int
		public static readonly int VIP		= 6; // VIP -- int
		public static readonly int Online		= 7; // Online -- int
		public static readonly int Title		= 8; // Title -- int
		public static readonly int GameID		= 9; // GameID -- int

	};

	public class ApplyList
	{
		public static readonly String ThisName = "ApplyList";
		public static readonly int GUID		= 0; // GUID -- object
		public static readonly int Name		= 1; // Name -- string
		public static readonly int Level		= 2; // Level -- int
		public static readonly int Job		= 3; // Job -- int
		public static readonly int Donation		= 4; // Donation -- int
		public static readonly int VIP		= 5; // VIP -- int
		public static readonly int Power		= 6; // Power -- int

	};

}

}