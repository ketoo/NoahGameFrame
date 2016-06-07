// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.cs
//    @Author           :    NFrame Studio
//    @Date             :    Tue Jun  7 15:22:29 2016
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
	// Record

}

public class BB_Build
{
	//Class name
	public static readonly string ThisName = "BB_Build";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String Type = "Type"; // int
	public static readonly String SubType = "SubType"; // int
	public static readonly String Prefab = "Prefab"; // string
	public static readonly String NormalStateFunc = "NormalStateFunc"; // string
	public static readonly String UpStateFunc = "UpStateFunc"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Desc = "Desc"; // string
	// Record

}

public class Block
{
	//Class name
	public static readonly string ThisName = "Block";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String Tag = "Tag"; // int
	public static readonly String SpriteList = "SpriteList"; // string
	public static readonly String SpritePath = "SpritePath"; // string
	public static readonly String LeftSide = "LeftSide"; // string
	public static readonly String RightSide = "RightSide"; // string
	public static readonly String TopSide = "TopSide"; // string
	public static readonly String DownSide = "DownSide"; // string
	public static readonly String LeftTopSide = "LeftTopSide"; // string
	public static readonly String LeftDownSide = "LeftDownSide"; // string
	public static readonly String RightTopSide = "RightTopSide"; // string
	public static readonly String RightDownSide = "RightDownSide"; // string
	public static readonly String LeftTopOutSide = "LeftTopOutSide"; // string
	public static readonly String LeftDownOutSide = "LeftDownOutSide"; // string
	public static readonly String RightTopOutSide = "RightTopOutSide"; // string
	public static readonly String RightDownOutSide = "RightDownOutSide"; // string
	public static readonly String GrassList = "GrassList"; // string
	public static readonly String JoinList = "JoinList"; // string
	public static readonly String CrackList = "CrackList"; // string
	public static readonly String TreasureList = "TreasureList"; // string
	public static readonly String TreeRootList = "TreeRootList"; // string
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

public class ChatGroup
{
	//Class name
	public static readonly string ThisName = "ChatGroup";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String Name = "Name"; // string
	public static readonly String CreateObject = "CreateObject"; // object
	// Record
	public static readonly String R_GroupMemberList = "GroupMemberList";
	public static readonly String R_ChatList = "ChatList";

	public enum GroupMemberList
	{
		GUID		= 0, // GUID -- object
		Online		= 1, // Online -- int
		GameID		= 2, // GameID -- int

	};

	public enum ChatList
	{
		GUID		= 0, // GUID -- object
		msg		= 1, // msg -- string
		time		= 2, // time -- int

	};

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
	// Property
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
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
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

public class EquipRef
{
	//Class name
	public static readonly string ThisName = "EquipRef";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String EQUIP1 = "EQUIP1"; // string
	public static readonly String EQUIP2 = "EQUIP2"; // string
	public static readonly String EQUIP3 = "EQUIP3"; // string
	public static readonly String EQUIP4 = "EQUIP4"; // string
	public static readonly String EQUIP5 = "EQUIP5"; // string
	public static readonly String EQUIP6 = "EQUIP6"; // string
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
	public static readonly String GuilID = "GuilID"; // object
	public static readonly String GuilIDIcon = "GuilIDIcon"; // int
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
	public static readonly String R_GuildBoss = "GuildBoss";
	public static readonly String R_GuildMemberList = "GuildMemberList";
	public static readonly String R_GuildAppyList = "GuildAppyList";
	public static readonly String R_GuildEvent = "GuildEvent";
	public static readonly String R_GuildHouse = "GuildHouse";
	public static readonly String R_GuildSkill = "GuildSkill";

	public enum GuildBoss
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Offline		= 6, // Offline -- int
		Power		= 7, // Power -- int

	};

	public enum GuildMemberList
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		Receive		= 5, // Receive -- int
		VIP		= 6, // VIP -- int
		Online		= 7, // Online -- int
		Power		= 8, // Power -- int
		Title		= 9, // Title -- int
		GameID		= 10, // GameID -- int
		JoinTime		= 11, // JoinTime -- int
		Contribution		= 12, // Contribution -- int
		AllContribution		= 13, // AllContribution -- int

	};

