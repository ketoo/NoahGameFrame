// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.java
//    @Author           :    NFrame Studio
//    @Date             :    2016/01/09
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

public class BB_Player {
	//Class name
	public static final String ThisName = "BB_Player";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	// Property
	public static final String RoleName = "RoleName"; // string
	public static final String Sex = "Sex"; // int
	public static final String Race = "Race"; // int
	public static final String Camp = "Camp"; // int
	public static final String LastContainerID = "LastContainerID"; // int
	public static final String Level = "Level"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String Gold = "Gold"; // int
	public static final String Stone = "Stone"; // int
	public static final String Steel = "Steel"; // int
	public static final String Diamond = "Diamond"; // int
	public static final String LoadPropertyFinish = "LoadPropertyFinish"; // int
	// Record
	public static final String BuildingList = "BuildingList";
	public static final String BuildingProduce = "BuildingProduce";

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

	public enum BuildingProduce
	{
		BuildingGUID		= 0, // BuildingGUID -- object
		ItemID		= 1, // ItemID -- string
		LeftCount		= 2, // LeftCount -- int
		OnceTime		= 3, // OnceTime -- int
		OnceStartTime		= 4, // OnceStartTime -- int
	};

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
	// Property
	public static final String Name = "Name"; // string
	public static final String CreateObject = "CreateObject"; // object
	// Record
	public static final String GroupMemberList = "GroupMemberList";
	public static final String ChatList = "ChatList";

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
	// Property
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
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
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
	// Property
	public static final String Sex = "Sex"; // int
	public static final String IntensiveBuffList = "IntensiveBuffList"; // string
	public static final String EnchantmentBuffList = "EnchantmentBuffList"; // string
	public static final String SuitID = "SuitID"; // int
	public static final String SuitBuffID = "SuitBuffID"; // string
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
	public static final String GuilID = "GuilID"; // object
	public static final String GuilIDIcon = "GuilIDIcon"; // int
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
	public static final String AutoRecruit = "AutoRecruit"; // string
	// Record
	public static final String GuildBoss = "GuildBoss";
	public static final String GuildMemberList = "GuildMemberList";
	public static final String GuildAppyList = "GuildAppyList";
	public static final String GuildEvent = "GuildEvent";
	public static final String GuildHouse = "GuildHouse";
	public static final String GuildSkill = "GuildSkill";

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
	// Property
	public static final String GuildID = "GuildID"; // object
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
	public static final String CoolDownTime = "CoolDownTime"; // float
	public static final String OverlayCount = "OverlayCount"; // int
	public static final String ExpiredType = "ExpiredType"; // int
	public static final String BuyPrice = "BuyPrice"; // int
	public static final String SalePrice = "SalePrice"; // int
	public static final String Script = "Script"; // string
	public static final String Extend = "Extend"; // string
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
	// Property
	public static final String English = "English"; // string
	public static final String Chinese = "Chinese"; // string
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
	// Property
	public static final String SeedID = "SeedID"; // string
	public static final String EXP = "EXP"; // int
	public static final String HP = "HP"; // int
	public static final String SP = "SP"; // int
	public static final String MP = "MP"; // int
	public static final String Money = "Money"; // int
	public static final String LastAttacker = "LastAttacker"; // object
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	public static final String TargetX = "TargetX"; // float
	public static final String TargetY = "TargetY"; // float
	public static final String TargetZ = "TargetZ"; // float
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
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
	public static final String DEF_ICE = "DEF_ICE"; // int
	public static final String DEF_POISON = "DEF_POISON"; // int
	public static final String DIZZY_GATE = "DIZZY_GATE"; // int
	public static final String MOVE_GATE = "MOVE_GATE"; // int
	public static final String SKILL_GATE = "SKILL_GATE"; // int
	public static final String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static final String MAGIC_GATE = "MAGIC_GATE"; // int
	public static final String BUFF_GATE = "BUFF_GATE"; // int
	public static final String Prefab = "Prefab"; // string
	public static final String MoveType = "MoveType"; // int
	public static final String AtkDis = "AtkDis"; // float
	public static final String DropPackList = "DropPackList"; // string
	public static final String SkillIDRef = "SkillIDRef"; // string
	public static final String Height = "Height"; // float
	// Record
	public static final String PosList = "PosList";

