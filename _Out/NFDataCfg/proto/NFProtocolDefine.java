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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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

public class ConsumeData {
	//Class name
	public static final String ThisName = "ConsumeData";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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

	public class GuildBoss
	{
		public static final String ThisName = "GuildBoss";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Offline		= 6; // Offline -- int
		public static final int Power		= 7; // Power -- int

	};

	public class GuildMemberList
	{
		public static final String ThisName = "GuildMemberList";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int Receive		= 5; // Receive -- int
		public static final int VIP		= 6; // VIP -- int
		public static final int Online		= 7; // Online -- int
		public static final int Power		= 8; // Power -- int
		public static final int Title		= 9; // Title -- int
		public static final int GameID		= 10; // GameID -- int
		public static final int JoinTime		= 11; // JoinTime -- int
		public static final int Contribution		= 12; // Contribution -- int
		public static final int AllContribution		= 13; // AllContribution -- int

	};

	public class GuildAppyList
	{
		public static final String ThisName = "GuildAppyList";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Power		= 6; // Power -- int

	};

	public class GuildEvent
	{
		public static final String ThisName = "GuildEvent";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Offline		= 6; // Offline -- int
		public static final int Power		= 7; // Power -- int
		public static final int EventID		= 8; // EventID -- int
		public static final int EventTime		= 9; // EventTime -- int
		public static final int Context		= 10; // Context -- string

	};

	public class GuildHouse
	{
		public static final String ThisName = "GuildHouse";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Offline		= 6; // Offline -- int
		public static final int Power		= 7; // Power -- int

	};

	public class GuildSkill
	{
		public static final String ThisName = "GuildSkill";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Offline		= 6; // Offline -- int
		public static final int Power		= 7; // Power -- int

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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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

public class HttpServer {
	//Class name
	public static final String ThisName = "HttpServer";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
	// Property
	public static final String MaxCount = "MaxCount"; // int
	public static final String InComeGold = "InComeGold"; // int
	public static final String InComeDiamond = "InComeDiamond"; // int
	public static final String InComeOil = "InComeOil"; // int
	public static final String MapLevel = "MapLevel"; // int
	// Record

	public class Station
	{
		public static final String ThisName = "Station";
		public static final int GUID		= 0; // GUID -- object
		public static final int GuildID		= 1; // GuildID -- object
		public static final int GuildName		= 2; // GuildName -- string
		public static final int Level		= 3; // Level -- int
		public static final int Title		= 4; // Title -- string
		public static final int Slogan		= 5; // Slogan -- string
		public static final int State		= 6; // State -- int
		public static final int CurMemberCount		= 7; // CurMemberCount -- int
		public static final int WinCount		= 8; // WinCount -- int

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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Climb = "Climb"; // int
	public static final String AtkDis = "AtkDis"; // float
	public static final String DropPackList = "DropPackList"; // string
	public static final String Idle = "Idle"; // string
	public static final String SkillNormal = "SkillNormal"; // string
	public static final String SkillAttack = "SkillAttack"; // string
	public static final String SkillTHUMP = "SkillTHUMP"; // string
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
	public static final String Position = "Position"; // vector3
	// Property
	public static final String Name = "Name"; // string
	public static final String Sex = "Sex"; // int
	public static final String Job = "Job"; // int
	public static final String Race = "Race"; // int
	public static final String Camp = "Camp"; // int
	public static final String Level = "Level"; // int
	public static final String ShowName = "ShowName"; // string
	public static final String PrefabPath = "PrefabPath"; // string
	public static final String FirstTarget = "FirstTarget"; // object
	public static final String FightHero = "FightHero"; // object
	public static final String HeroPos1 = "HeroPos1"; // object
	public static final String HeroPos2 = "HeroPos2"; // object
	public static final String HeroPos3 = "HeroPos3"; // object
	public static final String Skill1 = "Skill1"; // string
	public static final String Skill2 = "Skill2"; // string
	public static final String Skill3 = "Skill3"; // string
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
	public static final String HomeSceneID = "HomeSceneID"; // int
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