	public enum GuildAppyList
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Power		= 6, // Power -- int

	};

	public enum GuildEvent
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Offline		= 6, // Offline -- int
		Power		= 7, // Power -- int
		EventID		= 8, // EventID -- int
		EventTime		= 9, // EventTime -- int
		Context		= 10, // Context -- string

	};

	public enum GuildHouse
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Offline		= 6, // Offline -- int
		Power		= 7, // Power -- int

	};

	public enum GuildSkill
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Offline		= 6, // Offline -- int
		Power		= 7, // Power -- int

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

public class GuildName
{
	//Class name
	public static readonly string ThisName = "GuildName";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String GuildID = "GuildID"; // object
	// Record

}

public class Hero
{
	//Class name
	public static readonly string ThisName = "Hero";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String SkillRef = "SkillRef"; // string
	public static readonly String TalentRef = "TalentRef"; // string
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
	public static readonly String AwardProperty = "AwardProperty"; // string
	public static readonly String CoolDownTime = "CoolDownTime"; // float
	public static readonly String OverlayCount = "OverlayCount"; // int
	public static readonly String ExpiredType = "ExpiredType"; // int
	public static readonly String BuyPrice = "BuyPrice"; // int
	public static readonly String SalePrice = "SalePrice"; // int
	public static readonly String Script = "Script"; // string
	public static readonly String Extend = "Extend"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String ShowName = "ShowName"; // string
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
	// Property
	public static readonly String MaxCount = "MaxCount"; // int
	public static readonly String InComeGold = "InComeGold"; // int
	public static readonly String InComeDiamond = "InComeDiamond"; // int
	public static readonly String InComeOil = "InComeOil"; // int
	public static readonly String X = "X"; // int
	public static readonly String Z = "Z"; // int
	public static readonly String MapLevel = "MapLevel"; // int
	// Record
	public static readonly String R_Station = "Station";

	public enum Station
	{
		GUID		= 0, // GUID -- object
		GuildID		= 1, // GuildID -- object
		GuildName		= 2, // GuildName -- string
		Level		= 3, // Level -- int
		Title		= 4, // Title -- string
		Slogan		= 5, // Slogan -- string
		State		= 6, // State -- int
		CurMemberCount		= 7, // CurMemberCount -- int

	};

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
	// Property
	public static readonly String SeedID = "SeedID"; // string
	public static readonly String VIPEXP = "VIPEXP"; // int
	public static readonly String EXP = "EXP"; // int
	public static readonly String HP = "HP"; // int
	public static readonly String SP = "SP"; // int
	public static readonly String MP = "MP"; // int
	public static readonly String Gold = "Gold"; // int
	public static readonly String Money = "Money"; // int
	public static readonly String X = "X"; // float
	public static readonly String Y = "Y"; // float
	public static readonly String Z = "Z"; // float
	public static readonly String TargetX = "TargetX"; // float
	public static readonly String TargetY = "TargetY"; // float
	public static readonly String TargetZ = "TargetZ"; // float
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
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
	public static readonly String DEF_ICE = "DEF_ICE"; // int
	public static readonly String DEF_POISON = "DEF_POISON"; // int
	public static readonly String DIZZY_GATE = "DIZZY_GATE"; // int
	public static readonly String MOVE_GATE = "MOVE_GATE"; // int
	public static readonly String SKILL_GATE = "SKILL_GATE"; // int
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static readonly String MAGIC_GATE = "MAGIC_GATE"; // int
	public static readonly String BUFF_GATE = "BUFF_GATE"; // int
	public static readonly String Prefab = "Prefab"; // string
	public static readonly String MoveType = "MoveType"; // int
	public static readonly String AtkDis = "AtkDis"; // float
	public static readonly String DropPackList = "DropPackList"; // string
	public static readonly String SkillIDRef = "SkillIDRef"; // string
	public static readonly String Height = "Height"; // float
	public static readonly String EffectData = "EffectData"; // string
	public static readonly String ConsumeData = "ConsumeData"; // string
	public static readonly String LastAttacker = "LastAttacker"; // object
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String EquipIDRef = "EquipIDRef"; // string
	public static readonly String Icon = "Icon"; // string
	public static readonly String ShowCard = "ShowCard"; // string
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
	// Property
	public static readonly String Name = "Name"; // string
	public static readonly String Sex = "Sex"; // int
	public static readonly String Race = "Race"; // int
	public static readonly String Camp = "Camp"; // int
	public static readonly String LastContainerID = "LastContainerID"; // int
	public static readonly String Level = "Level"; // int
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String PrefabPath = "PrefabPath"; // string
	public static readonly String FirstTarget = "FirstTarget"; // object
	public static readonly String CharType = "CharType"; // int
	public static readonly String Job = "Job"; // int
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
	public static readonly String ATK_ICE = "ATK_ICE"; // int
	public static readonly String ATK_POISON = "ATK_POISON"; // int
	public static readonly String DEF_FIRE = "DEF_FIRE"; // int
	public static readonly String DEF_LIGHT = "DEF_LIGHT"; // int
	public static readonly String DEF_ICE = "DEF_ICE"; // int
	public static readonly String DEF_POISON = "DEF_POISON"; // int
	public static readonly String DIZZY_GATE = "DIZZY_GATE"; // int
	public static readonly String MOVE_GATE = "MOVE_GATE"; // int
	public static readonly String SKILL_GATE = "SKILL_GATE"; // int
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static readonly String MAGIC_GATE = "MAGIC_GATE"; // int
	public static readonly String BUFF_GATE = "BUFF_GATE"; // int
	public static readonly String LoadPropertyFinish = "LoadPropertyFinish"; // int
	public static readonly String GameID = "GameID"; // int
	public static readonly String GateID = "GateID"; // int
	public static readonly String GuildID = "GuildID"; // object
	// Record
	public static readonly String R_PlayerHero = "PlayerHero";
	public static readonly String R_PlayerFightHero = "PlayerFightHero";
	public static readonly String R_BagEquipList = "BagEquipList";
	public static readonly String R_BagItemList = "BagItemList";
	public static readonly String R_HeroPropertyValue = "HeroPropertyValue";
	public static readonly String R_CommPropertyValue = "CommPropertyValue";
	public static readonly String R_TaskMonsterList = "TaskMonsterList";
	public static readonly String R_TaskList = "TaskList";
	public static readonly String R_BuildingList = "BuildingList";
	public static readonly String R_BuildingListProduce = "BuildingListProduce";