	public enum PosList
	{
		X		= 0, // X -- float
		Y		= 1, // Y -- float
		Z		= 2, // Z -- float
		StayTime		= 3, // StayTime -- float
	};

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
	// Property
	public static final String Name = "Name"; // string
	public static final String Sex = "Sex"; // int
	public static final String Race = "Race"; // int
	public static final String Camp = "Camp"; // int
	public static final String LastContainerID = "LastContainerID"; // int
	public static final String Level = "Level"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String PrefabPath = "PrefabPath"; // string
	public static final String FirstTarget = "FirstTarget"; // object
	public static final String State = "State"; // int
	public static final String LogicState = "LogicState"; // int
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
	public static final String DEAD_COUNT = "DEAD_COUNT"; // int
	public static final String RELIVE_SOUL = "RELIVE_SOUL"; // int
	public static final String GUILD_NAME = "GUILD_NAME"; // string
	public static final String SPOUSE_NAME = "SPOUSE_NAME"; // string
	public static final String TEAM_CARD = "TEAM_CARD"; // string
	public static final String PVP_ATK_CARD = "PVP_ATK_CARD"; // string
	public static final String PVP_DEF_CARD = "PVP_DEF_CARD"; // string
	public static final String LUCKLY_CARD = "LUCKLY_CARD"; // string
	public static final String ATK_PVP = "ATK_PVP"; // int
	public static final String DEF_PVP = "DEF_PVP"; // int
	public static final String CreditValue = "CreditValue"; // int
	public static final String SoulValue = "SoulValue"; // int
	public static final String OnlineCount = "OnlineCount"; // int
	public static final String TotalTime = "TotalTime"; // int
	public static final String Citizenship = "Citizenship"; // int
	public static final String LastOfflineTime = "LastOfflineTime"; // object
	public static final String OnlineTime = "OnlineTime"; // object
	public static final String TotalLineTime = "TotalLineTime"; // object
	public static final String GMLevel = "GMLevel"; // int
	public static final String LastAttacker = "LastAttacker"; // object
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
	public static final String ATK_ICE = "ATK_ICE"; // int
	public static final String ATK_POISON = "ATK_POISON"; // int
	public static final String DEF_FIRE = "DEF_FIRE"; // int
	public static final String DEF_LIGHT = "DEF_LIGHT"; // int
	public static final String DEF_ICE = "DEF_ICE"; // int
	public static final String DEF_POISON = "DEF_POISON"; // int
	public static final String DIZZY_GATE = "DIZZY_GATE"; // int
	public static final String MOVE_GATE = "MOVE_GATE"; // int
	public static final String SKILL_GATE = "SKILL_GATE"; // int
	public static final String PHYSICAL_GATE = "PHYSICAL_GATE"; // int
	public static final String MAGIC_GATE = "MAGIC_GATE"; // int
	public static final String BUFF_GATE = "BUFF_GATE"; // int
	public static final String X = "X"; // float
	public static final String Y = "Y"; // float
	public static final String Z = "Z"; // float
	public static final String TargetX = "TargetX"; // float
	public static final String TargetY = "TargetY"; // float
	public static final String TargetZ = "TargetZ"; // float
	public static final String PathStep = "PathStep"; // int
	public static final String LoadPropertyFinish = "LoadPropertyFinish"; // int
	public static final String GameID = "GameID"; // int
	public static final String GateID = "GateID"; // int
	public static final String GuildID = "GuildID"; // object
	// Record
	public static final String PlayerViewItem = "PlayerViewItem";
	public static final String PlayerHero = "PlayerHero";
	public static final String BagEquipList = "BagEquipList";
	public static final String BagItemList = "BagItemList";
	public static final String CommPropertyValue = "CommPropertyValue";
	public static final String EctypeList = "EctypeList";
	public static final String DropItemList = "DropItemList";
	public static final String SkillTable = "SkillTable";
	public static final String TaskMonsterList = "TaskMonsterList";
	public static final String TaskList = "TaskList";
	public static final String PVPList = "PVPList";
	public static final String ChatGroup = "ChatGroup";