	public class CommValue
	{
		public static final String ThisName = "CommValue";
		public static final int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		public static final int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		public static final int CRITICAL		= 2; // CRITICAL -- int
		public static final int MAXHP		= 3; // MAXHP -- int
		public static final int MAXMP		= 4; // MAXMP -- int
		public static final int MAXSP		= 5; // MAXSP -- int
		public static final int HPREGEN		= 6; // HPREGEN -- int
		public static final int SPREGEN		= 7; // SPREGEN -- int
		public static final int MPREGEN		= 8; // MPREGEN -- int
		public static final int ATK_VALUE		= 9; // ATK_VALUE -- int
		public static final int DEF_VALUE		= 10; // DEF_VALUE -- int
		public static final int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		public static final int ATK_SPEED		= 12; // ATK_SPEED -- int
		public static final int ATK_FIRE		= 13; // ATK_FIRE -- int
		public static final int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		public static final int ATK_WIND		= 15; // ATK_WIND -- int
		public static final int ATK_ICE		= 16; // ATK_ICE -- int
		public static final int ATK_POISON		= 17; // ATK_POISON -- int
		public static final int DEF_FIRE		= 18; // DEF_FIRE -- int
		public static final int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		public static final int DEF_WIND		= 20; // DEF_WIND -- int
		public static final int DEF_ICE		= 21; // DEF_ICE -- int
		public static final int DEF_POISON		= 22; // DEF_POISON -- int
		public static final int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		public static final int MOVE_GATE		= 24; // MOVE_GATE -- int
		public static final int SKILL_GATE		= 25; // SKILL_GATE -- int
		public static final int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		public static final int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		public static final int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	public class PlayerHero
	{
		public static final String ThisName = "PlayerHero";
		public static final int GUID		= 0; // GUID -- object
		public static final int ConfigID		= 1; // ConfigID -- string
		public static final int Activated		= 2; // Activated -- int
		public static final int Level		= 3; // Level -- int
		public static final int Exp		= 4; // Exp -- int
		public static final int Star		= 5; // Star -- int
		public static final int Equip1		= 6; // Equip1 -- object
		public static final int Equip2		= 7; // Equip2 -- object
		public static final int Equip3		= 8; // Equip3 -- object
		public static final int Equip4		= 9; // Equip4 -- object
		public static final int Equip5		= 10; // Equip5 -- object
		public static final int Equip6		= 11; // Equip6 -- object
		public static final int Talent1		= 12; // Talent1 -- string
		public static final int Talent2		= 13; // Talent2 -- string
		public static final int Talent3		= 14; // Talent3 -- string
		public static final int Talent4		= 15; // Talent4 -- string
		public static final int Talent5		= 16; // Talent5 -- string
		public static final int Skill1		= 17; // Skill1 -- string
		public static final int Skill2		= 18; // Skill2 -- string
		public static final int Skill3		= 19; // Skill3 -- string
		public static final int Skill4		= 20; // Skill4 -- string
		public static final int Skill5		= 21; // Skill5 -- string
		public static final int FightSkill		= 22; // FightSkill -- string

	};