	public enum PlayerHero
	{
		GUID		= 0, // GUID -- object
		ConfigID		= 1, // ConfigID -- string
		Level		= 2, // Level -- int
		Exp		= 3, // Exp -- int
		Star		= 4, // Star -- int
		Equip1		= 5, // Equip1 -- object
		Equip2		= 6, // Equip2 -- object
		Equip3		= 7, // Equip3 -- object
		Equip4		= 8, // Equip4 -- object
		Equip5		= 9, // Equip5 -- object
		Equip6		= 10, // Equip6 -- object
		Equip7		= 11, // Equip7 -- object
		Equip8		= 12, // Equip8 -- object
		Talent1		= 13, // Talent1 -- string
		Talent2		= 14, // Talent2 -- string
		Talent3		= 15, // Talent3 -- string
		Talent4		= 16, // Talent4 -- string
		Talent5		= 17, // Talent5 -- string
		Talent6		= 18, // Talent6 -- string
		Talent7		= 19, // Talent7 -- string
		Talent8		= 20, // Talent8 -- string
		TalentLevel1		= 21, // TalentLevel1 -- int
		TalentLevel2		= 22, // TalentLevel2 -- int
		TalentLevel3		= 23, // TalentLevel3 -- int
		TalentLevel4		= 24, // TalentLevel4 -- int
		TalentLevel5		= 25, // TalentLevel5 -- int
		TalentLevel6		= 26, // TalentLevel6 -- int
		TalentLevel7		= 27, // TalentLevel7 -- int
		TalentLevel8		= 28, // TalentLevel8 -- int
		Skill1		= 29, // Skill1 -- string
		Skill2		= 30, // Skill2 -- string
		Skill3		= 31, // Skill3 -- string
		Skill4		= 32, // Skill4 -- string
		Skill5		= 33, // Skill5 -- string
		SkillLevel1		= 34, // SkillLevel1 -- int
		SkillLevel2		= 35, // SkillLevel2 -- int
		SkillLevel3		= 36, // SkillLevel3 -- int
		SkillLevel4		= 37, // SkillLevel4 -- int
		SkillLevel5		= 38, // SkillLevel5 -- int
		FightState		= 39, // FightState -- int
		FightSkill		= 40, // FightSkill -- string

	};

	public enum PlayerFightHero
	{
		GUID		= 0, // GUID -- object
		FightPos		= 1, // FightPos -- int

	};

