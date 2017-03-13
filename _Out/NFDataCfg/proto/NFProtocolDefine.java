// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.java
//    @Author           :    NFrame Studio
//    @Module           :    NFProtocolDefine
// -------------------------------------------------------------------------

package nframe;
public class IObject {
	//Class name
	public static final String ThisName = "IObject";
	// IObject
	// Property
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Record

}

public class BB_Build {
	//Class name
	public static final String ThisName = "BB_Build";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Type = "Type"; // int
	public static final String SubType = "SubType"; // int
	public static final String Prefab = "Prefab"; // string
	public static final String NormalStateFunc = "NormalStateFunc"; // string
	public static final String UpStateFunc = "UpStateFunc"; // string
	public static final String Icon = "Icon"; // string
	public static final String ShowName = "ShowName"; // string
	public static final String Desc = "Desc"; // string
	// Record

}

public class Block {
	//Class name
	public static final String ThisName = "Block";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Tag = "Tag"; // int
	public static final String SpriteList = "SpriteList"; // string
	public static final String SpritePath = "SpritePath"; // string
	public static final String LeftSide = "LeftSide"; // string
	public static final String RightSide = "RightSide"; // string
	public static final String TopSide = "TopSide"; // string
	public static final String DownSide = "DownSide"; // string
	public static final String LeftTopSide = "LeftTopSide"; // string
	public static final String LeftDownSide = "LeftDownSide"; // string
	public static final String RightTopSide = "RightTopSide"; // string
	public static final String RightDownSide = "RightDownSide"; // string
	public static final String LeftTopOutSide = "LeftTopOutSide"; // string
	public static final String LeftDownOutSide = "LeftDownOutSide"; // string
	public static final String RightTopOutSide = "RightTopOutSide"; // string
	public static final String RightDownOutSide = "RightDownOutSide"; // string
	public static final String GrassList = "GrassList"; // string
	public static final String JoinList = "JoinList"; // string
	public static final String CrackList = "CrackList"; // string
	public static final String TreasureList = "TreasureList"; // string
	public static final String TreeRootList = "TreeRootList"; // string
	// Record

}

public class Buff {
	//Class name
	public static final String ThisName = "Buff";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String EffectType = "EffectType"; // int
	public static final String EffectValueType = "EffectValueType"; // int
	public static final String EffectValueReferType = "EffectValueReferType"; // int
	public static final String EffectTimeValue = "EffectTimeValue"; // int
	public static final String EffectTimeInterval = "EffectTimeInterval"; // float
	public static final String WashGroupID = "WashGroupID"; // int
	public static final String ReverseReferType = "ReverseReferType"; // int
	public static final String EffectClearOnDead = "EffectClearOnDead"; // int
	public static final String DownSaveType = "DownSaveType"; // int
	// Record

}