	public enum PlayerViewItem
	{
		ConfigID		= 0, // ConfigID -- string
		EnhanceLevel		= 1, // EnhanceLevel -- int
		InlayStone		= 2, // InlayStone -- string
		SagecraftLevel		= 3, // SagecraftLevel -- string
	};

	public enum PlayerHero
	{
		GUID		= 0, // GUID -- object
		ConfigID		= 1, // ConfigID -- string
		Level		= 2, // Level -- int
		Exp		= 3, // Exp -- string
		Equip1		= 4, // Equip1 -- object
		Equip2		= 5, // Equip2 -- object
		Equip3		= 6, // Equip3 -- object
		Equip4		= 7, // Equip4 -- object
		Equip5		= 8, // Equip5 -- object
		Equip6		= 9, // Equip6 -- object
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

	public enum EctypeList
	{
		EctypeID		= 0, // EctypeID -- int
		IsPass		= 1, // IsPass -- int
		Star		= 2, // Star -- int
	};

	public enum DropItemList
	{
		MonsterID		= 0, // MonsterID -- object
		ItemID		= 1, // ItemID -- string
		ItemCount		= 2, // ItemCount -- int
		DrawState		= 3, // DrawState -- int
	};

	public enum SkillTable
	{
		SkillID		= 0, // SkillID -- string
		SkillLevel		= 1, // SkillLevel -- int
		SkillStone		= 2, // SkillStone -- string
		SkillSoulLevel		= 3, // SkillSoulLevel -- int
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

	public enum PVPList
	{
		ID		= 0, // ID -- object
		Name		= 1, // Name -- string
		Level		= 2, // Level -- int
	};

	public enum ChatGroup
	{
		ChatType		= 0, // ChatType -- int
		GroupGUID		= 1, // GroupGUID -- object
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
	// Property
	public static final String SceneName = "SceneName"; // string
	public static final String SceneShowName = "SceneShowName"; // string
	public static final String MaxGroup = "MaxGroup"; // int
	public static final String MaxGroupPlayers = "MaxGroupPlayers"; // int
	public static final String FilePath = "FilePath"; // string
	public static final String RelivePos = "RelivePos"; // string
	public static final String Width = "Width"; // int
	public static final String SoundList = "SoundList"; // string
	public static final String Share = "Share"; // int
	public static final String CanClone = "CanClone"; // int
	public static final String ActorID = "ActorID"; // int
	public static final String LoadingUI = "LoadingUI"; // string
	public static final String CamOffestPos = "CamOffestPos"; // string
	public static final String CamOffestRot = "CamOffestRot"; // string
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
	// Property
	public static final String ServerID = "ServerID"; // int
	public static final String Name = "Name"; // string
	public static final String MaxOnline = "MaxOnline"; // int
	public static final String CpuCount = "CpuCount"; // int
	public static final String IP = "IP"; // string
	public static final String Port = "Port"; // int
	public static final String Type = "Type"; // int
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
	// Property
	public static final String Type = "Type"; // int
	public static final String ItemID = "ItemID"; // string
	public static final String Gold = "Gold"; // int
	public static final String Steel = "Steel"; // int
	public static final String Stone = "Stone"; // int
	public static final String Diamond = "Diamond"; // int
	public static final String Level = "Level"; // int
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
	public static final String AfterUpSkillID = "AfterUpSkillID"; // string
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

public class Task {
	//Class name
	public static final String ThisName = "Task";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
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