	public enum BagEquipList
	{
		GUID		= 0, // GUID -- object
		WearGUID		= 1, // WearGUID -- object
		ConfigID		= 2, // ConfigID -- string
		Bound		= 3, // Bound -- int
		ExpiredType		= 4, // ExpiredType -- int
		RandPropertyID		= 5, // RandPropertyID -- string
		Date		= 6, // Date -- object
		SlotCount		= 7, // SlotCount -- int
		InlayStone1		= 8, // InlayStone1 -- string
		InlayStone2		= 9, // InlayStone2 -- string
		InlayStone3		= 10, // InlayStone3 -- string
		InlayStone4		= 11, // InlayStone4 -- string
		InlayStone5		= 12, // InlayStone5 -- string
		InlayStone6		= 13, // InlayStone6 -- string
		InlayStone7		= 14, // InlayStone7 -- string
		InlayStone8		= 15, // InlayStone8 -- string
		InlayStone9		= 16, // InlayStone9 -- string
		InlayStone10		= 17, // InlayStone10 -- string
		IntensifyLevel		= 18, // IntensifyLevel -- string
		ElementLevel1		= 19, // ElementLevel1 -- int
		ElementLevel2		= 20, // ElementLevel2 -- int
		ElementLevel3		= 21, // ElementLevel3 -- int
		ElementLevel4		= 22, // ElementLevel4 -- int
		ElementLevel5		= 23, // ElementLevel5 -- int
		ElementLevel6		= 24, // ElementLevel6 -- int
		ElementLevel7		= 25, // ElementLevel7 -- int
		ElementLevel8		= 26, // ElementLevel8 -- int
		ElementLevel9		= 27, // ElementLevel9 -- int
		ElementLevel10		= 28, // ElementLevel10 -- int

	};

	public enum BagItemList
	{
		ConfigID		= 0, // ConfigID -- string
		ItemCount		= 1, // ItemCount -- int
		Bound		= 2, // Bound -- int
		ExpiredType		= 3, // ExpiredType -- int
		Date		= 4, // Date -- object

	};

	public enum HeroPropertyValue
	{
		HeroGUID		= 0, // HeroGUID -- object
		Group		= 1, // Group -- int
		MAXHP		= 2, // MAXHP -- int
		MAXMP		= 3, // MAXMP -- int
		MAXSP		= 4, // MAXSP -- int
		HPREGEN		= 5, // HPREGEN -- int
		SPREGEN		= 6, // SPREGEN -- int
		MPREGEN		= 7, // MPREGEN -- int
		ATK_VALUE		= 8, // ATK_VALUE -- int
		DEF_VALUE		= 9, // DEF_VALUE -- int
		MOVE_SPEED		= 10, // MOVE_SPEED -- int
		ATK_SPEED		= 11, // ATK_SPEED -- int
		ATK_FIRE		= 12, // ATK_FIRE -- int
		ATK_LIGHT		= 13, // ATK_LIGHT -- int
		ATK_ICE		= 14, // ATK_ICE -- int
		ATK_POISON		= 15, // ATK_POISON -- int
		DEF_FIRE		= 16, // DEF_FIRE -- int
		DEF_LIGHT		= 17, // DEF_LIGHT -- int
		DEF_ICE		= 18, // DEF_ICE -- int
		DEF_POISON		= 19, // DEF_POISON -- int
		DIZZY_GATE		= 20, // DIZZY_GATE -- int
		MOVE_GATE		= 21, // MOVE_GATE -- int
		SKILL_GATE		= 22, // SKILL_GATE -- int
		PHYSICAL_GATE		= 23, // PHYSICAL_GATE -- int
		MAGIC_GATE		= 24, // MAGIC_GATE -- int

	};

