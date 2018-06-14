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
		public static readonly String ATK_DARK = "ATK_DARK";// int
		public static readonly String ATK_FIRE = "ATK_FIRE";// int
		public static readonly String ATK_ICE = "ATK_ICE";// int
		public static readonly String ATK_LIGHT = "ATK_LIGHT";// int
		public static readonly String ATK_POISON = "ATK_POISON";// int
		public static readonly String ATK_SPEED = "ATK_SPEED";// int
		public static readonly String ATK_VALUE = "ATK_VALUE";// int
		public static readonly String ATK_WIND = "ATK_WIND";// int
		public static readonly String BUFF_GATE = "BUFF_GATE";// int
		public static readonly String CRITICAL = "CRITICAL";// int
		public static readonly String DEF_DARK = "DEF_DARK";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
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
	public class GM
	{
		//Class name
		public static readonly String ThisName = "GM";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Level = "Level";// int
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
		public static readonly String Guild_AD = "Guild_AD";// string
		public static readonly String Guild_ContinueDay = "Guild_ContinueDay";// int
		public static readonly String Guild_CreateTime = "Guild_CreateTime";// int
		public static readonly String Guild_Desc = "Guild_Desc";// string
		public static readonly String Guild_EctypID = "Guild_EctypID";// int
		public static readonly String Guild_EctypIDGroup = "Guild_EctypIDGroup";// int
		public static readonly String Guild_EctypServer = "Guild_EctypServer";// object
		public static readonly String Guild_Exp = "Guild_Exp";// int
		public static readonly String Guild_Gold = "Guild_Gold";// int
		public static readonly String Guild_Honor = "Guild_Honor";// int
		public static readonly String Guild_ID = "Guild_ID";// object
		public static readonly String Guild_Icon = "Guild_Icon";// string
		public static readonly String Guild_KingWarResource = "Guild_KingWarResource";// int
		public static readonly String Guild_Level = "Guild_Level";// int
		public static readonly String Guild_MemeberCount = "Guild_MemeberCount";// int
		public static readonly String Guild_MemeberMaxCount = "Guild_MemeberMaxCount";// int
		public static readonly String Guild_Name = "Guild_Name";// string
		public static readonly String Guild_PresidentID = "Guild_PresidentID";// object
		public static readonly String Guild_PresidentName = "Guild_PresidentName";// string
		public static readonly String Guild_Rank = "Guild_Rank";// int
		public static readonly String Guild_Status = "Guild_Status";// int
		// Record
		public class Guild_AppyList
		{
			//Class name
			public static readonly String ThisName = "Guild_AppyList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int VIP = 3;//int

		}
		public class Guild_Building
		{
			//Class name
			public static readonly String ThisName = "Guild_Building";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int

		}
		public class Guild_MemberList
		{
			//Class name
			public static readonly String ThisName = "Guild_MemberList";
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
			public const int LastTime = 13;//string
			public const int AllContribution = 14;//int

		}
		public class Guild_WarList
		{
			//Class name
			public static readonly String ThisName = "Guild_WarList";
			public const int GUID = 0;//object
			public const int Level = 1;//int

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
		public static readonly String DismissTime = "DismissTime";// int
		public static readonly String GuildLevel = "GuildLevel";// int
		public static readonly String Level = "Level";// int
		public static readonly String MaxMember = "MaxMember";// int
		public static readonly String Money = "Money";// int
		public static readonly String SkillNum = "SkillNum";// int
		public static readonly String VIP = "VIP";// int
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
		public static readonly String HeroType = "HeroType";// int
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
		public static readonly String AIOwnerID = "AIOwnerID";// object
		public static readonly String ATK_DARK = "ATK_DARK";// int
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
		public static readonly String DEF_DARK = "DEF_DARK";// int
		public static readonly String DEF_FIRE = "DEF_FIRE";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
		public static readonly String DIZZY_GATE = "DIZZY_GATE";// int
		public static readonly String DescID = "DescID";// string
		public static readonly String Diamond = "Diamond";// int
		public static readonly String DropPackList = "DropPackList";// string
		public static readonly String DropProbability = "DropProbability";// int
		public static readonly String EXP = "EXP";// int
		public static readonly String EffectData = "EffectData";// string
		public static readonly String Gold = "Gold";// int
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String Height = "Height";// float
		public static readonly String HeroStar = "HeroStar";// int
		public static readonly String Icon = "Icon";// string
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
		public class Cooldown
		{
			//Class name
			public static readonly String ThisName = "Cooldown";
			public const int SkillID = 0;//string
			public const int Time = 1;//int

		}

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
		public static readonly String ATK_DARK = "ATK_DARK";// int
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
		public static readonly String Cup = "Cup";// int
		public static readonly String DEF_DARK = "DEF_DARK";// int
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
		public static readonly String FightingOpponent = "FightingOpponent";// object
		public static readonly String FightingStar = "FightingStar";// int
		public static readonly String FirstTarget = "FirstTarget";// object
		public static readonly String GMLevel = "GMLevel";// int
		public static readonly String GambleDiamond = "GambleDiamond";// int
		public static readonly String GambleGold = "GambleGold";// int
		public static readonly String GameID = "GameID";// int
		public static readonly String GateID = "GateID";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String GuildID = "GuildID";// object
		public static readonly String Guild_AD = "Guild_AD";// string
		public static readonly String Guild_ContinueDay = "Guild_ContinueDay";// int
		public static readonly String Guild_CreateTime = "Guild_CreateTime";// int
		public static readonly String Guild_Desc = "Guild_Desc";// string
		public static readonly String Guild_EctypID = "Guild_EctypID";// int
		public static readonly String Guild_EctypIDGroup = "Guild_EctypIDGroup";// int
		public static readonly String Guild_EctypServer = "Guild_EctypServer";// object
		public static readonly String Guild_Exp = "Guild_Exp";// int
		public static readonly String Guild_Gold = "Guild_Gold";// int
		public static readonly String Guild_Honor = "Guild_Honor";// int
		public static readonly String Guild_ID = "Guild_ID";// object
		public static readonly String Guild_Icon = "Guild_Icon";// string
		public static readonly String Guild_KingWarResource = "Guild_KingWarResource";// int
		public static readonly String Guild_Level = "Guild_Level";// int
		public static readonly String Guild_MemeberCount = "Guild_MemeberCount";// int
		public static readonly String Guild_MemeberMaxCount = "Guild_MemeberMaxCount";// int
		public static readonly String Guild_Name = "Guild_Name";// string
		public static readonly String Guild_PresidentID = "Guild_PresidentID";// object
		public static readonly String Guild_PresidentName = "Guild_PresidentName";// string
		public static readonly String Guild_Rank = "Guild_Rank";// int
		public static readonly String Guild_Status = "Guild_Status";// int
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String Head = "Head";// string
		public static readonly String Hero1BornPos = "Hero1BornPos";// vector3
		public static readonly String Hero1HP = "Hero1HP";// int
		public static readonly String Hero2BornPos = "Hero2BornPos";// vector3
		public static readonly String Hero2HP = "Hero2HP";// int
		public static readonly String Hero3BornPos = "Hero3BornPos";// vector3
		public static readonly String Hero3HP = "Hero3HP";// int
		public static readonly String HeroID1 = "HeroID1";// object
		public static readonly String HeroID2 = "HeroID2";// object
		public static readonly String HeroID3 = "HeroID3";// object
		public static readonly String HeroLevel = "HeroLevel";// int
		public static readonly String HeroPos1CnfID = "HeroPos1CnfID";// string
		public static readonly String HeroPos1Star = "HeroPos1Star";// int
		public static readonly String HeroPos2CnfID = "HeroPos2CnfID";// string
		public static readonly String HeroPos2Star = "HeroPos2Star";// int
		public static readonly String HeroPos3CnfID = "HeroPos3CnfID";// string
		public static readonly String HeroPos3Star = "HeroPos3Star";// int
		public static readonly String HomeSceneID = "HomeSceneID";// int
		public static readonly String Item1 = "Item1";// string
		public static readonly String Item1Count = "Item1Count";// int
		public static readonly String Item1UsedCount = "Item1UsedCount";// int
		public static readonly String Item2 = "Item2";// string
		public static readonly String Item2Count = "Item2Count";// int
		public static readonly String Item2UsedCount = "Item2UsedCount";// int
		public static readonly String Item3 = "Item3";// string
		public static readonly String Item3Count = "Item3Count";// int
		public static readonly String Item3UsedCount = "Item3UsedCount";// int
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
		public static readonly String OpponentCup = "OpponentCup";// int
		public static readonly String OpponentDiamond = "OpponentDiamond";// int
		public static readonly String OpponentGold = "OpponentGold";// int
		public static readonly String OpponentHead = "OpponentHead";// string
		public static readonly String OpponentHero1 = "OpponentHero1";// string
		public static readonly String OpponentHero1BornPos = "OpponentHero1BornPos";// vector3
		public static readonly String OpponentHero1Star = "OpponentHero1Star";// int
		public static readonly String OpponentHero2 = "OpponentHero2";// string
		public static readonly String OpponentHero2BornPos = "OpponentHero2BornPos";// vector3
		public static readonly String OpponentHero2Star = "OpponentHero2Star";// int
		public static readonly String OpponentHero3 = "OpponentHero3";// string
		public static readonly String OpponentHero3BornPos = "OpponentHero3BornPos";// vector3
		public static readonly String OpponentHero3Star = "OpponentHero3Star";// int
		public static readonly String OpponentLevel = "OpponentLevel";// int
		public static readonly String OpponentName = "OpponentName";// string
		public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";// int
		public static readonly String PVPType = "PVPType";// int
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
		public static readonly String ViewOpponent = "ViewOpponent";// object
		public static readonly String WarEventTime = "WarEventTime";// int
		public static readonly String WarID = "WarID";// object
		public static readonly String WonCup = "WonCup";// int
		public static readonly String WonDiamond = "WonDiamond";// int
		public static readonly String WonGold = "WonGold";// int
		// Record
		public class AttackList
		{
			//Class name
			public static readonly String ThisName = "AttackList";
			public const int WarID = 0;//object
			public const int AttackerID = 1;//object
			public const int BeAttackerID = 2;//object
			public const int AttackerName = 3;//string
			public const int BeAttackerName = 4;//string
			public const int AttackerLevel = 5;//int
			public const int BeAttackerLevel = 6;//int
			public const int AttackerCup = 7;//int
			public const int BeAttackerCup = 8;//int
			public const int AttackerHero1 = 9;//string
			public const int AttackerHero2 = 10;//string
			public const int AttackerHero3 = 11;//string
			public const int AttackerHero1Star = 12;//int
			public const int AttackerHero2Star = 13;//int
			public const int AttackerHero3Star = 14;//int
			public const int BeAttackerHero1 = 15;//string
			public const int BeAttackerHero2 = 16;//string
			public const int BeAttackerHero3 = 17;//string
			public const int BeAttackerHero1Star = 18;//int
			public const int BeAttackerHero2Star = 19;//int
			public const int BeAttackerHero3Star = 20;//int
			public const int Gold = 21;//int
			public const int Diamond = 22;//int
			public const int Winner = 23;//object
			public const int WarStar = 24;//int
			public const int EventTime = 25;//int
			public const int CostTime = 26;//int
			public const int Item1 = 27;//string
			public const int Item1Count = 28;//int
			public const int Item2 = 29;//string
			public const int Item2Count = 30;//int
			public const int Item3 = 31;//string
			public const int Item3Count = 32;//int
			public const int BuildingCount = 33;//int
			public const int DestroiedBuilding = 34;//int
			public const int MasterCount = 35;//int
			public const int KilledMasterCount = 36;//int
			public const int KilledHero1 = 37;//int
			public const int KilledHero2 = 38;//int
			public const int KilledHero3 = 39;//int
			public const int DeadHero1 = 40;//int
			public const int DeadHero2 = 41;//int
			public const int DeadHero3 = 42;//int

		}
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
		public class BeAccackList
		{
			//Class name
			public static readonly String ThisName = "BeAccackList";
			public const int WarID = 0;//object
			public const int AttackerID = 1;//object
			public const int BeAttackerID = 2;//object
			public const int AttackerName = 3;//string
			public const int BeAttackerName = 4;//string
			public const int AttackerLevel = 5;//int
			public const int BeAttackerLevel = 6;//int
			public const int AttackerCup = 7;//int
			public const int BeAttackerCup = 8;//int
			public const int AttackerHero1 = 9;//string
			public const int AttackerHero2 = 10;//string
			public const int AttackerHero3 = 11;//string
			public const int AttackerHero1Star = 12;//int
			public const int AttackerHero2Star = 13;//int
			public const int AttackerHero3Star = 14;//int
			public const int BeAttackerHero1 = 15;//string
			public const int BeAttackerHero2 = 16;//string
			public const int BeAttackerHero3 = 17;//string
			public const int BeAttackerHero1Star = 18;//int
			public const int BeAttackerHero2Star = 19;//int
			public const int BeAttackerHero3Star = 20;//int
			public const int Gold = 21;//int
			public const int Diamond = 22;//int
			public const int Winner = 23;//object
			public const int WarStar = 24;//int
			public const int EventTime = 25;//int
			public const int CostTime = 26;//int
			public const int Item1 = 27;//string
			public const int Item1Count = 28;//int
			public const int Item2 = 29;//string
			public const int Item2Count = 30;//int
			public const int Item3 = 31;//string
			public const int Item3Count = 32;//int
			public const int BuildingCount = 33;//int
			public const int DestroiedBuilding = 34;//int
			public const int MasterCount = 35;//int
			public const int KilledMasterCount = 36;//int
			public const int KilledHero1 = 37;//int
			public const int KilledHero2 = 38;//int
			public const int KilledHero3 = 39;//int
			public const int DeadHero1 = 40;//int
			public const int DeadHero2 = 41;//int
			public const int DeadHero3 = 42;//int

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
			public const int ATK_DARK = 15;//int
			public const int ATK_WIND = 16;//int
			public const int ATK_ICE = 17;//int
			public const int ATK_POISON = 18;//int
			public const int DEF_FIRE = 19;//int
			public const int DEF_LIGHT = 20;//int
			public const int DEF_DARK = 21;//int
			public const int DEF_WIND = 22;//int
			public const int DEF_ICE = 23;//int
			public const int DEF_POISON = 24;//int
			public const int DIZZY_GATE = 25;//int
			public const int MOVE_GATE = 26;//int
			public const int SKILL_GATE = 27;//int
			public const int PHYSICAL_GATE = 28;//int
			public const int MAGIC_GATE = 29;//int
			public const int BUFF_GATE = 30;//int

		}
		public class Cooldown
		{
			//Class name
			public static readonly String ThisName = "Cooldown";
			public const int SkillID = 0;//string
			public const int Time = 1;//int

		}
		public class DropItemList
		{
			//Class name
			public static readonly String ThisName = "DropItemList";
			public const int GUID = 0;//object
			public const int ConfigID = 1;//string
			public const int ItemCount = 2;//int
			public const int Postion = 3;//vector3

		}
		public class Guild_AppyList
		{
			//Class name
			public static readonly String ThisName = "Guild_AppyList";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int
			public const int VIP = 3;//int

		}
		public class Guild_Building
		{
			//Class name
			public static readonly String ThisName = "Guild_Building";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int Level = 2;//int

		}
		public class Guild_MemberList
		{
			//Class name
			public static readonly String ThisName = "Guild_MemberList";
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
			public const int LastTime = 13;//string
			public const int AllContribution = 14;//int

		}
		public class Guild_WarList
		{
			//Class name
			public static readonly String ThisName = "Guild_WarList";
			public const int GUID = 0;//object
			public const int Level = 1;//int

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
			public const int ATK_DARK = 15;//int
			public const int ATK_WIND = 16;//int
			public const int ATK_ICE = 17;//int
			public const int ATK_POISON = 18;//int
			public const int DEF_FIRE = 19;//int
			public const int DEF_LIGHT = 20;//int
			public const int DEF_DARK = 21;//int
			public const int DEF_WIND = 22;//int
			public const int DEF_ICE = 23;//int
			public const int DEF_POISON = 24;//int
			public const int DIZZY_GATE = 25;//int
			public const int MOVE_GATE = 26;//int
			public const int SKILL_GATE = 27;//int
			public const int PHYSICAL_GATE = 28;//int
			public const int MAGIC_GATE = 29;//int
			public const int BUFF_GATE = 30;//int

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
			public const int Ice_Talent = 6;//int
			public const int Fire_Talent = 7;//int
			public const int Light_Talent = 8;//int
			public const int Wind_Talent = 9;//int
			public const int Poison_Talent = 10;//int
			public const int Dark_Talent = 11;//int

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
		public class TempItemList
		{
			//Class name
			public static readonly String ThisName = "TempItemList";
			public const int ConfigID = 0;//string
			public const int ItemCount = 1;//int

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
		public static readonly String ResPath = "ResPath";// string
		public static readonly String SceneName = "SceneName";// string
		public static readonly String SceneShowName = "SceneShowName";// string
		public static readonly String SoundList = "SoundList";// string
		public static readonly String TagPos = "TagPos";// string
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