	public class HeroPropertyValue
	{
		public static final String ThisName = "HeroPropertyValue";
		public static final int SUCKBLOOD		= 0; // SUCKBLOOD -- int
		public static final int REFLECTDAMAGE		= 1; // REFLECTDAMAGE -- int
		public static final int CRITICAL		= 2; // CRITICAL -- int
		public static final int MAXHP		= 3; // MAXHP -- int
		public static final int MAXMP		= 4; // MAXMP -- int
		public static final int MAXSP		= 5; // MAXSP -- int
		public static final int HPREGEN		= 6; // HPREGEN -- int
		public static final int SPREGEN		= 7; // SPREGEN -- int
		public static final int MPREGEN		= 8; // MPREGEN -- int
		public static final int ATK_VALUE		= 9; // ATK_VALUE -- int
		public static final int DEF_VALUE		= 10; // DEF_VALUE -- int
		public static final int MOVE_SPEED		= 11; // MOVE_SPEED -- int
		public static final int ATK_SPEED		= 12; // ATK_SPEED -- int
		public static final int ATK_FIRE		= 13; // ATK_FIRE -- int
		public static final int ATK_LIGHT		= 14; // ATK_LIGHT -- int
		public static final int ATK_WIND		= 15; // ATK_WIND -- int
		public static final int ATK_ICE		= 16; // ATK_ICE -- int
		public static final int ATK_POISON		= 17; // ATK_POISON -- int
		public static final int DEF_FIRE		= 18; // DEF_FIRE -- int
		public static final int DEF_LIGHT		= 19; // DEF_LIGHT -- int
		public static final int DEF_WIND		= 20; // DEF_WIND -- int
		public static final int DEF_ICE		= 21; // DEF_ICE -- int
		public static final int DEF_POISON		= 22; // DEF_POISON -- int
		public static final int DIZZY_GATE		= 23; // DIZZY_GATE -- int
		public static final int MOVE_GATE		= 24; // MOVE_GATE -- int
		public static final int SKILL_GATE		= 25; // SKILL_GATE -- int
		public static final int PHYSICAL_GATE		= 26; // PHYSICAL_GATE -- int
		public static final int MAGIC_GATE		= 27; // MAGIC_GATE -- int
		public static final int BUFF_GATE		= 28; // BUFF_GATE -- int

	};

	public class BagEquipList
	{
		public static final String ThisName = "BagEquipList";
		public static final int GUID		= 0; // GUID -- object
		public static final int WearGUID		= 1; // WearGUID -- object
		public static final int ConfigID		= 2; // ConfigID -- string
		public static final int ExpiredType		= 3; // ExpiredType -- int
		public static final int Date		= 4; // Date -- int
		public static final int RandPropertyID		= 5; // RandPropertyID -- string
		public static final int SlotCount		= 6; // SlotCount -- int
		public static final int InlayStone1		= 7; // InlayStone1 -- string
		public static final int InlayStone2		= 8; // InlayStone2 -- string
		public static final int InlayStone3		= 9; // InlayStone3 -- string
		public static final int InlayStone4		= 10; // InlayStone4 -- string
		public static final int InlayStone5		= 11; // InlayStone5 -- string
		public static final int InlayStone6		= 12; // InlayStone6 -- string
		public static final int InlayStone7		= 13; // InlayStone7 -- string
		public static final int InlayStone8		= 14; // InlayStone8 -- string
		public static final int InlayStone9		= 15; // InlayStone9 -- string
		public static final int InlayStone10		= 16; // InlayStone10 -- string
		public static final int IntensifyLevel		= 17; // IntensifyLevel -- string
		public static final int ElementLevel1_FIRE		= 18; // ElementLevel1_FIRE -- int
		public static final int ElementLevel2_LIGHT		= 19; // ElementLevel2_LIGHT -- int
		public static final int ElementLevel3_Wind		= 20; // ElementLevel3_Wind -- int
		public static final int ElementLevel4_ICE		= 21; // ElementLevel4_ICE -- int
		public static final int ElementLevel5_POISON		= 22; // ElementLevel5_POISON -- int

	};

	public class BagItemList
	{
		public static final String ThisName = "BagItemList";
		public static final int ConfigID		= 0; // ConfigID -- string
		public static final int ItemCount		= 1; // ItemCount -- int
		public static final int Bound		= 2; // Bound -- int
		public static final int ExpiredType		= 3; // ExpiredType -- int
		public static final int Date		= 4; // Date -- int

	};