	public enum CommPropertyValue
	{
		MAXHP		= 0, // MAXHP -- int
		MAXMP		= 1, // MAXMP -- int
		MAXSP		= 2, // MAXSP -- int
		HPREGEN		= 3, // HPREGEN -- int
		SPREGEN		= 4, // SPREGEN -- int
		MPREGEN		= 5, // MPREGEN -- int
		ATK_VALUE		= 6, // ATK_VALUE -- int
		DEF_VALUE		= 7, // DEF_VALUE -- int
		MOVE_SPEED		= 8, // MOVE_SPEED -- int
		ATK_SPEED		= 9, // ATK_SPEED -- int
		ATK_FIRE		= 10, // ATK_FIRE -- int
		ATK_LIGHT		= 11, // ATK_LIGHT -- int
		ATK_ICE		= 12, // ATK_ICE -- int
		ATK_POISON		= 13, // ATK_POISON -- int
		DEF_FIRE		= 14, // DEF_FIRE -- int
		DEF_LIGHT		= 15, // DEF_LIGHT -- int
		DEF_ICE		= 16, // DEF_ICE -- int
		DEF_POISON		= 17, // DEF_POISON -- int
		DIZZY_GATE		= 18, // DIZZY_GATE -- int
		MOVE_GATE		= 19, // MOVE_GATE -- int
		SKILL_GATE		= 20, // SKILL_GATE -- int
		PHYSICAL_GATE		= 21, // PHYSICAL_GATE -- int
		MAGIC_GATE		= 22, // MAGIC_GATE -- int
		BUFF_GATE		= 23, // BUFF_GATE -- int

	};

	public enum TaskMonsterList
	{
		MonsterID		= 0, // MonsterID -- string
		CurrentKillCount		= 1, // CurrentKillCount -- int
		RequireKillCount		= 2, // RequireKillCount -- int
		TaskID		= 3, // TaskID -- string

	};

	public enum TaskList
	{
		TaskID		= 0, // TaskID -- string
		TaskStatus		= 1, // TaskStatus -- int
		Process		= 2, // Process -- int

	};

	public enum BuildingList
	{
		BuildingID		= 0, // BuildingID -- string
		BuildingGUID		= 1, // BuildingGUID -- object
		State		= 2, // State -- int
		PosX		= 3, // PosX -- int
		PosY		= 4, // PosY -- int
		PosZ		= 5, // PosZ -- int
		StateStartTime		= 6, // StateStartTime -- int
		StateEndTime		= 7, // StateEndTime -- int

	};