public class ChatGroup {
	//Class name
	public static final String ThisName = "ChatGroup";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Name = "Name"; // string
	public static final String CreateObject = "CreateObject"; // object
	// Record
	public static final String R_GroupMemberList = "GroupMemberList";
	public static final String R_ChatList = "ChatList";

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

public class ConsumeData {
	//Class name
	public static final String ThisName = "ConsumeData";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String VIPEXP = "VIPEXP"; // int
	public static final String EXP = "EXP"; // int
	public static final String HP = "HP"; // int
	public static final String SP = "SP"; // int
	public static final String MP = "MP"; // int
	public static final String Gold = "Gold"; // int
	public static final String Money = "Money"; // int
	// Record

}

public class Cost {
	//Class name
	public static final String ThisName = "Cost";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String CostMoney = "CostMoney"; // int
	public static final String CostDiamond = "CostDiamond"; // int
	public static final String CostVP = "CostVP"; // int
	public static final String CostHonour = "CostHonour"; // int
	// Record

}

public class DescData {
	//Class name
	public static final String ThisName = "DescData";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ShowName = "ShowName"; // string
	public static final String Icon = "Icon"; // string
	public static final String Atlas = "Atlas"; // string
	public static final String PrefabPath = "PrefabPath"; // string
	public static final String PerformanceEffect = "PerformanceEffect"; // string
	public static final String PerformanceSound = "PerformanceSound"; // string
	public static final String DescText = "DescText"; // string
	// Record

}

public class EffectData {
	//Class name
	public static final String ThisName = "EffectData";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String SUCKBLOOD = "SUCKBLOOD"; // int
	public static final String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static final String CRITICAL = "CRITICAL"; // int
	public static final String MAXHP = "MAXHP"; // int
	public static final String MAXMP = "MAXMP"; // int
	public static final String MAXSP = "MAXSP"; // int
	public static final String HP = "HP"; // int
	public static final String MP = "MP"; // int
	public static final String SP = "SP"; // int
	public static final String HPREGEN = "HPREGEN"; // int
	public static final String SPREGEN = "SPREGEN"; // int
	public static final String MPREGEN = "MPREGEN"; // int
	public static final String ATK_VALUE = "ATK_VALUE"; // int
	public static final String DEF_VALUE = "DEF_VALUE"; // int
	public static final String MOVE_SPEED = "MOVE_SPEED"; // int
	public static final String ATK_SPEED = "ATK_SPEED"; // int
	public static final String ATK_FIRE = "ATK_FIRE"; // int
	public static final String ATK_LIGHT = "ATK_LIGHT"; // int
	public static final String ATK_WIND = "ATK_WIND"; // int
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
	public static final String DEF_WIND = "DEF_WIND"; // int
	public static final String DEF_ICE = "DEF_ICE"; // int
	public static final String DEF_POISON = "DEF_POISON"; // int
	public static final String DIZZY_GATE = "DIZZY_GATE"; // int
	public static final String MOVE_GATE = "MOVE_GATE"; // int
	public static final String SKILL_GATE = "SKILL_GATE"; // int
	public static final String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static final String MAGIC_GATE = "MAGIC_GATE"; // int
	public static final String BUFF_GATE = "BUFF_GATE"; // int
	// Record

}

public class Equip {
	//Class name
	public static final String ThisName = "Equip";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Sex = "Sex"; // int
	public static final String IntensiveBuffList = "IntensiveBuffList"; // string
	public static final String EnchantmentBuffList = "EnchantmentBuffList"; // string
	public static final String SuitID = "SuitID"; // int
	public static final String SuitBuffID = "SuitBuffID"; // string
	public static final String ItemType = "ItemType"; // int
	public static final String ItemSubType = "ItemSubType"; // int
	public static final String Level = "Level"; // int
	public static final String Job = "Job"; // string
	public static final String Quality = "Quality"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String Desc = "Desc"; // string
	public static final String EffectData = "EffectData"; // string
	public static final String PrefabPath = "PrefabPath"; // string
	public static final String DropPrePath = "DropPrePath"; // string
	public static final String BuyPrice = "BuyPrice"; // int
	public static final String SalePrice = "SalePrice"; // int
	public static final String Icon = "Icon"; // string
	// Record

}

public class Guild {
	//Class name
	public static final String ThisName = "Guild";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Name = "Name"; // string
	public static final String PresidentID = "PresidentID"; // object
	public static final String PresidentName = "PresidentName"; // string
	public static final String Rank = "Rank"; // int
	public static final String GuildAD = "GuildAD"; // string
	public static final String GuildDesc = "GuildDesc"; // string
	public static final String GuildMoney = "GuildMoney"; // int
	public static final String GuildLevel = "GuildLevel"; // int
	public static final String GuildContinueDay = "GuildContinueDay"; // int
	public static final String GuildID = "GuildID"; // object
	public static final String GuildIcon = "GuildIcon"; // int
	public static final String GuildMemeberCount = "GuildMemeberCount"; // int
	public static final String GuildMemeberMaxCount = "GuildMemeberMaxCount"; // int
	public static final String GuildHonor = "GuildHonor"; // int
	public static final String GuildCreateTime = "GuildCreateTime"; // int
	public static final String GuildCreateter = "GuildCreateter"; // int
	public static final String GuildExp = "GuildExp"; // int
	public static final String GuildStatus = "GuildStatus"; // int
	public static final String DismissTime = "DismissTime"; // int
	public static final String RecruitAD = "RecruitAD"; // string
	public static final String RecruitLevel = "RecruitLevel"; // int
	public static final String KingWarResource = "KingWarResource"; // int
	public static final String AutoRecruit = "AutoRecruit"; // string
	public static final String EctypServer = "EctypServer"; // int
	public static final String EctypID = "EctypID"; // int
	public static final String EctypIDGroup = "EctypIDGroup"; // int
	// Record
	public static final String R_GuildBoss = "GuildBoss";
	public static final String R_GuildMemberList = "GuildMemberList";
	public static final String R_GuildAppyList = "GuildAppyList";
	public static final String R_GuildEvent = "GuildEvent";
	public static final String R_GuildHouse = "GuildHouse";
	public static final String R_GuildSkill = "GuildSkill";

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

public class GuildConfig {
	//Class name
	public static final String ThisName = "GuildConfig";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ConditionPlayerLevel = "ConditionPlayerLevel"; // int
	public static final String ConditionPlayerVIP = "ConditionPlayerVIP"; // int
	public static final String CostMoney = "CostMoney"; // int
	public static final String DismissTime = "DismissTime"; // int
	public static final String GuildLevel = "GuildLevel"; // int
	public static final String MaxMember = "MaxMember"; // int
	// Record

}

public class GuildJob {
	//Class name
	public static final String ThisName = "GuildJob";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Job = "Job"; // int
	public static final String JobCount = "JobCount"; // object
	public static final String Appoint = "Appoint"; // object
	public static final String Fire = "Fire"; // object
	public static final String Demise = "Demise"; // object
	public static final String ApplyDismiss = "ApplyDismiss"; // object
	public static final String StopDismiss = "StopDismiss"; // object
	public static final String AcceptApply = "AcceptApply"; // object
	public static final String DenyApply = "DenyApply"; // object
	public static final String Kickout = "Kickout"; // object
	public static final String SetRecruit = "SetRecruit"; // object
	public static final String PublishRecruit = "PublishRecruit"; // object
	public static final String EditAD = "EditAD"; // object
	public static final String Leave = "Leave"; // object
	public static final String LevelUp = "LevelUp"; // object
	// Record

}

public class GuildName {
	//Class name
	public static final String ThisName = "GuildName";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String GuildID = "GuildID"; // object
	// Record

}

public class HttpServer {
	//Class name
	public static final String ThisName = "HttpServer";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ServerID = "ServerID"; // int
	public static final String WebPort = "WebPort"; // int
	public static final String WebRootPath = "WebRootPath"; // string
	// Record

}

public class InitProperty {
	//Class name
	public static final String ThisName = "InitProperty";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Job = "Job"; // int
	public static final String Level = "Level"; // int
	public static final String EffectData = "EffectData"; // string
	public static final String SkillIDRef = "SkillIDRef"; // string
	public static final String ModelPtah = "ModelPtah"; // string
	// Record

}

public class Item {
	//Class name
	public static final String ThisName = "Item";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ItemType = "ItemType"; // int
	public static final String ItemSubType = "ItemSubType"; // int
	public static final String Level = "Level"; // int
	public static final String Job = "Job"; // string
	public static final String Quality = "Quality"; // int
	public static final String DescID = "DescID"; // string
	public static final String EffectData = "EffectData"; // string
	public static final String ConsumeData = "ConsumeData"; // string
	public static final String AwardData = "AwardData"; // string
	public static final String AwardProperty = "AwardProperty"; // int
	public static final String CoolDownTime = "CoolDownTime"; // float
	public static final String OverlayCount = "OverlayCount"; // int
	public static final String ExpiredType = "ExpiredType"; // int
	public static final String BuyPrice = "BuyPrice"; // int
	public static final String SalePrice = "SalePrice"; // int
	public static final String Script = "Script"; // string
	public static final String Extend = "Extend"; // string
	public static final String SpriteFile = "SpriteFile"; // string
	public static final String Icon = "Icon"; // string
	public static final String ShowName = "ShowName"; // string
	public static final String HeroTye = "HeroTye"; // int
	// Record

}

public class Language {
	//Class name
	public static final String ThisName = "Language";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String English = "English"; // string
	public static final String Chinese = "Chinese"; // string
	// Record

}

public class Map {
	//Class name
	public static final String ThisName = "Map";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String MaxCount = "MaxCount"; // int
	public static final String InComeGold = "InComeGold"; // int
	public static final String InComeDiamond = "InComeDiamond"; // int
	public static final String InComeOil = "InComeOil"; // int
	public static final String MapLevel = "MapLevel"; // int
	// Record
	public static final String R_Station = "Station";

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
		WinCount		= 8, // WinCount -- int

	};

}

public class NoSqlServer {
	//Class name
	public static final String ThisName = "NoSqlServer";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ServerID = "ServerID"; // int
	public static final String IP = "IP"; // string
	public static final String Port = "Port"; // int
	public static final String Auth = "Auth"; // string
	// Record

}

public class NPC {
	//Class name
	public static final String ThisName = "NPC";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String SeedID = "SeedID"; // string
	public static final String VIPEXP = "VIPEXP"; // int
	public static final String EXP = "EXP"; // int
	public static final String HP = "HP"; // int
	public static final String SP = "SP"; // int
	public static final String MP = "MP"; // int
	public static final String Gold = "Gold"; // int
	public static final String Money = "Money"; // int
	public static final String TargetX = "TargetX"; // float
	public static final String TargetY = "TargetY"; // float
	public static final String Prefab = "Prefab"; // string
	public static final String MoveType = "MoveType"; // int
	public static final String AtkDis = "AtkDis"; // float
	public static final String DropPackList = "DropPackList"; // string
	public static final String SkillIDRef = "SkillIDRef"; // string
	public static final String Height = "Height"; // float
	public static final String EffectData = "EffectData"; // string
	public static final String ConsumeData = "ConsumeData"; // string
	public static final String LastAttacker = "LastAttacker"; // object
	public static final String ShowName = "ShowName"; // string
	public static final String EquipIDRef = "EquipIDRef"; // string
	public static final String SpriteFile = "SpriteFile"; // string
	public static final String Icon = "Icon"; // string
	public static final String ShowCard = "ShowCard"; // string
	public static final String HeroType = "HeroType"; // int
	public static final String Camp = "Camp"; // int
	public static final String MasterID = "MasterID"; // object
	public static final String NPCType = "NPCType"; // int
	public static final String SUCKBLOOD = "SUCKBLOOD"; // int
	public static final String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static final String CRITICAL = "CRITICAL"; // int
	public static final String MAXHP = "MAXHP"; // int
	public static final String MAXMP = "MAXMP"; // int
	public static final String MAXSP = "MAXSP"; // int
	public static final String HPREGEN = "HPREGEN"; // int
	public static final String SPREGEN = "SPREGEN"; // int
	public static final String MPREGEN = "MPREGEN"; // int
	public static final String ATK_VALUE = "ATK_VALUE"; // int
	public static final String DEF_VALUE = "DEF_VALUE"; // int
	public static final String MOVE_SPEED = "MOVE_SPEED"; // int
	public static final String ATK_SPEED = "ATK_SPEED"; // int
	public static final String ATK_FIRE = "ATK_FIRE"; // int
	public static final String ATK_LIGHT = "ATK_LIGHT"; // int
	public static final String ATK_WIND = "ATK_WIND"; // int
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
	public static final String DEF_WIND = "DEF_WIND"; // int
	public static final String DEF_ICE = "DEF_ICE"; // int
	public static final String DEF_POISON = "DEF_POISON"; // int
	public static final String DIZZY_GATE = "DIZZY_GATE"; // int
	public static final String MOVE_GATE = "MOVE_GATE"; // int
	public static final String SKILL_GATE = "SKILL_GATE"; // int
	public static final String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static final String MAGIC_GATE = "MAGIC_GATE"; // int
	public static final String BUFF_GATE = "BUFF_GATE"; // int
	// Record

}

public class Player {
	//Class name
	public static final String ThisName = "Player";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Name = "Name"; // string
	public static final String Sex = "Sex"; // int
	public static final String Race = "Race"; // int
	public static final String Camp = "Camp"; // int
	public static final String HomeSceneID = "HomeSceneID"; // int
	public static final String Level = "Level"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String PrefabPath = "PrefabPath"; // string
	public static final String FirstTarget = "FirstTarget"; // object
	public static final String CharType = "CharType"; // int
	public static final String Job = "Job"; // int
	public static final String VIPLevel = "VIPLevel"; // int
	public static final String VIPEXP = "VIPEXP"; // int
	public static final String EXP = "EXP"; // int
	public static final String HP = "HP"; // int
	public static final String SP = "SP"; // int
	public static final String MP = "MP"; // int
	public static final String Gold = "Gold"; // int
	public static final String Money = "Money"; // int
	public static final String Account = "Account"; // string
	public static final String ConnectKey = "ConnectKey"; // string
	public static final String MAXEXP = "MAXEXP"; // int
	public static final String RELIVE_SOUL = "RELIVE_SOUL"; // int
	public static final String ATK_PVP = "ATK_PVP"; // int
	public static final String DEF_PVP = "DEF_PVP"; // int
	public static final String OnlineCount = "OnlineCount"; // int
	public static final String TotalTime = "TotalTime"; // int
	public static final String LastOfflineTime = "LastOfflineTime"; // object
	public static final String OnlineTime = "OnlineTime"; // object
	public static final String TotalLineTime = "TotalLineTime"; // object
	public static final String GMLevel = "GMLevel"; // int
	public static final String GameID = "GameID"; // int
	public static final String GateID = "GateID"; // int
	public static final String GuildID = "GuildID"; // object
	public static final String TeamID = "TeamID"; // object
	public static final String ViewOppnent = "ViewOppnent"; // object
	public static final String FightOppnent = "FightOppnent"; // object
	public static final String GambleGold = "GambleGold"; // int
	public static final String GambleDiamond = "GambleDiamond"; // int
	public static final String SUCKBLOOD = "SUCKBLOOD"; // int
	public static final String REFLECTDAMAGE = "REFLECTDAMAGE"; // int
	public static final String CRITICAL = "CRITICAL"; // int
	public static final String MAXHP = "MAXHP"; // int
	public static final String MAXMP = "MAXMP"; // int
	public static final String MAXSP = "MAXSP"; // int
	public static final String HPREGEN = "HPREGEN"; // int
	public static final String SPREGEN = "SPREGEN"; // int
	public static final String MPREGEN = "MPREGEN"; // int
	public static final String ATK_VALUE = "ATK_VALUE"; // int
	public static final String DEF_VALUE = "DEF_VALUE"; // int
	public static final String MOVE_SPEED = "MOVE_SPEED"; // int
	public static final String ATK_SPEED = "ATK_SPEED"; // int
	public static final String ATK_FIRE = "ATK_FIRE"; // int
	public static final String ATK_LIGHT = "ATK_LIGHT"; // int
	public static final String ATK_WIND = "ATK_WIND"; // int
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
	public static final String DEF_WIND = "DEF_WIND"; // int
	public static final String DEF_ICE = "DEF_ICE"; // int
	public static final String DEF_POISON = "DEF_POISON"; // int
	public static final String DIZZY_GATE = "DIZZY_GATE"; // int
	public static final String MOVE_GATE = "MOVE_GATE"; // int
	public static final String SKILL_GATE = "SKILL_GATE"; // int
	public static final String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static final String MAGIC_GATE = "MAGIC_GATE"; // int
	public static final String BUFF_GATE = "BUFF_GATE"; // int
	// Record
	public static final String R_PlayerHero = "PlayerHero";
	public static final String R_PlayerFightHero = "PlayerFightHero";
	public static final String R_HeroPropertyValue = "HeroPropertyValue";
	public static final String R_BagEquipList = "BagEquipList";
	public static final String R_BagItemList = "BagItemList";
	public static final String R_CommPropertyValue = "CommPropertyValue";
	public static final String R_TaskMonsterList = "TaskMonsterList";
	public static final String R_TaskList = "TaskList";
	public static final String R_BuildingList = "BuildingList";
	public static final String R_BuildingListProduce = "BuildingListProduce";

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
		Talent1		= 11, // Talent1 -- string
		Talent2		= 12, // Talent2 -- string
		Talent3		= 13, // Talent3 -- string
		Talent4		= 14, // Talent4 -- string
		Talent5		= 15, // Talent5 -- string
		Skill1		= 16, // Skill1 -- string
		Skill2		= 17, // Skill2 -- string
		Skill3		= 18, // Skill3 -- string
		Skill4		= 19, // Skill4 -- string
		Skill5		= 20, // Skill5 -- string
		FightSkill		= 21, // FightSkill -- string

	};