	public class TaskMonsterList
	{
		public static final String ThisName = "TaskMonsterList";
		public static final int MonsterID		= 0; // MonsterID -- string
		public static final int CurrentKillCount		= 1; // CurrentKillCount -- int
		public static final int RequireKillCount		= 2; // RequireKillCount -- int
		public static final int TaskID		= 3; // TaskID -- string

	};

	public class TaskList
	{
		public static final String ThisName = "TaskList";
		public static final int TaskID		= 0; // TaskID -- string
		public static final int TaskStatus		= 1; // TaskStatus -- int
		public static final int Process		= 2; // Process -- int

	};

	public class BuildingList
	{
		public static final String ThisName = "BuildingList";
		public static final int BuildingID		= 0; // BuildingID -- string
		public static final int BuildingGUID		= 1; // BuildingGUID -- object
		public static final int State		= 2; // State -- int
		public static final int PosX		= 3; // PosX -- float
		public static final int Pos		= 4; // Pos -- vector3
		public static final int StateEndTime		= 5; // StateEndTime -- int

	};

	public class BuildingListProduce
	{
		public static final String ThisName = "BuildingListProduce";
		public static final int BuildingID		= 0; // BuildingID -- string
		public static final int BuildingGUID		= 1; // BuildingGUID -- object
		public static final int State		= 2; // State -- int
		public static final int PosX		= 3; // PosX -- float
		public static final int PosY		= 4; // PosY -- float
		public static final int StateEndTime		= 5; // StateEndTime -- int

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
	public static final String Position = "Position"; // vector3
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
	public static final String NavigationResPath = "NavigationResPath"; // string
	// Record

}

public class Security {
	//Class name
	public static final String ThisName = "Security";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String Position = "Position"; // vector3
	// Property
	public static final String SecurityData = "SecurityData"; // string
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
	// Property
	public static final String SkillType = "SkillType"; // int
	public static final String AnimaState = "AnimaState"; // string
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
	public static final String SpriteFile = "SpriteFile"; // string
	public static final String PlayerSkill = "PlayerSkill"; // int
	public static final String AtkDis = "AtkDis"; // float
	public static final String DefaultHitTime = "DefaultHitTime"; // float
	public static final String NextID = "NextID"; // string
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
	public static final String Position = "Position"; // vector3
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

public class Talent {
	//Class name
	public static final String ThisName = "Talent";
	// IObject
	public static final String ID = "ID"; // string
	public static final String ClassName = "ClassName"; // string
	public static final String SceneID = "SceneID"; // int
	public static final String GroupID = "GroupID"; // int
	public static final String ConfigID = "ConfigID"; // string
	public static final String Position = "Position"; // vector3
	// Property
	public static final String ShowName = "ShowName"; // string
	public static final String Desc = "Desc"; // string
	public static final String EffectData = "EffectData"; // string
	public static final String Icon = "Icon"; // string
	public static final String SpriteFile = "SpriteFile"; // string
	public static final String NextID = "NextID"; // string
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
	public static final String Position = "Position"; // vector3
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
	public static final String Position = "Position"; // vector3
	// Property
	public static final String Captain = "Captain"; // object
	public static final String PresidentName = "PresidentName"; // string
	// Record

	public class MemberList
	{
		public static final String ThisName = "MemberList";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int Receive		= 5; // Receive -- int
		public static final int VIP		= 6; // VIP -- int
		public static final int Online		= 7; // Online -- int
		public static final int Title		= 8; // Title -- int
		public static final int GameID		= 9; // GameID -- int

	};

	public class ApplyList
	{
		public static final String ThisName = "ApplyList";
		public static final int GUID		= 0; // GUID -- object
		public static final int Name		= 1; // Name -- string
		public static final int Level		= 2; // Level -- int
		public static final int Job		= 3; // Job -- int
		public static final int Donation		= 4; // Donation -- int
		public static final int VIP		= 5; // VIP -- int
		public static final int Power		= 6; // Power -- int

	};

}