	public enum BuildingListProduce
	{
		BuildingID		= 0, // BuildingID -- string
		BuildingGUID		= 1, // BuildingGUID -- object
		State		= 2, // State -- int
		PosX		= 3, // PosX -- int
		PosY		= 4, // PosY -- int
		PosZ		= 5, // PosZ -- int
		StateStartTime		= 6, // StateStartTime -- int
		StateEndTime		= 7, // StateEndTime -- int

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
	// Property
	public static readonly String SceneName = "SceneName"; // string
	public static readonly String SceneShowName = "SceneShowName"; // string
	public static readonly String MaxGroup = "MaxGroup"; // int
	public static readonly String MaxGroupPlayers = "MaxGroupPlayers"; // int
	public static readonly String FilePath = "FilePath"; // string
	public static readonly String RelivePos = "RelivePos"; // string
	public static readonly String Width = "Width"; // int
	public static readonly String SoundList = "SoundList"; // string
	public static readonly String Share = "Share"; // int
	public static readonly String CanClone = "CanClone"; // int
	public static readonly String ActorID = "ActorID"; // int
	public static readonly String LoadingUI = "LoadingUI"; // string
	public static readonly String CamOffestPos = "CamOffestPos"; // string
	public static readonly String CamOffestRot = "CamOffestRot"; // string
	public static readonly String SyncObject = "SyncObject"; // int
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
	// Property
	public static readonly String ServerID = "ServerID"; // int
	public static readonly String Name = "Name"; // string
	public static readonly String MaxOnline = "MaxOnline"; // int
	public static readonly String CpuCount = "CpuCount"; // int
	public static readonly String IP = "IP"; // string
	public static readonly String Port = "Port"; // int
	public static readonly String Type = "Type"; // int
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
	// Property
	public static readonly String Type = "Type"; // int
	public static readonly String ItemID = "ItemID"; // string
	public static readonly String Gold = "Gold"; // int
	public static readonly String Steel = "Steel"; // int
	public static readonly String Stone = "Stone"; // int
	public static readonly String Diamond = "Diamond"; // int
	public static readonly String Level = "Level"; // int
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
	// Property
	public static readonly String SkillType = "SkillType"; // int
	public static readonly String AnimaState = "AnimaState"; // int
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
	public static readonly String Atlas = "Atlas"; // string
	public static readonly String UpLevel = "UpLevel"; // int
	public static readonly String AfterUpSkillID = "AfterUpSkillID"; // string
	public static readonly String PlayerSkill = "PlayerSkill"; // int
	public static readonly String AtkDis = "AtkDis"; // float
	public static readonly String NeedTar = "NeedTar"; // int
	public static readonly String DefaultHitTime = "DefaultHitTime"; // float
	// Record

}

public class SkillRef
{
	//Class name
	public static readonly string ThisName = "SkillRef";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String NORMALATTACK1 = "NORMALATTACK1"; // string
	public static readonly String NORMALATTACK2 = "NORMALATTACK2"; // string
	public static readonly String NORMALATTACK3 = "NORMALATTACK3"; // string
	public static readonly String NORMALTHUMP = "NORMALTHUMP"; // string
	public static readonly String SKILL1 = "SKILL1"; // string
	public static readonly String SKILL2 = "SKILL2"; // string
	public static readonly String SKILL3 = "SKILL3"; // string
	public static readonly String SKILL4 = "SKILL4"; // string
	public static readonly String SKILL5 = "SKILL5"; // string
	public static readonly String SKILL6 = "SKILL6"; // string
	public static readonly String SKILL7 = "SKILL7"; // string
	public static readonly String SKILL8 = "SKILL8"; // string
	public static readonly String SKILL9 = "SKILL9"; // string
	public static readonly String SKILL10 = "SKILL10"; // string
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

public class StateFuncResources
{
	//Class name
	public static readonly string ThisName = "StateFuncResources";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String Atlas_ResID = "Atlas_ResID"; // string
	// Record

}

public class StateFunction
{
	//Class name
	public static readonly string ThisName = "StateFunction";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String EFT_INFO = "EFT_INFO"; // int
	public static readonly String EFT_BOOOST = "EFT_BOOOST"; // int
	public static readonly String EFT_LVLUP = "EFT_LVLUP"; // int
	public static readonly String EFT_CREATE_SOLDER = "EFT_CREATE_SOLDER"; // int
	public static readonly String EFT_CREATE_SPEEL = "EFT_CREATE_SPEEL"; // int
	public static readonly String EFT_RESEARCH = "EFT_RESEARCH"; // int
	public static readonly String EFT_COLLECT_GOLD = "EFT_COLLECT_GOLD"; // int
	public static readonly String EFT_COLLECT_STONE = "EFT_COLLECT_STONE"; // int
	public static readonly String EFT_COLLECT_STEEL = "EFT_COLLECT_STEEL"; // int
	public static readonly String EFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND"; // int
	public static readonly String EFT_SELL = "EFT_SELL"; // int
	public static readonly String EFT_REPAIR = "EFT_REPAIR"; // int
	public static readonly String EFT_CANCEL = "EFT_CANCEL"; // int
	public static readonly String EFT_FINISH = "EFT_FINISH"; // int
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
	// Property
	public static readonly String TalentType = "TalentType"; // int
	public static readonly String ShowName = "ShowName"; // string
	public static readonly String Desc = "Desc"; // string
	// Record

}

public class TalentRef
{
	//Class name
	public static readonly string ThisName = "TalentRef";
	// IObject
	public static readonly String ID = "ID"; // string
	public static readonly String ClassName = "ClassName"; // string
	public static readonly String SceneID = "SceneID"; // int
	public static readonly String GroupID = "GroupID"; // int
	public static readonly String ConfigID = "ConfigID"; // string
	// Property
	public static readonly String TALENT1 = "TALENT1"; // string
	public static readonly String TALENT2 = "TALENT2"; // string
	public static readonly String TALENT3 = "TALENT3"; // string
	public static readonly String TALENT4 = "TALENT4"; // string
	public static readonly String TALENT5 = "TALENT5"; // string
	public static readonly String TALENT6 = "TALENT6"; // string
	public static readonly String TALENT7 = "TALENT7"; // string
	public static readonly String TALENT8 = "TALENT8"; // string
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
	// Property
	public static readonly String Captain = "Captain"; // object
	public static readonly String PresidentName = "PresidentName"; // string
	// Record
	public static readonly String R_MemberList = "MemberList";
	public static readonly String R_ApplyList = "ApplyList";

	public enum MemberList
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		Receive		= 5, // Receive -- int
		VIP		= 6, // VIP -- int
		Online		= 7, // Online -- int
		Title		= 8, // Title -- int
		GameID		= 9, // GameID -- int

	};

	public enum ApplyList
	{
		GUID		= 0, // GUID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
		Job		= 3, // Job -- int
		Donation		= 4, // Donation -- int
		VIP		= 5, // VIP -- int
		Power		= 6, // Power -- int

	};

}

} 