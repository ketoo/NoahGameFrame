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
	class Buff
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string DownSaveType;// int
		static const std::string EffectClearOnDead;// int
		static const std::string EffectTimeInterval;// float
		static const std::string EffectTimeValue;// int
		static const std::string EffectType;// int
		static const std::string EffectValueReferType;// int
		static const std::string EffectValueType;// int
		static const std::string ReverseReferType;// int
		static const std::string WashGroupID;// int

	};
	class ConsumeData
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string EXP;// int
		static const std::string Gold;// int
		static const std::string HP;// int
		static const std::string MP;// int
		static const std::string Money;// int
		static const std::string SP;// int
		static const std::string VIPEXP;// int

	};
	class DescData
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Atlas;// string
		static const std::string DescText;// string
		static const std::string Icon;// string
		static const std::string PerformanceEffect;// string
		static const std::string PerformanceSound;// string
		static const std::string PrefabPath;// string
		static const std::string ShowName;// string

	};
	class EffectData
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ATK_FIRE;// int
		static const std::string ATK_ICE;// int
		static const std::string ATK_LIGHT;// int
		static const std::string ATK_POISON;// int
		static const std::string ATK_SPEED;// int
		static const std::string ATK_VALUE;// int
		static const std::string ATK_WIND;// int
		static const std::string BUFF_GATE;// int
		static const std::string CRITICAL;// int
		static const std::string DEF_FIRE;// int
		static const std::string DEF_ICE;// int
		static const std::string DEF_LIGHT;// int
		static const std::string DEF_POISON;// int
		static const std::string DEF_VALUE;// int
		static const std::string DEF_WIND;// int
		static const std::string DIZZY_GATE;// int
		static const std::string HP;// int
		static const std::string HPREGEN;// int
		static const std::string MAGIC_GATE;// int
		static const std::string MAXHP;// int
		static const std::string MAXMP;// int
		static const std::string MAXSP;// int
		static const std::string MOVE_GATE;// int
		static const std::string MOVE_SPEED;// int
		static const std::string MP;// int
		static const std::string MPREGEN;// int
		static const std::string PHYSICAL_GATE;// int
		static const std::string REFLECTDAMAGE;// int
		static const std::string SKILL_GATE;// int
		static const std::string SP;// int
		static const std::string SPREGEN;// int
		static const std::string SUCKBLOOD;// int

	};
	class Equip
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string BuyPrice;// int
		static const std::string Desc;// string
		static const std::string DropPrePath;// string
		static const std::string EffectData;// string
		static const std::string EnchantmentBuffList;// string
		static const std::string Icon;// string
		static const std::string IntensiveBuffList;// string
		static const std::string ItemSubType;// int
		static const std::string ItemType;// int
		static const std::string Job;// string
		static const std::string Level;// int
		static const std::string PrefabPath;// string
		static const std::string Quality;// int
		static const std::string SalePrice;// int
		static const std::string Sex;// int
		static const std::string ShowName;// string
		static const std::string SuitBuffID;// string
		static const std::string SuitID;// int

	};
	class Guild
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string AutoRecruit;// string
		static const std::string DismissTime;// int
		static const std::string EctypID;// int
		static const std::string EctypIDGroup;// int
		static const std::string EctypServer;// int
		static const std::string GuildAD;// string
		static const std::string GuildContinueDay;// int
		static const std::string GuildCreateTime;// int
		static const std::string GuildCreateter;// int
		static const std::string GuildDesc;// string
		static const std::string GuildExp;// int
		static const std::string GuildHonor;// int
		static const std::string GuildID;// object
		static const std::string GuildIcon;// int
		static const std::string GuildLevel;// int
		static const std::string GuildMemeberCount;// int
		static const std::string GuildMemeberMaxCount;// int
		static const std::string GuildMoney;// int
		static const std::string GuildStatus;// int
		static const std::string KingWarResource;// int
		static const std::string Name;// string
		static const std::string PresidentID;// object
		static const std::string PresidentName;// string
		static const std::string Rank;// int
		static const std::string RecruitAD;// string
		static const std::string RecruitLevel;// int
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class GuildBoss
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class GuildBoss
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildEvent
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int
			static const int EventID = 8;//int
			static const int EventTime = 9;//int
			static const int Context = 10;//string

		};
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class GuildBoss
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildEvent
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int
			static const int EventID = 8;//int
			static const int EventTime = 9;//int
			static const int Context = 10;//string

		};
		class GuildHouse
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class GuildBoss
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildEvent
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int
			static const int EventID = 8;//int
			static const int EventTime = 9;//int
			static const int Context = 10;//string

		};
		class GuildHouse
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildMemberList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int Receive = 5;//int
			static const int VIP = 6;//int
			static const int Online = 7;//int
			static const int Power = 8;//int
			static const int Title = 9;//int
			static const int GameID = 10;//int
			static const int JoinTime = 11;//int
			static const int Contribution = 12;//int
			static const int AllContribution = 13;//int

		};
		// Record
		class GuildAppyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class GuildBoss
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildEvent
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int
			static const int EventID = 8;//int
			static const int EventTime = 9;//int
			static const int Context = 10;//string

		};
		class GuildHouse
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};
		class GuildMemberList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int Receive = 5;//int
			static const int VIP = 6;//int
			static const int Online = 7;//int
			static const int Power = 8;//int
			static const int Title = 9;//int
			static const int GameID = 10;//int
			static const int JoinTime = 11;//int
			static const int Contribution = 12;//int
			static const int AllContribution = 13;//int

		};
		class GuildSkill
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Offline = 6;//int
			static const int Power = 7;//int

		};

	};
	class GuildConfig
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ConditionPlayerLevel;// int
		static const std::string ConditionPlayerVIP;// int
		static const std::string CostMoney;// int
		static const std::string DismissTime;// int
		static const std::string GuildLevel;// int
		static const std::string MaxMember;// int

	};
	class GuildJob
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string AcceptApply;// object
		static const std::string ApplyDismiss;// object
		static const std::string Appoint;// object
		static const std::string Demise;// object
		static const std::string DenyApply;// object
		static const std::string EditAD;// object
		static const std::string Fire;// object
		static const std::string Job;// int
		static const std::string JobCount;// object
		static const std::string Kickout;// object
		static const std::string Leave;// object
		static const std::string LevelUp;// object
		static const std::string PublishRecruit;// object
		static const std::string SetRecruit;// object
		static const std::string StopDismiss;// object

	};
	class HttpServer
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ServerID;// int
		static const std::string WebPort;// int
		static const std::string WebRootPath;// string

	};
	class IObject
	{
	public:
		//Class name
		static const std::string ThisName;
		// Property
		static const std::string ClassName;// string
		static const std::string ConfigID;// string
		static const std::string GroupID;// int
		static const std::string ID;// string
		static const std::string Position;// vector3
		static const std::string SceneID;// int

	};
	class InitProperty
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string EffectData;// string
		static const std::string Job;// int
		static const std::string Level;// int
		static const std::string ModelPtah;// string
		static const std::string SkillIDRef;// string

	};
	class Item
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string AwardData;// string
		static const std::string AwardProperty;// int
		static const std::string BuyPrice;// int
		static const std::string ConsumeData;// string
		static const std::string CoolDownTime;// float
		static const std::string DescID;// string
		static const std::string EffectData;// string
		static const std::string ExpiredType;// int
		static const std::string Extend;// string
		static const std::string HeroTye;// int
		static const std::string Icon;// string
		static const std::string ItemSubType;// int
		static const std::string ItemType;// int
		static const std::string Job;// string
		static const std::string Level;// int
		static const std::string OverlayCount;// int
		static const std::string Quality;// int
		static const std::string SalePrice;// int
		static const std::string Script;// string
		static const std::string ShowName;// string
		static const std::string SpriteFile;// string

	};
	class Language
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Chinese;// string
		static const std::string English;// string

	};
	class Map
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string InComeDiamond;// int
		static const std::string InComeGold;// int
		static const std::string InComeOil;// int
		static const std::string MapLevel;// int
		static const std::string MaxCount;// int
		// Record
		class Station
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int GuildID = 1;//object
			static const int GuildName = 2;//string
			static const int Level = 3;//int
			static const int Title = 4;//string
			static const int Slogan = 5;//string
			static const int State = 6;//int
			static const int CurMemberCount = 7;//int
			static const int WinCount = 8;//int

		};

	};
	class NPC
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ATK_FIRE;// int
		static const std::string ATK_ICE;// int
		static const std::string ATK_LIGHT;// int
		static const std::string ATK_POISON;// int
		static const std::string ATK_SPEED;// int
		static const std::string ATK_VALUE;// int
		static const std::string ATK_WIND;// int
		static const std::string AtkDis;// float
		static const std::string BUFF_GATE;// int
		static const std::string CRITICAL;// int
		static const std::string Camp;// int
		static const std::string Climb;// int
		static const std::string ConsumeData;// string
		static const std::string DEF_FIRE;// int
		static const std::string DEF_ICE;// int
		static const std::string DEF_LIGHT;// int
		static const std::string DEF_POISON;// int
		static const std::string DEF_VALUE;// int
		static const std::string DEF_WIND;// int
		static const std::string DIZZY_GATE;// int
		static const std::string DropPackList;// string
		static const std::string EXP;// int
		static const std::string EffectData;// string
		static const std::string EquipIDRef;// string
		static const std::string Gold;// int
		static const std::string HP;// int
		static const std::string HPREGEN;// int
		static const std::string Height;// float
		static const std::string HeroType;// int
		static const std::string Icon;// string
		static const std::string Idle;// string
		static const std::string LastAttacker;// object
		static const std::string MAGIC_GATE;// int
		static const std::string MAXHP;// int
		static const std::string MAXMP;// int
		static const std::string MAXSP;// int
		static const std::string MOVE_GATE;// int
		static const std::string MOVE_SPEED;// int
		static const std::string MP;// int
		static const std::string MPREGEN;// int
		static const std::string MasterID;// object
		static const std::string Money;// int
		static const std::string NPCType;// int
		static const std::string PHYSICAL_GATE;// int
		static const std::string Prefab;// string
		static const std::string REFLECTDAMAGE;// int
		static const std::string SKILL_GATE;// int
		static const std::string SP;// int
		static const std::string SPREGEN;// int
		static const std::string SUCKBLOOD;// int
		static const std::string SeedID;// string
		static const std::string ShowCard;// string
		static const std::string ShowName;// string
		static const std::string SkillAttack;// string
		static const std::string SkillNormal;// string
		static const std::string SkillTHUMP;// string
		static const std::string SpriteFile;// string
		static const std::string TargetX;// float
		static const std::string TargetY;// float
		static const std::string VIPEXP;// int

	};
	class NoSqlServer
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Auth;// string
		static const std::string IP;// string
		static const std::string Port;// int
		static const std::string ServerID;// int

	};
	class Player
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ATK_FIRE;// int
		static const std::string ATK_ICE;// int
		static const std::string ATK_LIGHT;// int
		static const std::string ATK_POISON;// int
		static const std::string ATK_PVP;// int
		static const std::string ATK_SPEED;// int
		static const std::string ATK_VALUE;// int
		static const std::string ATK_WIND;// int
		static const std::string Account;// string
		static const std::string BUFF_GATE;// int
		static const std::string CRITICAL;// int
		static const std::string Camp;// int
		static const std::string ConnectKey;// string
		static const std::string DEF_FIRE;// int
		static const std::string DEF_ICE;// int
		static const std::string DEF_LIGHT;// int
		static const std::string DEF_POISON;// int
		static const std::string DEF_PVP;// int
		static const std::string DEF_VALUE;// int
		static const std::string DEF_WIND;// int
		static const std::string DIZZY_GATE;// int
		static const std::string EXP;// int
		static const std::string FightHero;// object
		static const std::string FightOppnent;// object
		static const std::string FirstTarget;// object
		static const std::string GMLevel;// int
		static const std::string GambleDiamond;// int
		static const std::string GambleGold;// int
		static const std::string GameID;// int
		static const std::string GateID;// int
		static const std::string Gold;// int
		static const std::string GuildID;// object
		static const std::string HP;// int
		static const std::string HPREGEN;// int
		static const std::string HeroPos1;// object
		static const std::string HeroPos2;// object
		static const std::string HeroPos3;// object
		static const std::string HomeSceneID;// int
		static const std::string Job;// int
		static const std::string LastOfflineTime;// object
		static const std::string Level;// int
		static const std::string MAGIC_GATE;// int
		static const std::string MAXEXP;// int
		static const std::string MAXHP;// int
		static const std::string MAXMP;// int
		static const std::string MAXSP;// int
		static const std::string MOVE_GATE;// int
		static const std::string MOVE_SPEED;// int
		static const std::string MP;// int
		static const std::string MPREGEN;// int
		static const std::string Money;// int
		static const std::string Name;// string
		static const std::string OnlineCount;// int
		static const std::string OnlineTime;// object
		static const std::string PHYSICAL_GATE;// int
		static const std::string PrefabPath;// string
		static const std::string REFLECTDAMAGE;// int
		static const std::string RELIVE_SOUL;// int
		static const std::string Race;// int
		static const std::string SKILL_GATE;// int
		static const std::string SP;// int
		static const std::string SPREGEN;// int
		static const std::string SUCKBLOOD;// int
		static const std::string Sex;// int
		static const std::string ShowName;// string
		static const std::string Skill1;// string
		static const std::string Skill2;// string
		static const std::string Skill3;// string
		static const std::string TeamID;// object
		static const std::string TotalLineTime;// object
		static const std::string TotalTime;// int
		static const std::string VIPEXP;// int
		static const std::string VIPLevel;// int
		static const std::string ViewOppnent;// object
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class HeroPropertyValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class HeroPropertyValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class PlayerHero
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Activated = 2;//int
			static const int Level = 3;//int
			static const int Exp = 4;//int
			static const int Star = 5;//int
			static const int Equip1 = 6;//object
			static const int Equip2 = 7;//object
			static const int Equip3 = 8;//object
			static const int Equip4 = 9;//object
			static const int Equip5 = 10;//object
			static const int Equip6 = 11;//object
			static const int Talent1 = 12;//string
			static const int Talent2 = 13;//string
			static const int Talent3 = 14;//string
			static const int Talent4 = 15;//string
			static const int Talent5 = 16;//string
			static const int Skill1 = 17;//string
			static const int Skill2 = 18;//string
			static const int Skill3 = 19;//string
			static const int Skill4 = 20;//string
			static const int Skill5 = 21;//string
			static const int FightSkill = 22;//string

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class HeroPropertyValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class PlayerHero
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Activated = 2;//int
			static const int Level = 3;//int
			static const int Exp = 4;//int
			static const int Star = 5;//int
			static const int Equip1 = 6;//object
			static const int Equip2 = 7;//object
			static const int Equip3 = 8;//object
			static const int Equip4 = 9;//object
			static const int Equip5 = 10;//object
			static const int Equip6 = 11;//object
			static const int Talent1 = 12;//string
			static const int Talent2 = 13;//string
			static const int Talent3 = 14;//string
			static const int Talent4 = 15;//string
			static const int Talent5 = 16;//string
			static const int Skill1 = 17;//string
			static const int Skill2 = 18;//string
			static const int Skill3 = 19;//string
			static const int Skill4 = 20;//string
			static const int Skill5 = 21;//string
			static const int FightSkill = 22;//string

		};
		class TaskList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int TaskID = 0;//string
			static const int TaskStatus = 1;//int
			static const int Process = 2;//int

		};
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int WearGUID = 1;//object
			static const int ConfigID = 2;//string
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int
			static const int RandPropertyID = 5;//string
			static const int SlotCount = 6;//int
			static const int InlayStone1 = 7;//string
			static const int InlayStone2 = 8;//string
			static const int InlayStone3 = 9;//string
			static const int InlayStone4 = 10;//string
			static const int InlayStone5 = 11;//string
			static const int InlayStone6 = 12;//string
			static const int InlayStone7 = 13;//string
			static const int InlayStone8 = 14;//string
			static const int InlayStone9 = 15;//string
			static const int InlayStone10 = 16;//string
			static const int IntensifyLevel = 17;//string
			static const int ElementLevel1_FIRE = 18;//int
			static const int ElementLevel2_LIGHT = 19;//int
			static const int ElementLevel3_Wind = 20;//int
			static const int ElementLevel4_ICE = 21;//int
			static const int ElementLevel5_POISON = 22;//int

		};
		class BagItemList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int Pos = 4;//vector3
			static const int StateEndTime = 5;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int BuildingID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int PosX = 3;//float
			static const int PosY = 4;//float
			static const int StateEndTime = 5;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class HeroPropertyValue
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int SPREGEN = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_WIND = 15;//int
			static const int ATK_ICE = 16;//int
			static const int ATK_POISON = 17;//int
			static const int DEF_FIRE = 18;//int
			static const int DEF_LIGHT = 19;//int
			static const int DEF_WIND = 20;//int
			static const int DEF_ICE = 21;//int
			static const int DEF_POISON = 22;//int
			static const int DIZZY_GATE = 23;//int
			static const int MOVE_GATE = 24;//int
			static const int SKILL_GATE = 25;//int
			static const int PHYSICAL_GATE = 26;//int
			static const int MAGIC_GATE = 27;//int
			static const int BUFF_GATE = 28;//int

		};
		class PlayerHero
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Activated = 2;//int
			static const int Level = 3;//int
			static const int Exp = 4;//int
			static const int Star = 5;//int
			static const int Equip1 = 6;//object
			static const int Equip2 = 7;//object
			static const int Equip3 = 8;//object
			static const int Equip4 = 9;//object
			static const int Equip5 = 10;//object
			static const int Equip6 = 11;//object
			static const int Talent1 = 12;//string
			static const int Talent2 = 13;//string
			static const int Talent3 = 14;//string
			static const int Talent4 = 15;//string
			static const int Talent5 = 16;//string
			static const int Skill1 = 17;//string
			static const int Skill2 = 18;//string
			static const int Skill3 = 19;//string
			static const int Skill4 = 20;//string
			static const int Skill5 = 21;//string
			static const int FightSkill = 22;//string

		};
		class TaskList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int TaskID = 0;//string
			static const int TaskStatus = 1;//int
			static const int Process = 2;//int

		};
		class TaskMonsterList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int MonsterID = 0;//string
			static const int CurrentKillCount = 1;//int
			static const int RequireKillCount = 2;//int
			static const int TaskID = 3;//string

		};

	};
	class Scene
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string ActorID;// int
		static const std::string CamOffestPos;// string
		static const std::string CamOffestRot;// string
		static const std::string CanClone;// int
		static const std::string FilePath;// string
		static const std::string LoadingUI;// string
		static const std::string MaxGroup;// int
		static const std::string MaxGroupPlayers;// int
		static const std::string NavigationResPath;// string
		static const std::string RelivePos;// string
		static const std::string SceneName;// string
		static const std::string SceneShowName;// string
		static const std::string Share;// int
		static const std::string SoundList;// string
		static const std::string SyncObject;// int
		static const std::string Tile;// int
		static const std::string Width;// int

	};
	class Security
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string SecurityData;// string

	};
	class Server
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Area;// int
		static const std::string CpuCount;// int
		static const std::string IP;// string
		static const std::string MaxOnline;// int
		static const std::string Name;// string
		static const std::string Port;// int
		static const std::string ServerID;// int
		static const std::string Type;// int

	};
	class Shop
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Count;// int
		static const std::string Diamond;// int
		static const std::string Gold;// int
		static const std::string ItemID;// string
		static const std::string Level;// int
		static const std::string Steel;// int
		static const std::string Stone;// int
		static const std::string Type;// int

	};
	class Skill
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string AnimaState;// string
		static const std::string AtkDis;// float
		static const std::string ConsumeProperty;// string
		static const std::string ConsumeType;// int
		static const std::string ConsumeValue;// string
		static const std::string CoolDownTime;// float
		static const std::string DamageDistance;// float
		static const std::string DamageProperty;// string
		static const std::string DamageType;// int
		static const std::string DamageValue;// string
		static const std::string DefaultHitTime;// float
		static const std::string Desc;// string
		static const std::string GetBuffList;// string
		static const std::string Icon;// string
		static const std::string NewObject;// string
		static const std::string NextID;// string
		static const std::string PlayerSkill;// int
		static const std::string RequireDistance;// float
		static const std::string SendBuffList;// string
		static const std::string ShowName;// string
		static const std::string SkillType;// int
		static const std::string SpriteFile;// string
		static const std::string TargetType;// int

	};
	class SqlServer
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string IP;// string
		static const std::string Port;// int
		static const std::string ServerID;// int
		static const std::string SqlIP;// string
		static const std::string SqlName;// string
		static const std::string SqlPort;// int
		static const std::string SqlPwd;// string
		static const std::string SqlUser;// string

	};
	class Talent
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Desc;// string
		static const std::string EffectData;// string
		static const std::string Icon;// string
		static const std::string NextID;// string
		static const std::string ShowName;// string
		static const std::string SpriteFile;// string

	};
	class Task
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string AwardExp;// int
		static const std::string AwardGold;// int
		static const std::string AwardPack;// string
		static const std::string Desc;// string
		static const std::string KillCount;// string
		static const std::string KillMonsterName;// string
		static const std::string LevelReq;// string
		static const std::string NextTaskID;// string
		static const std::string Type;// int

	};
	class Team
	{
	public:
		//Class name
		static const std::string ThisName;
		// IObject
		// Property
		static const std::string Captain;// object
		static const std::string PresidentName;// string
		// Record
		class ApplyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		// Record
		class ApplyList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int VIP = 5;//int
			static const int Power = 6;//int

		};
		class MemberList
		{
		public:
			//Class name
			static const std::string ThisName;
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int Receive = 5;//int
			static const int VIP = 6;//int
			static const int Online = 7;//int
			static const int Title = 8;//int
			static const int GameID = 9;//int

		};

	};

	const std::string Buff::ThisName = "Buff";
	const std::string Buff::DownSaveType = "DownSaveType";
	const std::string Buff::EffectClearOnDead = "EffectClearOnDead";
	const std::string Buff::EffectTimeInterval = "EffectTimeInterval";
	const std::string Buff::EffectTimeValue = "EffectTimeValue";
	const std::string Buff::EffectType = "EffectType";
	const std::string Buff::EffectValueReferType = "EffectValueReferType";
	const std::string Buff::EffectValueType = "EffectValueType";
	const std::string Buff::ReverseReferType = "ReverseReferType";
	const std::string Buff::WashGroupID = "WashGroupID";
	const std::string ConsumeData::ThisName = "ConsumeData";
	const std::string ConsumeData::EXP = "EXP";
	const std::string ConsumeData::Gold = "Gold";
	const std::string ConsumeData::HP = "HP";
	const std::string ConsumeData::MP = "MP";
	const std::string ConsumeData::Money = "Money";
	const std::string ConsumeData::SP = "SP";
	const std::string ConsumeData::VIPEXP = "VIPEXP";
	const std::string DescData::ThisName = "DescData";
	const std::string DescData::Atlas = "Atlas";
	const std::string DescData::DescText = "DescText";
	const std::string DescData::Icon = "Icon";
	const std::string DescData::PerformanceEffect = "PerformanceEffect";
	const std::string DescData::PerformanceSound = "PerformanceSound";
	const std::string DescData::PrefabPath = "PrefabPath";
	const std::string DescData::ShowName = "ShowName";
	const std::string EffectData::ThisName = "EffectData";
	const std::string EffectData::ATK_FIRE = "ATK_FIRE";
	const std::string EffectData::ATK_ICE = "ATK_ICE";
	const std::string EffectData::ATK_LIGHT = "ATK_LIGHT";
	const std::string EffectData::ATK_POISON = "ATK_POISON";
	const std::string EffectData::ATK_SPEED = "ATK_SPEED";
	const std::string EffectData::ATK_VALUE = "ATK_VALUE";
	const std::string EffectData::ATK_WIND = "ATK_WIND";
	const std::string EffectData::BUFF_GATE = "BUFF_GATE";
	const std::string EffectData::CRITICAL = "CRITICAL";
	const std::string EffectData::DEF_FIRE = "DEF_FIRE";
	const std::string EffectData::DEF_ICE = "DEF_ICE";
	const std::string EffectData::DEF_LIGHT = "DEF_LIGHT";
	const std::string EffectData::DEF_POISON = "DEF_POISON";
	const std::string EffectData::DEF_VALUE = "DEF_VALUE";
	const std::string EffectData::DEF_WIND = "DEF_WIND";
	const std::string EffectData::DIZZY_GATE = "DIZZY_GATE";
	const std::string EffectData::HP = "HP";
	const std::string EffectData::HPREGEN = "HPREGEN";
	const std::string EffectData::MAGIC_GATE = "MAGIC_GATE";
	const std::string EffectData::MAXHP = "MAXHP";
	const std::string EffectData::MAXMP = "MAXMP";
	const std::string EffectData::MAXSP = "MAXSP";
	const std::string EffectData::MOVE_GATE = "MOVE_GATE";
	const std::string EffectData::MOVE_SPEED = "MOVE_SPEED";
	const std::string EffectData::MP = "MP";
	const std::string EffectData::MPREGEN = "MPREGEN";
	const std::string EffectData::PHYSICAL_GATE = "PHYSICAL_GATE";
	const std::string EffectData::REFLECTDAMAGE = "REFLECTDAMAGE";
	const std::string EffectData::SKILL_GATE = "SKILL_GATE";
	const std::string EffectData::SP = "SP";
	const std::string EffectData::SPREGEN = "SPREGEN";
	const std::string EffectData::SUCKBLOOD = "SUCKBLOOD";
	const std::string Equip::ThisName = "Equip";
	const std::string Equip::BuyPrice = "BuyPrice";
	const std::string Equip::Desc = "Desc";
	const std::string Equip::DropPrePath = "DropPrePath";
	const std::string Equip::EffectData = "EffectData";
	const std::string Equip::EnchantmentBuffList = "EnchantmentBuffList";
	const std::string Equip::Icon = "Icon";
	const std::string Equip::IntensiveBuffList = "IntensiveBuffList";
	const std::string Equip::ItemSubType = "ItemSubType";
	const std::string Equip::ItemType = "ItemType";
	const std::string Equip::Job = "Job";
	const std::string Equip::Level = "Level";
	const std::string Equip::PrefabPath = "PrefabPath";
	const std::string Equip::Quality = "Quality";
	const std::string Equip::SalePrice = "SalePrice";
	const std::string Equip::Sex = "Sex";
	const std::string Equip::ShowName = "ShowName";
	const std::string Equip::SuitBuffID = "SuitBuffID";
	const std::string Equip::SuitID = "SuitID";
	const std::string Guild::ThisName = "Guild";
	const std::string Guild::AutoRecruit = "AutoRecruit";
	const std::string Guild::DismissTime = "DismissTime";
	const std::string Guild::EctypID = "EctypID";
	const std::string Guild::EctypIDGroup = "EctypIDGroup";
	const std::string Guild::EctypServer = "EctypServer";
	const std::string Guild::GuildAD = "GuildAD";
	const std::string Guild::GuildContinueDay = "GuildContinueDay";
	const std::string Guild::GuildCreateTime = "GuildCreateTime";
	const std::string Guild::GuildCreateter = "GuildCreateter";
	const std::string Guild::GuildDesc = "GuildDesc";
	const std::string Guild::GuildExp = "GuildExp";
	const std::string Guild::GuildHonor = "GuildHonor";
	const std::string Guild::GuildID = "GuildID";
	const std::string Guild::GuildIcon = "GuildIcon";
	const std::string Guild::GuildLevel = "GuildLevel";
	const std::string Guild::GuildMemeberCount = "GuildMemeberCount";
	const std::string Guild::GuildMemeberMaxCount = "GuildMemeberMaxCount";
	const std::string Guild::GuildMoney = "GuildMoney";
	const std::string Guild::GuildStatus = "GuildStatus";
	const std::string Guild::KingWarResource = "KingWarResource";
	const std::string Guild::Name = "Name";
	const std::string Guild::PresidentID = "PresidentID";
	const std::string Guild::PresidentName = "PresidentName";
	const std::string Guild::Rank = "Rank";
	const std::string Guild::RecruitAD = "RecruitAD";
	const std::string Guild::RecruitLevel = "RecruitLevel";
	const std::string Guild::GuildAppyList::ThisName = "GuildAppyList";
	const std::string Guild::GuildBoss::ThisName = "GuildBoss";
	const std::string Guild::GuildEvent::ThisName = "GuildEvent";
	const std::string Guild::GuildHouse::ThisName = "GuildHouse";
	const std::string Guild::GuildMemberList::ThisName = "GuildMemberList";
	const std::string Guild::GuildSkill::ThisName = "GuildSkill";
	const std::string GuildConfig::ThisName = "GuildConfig";
	const std::string GuildConfig::ConditionPlayerLevel = "ConditionPlayerLevel";
	const std::string GuildConfig::ConditionPlayerVIP = "ConditionPlayerVIP";
	const std::string GuildConfig::CostMoney = "CostMoney";
	const std::string GuildConfig::DismissTime = "DismissTime";
	const std::string GuildConfig::GuildLevel = "GuildLevel";
	const std::string GuildConfig::MaxMember = "MaxMember";
	const std::string GuildJob::ThisName = "GuildJob";
	const std::string GuildJob::AcceptApply = "AcceptApply";
	const std::string GuildJob::ApplyDismiss = "ApplyDismiss";
	const std::string GuildJob::Appoint = "Appoint";
	const std::string GuildJob::Demise = "Demise";
	const std::string GuildJob::DenyApply = "DenyApply";
	const std::string GuildJob::EditAD = "EditAD";
	const std::string GuildJob::Fire = "Fire";
	const std::string GuildJob::Job = "Job";
	const std::string GuildJob::JobCount = "JobCount";
	const std::string GuildJob::Kickout = "Kickout";
	const std::string GuildJob::Leave = "Leave";
	const std::string GuildJob::LevelUp = "LevelUp";
	const std::string GuildJob::PublishRecruit = "PublishRecruit";
	const std::string GuildJob::SetRecruit = "SetRecruit";
	const std::string GuildJob::StopDismiss = "StopDismiss";
	const std::string HttpServer::ThisName = "HttpServer";
	const std::string HttpServer::ServerID = "ServerID";
	const std::string HttpServer::WebPort = "WebPort";
	const std::string HttpServer::WebRootPath = "WebRootPath";
	const std::string IObject::ThisName = "IObject";
	const std::string IObject::ClassName = "ClassName";
	const std::string IObject::ConfigID = "ConfigID";
	const std::string IObject::GroupID = "GroupID";
	const std::string IObject::ID = "ID";
	const std::string IObject::Position = "Position";
	const std::string IObject::SceneID = "SceneID";
	const std::string InitProperty::ThisName = "InitProperty";
	const std::string InitProperty::EffectData = "EffectData";
	const std::string InitProperty::Job = "Job";
	const std::string InitProperty::Level = "Level";
	const std::string InitProperty::ModelPtah = "ModelPtah";
	const std::string InitProperty::SkillIDRef = "SkillIDRef";
	const std::string Item::ThisName = "Item";
	const std::string Item::AwardData = "AwardData";
	const std::string Item::AwardProperty = "AwardProperty";
	const std::string Item::BuyPrice = "BuyPrice";
	const std::string Item::ConsumeData = "ConsumeData";
	const std::string Item::CoolDownTime = "CoolDownTime";
	const std::string Item::DescID = "DescID";
	const std::string Item::EffectData = "EffectData";
	const std::string Item::ExpiredType = "ExpiredType";
	const std::string Item::Extend = "Extend";
	const std::string Item::HeroTye = "HeroTye";
	const std::string Item::Icon = "Icon";
	const std::string Item::ItemSubType = "ItemSubType";
	const std::string Item::ItemType = "ItemType";
	const std::string Item::Job = "Job";
	const std::string Item::Level = "Level";
	const std::string Item::OverlayCount = "OverlayCount";
	const std::string Item::Quality = "Quality";
	const std::string Item::SalePrice = "SalePrice";
	const std::string Item::Script = "Script";
	const std::string Item::ShowName = "ShowName";
	const std::string Item::SpriteFile = "SpriteFile";
	const std::string Language::ThisName = "Language";
	const std::string Language::Chinese = "Chinese";
	const std::string Language::English = "English";
	const std::string Map::ThisName = "Map";
	const std::string Map::InComeDiamond = "InComeDiamond";
	const std::string Map::InComeGold = "InComeGold";
	const std::string Map::InComeOil = "InComeOil";
	const std::string Map::MapLevel = "MapLevel";
	const std::string Map::MaxCount = "MaxCount";
	const std::string Map::Station::ThisName = "Station";
	const std::string NPC::ThisName = "NPC";
	const std::string NPC::ATK_FIRE = "ATK_FIRE";
	const std::string NPC::ATK_ICE = "ATK_ICE";
	const std::string NPC::ATK_LIGHT = "ATK_LIGHT";
	const std::string NPC::ATK_POISON = "ATK_POISON";
	const std::string NPC::ATK_SPEED = "ATK_SPEED";
	const std::string NPC::ATK_VALUE = "ATK_VALUE";
	const std::string NPC::ATK_WIND = "ATK_WIND";
	const std::string NPC::AtkDis = "AtkDis";
	const std::string NPC::BUFF_GATE = "BUFF_GATE";
	const std::string NPC::CRITICAL = "CRITICAL";
	const std::string NPC::Camp = "Camp";
	const std::string NPC::Climb = "Climb";
	const std::string NPC::ConsumeData = "ConsumeData";
	const std::string NPC::DEF_FIRE = "DEF_FIRE";
	const std::string NPC::DEF_ICE = "DEF_ICE";
	const std::string NPC::DEF_LIGHT = "DEF_LIGHT";
	const std::string NPC::DEF_POISON = "DEF_POISON";
	const std::string NPC::DEF_VALUE = "DEF_VALUE";
	const std::string NPC::DEF_WIND = "DEF_WIND";
	const std::string NPC::DIZZY_GATE = "DIZZY_GATE";
	const std::string NPC::DropPackList = "DropPackList";
	const std::string NPC::EXP = "EXP";
	const std::string NPC::EffectData = "EffectData";
	const std::string NPC::EquipIDRef = "EquipIDRef";
	const std::string NPC::Gold = "Gold";
	const std::string NPC::HP = "HP";
	const std::string NPC::HPREGEN = "HPREGEN";
	const std::string NPC::Height = "Height";
	const std::string NPC::HeroType = "HeroType";
	const std::string NPC::Icon = "Icon";
	const std::string NPC::Idle = "Idle";
	const std::string NPC::LastAttacker = "LastAttacker";
	const std::string NPC::MAGIC_GATE = "MAGIC_GATE";
	const std::string NPC::MAXHP = "MAXHP";
	const std::string NPC::MAXMP = "MAXMP";
	const std::string NPC::MAXSP = "MAXSP";
	const std::string NPC::MOVE_GATE = "MOVE_GATE";
	const std::string NPC::MOVE_SPEED = "MOVE_SPEED";
	const std::string NPC::MP = "MP";
	const std::string NPC::MPREGEN = "MPREGEN";
	const std::string NPC::MasterID = "MasterID";
	const std::string NPC::Money = "Money";
	const std::string NPC::NPCType = "NPCType";
	const std::string NPC::PHYSICAL_GATE = "PHYSICAL_GATE";
	const std::string NPC::Prefab = "Prefab";
	const std::string NPC::REFLECTDAMAGE = "REFLECTDAMAGE";
	const std::string NPC::SKILL_GATE = "SKILL_GATE";
	const std::string NPC::SP = "SP";
	const std::string NPC::SPREGEN = "SPREGEN";
	const std::string NPC::SUCKBLOOD = "SUCKBLOOD";
	const std::string NPC::SeedID = "SeedID";
	const std::string NPC::ShowCard = "ShowCard";
	const std::string NPC::ShowName = "ShowName";
	const std::string NPC::SkillAttack = "SkillAttack";
	const std::string NPC::SkillNormal = "SkillNormal";
	const std::string NPC::SkillTHUMP = "SkillTHUMP";
	const std::string NPC::SpriteFile = "SpriteFile";
	const std::string NPC::TargetX = "TargetX";
	const std::string NPC::TargetY = "TargetY";
	const std::string NPC::VIPEXP = "VIPEXP";
	const std::string NoSqlServer::ThisName = "NoSqlServer";
	const std::string NoSqlServer::Auth = "Auth";
	const std::string NoSqlServer::IP = "IP";
	const std::string NoSqlServer::Port = "Port";
	const std::string NoSqlServer::ServerID = "ServerID";
	const std::string Player::ThisName = "Player";
	const std::string Player::ATK_FIRE = "ATK_FIRE";
	const std::string Player::ATK_ICE = "ATK_ICE";
	const std::string Player::ATK_LIGHT = "ATK_LIGHT";
	const std::string Player::ATK_POISON = "ATK_POISON";
	const std::string Player::ATK_PVP = "ATK_PVP";
	const std::string Player::ATK_SPEED = "ATK_SPEED";
	const std::string Player::ATK_VALUE = "ATK_VALUE";
	const std::string Player::ATK_WIND = "ATK_WIND";
	const std::string Player::Account = "Account";
	const std::string Player::BUFF_GATE = "BUFF_GATE";
	const std::string Player::CRITICAL = "CRITICAL";
	const std::string Player::Camp = "Camp";
	const std::string Player::ConnectKey = "ConnectKey";
	const std::string Player::DEF_FIRE = "DEF_FIRE";
	const std::string Player::DEF_ICE = "DEF_ICE";
	const std::string Player::DEF_LIGHT = "DEF_LIGHT";
	const std::string Player::DEF_POISON = "DEF_POISON";
	const std::string Player::DEF_PVP = "DEF_PVP";
	const std::string Player::DEF_VALUE = "DEF_VALUE";
	const std::string Player::DEF_WIND = "DEF_WIND";
	const std::string Player::DIZZY_GATE = "DIZZY_GATE";
	const std::string Player::EXP = "EXP";
	const std::string Player::FightHero = "FightHero";
	const std::string Player::FightOppnent = "FightOppnent";
	const std::string Player::FirstTarget = "FirstTarget";
	const std::string Player::GMLevel = "GMLevel";
	const std::string Player::GambleDiamond = "GambleDiamond";
	const std::string Player::GambleGold = "GambleGold";
	const std::string Player::GameID = "GameID";
	const std::string Player::GateID = "GateID";
	const std::string Player::Gold = "Gold";
	const std::string Player::GuildID = "GuildID";
	const std::string Player::HP = "HP";
	const std::string Player::HPREGEN = "HPREGEN";
	const std::string Player::HeroPos1 = "HeroPos1";
	const std::string Player::HeroPos2 = "HeroPos2";
	const std::string Player::HeroPos3 = "HeroPos3";
	const std::string Player::HomeSceneID = "HomeSceneID";
	const std::string Player::Job = "Job";
	const std::string Player::LastOfflineTime = "LastOfflineTime";
	const std::string Player::Level = "Level";
	const std::string Player::MAGIC_GATE = "MAGIC_GATE";
	const std::string Player::MAXEXP = "MAXEXP";
	const std::string Player::MAXHP = "MAXHP";
	const std::string Player::MAXMP = "MAXMP";
	const std::string Player::MAXSP = "MAXSP";
	const std::string Player::MOVE_GATE = "MOVE_GATE";
	const std::string Player::MOVE_SPEED = "MOVE_SPEED";
	const std::string Player::MP = "MP";
	const std::string Player::MPREGEN = "MPREGEN";
	const std::string Player::Money = "Money";
	const std::string Player::Name = "Name";
	const std::string Player::OnlineCount = "OnlineCount";
	const std::string Player::OnlineTime = "OnlineTime";
	const std::string Player::PHYSICAL_GATE = "PHYSICAL_GATE";
	const std::string Player::PrefabPath = "PrefabPath";
	const std::string Player::REFLECTDAMAGE = "REFLECTDAMAGE";
	const std::string Player::RELIVE_SOUL = "RELIVE_SOUL";
	const std::string Player::Race = "Race";
	const std::string Player::SKILL_GATE = "SKILL_GATE";
	const std::string Player::SP = "SP";
	const std::string Player::SPREGEN = "SPREGEN";
	const std::string Player::SUCKBLOOD = "SUCKBLOOD";
	const std::string Player::Sex = "Sex";
	const std::string Player::ShowName = "ShowName";
	const std::string Player::Skill1 = "Skill1";
	const std::string Player::Skill2 = "Skill2";
	const std::string Player::Skill3 = "Skill3";
	const std::string Player::TeamID = "TeamID";
	const std::string Player::TotalLineTime = "TotalLineTime";
	const std::string Player::TotalTime = "TotalTime";
	const std::string Player::VIPEXP = "VIPEXP";
	const std::string Player::VIPLevel = "VIPLevel";
	const std::string Player::ViewOppnent = "ViewOppnent";
	const std::string Player::BagEquipList::ThisName = "BagEquipList";
	const std::string Player::BagItemList::ThisName = "BagItemList";
	const std::string Player::BuildingList::ThisName = "BuildingList";
	const std::string Player::BuildingListProduce::ThisName = "BuildingListProduce";
	const std::string Player::CommValue::ThisName = "CommValue";
	const std::string Player::HeroPropertyValue::ThisName = "HeroPropertyValue";
	const std::string Player::PlayerHero::ThisName = "PlayerHero";
	const std::string Player::TaskList::ThisName = "TaskList";
	const std::string Player::TaskMonsterList::ThisName = "TaskMonsterList";
	const std::string Scene::ThisName = "Scene";
	const std::string Scene::ActorID = "ActorID";
	const std::string Scene::CamOffestPos = "CamOffestPos";
	const std::string Scene::CamOffestRot = "CamOffestRot";
	const std::string Scene::CanClone = "CanClone";
	const std::string Scene::FilePath = "FilePath";
	const std::string Scene::LoadingUI = "LoadingUI";
	const std::string Scene::MaxGroup = "MaxGroup";
	const std::string Scene::MaxGroupPlayers = "MaxGroupPlayers";
	const std::string Scene::NavigationResPath = "NavigationResPath";
	const std::string Scene::RelivePos = "RelivePos";
	const std::string Scene::SceneName = "SceneName";
	const std::string Scene::SceneShowName = "SceneShowName";
	const std::string Scene::Share = "Share";
	const std::string Scene::SoundList = "SoundList";
	const std::string Scene::SyncObject = "SyncObject";
	const std::string Scene::Tile = "Tile";
	const std::string Scene::Width = "Width";
	const std::string Security::ThisName = "Security";
	const std::string Security::SecurityData = "SecurityData";
	const std::string Server::ThisName = "Server";
	const std::string Server::Area = "Area";
	const std::string Server::CpuCount = "CpuCount";
	const std::string Server::IP = "IP";
	const std::string Server::MaxOnline = "MaxOnline";
	const std::string Server::Name = "Name";
	const std::string Server::Port = "Port";
	const std::string Server::ServerID = "ServerID";
	const std::string Server::Type = "Type";
	const std::string Shop::ThisName = "Shop";
	const std::string Shop::Count = "Count";
	const std::string Shop::Diamond = "Diamond";
	const std::string Shop::Gold = "Gold";
	const std::string Shop::ItemID = "ItemID";
	const std::string Shop::Level = "Level";
	const std::string Shop::Steel = "Steel";
	const std::string Shop::Stone = "Stone";
	const std::string Shop::Type = "Type";
	const std::string Skill::ThisName = "Skill";
	const std::string Skill::AnimaState = "AnimaState";
	const std::string Skill::AtkDis = "AtkDis";
	const std::string Skill::ConsumeProperty = "ConsumeProperty";
	const std::string Skill::ConsumeType = "ConsumeType";
	const std::string Skill::ConsumeValue = "ConsumeValue";
	const std::string Skill::CoolDownTime = "CoolDownTime";
	const std::string Skill::DamageDistance = "DamageDistance";
	const std::string Skill::DamageProperty = "DamageProperty";
	const std::string Skill::DamageType = "DamageType";
	const std::string Skill::DamageValue = "DamageValue";
	const std::string Skill::DefaultHitTime = "DefaultHitTime";
	const std::string Skill::Desc = "Desc";
	const std::string Skill::GetBuffList = "GetBuffList";
	const std::string Skill::Icon = "Icon";
	const std::string Skill::NewObject = "NewObject";
	const std::string Skill::NextID = "NextID";
	const std::string Skill::PlayerSkill = "PlayerSkill";
	const std::string Skill::RequireDistance = "RequireDistance";
	const std::string Skill::SendBuffList = "SendBuffList";
	const std::string Skill::ShowName = "ShowName";
	const std::string Skill::SkillType = "SkillType";
	const std::string Skill::SpriteFile = "SpriteFile";
	const std::string Skill::TargetType = "TargetType";
	const std::string SqlServer::ThisName = "SqlServer";
	const std::string SqlServer::IP = "IP";
	const std::string SqlServer::Port = "Port";
	const std::string SqlServer::ServerID = "ServerID";
	const std::string SqlServer::SqlIP = "SqlIP";
	const std::string SqlServer::SqlName = "SqlName";
	const std::string SqlServer::SqlPort = "SqlPort";
	const std::string SqlServer::SqlPwd = "SqlPwd";
	const std::string SqlServer::SqlUser = "SqlUser";
	const std::string Talent::ThisName = "Talent";
	const std::string Talent::Desc = "Desc";
	const std::string Talent::EffectData = "EffectData";
	const std::string Talent::Icon = "Icon";
	const std::string Talent::NextID = "NextID";
	const std::string Talent::ShowName = "ShowName";
	const std::string Talent::SpriteFile = "SpriteFile";
	const std::string Task::ThisName = "Task";
	const std::string Task::AwardExp = "AwardExp";
	const std::string Task::AwardGold = "AwardGold";
	const std::string Task::AwardPack = "AwardPack";
	const std::string Task::Desc = "Desc";
	const std::string Task::KillCount = "KillCount";
	const std::string Task::KillMonsterName = "KillMonsterName";
	const std::string Task::LevelReq = "LevelReq";
	const std::string Task::NextTaskID = "NextTaskID";
	const std::string Task::Type = "Type";
	const std::string Team::ThisName = "Team";
	const std::string Team::Captain = "Captain";
	const std::string Team::PresidentName = "PresidentName";
	const std::string Team::ApplyList::ThisName = "ApplyList";
	const std::string Team::MemberList::ThisName = "MemberList";

}