	public enum PlayerFightHero
	{
		GUID		= 0, // GUID -- object
		FightPos		= 1, // FightPos -- int

	};

	public enum HeroPropertyValue
	{
		SUCKBLOOD		= 0, // SUCKBLOOD -- int
		REFLECTDAMAGE		= 1, // REFLECTDAMAGE -- int
		CRITICAL		= 2, // CRITICAL -- int
		MAXHP		= 3, // MAXHP -- int
		MAXMP		= 4, // MAXMP -- int
		MAXSP		= 5, // MAXSP -- int
		HPREGEN		= 6, // HPREGEN -- int
		SPREGEN		= 7, // SPREGEN -- int
		MPREGEN		= 8, // MPREGEN -- int
		ATK_VALUE		= 9, // ATK_VALUE -- int
		DEF_VALUE		= 10, // DEF_VALUE -- int
		MOVE_SPEED		= 11, // MOVE_SPEED -- int
		ATK_SPEED		= 12, // ATK_SPEED -- int
		ATK_FIRE		= 13, // ATK_FIRE -- int
		ATK_LIGHT		= 14, // ATK_LIGHT -- int
		ATK_WIND		= 15, // ATK_WIND -- int
		ATK_ICE		= 16, // ATK_ICE -- int
		ATK_POISON		= 17, // ATK_POISON -- int
		DEF_FIRE		= 18, // DEF_FIRE -- int
		DEF_LIGHT		= 19, // DEF_LIGHT -- int
		DEF_WIND		= 20, // DEF_WIND -- int
		DEF_ICE		= 21, // DEF_ICE -- int
		DEF_POISON		= 22, // DEF_POISON -- int
		DIZZY_GATE		= 23, // DIZZY_GATE -- int
		MOVE_GATE		= 24, // MOVE_GATE -- int
		SKILL_GATE		= 25, // SKILL_GATE -- int
		PHYSICAL_GATE		= 26, // PHYSICAL_GATE -- int
		MAGIC_GATE		= 27, // MAGIC_GATE -- int
		BUFF_GATE		= 28, // BUFF_GATE -- int

	};

