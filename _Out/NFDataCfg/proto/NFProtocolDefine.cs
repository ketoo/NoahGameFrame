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
	public class DescData
	{
		//Class name
		public static readonly String ThisName = "DescData";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String DEF_FIRE = "DEF_FIRE";// int
		public static readonly String DEF_ICE = "DEF_ICE";// int
		public static readonly String DEF_LIGHT = "DEF_LIGHT";// int
		public static readonly String DEF_POISON = "DEF_POISON";// int
		public static readonly String DEF_VALUE = "DEF_VALUE";// int
		public static readonly String DEF_WIND = "DEF_WIND";// int
		public static readonly String DIZZY_GATE = "DIZZY_GATE";// int
		public static readonly String HP = "HP";// int
		public static readonly String HPREGEN = "HPREGEN";// int
		public static readonly String LUCK = "LUCK";// int
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
			public const int LUCK = 31;//int

		}

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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Level = "Level";// int
		// Record

	}
	public class Group
	{
		//Class name
		public static readonly String ThisName = "Group";
		// IObject
		public static readonly String ClassName = "ClassName";// string
		public static readonly String ConfigID = "ConfigID";// string
		public static readonly String GroupID = "GroupID";// int
		public static readonly String ID = "ID";// string
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String MatchGambleDiamond = "MatchGambleDiamond";// int
		public static readonly String MatchID = "MatchID";// object
		public static readonly String MatchOpponentCup = "MatchOpponentCup";// int
		public static readonly String MatchOpponentD = "MatchOpponentD";// int
		public static readonly String MatchOpponentDiamond = "MatchOpponentDiamond";// int
		public static readonly String MatchOpponentGold = "MatchOpponentGold";// int
		public static readonly String MatchOpponentHead = "MatchOpponentHead";// string
		public static readonly String MatchOpponentHeroCnf1 = "MatchOpponentHeroCnf1";// string
		public static readonly String MatchOpponentHeroCnf2 = "MatchOpponentHeroCnf2";// string
		public static readonly String MatchOpponentHeroCnf3 = "MatchOpponentHeroCnf3";// string
		public static readonly String MatchOpponentHeroHP1 = "MatchOpponentHeroHP1";// int
		public static readonly String MatchOpponentHeroHP2 = "MatchOpponentHeroHP2";// int
		public static readonly String MatchOpponentHeroHP3 = "MatchOpponentHeroHP3";// int
		public static readonly String MatchOpponentHeroID1 = "MatchOpponentHeroID1";// object
		public static readonly String MatchOpponentHeroID2 = "MatchOpponentHeroID2";// object
		public static readonly String MatchOpponentHeroID3 = "MatchOpponentHeroID3";// object
		public static readonly String MatchOpponentHeroStar1 = "MatchOpponentHeroStar1";// int
		public static readonly String MatchOpponentHeroStar2 = "MatchOpponentHeroStar2";// int
		public static readonly String MatchOpponentHeroStar3 = "MatchOpponentHeroStar3";// int
		public static readonly String MatchOpponentID = "MatchOpponentID";// object
		public static readonly String MatchOpponentK = "MatchOpponentK";// int
		public static readonly String MatchOpponentLevel = "MatchOpponentLevel";// int
		public static readonly String MatchOpponentName = "MatchOpponentName";// string
		public static readonly String MatchStar = "MatchStar";// int
		public static readonly String MatchTeamID = "MatchTeamID";// object
		// Record
		public class MatchMember
		{
			//Class name
			public static readonly String ThisName = "MatchMember";
			public const int GUID = 0;//object
			public const int Name = 1;//string
			public const int K = 2;//int
			public const int D = 3;//int
			public const int A = 4;//int
			public const int Diamond = 5;//int
			public const int Cup = 6;//int
			public const int MVP = 7;//int
			public const int Streak = 8;//int
			public const int HeroCnf1 = 9;//string
			public const int HeroCnf2 = 10;//string
			public const int HeroCnf3 = 11;//string
			public const int HeroStar1 = 12;//int
			public const int HeroStar2 = 13;//int
			public const int HeroStar3 = 14;//int
			public const int HP1 = 15;//int
			public const int HP2 = 16;//int
			public const int HP3 = 17;//int

		}

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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String HeroConfigID = "HeroConfigID";// string
		public static readonly String Job = "Job";// int
		public static readonly String Level = "Level";// int
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Chinese = "Chinese";// string
		public static readonly String English = "English";// string
		public static readonly String French = "French";// string
		public static readonly String Spanish = "Spanish";// string
		// Record

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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String AIOwnerID = "AIOwnerID";// object
		public static readonly String Camp = "Camp";// object
		public static readonly String Climb = "Climb";// int
		public static readonly String ConsumeData = "ConsumeData";// string
		public static readonly String CrisisDis = "CrisisDis";// float
		public static readonly String DescID = "DescID";// string
		public static readonly String Diamond = "Diamond";// int
		public static readonly String DropPackList = "DropPackList";// string
		public static readonly String DropProbability = "DropProbability";// int
		public static readonly String EXP = "EXP";// int
		public static readonly String EffectData = "EffectData";// string
		public static readonly String FearOfFire = "FearOfFire";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String HP = "HP";// int
		public static readonly String Height = "Height";// float
		public static readonly String HeroStar = "HeroStar";// int
		public static readonly String Icon = "Icon";// string
		public static readonly String LastAttacker = "LastAttacker";// object
		public static readonly String Level = "Level";// int
		public static readonly String MP = "MP";// int
		public static readonly String MasterID = "MasterID";// object
		public static readonly String MasterName = "MasterName";// string
		public static readonly String MeleeType = "MeleeType";// int
		public static readonly String NPCSubType = "NPCSubType";// int
		public static readonly String NPCType = "NPCType";// int
		public static readonly String Prefab = "Prefab";// string
		public static readonly String SP = "SP";// int
		public static readonly String SeedID = "SeedID";// string
		public static readonly String ShowCard = "ShowCard";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SkillNormal = "SkillNormal";// string
		public static readonly String SkillSpecial1 = "SkillSpecial1";// string
		public static readonly String SkillSpecial2 = "SkillSpecial2";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Account = "Account";// string
		public static readonly String Camp = "Camp";// object
		public static readonly String ConnectKey = "ConnectKey";// string
		public static readonly String Diamond = "Diamond";// int
		public static readonly String EXP = "EXP";// int
		public static readonly String GMLevel = "GMLevel";// int
		public static readonly String GameID = "GameID";// int
		public static readonly String GateID = "GateID";// int
		public static readonly String Gold = "Gold";// int
		public static readonly String HP = "HP";// int
		public static readonly String Head = "Head";// string
		public static readonly String Job = "Job";// int
		public static readonly String LastOfflineTime = "LastOfflineTime";// object
		public static readonly String Level = "Level";// int
		public static readonly String MAXEXP = "MAXEXP";// int
		public static readonly String MP = "MP";// int
		public static readonly String OnlineCount = "OnlineCount";// int
		public static readonly String OnlineTime = "OnlineTime";// object
		public static readonly String Race = "Race";// int
		public static readonly String SP = "SP";// int
		public static readonly String Sex = "Sex";// int
		public static readonly String SkillNormal = "SkillNormal";// string
		public static readonly String SkillSpecial1 = "SkillSpecial1";// string
		public static readonly String SkillSpecial2 = "SkillSpecial2";// string
		public static readonly String SkillTHUMP = "SkillTHUMP";// string
		public static readonly String TotalTime = "TotalTime";// int
		// Record
		public class Cooldown
		{
			//Class name
			public static readonly String ThisName = "Cooldown";
			public const int SkillID = 0;//string
			public const int Time = 1;//int

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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String ActorID = "ActorID";// int
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
		public static readonly String Position = "Position";// vector3
		public static readonly String SceneID = "SceneID";// int
		// Property
		public static readonly String Area = "Area";// int
		public static readonly String Cell = "Cell";// int
		public static readonly String CpuCount = "CpuCount";// int
		public static readonly String IP = "IP";// string
		public static readonly String MaxOnline = "MaxOnline";// int
		public static readonly String Port = "Port";// int
		public static readonly String ServerID = "ServerID";// int
		public static readonly String Type = "Type";// int
		public static readonly String WSPort = "WSPort";// int
		public static readonly String WebPort = "WebPort";// int
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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
		public static readonly String EffectObjType = "EffectObjType";// int
		public static readonly String GetBuffList = "GetBuffList";// string
		public static readonly String Icon = "Icon";// string
		public static readonly String Melee = "Melee";// int
		public static readonly String NewObject = "NewObject";// string
		public static readonly String NextID = "NextID";// string
		public static readonly String PlayerSkill = "PlayerSkill";// int
		public static readonly String SendBuffList = "SendBuffList";// string
		public static readonly String ShowName = "ShowName";// string
		public static readonly String SkillType = "SkillType";// int
		public static readonly String SpriteFile = "SpriteFile";// string
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
		public static readonly String MoveTo = "MoveTo";// vector3
		public static readonly String Name = "Name";// string
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

}