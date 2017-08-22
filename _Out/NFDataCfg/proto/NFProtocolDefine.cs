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
	public class Buff
	{
		//Class name
		public static readonly String ThisName = "Buff";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String DownSaveType = "DownSaveType";// int
		public static readonly String EffectClearOnDead = "EffectClearOnDead";// int
		public static readonly String EffectTimeInterval = "EffectTimeInterval";// float
		public static readonly String EffectTimeValue = "EffectTimeValue";// int
		public static readonly String EffectType = "EffectType";// int
		public static readonly String EffectValueReferType = "EffectValueReferType";// int
		public static readonly String EffectValueType = "EffectValueType";// int
		public static readonly String ReverseReferType = "ReverseReferType";// int
		public static readonly String WashGroupID = "WashGroupID";// int
		// Record

	}
	public class ConsumeData
	{
		//Class name
		public static readonly String ThisName = "ConsumeData";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Diamond = "Diamond";// int
		public static readonly String EXP = "EXP";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String HP = "HP";// int
		public static readonly String MP = "MP";// int
		public static readonly String SP = "SP";// int
		public static readonly String VIPEXP = "VIPEXP";// int
		// Record

	}
	public class DescData
	{
		//Class name
		public static readonly String ThisName = "DescData";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Atlas = "Atlas";// string
		public static readonly String DescText = "DescText";// string
		public static readonly String Icon = "Icon";// string
		public static readonly String PerformanceEffect = "PerformanceEffect";// string
		public static readonly String PerformanceSound = "PerformanceSound";// string
		public static readonly String PrefabPath = "PrefabPath";// string
		public static readonly String ShowName = "ShowName";// string
		// Record

	}
	public class EffectData
	{
		//Class name
		public static readonly String ThisName = "EffectData";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ATK_FIRE = "ATK_FIRE";// int
		public static readonly String ATK_ICE = "ATK_ICE";// int
		public static readonly String ATK_LIGHT = "ATK_LIGHT";// int
		public static readonly String ATK_POISON = "ATK_POISON";// int
		public static readonly String ATK_SPEED = "ATK_SPEED";// int
		public static readonly String ATK_VALUE = "ATK_VALUE";// int
		public static readonly String ATK_WIND = "ATK_WIND";// int
		public static readonly String BUFF_GATE = "BUFF_GATE";// int
		public static readonly String CRITICAL = "CRITICAL";// int
		public static readonly String DEF_FIRE = "DEF_FIRE";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
		public static readonly String DIZZY_GATE = "DIZZY_GATE";// int
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String MAGIC_GATE = "MAGIC_GATE";// int
		public static readonly String MAXHP = "MAXHP";// int
		public static readonly String MAXMP = "MAXMP";// int
		public static readonly String MAXSP = "MAXSP";// int
		public static readonly String MOVE_GATE = "MOVE_GATE";// int
		public static readonly String MOVE_SPEED = "MOVE_SPEED";// int
		public static readonly String MP = "MP";// int
		public static readonly String MPREGEN = "MPREGEN";// int
		public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";// int
		public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE";// int
		public static readonly String SKILL_GATE = "SKILL_GATE";// int
		public static readonly String SP = "SP";// int
		public static readonly String SPREGEN = "SPREGEN";// int
		public static readonly String SUCKBLOOD = "SUCKBLOOD";// int
		// Record

	}
	public class Equip
	{
		//Class name
		public static readonly String ThisName = "Equip";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String BuyPrice = "BuyPrice";// int
		public static readonly String Desc = "Desc";// string
		public static readonly String DropPrePath = "DropPrePath";// string
		public static readonly String EffectData = "EffectData";// string
		public static readonly String EnchantmentBuffList = "EnchantmentBuffList";// string
		public static readonly String Icon = "Icon";// string
		public static readonly String IntensiveBuffList = "IntensiveBuffList";// string
		public static readonly String ItemSubType = "ItemSubType";// int
		public static readonly String ItemType = "ItemType";// int
		public static readonly String Job = "Job";// string
		public static readonly String Level = "Level";// int
		public static readonly String PrefabPath = "PrefabPath";// string
		public static readonly String Quality = "Quality";// int
		public static readonly String SalePrice = "SalePrice";// int
		public static readonly String Sex = "Sex";// int
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SuitBuffID = "SuitBuffID";// string
		public static readonly String SuitID = "SuitID";// int
		// Record

	}
	public class Guild
	{
		//Class name
		public static readonly String ThisName = "Guild";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AutoRecruit = "AutoRecruit";// string
		public static readonly String DismissTime = "DismissTime";// int
		public static readonly String EctypID = "EctypID";// int
		public static readonly String EctypIDGroup = "EctypIDGroup";// int
		public static readonly String EctypServer = "EctypServer";// int
		public static readonly String GuildAD = "GuildAD";// string
		public static readonly String GuildContinueDay = "GuildContinueDay";// int
		public static readonly String GuildCreateTime = "GuildCreateTime";// int
		public static readonly String GuildCreateter = "GuildCreateter";// int
		public static readonly String GuildDesc = "GuildDesc";// string
		public static readonly String GuildExp = "GuildExp";// int
		public static readonly String GuildHonor = "GuildHonor";// int
		public static readonly String GuildID = "GuildID";// object
		public static readonly String GuildIcon = "GuildIcon";// int
		public static readonly String GuildLevel = "GuildLevel";// int
		public static readonly String GuildMemeberCount = "GuildMemeberCount";// int
		public static readonly String GuildMemeberMaxCount = "GuildMemeberMaxCount";// int
		public static readonly String GuildMoney = "GuildMoney";// int
		public static readonly String GuildStatus = "GuildStatus";// int
		public static readonly String KingWarResource = "KingWarResource";// int
		public static readonly String Name = "Name";// string
		public static readonly String PresidentID = "PresidentID";// object
		public static readonly String PresidentName = "PresidentName";// string
		public static readonly String Rank = "Rank";// int
		public static readonly String RecruitAD = "RecruitAD";// string
		public static readonly String RecruitLevel = "RecruitLevel";// int
		// Record
		public class GuildAppyList
		{
			//Class name
			public static readonly String ThisName = "GuildAppyList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Power = 6;//int

		}
		public class GuildBoss
		{
			//Class name
			public static readonly String ThisName = "GuildBoss";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Offline = 6;//int
			public const int Power = 7;//int

		}
		public class GuildEvent
		{
			//Class name
			public static readonly String ThisName = "GuildEvent";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Offline = 6;//int
			public const int Power = 7;//int
			public const int EventID = 8;//int
			public const int EventTime = 9;//int
			public const int Context = 10;//string

		}
		public class GuildHouse
		{
			//Class name
			public static readonly String ThisName = "GuildHouse";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Offline = 6;//int
			public const int Power = 7;//int

		}
		public class GuildMemberList
		{
			//Class name
			public static readonly String ThisName = "GuildMemberList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int Receive = 5;//int
			public const int VIP = 6;//int
			public const int Online = 7;//int
			public const int Power = 8;//int
			public const int Title = 9;//int
			public const int GameID = 10;//int
			public const int JoinTime = 11;//int
			public const int Contribution = 12;//int
			public const int AllContribution = 13;//int

		}
		public class GuildSkill
		{
			//Class name
			public static readonly String ThisName = "GuildSkill";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Offline = 6;//int
			public const int Power = 7;//int

		}

	}
	public class GuildConfig
	{
		//Class name
		public static readonly String ThisName = "GuildConfig";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ConditionPlayerLevel = "ConditionPlayerLevel";// int
		public static readonly String ConditionPlayerVIP = "ConditionPlayerVIP";// int
		public static readonly String CostMoney = "CostMoney";// int
		public static readonly String DismissTime = "DismissTime";// int
		public static readonly String GuildLevel = "GuildLevel";// int
		public static readonly String MaxMember = "MaxMember";// int
		// Record

	}
	public class GuildJob
	{
		//Class name
		public static readonly String ThisName = "GuildJob";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AcceptApply = "AcceptApply";// object
		public static readonly String ApplyDismiss = "ApplyDismiss";// object
		public static readonly String Appoint = "Appoint";// object
		public static readonly String Demise = "Demise";// object
		public static readonly String DenyApply = "DenyApply";// object
		public static readonly String EditAD = "EditAD";// object
		public static readonly String Fire = "Fire";// object
		public static readonly String Job = "Job";// int
		public static readonly String JobCount = "JobCount";// object
		public static readonly String Kickout = "Kickout";// object
		public static readonly String Leave = "Leave";// object
		public static readonly String LevelUp = "LevelUp";// object
		public static readonly String PublishRecruit = "PublishRecruit";// object
		public static readonly String SetRecruit = "SetRecruit";// object
		public static readonly String StopDismiss = "StopDismiss";// object
		// Record

	}
	public class HttpServer
	{
		//Class name
		public static readonly String ThisName = "HttpServer";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ServerID = "ServerID";// int
		public static readonly String WebPort = "WebPort";// int
		public static readonly String WebRootPath = "WebRootPath";// string
		// Record

	}
	public class IObject
	{
		//Class name
		public static readonly String ThisName = "IObject";
		// Property
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Record

	}
	public class InitProperty
	{
		//Class name
		public static readonly String ThisName = "InitProperty";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String EffectData = "EffectData";// string
		public static readonly String Job = "Job";// int
		public static readonly String Level = "Level";// int
		public static readonly String ModelPtah = "ModelPtah";// string
		public static readonly String SkillIDRef = "SkillIDRef";// string
		// Record

	}
	public class Item
	{
		//Class name
		public static readonly String ThisName = "Item";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AwardData = "AwardData";// string
		public static readonly String AwardProperty = "AwardProperty";// int
		public static readonly String BuyPrice = "BuyPrice";// int
		public static readonly String ConsumeData = "ConsumeData";// string
		public static readonly String CoolDownTime = "CoolDownTime";// float
		public static readonly String DescID = "DescID";// string
		public static readonly String EffectData = "EffectData";// string
		public static readonly String ExpiredType = "ExpiredType";// int
		public static readonly String Extend = "Extend";// string
		public static readonly String HeroTye = "HeroTye";// int
		public static readonly String Icon = "Icon";// string
		public static readonly String ItemSubType = "ItemSubType";// int
		public static readonly String ItemType = "ItemType";// int
		public static readonly String Job = "Job";// string
		public static readonly String Level = "Level";// int
		public static readonly String OverlayCount = "OverlayCount";// int
		public static readonly String Quality = "Quality";// int
		public static readonly String SalePrice = "SalePrice";// int
		public static readonly String Script = "Script";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SpriteFile = "SpriteFile";// string
		// Record

	}
	public class Language
	{
		//Class name
		public static readonly String ThisName = "Language";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Chinese = "Chinese";// string
		public static readonly String English = "English";// string
		// Record

	}
	public class Map
	{
		//Class name
		public static readonly String ThisName = "Map";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String InComeDiamond = "InComeDiamond";// int
		public static readonly String InComeGold = "InComeGold";// int
		public static readonly String InComeOil = "InComeOil";// int
		public static readonly String MapLevel = "MapLevel";// int
		public static readonly String MaxCount = "MaxCount";// int
		// Record
		public class Station
		{
			//Class name
			public static readonly String ThisName = "Station";
			public const int GUID = 0;//object
			public const int GuildID = 1;//object
			public const int GuildName = 2;//string
			public const int Level = 3;//int
			public const int Title = 4;//string
			public const int Slogan = 5;//string
			public const int State = 6;//int
			public const int CurMemberCount = 7;//int
			public const int WinCount = 8;//int

		}

	}
	public class NPC
	{
		//Class name
		public static readonly String ThisName = "NPC";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ATK_FIRE = "ATK_FIRE";// int
		public static readonly String ATK_ICE = "ATK_ICE";// int
		public static readonly String ATK_LIGHT = "ATK_LIGHT";// int
		public static readonly String ATK_POISON = "ATK_POISON";// int
		public static readonly String ATK_SPEED = "ATK_SPEED";// int
		public static readonly String ATK_VALUE = "ATK_VALUE";// int
		public static readonly String ATK_WIND = "ATK_WIND";// int
		public static readonly String AtkDis = "AtkDis";// float
		public static readonly String BUFF_GATE = "BUFF_GATE";// int
		public static readonly String CRITICAL = "CRITICAL";// int
		public static readonly String Camp = "Camp";// int
		public static readonly String Climb = "Climb";// int
		public static readonly String ConsumeData = "ConsumeData";// string
		public static readonly String DEF_FIRE = "DEF_FIRE";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
		public static readonly String DIZZY_GATE = "DIZZY_GATE";// int
		public static readonly String Diamond = "Diamond";// int
		public static readonly String DropPackList = "DropPackList";// string
		public static readonly String EXP = "EXP";// int
		public static readonly String EffectData = "EffectData";// string
		public static readonly String EquipIDRef = "EquipIDRef";// string
		public static readonly String Gold = "Gold";// int
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String Height = "Height";// float
		public static readonly String HeroType = "HeroType";// int
		public static readonly String Icon = "Icon";// string
		public static readonly String Idle = "Idle";// string
		public static readonly String LastAttacker = "LastAttacker";// object
		public static readonly String Level = "Level";// int
		public static readonly String MAGIC_GATE = "MAGIC_GATE";// int
		public static readonly String MAXHP = "MAXHP";// int
		public static readonly String MAXMP = "MAXMP";// int
		public static readonly String MAXSP = "MAXSP";// int
		public static readonly String MOVE_GATE = "MOVE_GATE";// int
		public static readonly String MOVE_SPEED = "MOVE_SPEED";// int
		public static readonly String MP = "MP";// int
		public static readonly String MPREGEN = "MPREGEN";// int
		public static readonly String MasterID = "MasterID";// object
		public static readonly String NPCType = "NPCType";// int
		public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";// int
		public static readonly String Prefab = "Prefab";// string
		public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE";// int
		public static readonly String SKILL_GATE = "SKILL_GATE";// int
		public static readonly String SP = "SP";// int
		public static readonly String SPREGEN = "SPREGEN";// int
		public static readonly String SUCKBLOOD = "SUCKBLOOD";// int
		public static readonly String SeedID = "SeedID";// string
		public static readonly String ShowCard = "ShowCard";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SkillAttack = "SkillAttack";// string
		public static readonly String SkillNormal = "SkillNormal";// string
		public static readonly String SkillTHUMP = "SkillTHUMP";// string
		public static readonly String SpriteFile = "SpriteFile";// string
		public static readonly String TargetX = "TargetX";// float
		public static readonly String TargetY = "TargetY";// float
		public static readonly String VIPEXP = "VIPEXP";// int
		public static readonly String Width = "Width";// float
		// Record

	}
	public class NoSqlServer
	{
		//Class name
		public static readonly String ThisName = "NoSqlServer";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Auth = "Auth";// string
		public static readonly String IP = "IP";// string
		public static readonly String Port = "Port";// int
		public static readonly String ServerID = "ServerID";// int
		// Record

	}
	public class Player
	{
		//Class name
		public static readonly String ThisName = "Player";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ATK_FIRE = "ATK_FIRE";// int
		public static readonly String ATK_ICE = "ATK_ICE";// int
		public static readonly String ATK_LIGHT = "ATK_LIGHT";// int
		public static readonly String ATK_POISON = "ATK_POISON";// int
		public static readonly String ATK_PVP = "ATK_PVP";// int
		public static readonly String ATK_SPEED = "ATK_SPEED";// int
		public static readonly String ATK_VALUE = "ATK_VALUE";// int
		public static readonly String ATK_WIND = "ATK_WIND";// int
		public static readonly String Account = "Account";// string
		public static readonly String BUFF_GATE = "BUFF_GATE";// int
		public static readonly String CRITICAL = "CRITICAL";// int
		public static readonly String Camp = "Camp";// int
		public static readonly String ConnectKey = "ConnectKey";// string
		public static readonly String DEF_FIRE = "DEF_FIRE";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_PVP = "DEF_PVP";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
		public static readonly String DIZZY_GATE = "DIZZY_GATE";// int
		public static readonly String Diamond = "Diamond";// int
		public static readonly String EXP = "EXP";// int
		public static readonly String FightHero = "FightHero";// object
		public static readonly String FightHeroCnfID = "FightHeroCnfID";// string
		public static readonly String FightOppnent = "FightOppnent";// object
		public static readonly String FirstTarget = "FirstTarget";// object
		public static readonly String GMLevel = "GMLevel";// int
		public static readonly String GambleDiamond = "GambleDiamond";// int
		public static readonly String GambleGold = "GambleGold";// int
		public static readonly String GameID = "GameID";// int
		public static readonly String GateID = "GateID";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String GuildID = "GuildID";// object
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String HeroPos1 = "HeroPos1";// object
		public static readonly String HeroPos2 = "HeroPos2";// object
		public static readonly String HeroPos3 = "HeroPos3";// object
		public static readonly String HomeSceneID = "HomeSceneID";// int
		public static readonly String Job = "Job";// int
		public static readonly String LastOfflineTime = "LastOfflineTime";// object
		public static readonly String Level = "Level";// int
		public static readonly String MAGIC_GATE = "MAGIC_GATE";// int
		public static readonly String MAXEXP = "MAXEXP";// int
		public static readonly String MAXHP = "MAXHP";// int
		public static readonly String MAXMP = "MAXMP";// int
		public static readonly String MAXSP = "MAXSP";// int
		public static readonly String MOVE_GATE = "MOVE_GATE";// int
		public static readonly String MOVE_SPEED = "MOVE_SPEED";// int
		public static readonly String MP = "MP";// int
		public static readonly String MPREGEN = "MPREGEN";// int
		public static readonly String Name = "Name";// string
		public static readonly String OnlineCount = "OnlineCount";// int
		public static readonly String OnlineTime = "OnlineTime";// object
		public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";// int
		public static readonly String PrefabPath = "PrefabPath";// string
		public static readonly String REFLECTDAMAGE = "REFLECTDAMAGE";// int
		public static readonly String RELIVE_SOUL = "RELIVE_SOUL";// int
		public static readonly String Race = "Race";// int
		public static readonly String SKILL_GATE = "SKILL_GATE";// int
		public static readonly String SP = "SP";// int
		public static readonly String SPREGEN = "SPREGEN";// int
		public static readonly String SUCKBLOOD = "SUCKBLOOD";// int
		public static readonly String Sex = "Sex";// int
		public static readonly String ShowName = "ShowName";// string
		public static readonly String Skill1 = "Skill1";// string
		public static readonly String Skill2 = "Skill2";// string
		public static readonly String Skill3 = "Skill3";// string
		public static readonly String TeamID = "TeamID";// object
		public static readonly String TotalLineTime = "TotalLineTime";// object
		public static readonly String TotalTime = "TotalTime";// int
		public static readonly String VIPEXP = "VIPEXP";// int
		public static readonly String VIPLevel = "VIPLevel";// int
		public static readonly String ViewOppnent = "ViewOppnent";// object
		// Record
		public class BagEquipList
		{
			//Class name
			public static readonly String ThisName = "BagEquipList";
			public const int GUID = 0;//object
			public const int WearGUID = 1;//object
			public const int ConfigID = 2;//string
			public const int ExpiredType = 3;//int
			public const int Date = 4;//int
			public const int RandPropertyID = 5;//string
			public const int SlotCount = 6;//int
			public const int InlayStone1 = 7;//string
			public const int InlayStone2 = 8;//string
			public const int InlayStone3 = 9;//string
			public const int InlayStone4 = 10;//string
			public const int InlayStone5 = 11;//string
			public const int InlayStone6 = 12;//string
			public const int InlayStone7 = 13;//string
			public const int InlayStone8 = 14;//string
			public const int InlayStone9 = 15;//string
			public const int InlayStone10 = 16;//string
			public const int IntensifyLevel = 17;//string
			public const int ElementLevel1_FIRE = 18;//int
			public const int ElementLevel2_LIGHT = 19;//int
			public const int ElementLevel3_Wind = 20;//int
			public const int ElementLevel4_ICE = 21;//int
			public const int ElementLevel5_POISON = 22;//int

		}
		public class BagItemList
		{
			//Class name
			public static readonly String ThisName = "BagItemList";
			public const int ConfigID = 0;//string
			public const int ItemCount = 1;//int
			public const int Bound = 2;//int
			public const int ExpiredType = 3;//int
			public const int Date = 4;//int

		}
		public class BuildingList
		{
			//Class name
			public static readonly String ThisName = "BuildingList";
			public const int BuildingCnfID = 0;//string
			public const int BuildingGUID = 1;//object
			public const int State = 2;//int
			public const int Pos = 3;//vector3
			public const int StateEndTime = 4;//int

		}
		public class BuildingListProduce
		{
			//Class name
			public static readonly String ThisName = "BuildingListProduce";
			public const int BuildingCnfID = 0;//string
			public const int BuildingGUID = 1;//object
			public const int State = 2;//int
			public const int Pos = 3;//vector3
			public const int StateEndTime = 4;//int

		}
		public class CommValue
		{
			//Class name
			public static readonly String ThisName = "CommValue";
			public const int SUCKBLOOD = 0;//int
			public const int REFLECTDAMAGE = 1;//int
			public const int CRITICAL = 2;//int
			public const int MAXHP = 3;//int
			public const int MAXMP = 4;//int
			public const int MAXSP = 5;//int
			public const int HPREGEN = 6;//int
			public const int SPREGEN = 7;//int
			public const int MPREGEN = 8;//int
			public const int ATK_VALUE = 9;//int
			public const int DEF_VALUE = 10;//int
			public const int MOVE_SPEED = 11;//int
			public const int ATK_SPEED = 12;//int
			public const int ATK_FIRE = 13;//int
			public const int ATK_LIGHT = 14;//int
			public const int ATK_WIND = 15;//int
			public const int ATK_ICE = 16;//int
			public const int ATK_POISON = 17;//int
			public const int DEF_FIRE = 18;//int
			public const int DEF_LIGHT = 19;//int
			public const int DEF_WIND = 20;//int
			public const int DEF_ICE = 21;//int
			public const int DEF_POISON = 22;//int
			public const int DIZZY_GATE = 23;//int
			public const int MOVE_GATE = 24;//int
			public const int SKILL_GATE = 25;//int
			public const int PHYSICAL_GATE = 26;//int
			public const int MAGIC_GATE = 27;//int
			public const int BUFF_GATE = 28;//int

		}
		public class DropItemList
		{
			//Class name
			public static readonly String ThisName = "DropItemList";
			public const int GUID = 0;//object
			public const int ConfigID = 1;//string
			public const int ItemCount = 2;//int

		}
		public class HeroValue
		{
			//Class name
			public static readonly String ThisName = "HeroValue";
			public const int SUCKBLOOD = 0;//int
			public const int REFLECTDAMAGE = 1;//int
			public const int CRITICAL = 2;//int
			public const int MAXHP = 3;//int
			public const int MAXMP = 4;//int
			public const int MAXSP = 5;//int
			public const int HPREGEN = 6;//int
			public const int SPREGEN = 7;//int
			public const int MPREGEN = 8;//int
			public const int ATK_VALUE = 9;//int
			public const int DEF_VALUE = 10;//int
			public const int MOVE_SPEED = 11;//int
			public const int ATK_SPEED = 12;//int
			public const int ATK_FIRE = 13;//int
			public const int ATK_LIGHT = 14;//int
			public const int ATK_WIND = 15;//int
			public const int ATK_ICE = 16;//int
			public const int ATK_POISON = 17;//int
			public const int DEF_FIRE = 18;//int
			public const int DEF_LIGHT = 19;//int
			public const int DEF_WIND = 20;//int
			public const int DEF_ICE = 21;//int
			public const int DEF_POISON = 22;//int
			public const int DIZZY_GATE = 23;//int
			public const int MOVE_GATE = 24;//int
			public const int SKILL_GATE = 25;//int
			public const int PHYSICAL_GATE = 26;//int
			public const int MAGIC_GATE = 27;//int
			public const int BUFF_GATE = 28;//int

		}
		public class PlayerHero
		{
			//Class name
			public static readonly String ThisName = "PlayerHero";
			public const int GUID = 0;//object
			public const int ConfigID = 1;//string
			public const int Activated = 2;//int
			public const int Level = 3;//int
			public const int Exp = 4;//int
			public const int Star = 5;//int
			public const int Equip1 = 6;//object
			public const int Equip2 = 7;//object
			public const int Equip3 = 8;//object
			public const int Equip4 = 9;//object
			public const int Equip5 = 10;//object
			public const int Equip6 = 11;//object
			public const int Talent1 = 12;//string
			public const int Talent2 = 13;//string
			public const int Talent3 = 14;//string
			public const int Talent4 = 15;//string
			public const int Talent5 = 16;//string
			public const int Skill1 = 17;//string
			public const int Skill2 = 18;//string
			public const int Skill3 = 19;//string
			public const int Skill4 = 20;//string
			public const int Skill5 = 21;//string
			public const int FightSkill = 22;//string

		}
		public class TaskList
		{
			//Class name
			public static readonly String ThisName = "TaskList";
			public const int TaskID = 0;//string
			public const int TaskStatus = 1;//int
			public const int Process = 2;//int

		}
		public class TaskMonsterList
		{
			//Class name
			public static readonly String ThisName = "TaskMonsterList";
			public const int MonsterID = 0;//string
			public const int CurrentKillCount = 1;//int
			public const int RequireKillCount = 2;//int
			public const int TaskID = 3;//string

		}

	}
	public class Scene
	{
		//Class name
		public static readonly String ThisName = "Scene";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ActorID = "ActorID";// int
		public static readonly String BossSoundList = "BossSoundList";// string
		public static readonly String CamOffestPos = "CamOffestPos";// string
		public static readonly String CamOffestRot = "CamOffestRot";// string
		public static readonly String FilePath = "FilePath";// string
		public static readonly String LoadingUI = "LoadingUI";// string
		public static readonly String MaxGroup = "MaxGroup";// int
		public static readonly String MaxGroupPlayers = "MaxGroupPlayers";// int
		public static readonly String NavigationResPath = "NavigationResPath";// string
		public static readonly String RelivePos = "RelivePos";// string
		public static readonly String SceneName = "SceneName";// string
		public static readonly String SceneShowName = "SceneShowName";// string
		public static readonly String SoundList = "SoundList";// string
		public static readonly String Tile = "Tile";// int
		public static readonly String Type = "Type";// int
		public static readonly String Width = "Width";// int
		// Record

	}
	public class Security
	{
		//Class name
		public static readonly String ThisName = "Security";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String SecurityData = "SecurityData";// string
		// Record

	}
	public class Server
	{
		//Class name
		public static readonly String ThisName = "Server";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Area = "Area";// int
		public static readonly String CpuCount = "CpuCount";// int
		public static readonly String IP = "IP";// string
		public static readonly String MaxOnline = "MaxOnline";// int
		public static readonly String Name = "Name";// string
		public static readonly String Port = "Port";// int
		public static readonly String ServerID = "ServerID";// int
		public static readonly String Type = "Type";// int
		// Record

	}
	public class Shop
	{
		//Class name
		public static readonly String ThisName = "Shop";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Count = "Count";// int
		public static readonly String Diamond = "Diamond";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String ItemID = "ItemID";// string
		public static readonly String Level = "Level";// int
		public static readonly String Steel = "Steel";// int
		public static readonly String Stone = "Stone";// int
		public static readonly String Type = "Type";// int
		// Record

	}
	public class Skill
	{
		//Class name
		public static readonly String ThisName = "Skill";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AnimaState = "AnimaState";// string
		public static readonly String AtkDis = "AtkDis";// float
		public static readonly String ConsumeProperty = "ConsumeProperty";// string
		public static readonly String ConsumeType = "ConsumeType";// int
		public static readonly String ConsumeValue = "ConsumeValue";// string
		public static readonly String CoolDownTime = "CoolDownTime";// float
		public static readonly String DamageCount = "DamageCount";// int
		public static readonly String DamageDistance = "DamageDistance";// float
		public static readonly String DamageIntervalTime = "DamageIntervalTime";// float
		public static readonly String DamageProperty = "DamageProperty";// string
		public static readonly String DamageType = "DamageType";// int
		public static readonly String DamageValue = "DamageValue";// int
		public static readonly String DefaultHitTime = "DefaultHitTime";// string
		public static readonly String Desc = "Desc";// string
		public static readonly String GetBuffList = "GetBuffList";// string
		public static readonly String Icon = "Icon";// string
		public static readonly String NewObject = "NewObject";// string
		public static readonly String NextID = "NextID";// string
		public static readonly String PlayerSkill = "PlayerSkill";// int
		public static readonly String RequireDistance = "RequireDistance";// float
		public static readonly String SendBuffList = "SendBuffList";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SkillType = "SkillType";// int
		public static readonly String SpriteFile = "SpriteFile";// string
		public static readonly String TargetType = "TargetType";// int
		// Record

	}
	public class SqlServer
	{
		//Class name
		public static readonly String ThisName = "SqlServer";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String IP = "IP";// string
		public static readonly String Port = "Port";// int
		public static readonly String ServerID = "ServerID";// int
		public static readonly String SqlIP = "SqlIP";// string
		public static readonly String SqlName = "SqlName";// string
		public static readonly String SqlPort = "SqlPort";// int
		public static readonly String SqlPwd = "SqlPwd";// string
		public static readonly String SqlUser = "SqlUser";// string
		// Record

	}
	public class Talent
	{
		//Class name
		public static readonly String ThisName = "Talent";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Desc = "Desc";// string
		public static readonly String EffectData = "EffectData";// string
		public static readonly String Icon = "Icon";// string
		public static readonly String NextID = "NextID";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SpriteFile = "SpriteFile";// string
		// Record

	}
	public class Task
	{
		//Class name
		public static readonly String ThisName = "Task";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AwardExp = "AwardExp";// int
		public static readonly String AwardGold = "AwardGold";// int
		public static readonly String AwardPack = "AwardPack";// string
		public static readonly String Desc = "Desc";// string
		public static readonly String KillCount = "KillCount";// string
		public static readonly String KillMonsterName = "KillMonsterName";// string
		public static readonly String LevelReq = "LevelReq";// string
		public static readonly String NextTaskID = "NextTaskID";// string
		public static readonly String Type = "Type";// int
		// Record

	}
	public class Team
	{
		//Class name
		public static readonly String ThisName = "Team";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Captain = "Captain";// object
		public static readonly String PresidentName = "PresidentName";// string
		// Record
		public class ApplyList
		{
			//Class name
			public static readonly String ThisName = "ApplyList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int VIP = 5;//int
			public const int Power = 6;//int

		}
		public class MemberList
		{
			//Class name
			public static readonly String ThisName = "MemberList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int Job = 3;//int
			public const int Donation = 4;//int
			public const int Receive = 5;//int
			public const int VIP = 6;//int
			public const int Online = 7;//int
			public const int Title = 8;//int
			public const int GameID = 9;//int

		}

	}

}