	public enum BagEquipList
	{
		GUID		= 0, // GUID -- object
		WearGUID		= 1, // WearGUID -- object
		ConfigID		= 2, // ConfigID -- string
		ExpiredType		= 3, // ExpiredType -- int
		Date		= 4, // Date -- int
		RandPropertyID		= 5, // RandPropertyID -- string
		SlotCount		= 6, // SlotCount -- int
		InlayStone1		= 7, // InlayStone1 -- string
		InlayStone2		= 8, // InlayStone2 -- string
		InlayStone3		= 9, // InlayStone3 -- string
		InlayStone4		= 10, // InlayStone4 -- string
		InlayStone5		= 11, // InlayStone5 -- string
		InlayStone6		= 12, // InlayStone6 -- string
		InlayStone7		= 13, // InlayStone7 -- string
		InlayStone8		= 14, // InlayStone8 -- string
		InlayStone9		= 15, // InlayStone9 -- string
		InlayStone10		= 16, // InlayStone10 -- string
		IntensifyLevel		= 17, // IntensifyLevel -- string
		ElementLevel1_FIRE		= 18, // ElementLevel1_FIRE -- int
		ElementLevel2_LIGHT		= 19, // ElementLevel2_LIGHT -- int
		ElementLevel3_Wind		= 20, // ElementLevel3_Wind -- int
		ElementLevel4_ICE		= 21, // ElementLevel4_ICE -- int
		ElementLevel5_POISON		= 22, // ElementLevel5_POISON -- int

	};

	public enum BagItemList
	{
		ConfigID		= 0, // ConfigID -- string
		ItemCount		= 1, // ItemCount -- int
		Bound		= 2, // Bound -- int
		ExpiredType		= 3, // ExpiredType -- int
		Date		= 4, // Date -- int

	};

	public enum CommPropertyValue
	{
		SUCKBLOOD		= 0, // SUCKBLOOD -- int
		REFLECTDAMAGE		= 1, // REFLECTDAMAGE -- int
		CRITICAL		= 2, // CRITICAL -- int
		MAXHP		= 3, // MAXHP -- int
		MAXMP		= 4, // MAXMP -- int
		MAXSP		= 5, // MAXSP -- int
		HPREGEN		= 6, // HPREGEN -- int
		SPREGEN		= 7, // SPREGEN -- int
		MPREGEN		= 8, // MPREGEN -- int
		ATK_VALUE		= 9, // ATK_VALUE -- int
		DEF_VALUE		= 10, // DEF_VALUE -- int
		MOVE_SPEED		= 11, // MOVE_SPEED -- int
		ATK_SPEED		= 12, // ATK_SPEED -- int
		ATK_FIRE		= 13, // ATK_FIRE -- int
		ATK_LIGHT		= 14, // ATK_LIGHT -- int
		ATK_WIND		= 15, // ATK_WIND -- int
		ATK_ICE		= 16, // ATK_ICE -- int
		ATK_POISON		= 17, // ATK_POISON -- int
		DEF_FIRE		= 18, // DEF_FIRE -- int
		DEF_LIGHT		= 19, // DEF_LIGHT -- int
		DEF_WIND		= 20, // DEF_WIND -- int
		DEF_ICE		= 21, // DEF_ICE -- int
		DEF_POISON		= 22, // DEF_POISON -- int
		DIZZY_GATE		= 23, // DIZZY_GATE -- int
		MOVE_GATE		= 24, // MOVE_GATE -- int
		SKILL_GATE		= 25, // SKILL_GATE -- int
		PHYSICAL_GATE		= 26, // PHYSICAL_GATE -- int
		MAGIC_GATE		= 27, // MAGIC_GATE -- int
		BUFF_GATE		= 28, // BUFF_GATE -- int

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

public class Scene {
	//Class name
	public static final String ThisName = "Scene";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String SceneName = "SceneName"; // string
	public static final String SceneShowName = "SceneShowName"; // string
	public static final String MaxGroup = "MaxGroup"; // int
	public static final String MaxGroupPlayers = "MaxGroupPlayers"; // int
	public static final String FilePath = "FilePath"; // string
	public static final String RelivePos = "RelivePos"; // string
	public static final String Width = "Width"; // int
	public static final String SoundList = "SoundList"; // string
	public static final String Tile = "Tile"; // int
	public static final String Share = "Share"; // int
	public static final String CanClone = "CanClone"; // int
	public static final String ActorID = "ActorID"; // int
	public static final String LoadingUI = "LoadingUI"; // string
	public static final String CamOffestPos = "CamOffestPos"; // string
	public static final String CamOffestRot = "CamOffestRot"; // string
	public static final String SyncObject = "SyncObject"; // int
	// Record

}

public class Server {
	//Class name
	public static final String ThisName = "Server";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ServerID = "ServerID"; // int
	public static final String Name = "Name"; // string
	public static final String MaxOnline = "MaxOnline"; // int
	public static final String CpuCount = "CpuCount"; // int
	public static final String IP = "IP"; // string
	public static final String Port = "Port"; // int
	public static final String Type = "Type"; // int
	public static final String Area = "Area"; // int
	// Record

}

public class Shop {
	//Class name
	public static final String ThisName = "Shop";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Type = "Type"; // int
	public static final String ItemID = "ItemID"; // string
	public static final String Gold = "Gold"; // int
	public static final String Steel = "Steel"; // int
	public static final String Stone = "Stone"; // int
	public static final String Diamond = "Diamond"; // int
	public static final String Level = "Level"; // int
	public static final String Count = "Count"; // int
	// Record

}

public class Skill {
	//Class name
	public static final String ThisName = "Skill";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String SkillType = "SkillType"; // int
	public static final String AnimaState = "AnimaState"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String Desc = "Desc"; // string
	public static final String ConsumeProperty = "ConsumeProperty"; // string
	public static final String ConsumeValue = "ConsumeValue"; // string
	public static final String ConsumeType = "ConsumeType"; // int
	public static final String DamageProperty = "DamageProperty"; // string
	public static final String DamageValue = "DamageValue"; // string
	public static final String DamageType = "DamageType"; // int
	public static final String GetBuffList = "GetBuffList"; // string
	public static final String SendBuffList = "SendBuffList"; // string
	public static final String CoolDownTime = "CoolDownTime"; // float
	public static final String RequireDistance = "RequireDistance"; // float
	public static final String DamageDistance = "DamageDistance"; // float
	public static final String TargetType = "TargetType"; // int
	public static final String NewObject = "NewObject"; // string
	public static final String Icon = "Icon"; // string
	public static final String Atlas = "Atlas"; // string
	public static final String UpLevel = "UpLevel"; // int
	public static final String AfterUpID = "AfterUpID"; // string
	public static final String PlayerSkill = "PlayerSkill"; // int
	public static final String AtkDis = "AtkDis"; // float
	public static final String NeedTar = "NeedTar"; // int
	public static final String DefaultHitTime = "DefaultHitTime"; // float
	// Record

}

public class SkillRef {
	//Class name
	public static final String ThisName = "SkillRef";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String NORMALATTACK1 = "NORMALATTACK1"; // string
	public static final String NORMALATTACK2 = "NORMALATTACK2"; // string
	public static final String NORMALATTACK3 = "NORMALATTACK3"; // string
	public static final String NORMALTHUMP = "NORMALTHUMP"; // string
	public static final String SKILL1 = "SKILL1"; // string
	public static final String SKILL2 = "SKILL2"; // string
	public static final String SKILL3 = "SKILL3"; // string
	public static final String SKILL4 = "SKILL4"; // string
	public static final String SKILL5 = "SKILL5"; // string
	public static final String SKILL6 = "SKILL6"; // string
	public static final String SKILL7 = "SKILL7"; // string
	public static final String SKILL8 = "SKILL8"; // string
	public static final String SKILL9 = "SKILL9"; // string
	public static final String SKILL10 = "SKILL10"; // string
	// Record

}

public class SqlServer {
	//Class name
	public static final String ThisName = "SqlServer";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ServerID = "ServerID"; // int
	public static final String IP = "IP"; // string
	public static final String Port = "Port"; // int
	public static final String SqlIP = "SqlIP"; // string
	public static final String SqlPort = "SqlPort"; // int
	public static final String SqlUser = "SqlUser"; // string
	public static final String SqlPwd = "SqlPwd"; // string
	public static final String SqlName = "SqlName"; // string
	// Record

}

public class StateFuncResources {
	//Class name
	public static final String ThisName = "StateFuncResources";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Atlas_ResID = "Atlas_ResID"; // string
	// Record

}

public class StateFunction {
	//Class name
	public static final String ThisName = "StateFunction";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String EFT_INFO = "EFT_INFO"; // int
	public static final String EFT_BOOOST = "EFT_BOOOST"; // int
	public static final String EFT_LVLUP = "EFT_LVLUP"; // int
	public static final String EFT_CREATE_SOLDER = "EFT_CREATE_SOLDER"; // int
	public static final String EFT_CREATE_SPEEL = "EFT_CREATE_SPEEL"; // int
	public static final String EFT_RESEARCH = "EFT_RESEARCH"; // int
	public static final String EFT_COLLECT_GOLD = "EFT_COLLECT_GOLD"; // int
	public static final String EFT_COLLECT_STONE = "EFT_COLLECT_STONE"; // int
	public static final String EFT_COLLECT_STEEL = "EFT_COLLECT_STEEL"; // int
	public static final String EFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND"; // int
	public static final String EFT_SELL = "EFT_SELL"; // int
	public static final String EFT_REPAIR = "EFT_REPAIR"; // int
	public static final String EFT_CANCEL = "EFT_CANCEL"; // int
	public static final String EFT_FINISH = "EFT_FINISH"; // int
	// Record

}

public class Talent {
	//Class name
	public static final String ThisName = "Talent";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String ShowName = "ShowName"; // string
	public static final String Desc = "Desc"; // string
	public static final String EffectData = "EffectData"; // string
	public static final String Icon = "Icon"; // string
	public static final String Atlas = "Atlas"; // string
	public static final String UpLevel = "UpLevel"; // int
	public static final String AfterUpID = "AfterUpID"; // string
	// Record

}

public class Task {
	//Class name
	public static final String ThisName = "Task";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Type = "Type"; // int
	public static final String NextTaskID = "NextTaskID"; // string
	public static final String KillMonsterName = "KillMonsterName"; // string
	public static final String KillCount = "KillCount"; // string
	public static final String LevelReq = "LevelReq"; // string
	public static final String AwardExp = "AwardExp"; // int
	public static final String AwardGold = "AwardGold"; // int
	public static final String AwardPack = "AwardPack"; // string
	public static final String Desc = "Desc"; // string
	// Record

}

public class Team {
	//Class name
	public static final String ThisName = "Team";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	// Property
	public static final String Captain = "Captain"; // object
	public static final String PresidentName = "PresidentName"; // string
	// Record
	public static final String R_MemberList = "MemberList";
	public static final String R_ApplyList = "ApplyList";

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

