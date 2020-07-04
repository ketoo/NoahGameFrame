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
		static const std::string& ThisName(){ static std::string x = "Buff"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& DownSaveType(){ static std::string x = "DownSaveType"; return x; };// int
		static const std::string& EffectClearOnDead(){ static std::string x = "EffectClearOnDead"; return x; };// int
		static const std::string& EffectTimeInterval(){ static std::string x = "EffectTimeInterval"; return x; };// float
		static const std::string& EffectTimeValue(){ static std::string x = "EffectTimeValue"; return x; };// int
		static const std::string& EffectType(){ static std::string x = "EffectType"; return x; };// int
		static const std::string& EffectValueReferType(){ static std::string x = "EffectValueReferType"; return x; };// int
		static const std::string& EffectValueType(){ static std::string x = "EffectValueType"; return x; };// int
		static const std::string& ReverseReferType(){ static std::string x = "ReverseReferType"; return x; };// int
		static const std::string& WashGroupID(){ static std::string x = "WashGroupID"; return x; };// int
		// Record

	};
	class ChessPool
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "ChessPool"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& ItemConfigID(){ static std::string x = "ItemConfigID"; return x; };// string
		static const std::string& Weight(){ static std::string x = "Weight"; return x; };// int
		// Record

	};
	class Clan
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Clan"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Clan_AD(){ static std::string x = "Clan_AD"; return x; };// string
		static const std::string& Clan_CreateTime(){ static std::string x = "Clan_CreateTime"; return x; };// int
		static const std::string& Clan_Desc(){ static std::string x = "Clan_Desc"; return x; };// string
		static const std::string& Clan_Gold(){ static std::string x = "Clan_Gold"; return x; };// int
		static const std::string& Clan_Icon(){ static std::string x = "Clan_Icon"; return x; };// string
		static const std::string& Clan_Level(){ static std::string x = "Clan_Level"; return x; };// int
		static const std::string& Clan_Name(){ static std::string x = "Clan_Name"; return x; };// string
		static const std::string& Clan_PresidentID(){ static std::string x = "Clan_PresidentID"; return x; };// object
		static const std::string& Clan_PresidentName(){ static std::string x = "Clan_PresidentName"; return x; };// string
		static const std::string& Clan_Rank(){ static std::string x = "Clan_Rank"; return x; };// int
		static const std::string& Guild_EctypID(){ static std::string x = "Guild_EctypID"; return x; };// int
		// Record
		class Clan_AppyList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_AppyList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int VIP = 3;//int

		};
		class Clan_Building
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_Building"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int

		};
		class Clan_MemberList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_MemberList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int Job = 3;//int
			static const int Donation = 4;//int
			static const int Receive = 5;//int
			static const int BP = 6;//int
			static const int Online = 7;//int
			static const int Title = 8;//int
			static const int GameID = 9;//int
			static const int JoinTime = 10;//int
			static const int Contribution = 11;//int
			static const int LastTime = 12;//string
			static const int AllContribution = 13;//int

		};
		class Clan_WarList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarList"; return x; };
			static const int GUID = 0;//object
			static const int Level = 1;//int

		};

	};
	class ClanConfig
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "ClanConfig"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& DismissTime(){ static std::string x = "DismissTime"; return x; };// int
		static const std::string& GuildLevel(){ static std::string x = "GuildLevel"; return x; };// int
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& MAXBuilding(){ static std::string x = "MAXBuilding"; return x; };// int
		static const std::string& MaxMember(){ static std::string x = "MaxMember"; return x; };// int
		static const std::string& Money(){ static std::string x = "Money"; return x; };// int
		static const std::string& VIP(){ static std::string x = "VIP"; return x; };// int
		// Record

	};
	class ConsumeData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "ConsumeData"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& VIPEXP(){ static std::string x = "VIPEXP"; return x; };// int
		// Record

	};
	class CooldownRecord
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "CooldownRecord"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		// Record
		class Cooldown
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Cooldown"; return x; };
			static const int ConfigID = 0;//string
			static const int Time = 1;//int

		};

	};
	class EffectData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "EffectData"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& ATK_DARK(){ static std::string x = "ATK_DARK"; return x; };// int
		static const std::string& ATK_FIRE(){ static std::string x = "ATK_FIRE"; return x; };// int
		static const std::string& ATK_ICE(){ static std::string x = "ATK_ICE"; return x; };// int
		static const std::string& ATK_LIGHT(){ static std::string x = "ATK_LIGHT"; return x; };// int
		static const std::string& ATK_POISON(){ static std::string x = "ATK_POISON"; return x; };// int
		static const std::string& ATK_SPEED(){ static std::string x = "ATK_SPEED"; return x; };// int
		static const std::string& ATK_VALUE(){ static std::string x = "ATK_VALUE"; return x; };// int
		static const std::string& ATK_WIND(){ static std::string x = "ATK_WIND"; return x; };// int
		static const std::string& BUFF_GATE(){ static std::string x = "BUFF_GATE"; return x; };// int
		static const std::string& CRITICAL(){ static std::string x = "CRITICAL"; return x; };// int
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& EVASION(){ static std::string x = "EVASION"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		// Record
		class CommValue
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "CommValue"; return x; };
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int EVASION = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_DARK = 15;//int
			static const int ATK_WIND = 16;//int
			static const int ATK_ICE = 17;//int
			static const int ATK_POISON = 18;//int
			static const int DEF_FIRE = 19;//int
			static const int DEF_LIGHT = 20;//int
			static const int DEF_DARK = 21;//int
			static const int DEF_WIND = 22;//int
			static const int DEF_ICE = 23;//int
			static const int DEF_POISON = 24;//int
			static const int DIZZY_GATE = 25;//int
			static const int MOVE_GATE = 26;//int
			static const int SKILL_GATE = 27;//int
			static const int PHYSICAL_GATE = 28;//int
			static const int MAGIC_GATE = 29;//int
			static const int BUFF_GATE = 30;//int
			static const int LUCK = 31;//int

		};

	};
	class GM
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "GM"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		// Record

	};
	class Group
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Group"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		// Include Property, come from GroupMatch 
		static const std::string& MatchID(){ static std::string x = "MatchID"; return x; };// object
		// Record
		class MatchMember
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchMember"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int TeamID = 2;//object
			static const int K = 3;//int
			static const int D = 4;//int
			static const int A = 5;//int
			static const int Diamond = 6;//int
			static const int Cup = 7;//int
			static const int MVP = 8;//int
			static const int HeroCnf1 = 9;//string
			static const int HeroCnf2 = 10;//string
			static const int HeroCnf3 = 11;//string
			static const int HeroStar1 = 12;//int
			static const int HeroStar2 = 13;//int
			static const int HeroStar3 = 14;//int
			static const int HP1 = 15;//int
			static const int HP2 = 16;//int
			static const int HP3 = 17;//int

		};
		class MatchTeam
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchTeam"; return x; };
			static const int GUID = 0;//object
			static const int LeaderID = 1;//object
			static const int Win = 2;//int

		};

	};
	class GroupMatch
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "GroupMatch"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& MatchID(){ static std::string x = "MatchID"; return x; };// object
		// Record
		class MatchMember
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchMember"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int TeamID = 2;//object
			static const int K = 3;//int
			static const int D = 4;//int
			static const int A = 5;//int
			static const int Diamond = 6;//int
			static const int Cup = 7;//int
			static const int MVP = 8;//int
			static const int HeroCnf1 = 9;//string
			static const int HeroCnf2 = 10;//string
			static const int HeroCnf3 = 11;//string
			static const int HeroStar1 = 12;//int
			static const int HeroStar2 = 13;//int
			static const int HeroStar3 = 14;//int
			static const int HP1 = 15;//int
			static const int HP2 = 16;//int
			static const int HP3 = 17;//int

		};
		class MatchTeam
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchTeam"; return x; };
			static const int GUID = 0;//object
			static const int LeaderID = 1;//object
			static const int Win = 2;//int

		};

	};
	class IObject
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "IObject"; return x; };		// Property
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Record

	};
	class Include
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Include"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		// Include Property, come from GroupMatch 
		static const std::string& MatchID(){ static std::string x = "MatchID"; return x; };// object
		// Record
		class MatchMember
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchMember"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int TeamID = 2;//object
			static const int K = 3;//int
			static const int D = 4;//int
			static const int A = 5;//int
			static const int Diamond = 6;//int
			static const int Cup = 7;//int
			static const int MVP = 8;//int
			static const int HeroCnf1 = 9;//string
			static const int HeroCnf2 = 10;//string
			static const int HeroCnf3 = 11;//string
			static const int HeroStar1 = 12;//int
			static const int HeroStar2 = 13;//int
			static const int HeroStar3 = 14;//int
			static const int HP1 = 15;//int
			static const int HP2 = 16;//int
			static const int HP3 = 17;//int

		};
		class MatchTeam
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchTeam"; return x; };
			static const int GUID = 0;//object
			static const int LeaderID = 1;//object
			static const int Win = 2;//int

		};

	};
	class InitProperty
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "InitProperty"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& HeroConfigID(){ static std::string x = "HeroConfigID"; return x; };// string
		static const std::string& Job(){ static std::string x = "Job"; return x; };// int
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		// Record

	};
	class Item
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Item"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& AwardData(){ static std::string x = "AwardData"; return x; };// string
		static const std::string& AwardProperty(){ static std::string x = "AwardProperty"; return x; };// string
		static const std::string& AwardPropertyValue(){ static std::string x = "AwardPropertyValue"; return x; };// int
		static const std::string& BuyPrice(){ static std::string x = "BuyPrice"; return x; };// int
		static const std::string& ConsumeData(){ static std::string x = "ConsumeData"; return x; };// string
		static const std::string& CoolDownTime(){ static std::string x = "CoolDownTime"; return x; };// float
		static const std::string& DescID(){ static std::string x = "DescID"; return x; };// string
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& Extend(){ static std::string x = "Extend"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& ItemSubType(){ static std::string x = "ItemSubType"; return x; };// int
		static const std::string& ItemType(){ static std::string x = "ItemType"; return x; };// int
		static const std::string& Job(){ static std::string x = "Job"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& Quality(){ static std::string x = "Quality"; return x; };// int
		static const std::string& SalePrice(){ static std::string x = "SalePrice"; return x; };// int
		static const std::string& Script(){ static std::string x = "Script"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SpriteFile(){ static std::string x = "SpriteFile"; return x; };// string
		// Record

	};
	class Language
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Language"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Chinese(){ static std::string x = "Chinese"; return x; };// string
		static const std::string& English(){ static std::string x = "English"; return x; };// string
		static const std::string& French(){ static std::string x = "French"; return x; };// string
		static const std::string& Spanish(){ static std::string x = "Spanish"; return x; };// string
		// Record

	};
	class NPC
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "NPC"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& CommonCD(){ static std::string x = "CommonCD"; return x; };// float
		static const std::string& ConsumeData(){ static std::string x = "ConsumeData"; return x; };// string
		static const std::string& DescID(){ static std::string x = "DescID"; return x; };// string
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& DropPackList(){ static std::string x = "DropPackList"; return x; };// string
		static const std::string& DropProbability(){ static std::string x = "DropProbability"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& FearOfFire(){ static std::string x = "FearOfFire"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& Height(){ static std::string x = "Height"; return x; };// float
		static const std::string& HeroStar(){ static std::string x = "HeroStar"; return x; };// int
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& LastAttacker(){ static std::string x = "LastAttacker"; return x; };// object
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& MeleeType(){ static std::string x = "MeleeType"; return x; };// int
		static const std::string& NPCSubType(){ static std::string x = "NPCSubType"; return x; };// int
		static const std::string& NPCType(){ static std::string x = "NPCType"; return x; };// int
		static const std::string& Prefab(){ static std::string x = "Prefab"; return x; };// string
		static const std::string& Refresh(){ static std::string x = "Refresh"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SeedID(){ static std::string x = "SeedID"; return x; };// string
		static const std::string& ShowCard(){ static std::string x = "ShowCard"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SkillNormal(){ static std::string x = "SkillNormal"; return x; };// string
		static const std::string& SkillSpecial1(){ static std::string x = "SkillSpecial1"; return x; };// string
		static const std::string& SkillSpecial2(){ static std::string x = "SkillSpecial2"; return x; };// string
		static const std::string& SkillTHUMP(){ static std::string x = "SkillTHUMP"; return x; };// string
		static const std::string& SpriteFile(){ static std::string x = "SpriteFile"; return x; };// string
		static const std::string& TargetX(){ static std::string x = "TargetX"; return x; };// float
		static const std::string& TargetY(){ static std::string x = "TargetY"; return x; };// float
		static const std::string& Width(){ static std::string x = "Width"; return x; };// float
		// Include Property, come from EffectData 
		static const std::string& ATK_DARK(){ static std::string x = "ATK_DARK"; return x; };// int
		static const std::string& ATK_FIRE(){ static std::string x = "ATK_FIRE"; return x; };// int
		static const std::string& ATK_ICE(){ static std::string x = "ATK_ICE"; return x; };// int
		static const std::string& ATK_LIGHT(){ static std::string x = "ATK_LIGHT"; return x; };// int
		static const std::string& ATK_POISON(){ static std::string x = "ATK_POISON"; return x; };// int
		static const std::string& ATK_SPEED(){ static std::string x = "ATK_SPEED"; return x; };// int
		static const std::string& ATK_VALUE(){ static std::string x = "ATK_VALUE"; return x; };// int
		static const std::string& ATK_WIND(){ static std::string x = "ATK_WIND"; return x; };// int
		static const std::string& BUFF_GATE(){ static std::string x = "BUFF_GATE"; return x; };// int
		static const std::string& CRITICAL(){ static std::string x = "CRITICAL"; return x; };// int
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& EVASION(){ static std::string x = "EVASION"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		// Include Property, come from CooldownRecord 
		// Record
		class CommValue
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "CommValue"; return x; };
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int EVASION = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_DARK = 15;//int
			static const int ATK_WIND = 16;//int
			static const int ATK_ICE = 17;//int
			static const int ATK_POISON = 18;//int
			static const int DEF_FIRE = 19;//int
			static const int DEF_LIGHT = 20;//int
			static const int DEF_DARK = 21;//int
			static const int DEF_WIND = 22;//int
			static const int DEF_ICE = 23;//int
			static const int DEF_POISON = 24;//int
			static const int DIZZY_GATE = 25;//int
			static const int MOVE_GATE = 26;//int
			static const int SKILL_GATE = 27;//int
			static const int PHYSICAL_GATE = 28;//int
			static const int MAGIC_GATE = 29;//int
			static const int BUFF_GATE = 30;//int
			static const int LUCK = 31;//int

		};
		class Cooldown
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Cooldown"; return x; };
			static const int ConfigID = 0;//string
			static const int Time = 1;//int

		};

	};
	class NoSqlServer
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "NoSqlServer"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Auth(){ static std::string x = "Auth"; return x; };// string
		static const std::string& IP(){ static std::string x = "IP"; return x; };// string
		static const std::string& Port(){ static std::string x = "Port"; return x; };// int
		static const std::string& ServerID(){ static std::string x = "ServerID"; return x; };// int
		// Record

	};
	class Player
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Player"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Account(){ static std::string x = "Account"; return x; };// string
		static const std::string& ConnectKey(){ static std::string x = "ConnectKey"; return x; };// string
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& GMLevel(){ static std::string x = "GMLevel"; return x; };// int
		static const std::string& GameID(){ static std::string x = "GameID"; return x; };// int
		static const std::string& GateID(){ static std::string x = "GateID"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& Head(){ static std::string x = "Head"; return x; };// string
		static const std::string& Job(){ static std::string x = "Job"; return x; };// int
		static const std::string& LastOfflineTime(){ static std::string x = "LastOfflineTime"; return x; };// object
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& MAXEXP(){ static std::string x = "MAXEXP"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& OnlineCount(){ static std::string x = "OnlineCount"; return x; };// int
		static const std::string& OnlineTime(){ static std::string x = "OnlineTime"; return x; };// object
		static const std::string& Race(){ static std::string x = "Race"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& Sex(){ static std::string x = "Sex"; return x; };// int
		static const std::string& SkillNormal(){ static std::string x = "SkillNormal"; return x; };// string
		static const std::string& SkillSpecial1(){ static std::string x = "SkillSpecial1"; return x; };// string
		static const std::string& SkillSpecial2(){ static std::string x = "SkillSpecial2"; return x; };// string
		static const std::string& SkillTHUMP(){ static std::string x = "SkillTHUMP"; return x; };// string
		static const std::string& TotalTime(){ static std::string x = "TotalTime"; return x; };// int
		// Include Property, come from EffectData 
		static const std::string& ATK_DARK(){ static std::string x = "ATK_DARK"; return x; };// int
		static const std::string& ATK_FIRE(){ static std::string x = "ATK_FIRE"; return x; };// int
		static const std::string& ATK_ICE(){ static std::string x = "ATK_ICE"; return x; };// int
		static const std::string& ATK_LIGHT(){ static std::string x = "ATK_LIGHT"; return x; };// int
		static const std::string& ATK_POISON(){ static std::string x = "ATK_POISON"; return x; };// int
		static const std::string& ATK_SPEED(){ static std::string x = "ATK_SPEED"; return x; };// int
		static const std::string& ATK_VALUE(){ static std::string x = "ATK_VALUE"; return x; };// int
		static const std::string& ATK_WIND(){ static std::string x = "ATK_WIND"; return x; };// int
		static const std::string& BUFF_GATE(){ static std::string x = "BUFF_GATE"; return x; };// int
		static const std::string& CRITICAL(){ static std::string x = "CRITICAL"; return x; };// int
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& EVASION(){ static std::string x = "EVASION"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		// Include Property, come from CooldownRecord 
		// Include Property, come from PlayerClan 
		static const std::string& ClanID(){ static std::string x = "ClanID"; return x; };// object
		static const std::string& Clan_AD(){ static std::string x = "Clan_AD"; return x; };// string
		static const std::string& Clan_ContinueDay(){ static std::string x = "Clan_ContinueDay"; return x; };// int
		static const std::string& Clan_CreateTime(){ static std::string x = "Clan_CreateTime"; return x; };// int
		static const std::string& Clan_Desc(){ static std::string x = "Clan_Desc"; return x; };// string
		static const std::string& Clan_EctypID(){ static std::string x = "Clan_EctypID"; return x; };// int
		static const std::string& Clan_Exp(){ static std::string x = "Clan_Exp"; return x; };// int
		static const std::string& Clan_Gold(){ static std::string x = "Clan_Gold"; return x; };// int
		static const std::string& Clan_Honor(){ static std::string x = "Clan_Honor"; return x; };// int
		static const std::string& Clan_Icon(){ static std::string x = "Clan_Icon"; return x; };// string
		static const std::string& Clan_Level(){ static std::string x = "Clan_Level"; return x; };// int
		static const std::string& Clan_MemeberMaxCount(){ static std::string x = "Clan_MemeberMaxCount"; return x; };// int
		static const std::string& Clan_Name(){ static std::string x = "Clan_Name"; return x; };// string
		static const std::string& Clan_PresidentID(){ static std::string x = "Clan_PresidentID"; return x; };// object
		static const std::string& Clan_PresidentName(){ static std::string x = "Clan_PresidentName"; return x; };// string
		static const std::string& Cland_MemeberCount(){ static std::string x = "Cland_MemeberCount"; return x; };// int
		static const std::string& Cland_Rank(){ static std::string x = "Cland_Rank"; return x; };// int
		// Include Property, come from PlayerHero 
		static const std::string& FightHeroCnfID(){ static std::string x = "FightHeroCnfID"; return x; };// string
		static const std::string& FightHeroID(){ static std::string x = "FightHeroID"; return x; };// object
		static const std::string& ForgeEquipID(){ static std::string x = "ForgeEquipID"; return x; };// object
		static const std::string& ForgeEquipLuckyItem(){ static std::string x = "ForgeEquipLuckyItem"; return x; };// string
		static const std::string& ForgeEquipRate(){ static std::string x = "ForgeEquipRate"; return x; };// int
		static const std::string& ForgeStoneID(){ static std::string x = "ForgeStoneID"; return x; };// string
		static const std::string& HeroCnfID1(){ static std::string x = "HeroCnfID1"; return x; };// string
		static const std::string& HeroCnfID2(){ static std::string x = "HeroCnfID2"; return x; };// string
		static const std::string& HeroCnfID3(){ static std::string x = "HeroCnfID3"; return x; };// string
		static const std::string& HeroID1(){ static std::string x = "HeroID1"; return x; };// object
		static const std::string& HeroID2(){ static std::string x = "HeroID2"; return x; };// object
		static const std::string& HeroID3(){ static std::string x = "HeroID3"; return x; };// object
		static const std::string& HeroStar1(){ static std::string x = "HeroStar1"; return x; };// int
		static const std::string& HeroStar2(){ static std::string x = "HeroStar2"; return x; };// int
		static const std::string& HeroStar3(){ static std::string x = "HeroStar3"; return x; };// int
		// Include Property, come from PlayerBuilding 
		// Include Property, come from PlayerPVP 
		static const std::string& Commend(){ static std::string x = "Commend"; return x; };// int
		static const std::string& Cup(){ static std::string x = "Cup"; return x; };// int
		static const std::string& IsReconnect(){ static std::string x = "IsReconnect"; return x; };// int
		static const std::string& MatchPassTime(){ static std::string x = "MatchPassTime"; return x; };// int
		static const std::string& Report(){ static std::string x = "Report"; return x; };// int
		static const std::string& Streak(){ static std::string x = "Streak"; return x; };// int
		static const std::string& TreasureChest1(){ static std::string x = "TreasureChest1"; return x; };// string
		static const std::string& TreasureChest2(){ static std::string x = "TreasureChest2"; return x; };// string
		static const std::string& TreasureChest3(){ static std::string x = "TreasureChest3"; return x; };// string
		static const std::string& TreasureChestView1(){ static std::string x = "TreasureChestView1"; return x; };// string
		static const std::string& TreasureChestView2(){ static std::string x = "TreasureChestView2"; return x; };// string
		static const std::string& TreasureChestView3(){ static std::string x = "TreasureChestView3"; return x; };// string
		// Include Property, come from PlayerFriend 
		// Include Property, come from PlayerAutoChess 
		static const std::string& AutoPick(){ static std::string x = "AutoPick"; return x; };// int
		static const std::string& ChessCardCost(){ static std::string x = "ChessCardCost"; return x; };// int
		static const std::string& LastPoolTime(){ static std::string x = "LastPoolTime"; return x; };// int
		static const std::string& LuckyItem(){ static std::string x = "LuckyItem"; return x; };// string
		static const std::string& LuckySpentCost(){ static std::string x = "LuckySpentCost"; return x; };// int
		// Include Property, come from PlayerTeamMember 
		static const std::string& TeamID(){ static std::string x = "TeamID"; return x; };// object
		// Record
		class HeroEquipmentList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "HeroEquipmentList"; return x; };
			static const int HeroID = 0;//object
			static const int EquipmentID = 1;//object
			static const int SlotIndex = 2;//int

		};
		class HeroList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "HeroList"; return x; };
			static const int GUID = 0;//object
			static const int ItemConfigID = 1;//string
			static const int ConfigID = 2;//string
			static const int Activated = 3;//int
			static const int Level = 4;//int
			static const int Exp = 5;//int
			static const int Star = 6;//int
			static const int HP = 7;//int

		};
		class Inventory
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Inventory"; return x; };
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int

		};
		class InventoryEquipment
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "InventoryEquipment"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int RandPropertyID = 2;//string
			static const int RandPropertyValue = 3;//int
			static const int IntensifyLevel = 4;//int
			static const int Date = 5;//int
			static const int Equipped = 6;//int

		};
		class CommValue
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "CommValue"; return x; };
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int EVASION = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_DARK = 15;//int
			static const int ATK_WIND = 16;//int
			static const int ATK_ICE = 17;//int
			static const int ATK_POISON = 18;//int
			static const int DEF_FIRE = 19;//int
			static const int DEF_LIGHT = 20;//int
			static const int DEF_DARK = 21;//int
			static const int DEF_WIND = 22;//int
			static const int DEF_ICE = 23;//int
			static const int DEF_POISON = 24;//int
			static const int DIZZY_GATE = 25;//int
			static const int MOVE_GATE = 26;//int
			static const int SKILL_GATE = 27;//int
			static const int PHYSICAL_GATE = 28;//int
			static const int MAGIC_GATE = 29;//int
			static const int BUFF_GATE = 30;//int
			static const int LUCK = 31;//int

		};
		class Cooldown
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Cooldown"; return x; };
			static const int ConfigID = 0;//string
			static const int Time = 1;//int

		};
		class HeroValue
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "HeroValue"; return x; };
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int EVASION = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_DARK = 15;//int
			static const int ATK_WIND = 16;//int
			static const int ATK_ICE = 17;//int
			static const int ATK_POISON = 18;//int
			static const int DEF_FIRE = 19;//int
			static const int DEF_LIGHT = 20;//int
			static const int DEF_DARK = 21;//int
			static const int DEF_WIND = 22;//int
			static const int DEF_ICE = 23;//int
			static const int DEF_POISON = 24;//int
			static const int DIZZY_GATE = 25;//int
			static const int MOVE_GATE = 26;//int
			static const int SKILL_GATE = 27;//int
			static const int PHYSICAL_GATE = 28;//int
			static const int MAGIC_GATE = 29;//int
			static const int BUFF_GATE = 30;//int
			static const int LUCK = 31;//int

		};
		class BuildingList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuildingList"; return x; };
			static const int BuildingCnfID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int Pos = 3;//vector3
			static const int StateEndTime = 4;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuildingListProduce"; return x; };
			static const int BuildingCnfID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int Pos = 3;//vector3
			static const int StateEndTime = 4;//int

		};
		class PVPMatch
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "PVPMatch"; return x; };
			static const int GUID = 0;//object
			static const int PVPMode = 1;//int
			static const int GameServerID = 2;//int
			static const int SceneID = 3;//int
			static const int GroupID = 4;//int
			static const int StartTime = 5;//int
			static const int Finished = 6;//int

		};
		class PlayingMatch
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "PlayingMatch"; return x; };
			static const int GUID = 0;//object
			static const int PVPMode = 1;//int
			static const int GameServerID = 2;//int
			static const int SceneID = 3;//int
			static const int GroupID = 4;//int
			static const int StartTime = 5;//int
			static const int Finished = 6;//int

		};
		class BlockList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BlockList"; return x; };
			static const int GUID = 0;//object

		};
		class FriendInviteList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendInviteList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int BP = 3;//int
			static const int Leader = 4;//int

		};
		class FriendList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Online = 2;//int
			static const int Time = 3;//int
			static const int GameID = 4;//int

		};
		class FriendSentList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendSentList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string

		};
		class ChessPool
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ChessPool"; return x; };
			static const int ConfigID = 0;//string
			static const int Star = 1;//int

		};
		class ChessPoolBackup
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ChessPoolBackup"; return x; };
			static const int ConfigID = 0;//string
			static const int Star = 1;//int

		};
		class FightChessHero
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FightChessHero"; return x; };
			static const int HeroID = 0;//object
			static const int ConfigID = 1;//string
			static const int Star = 2;//int
			static const int EquipConfigID = 3;//string
			static const int LastUseTime = 4;//int
			static const int SkillID = 5;//string
			static const int LastSkillUseTime = 6;//int

		};
		class TeamMemberList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "TeamMemberList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Leader = 2;//int
			static const int bp = 3;//int
			static const int GameID = 4;//int
			static const int Pending = 5;//int

		};

	};
	class PlayerAutoChess
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerAutoChess"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& AutoPick(){ static std::string x = "AutoPick"; return x; };// int
		static const std::string& ChessCardCost(){ static std::string x = "ChessCardCost"; return x; };// int
		static const std::string& LastPoolTime(){ static std::string x = "LastPoolTime"; return x; };// int
		static const std::string& LuckyItem(){ static std::string x = "LuckyItem"; return x; };// string
		static const std::string& LuckySpentCost(){ static std::string x = "LuckySpentCost"; return x; };// int
		// Record
		class ChessPool
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ChessPool"; return x; };
			static const int ConfigID = 0;//string
			static const int Star = 1;//int

		};
		class ChessPoolBackup
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ChessPoolBackup"; return x; };
			static const int ConfigID = 0;//string
			static const int Star = 1;//int

		};
		class FightChessHero
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FightChessHero"; return x; };
			static const int HeroID = 0;//object
			static const int ConfigID = 1;//string
			static const int Star = 2;//int
			static const int EquipConfigID = 3;//string
			static const int LastUseTime = 4;//int
			static const int SkillID = 5;//string
			static const int LastSkillUseTime = 6;//int

		};

	};
	class PlayerBuilding
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerBuilding"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		// Record
		class BuildingList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuildingList"; return x; };
			static const int BuildingCnfID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int Pos = 3;//vector3
			static const int StateEndTime = 4;//int

		};
		class BuildingListProduce
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuildingListProduce"; return x; };
			static const int BuildingCnfID = 0;//string
			static const int BuildingGUID = 1;//object
			static const int State = 2;//int
			static const int Pos = 3;//vector3
			static const int StateEndTime = 4;//int

		};

	};
	class PlayerClan
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerClan"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& ClanID(){ static std::string x = "ClanID"; return x; };// object
		static const std::string& Clan_AD(){ static std::string x = "Clan_AD"; return x; };// string
		static const std::string& Clan_ContinueDay(){ static std::string x = "Clan_ContinueDay"; return x; };// int
		static const std::string& Clan_CreateTime(){ static std::string x = "Clan_CreateTime"; return x; };// int
		static const std::string& Clan_Desc(){ static std::string x = "Clan_Desc"; return x; };// string
		static const std::string& Clan_EctypID(){ static std::string x = "Clan_EctypID"; return x; };// int
		static const std::string& Clan_Exp(){ static std::string x = "Clan_Exp"; return x; };// int
		static const std::string& Clan_Gold(){ static std::string x = "Clan_Gold"; return x; };// int
		static const std::string& Clan_Honor(){ static std::string x = "Clan_Honor"; return x; };// int
		static const std::string& Clan_Icon(){ static std::string x = "Clan_Icon"; return x; };// string
		static const std::string& Clan_Level(){ static std::string x = "Clan_Level"; return x; };// int
		static const std::string& Clan_MemeberMaxCount(){ static std::string x = "Clan_MemeberMaxCount"; return x; };// int
		static const std::string& Clan_Name(){ static std::string x = "Clan_Name"; return x; };// string
		static const std::string& Clan_PresidentID(){ static std::string x = "Clan_PresidentID"; return x; };// object
		static const std::string& Clan_PresidentName(){ static std::string x = "Clan_PresidentName"; return x; };// string
		static const std::string& Cland_MemeberCount(){ static std::string x = "Cland_MemeberCount"; return x; };// int
		static const std::string& Cland_Rank(){ static std::string x = "Cland_Rank"; return x; };// int
		// Record

	};
	class PlayerFriend
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerFriend"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		// Record
		class BlockList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BlockList"; return x; };
			static const int GUID = 0;//object

		};
		class FriendInviteList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendInviteList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Level = 2;//int
			static const int BP = 3;//int
			static const int Leader = 4;//int

		};
		class FriendList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Online = 2;//int
			static const int Time = 3;//int
			static const int GameID = 4;//int

		};
		class FriendSentList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FriendSentList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string

		};

	};
	class PlayerHero
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerHero"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& FightHeroCnfID(){ static std::string x = "FightHeroCnfID"; return x; };// string
		static const std::string& FightHeroID(){ static std::string x = "FightHeroID"; return x; };// object
		static const std::string& ForgeEquipID(){ static std::string x = "ForgeEquipID"; return x; };// object
		static const std::string& ForgeEquipLuckyItem(){ static std::string x = "ForgeEquipLuckyItem"; return x; };// string
		static const std::string& ForgeEquipRate(){ static std::string x = "ForgeEquipRate"; return x; };// int
		static const std::string& ForgeStoneID(){ static std::string x = "ForgeStoneID"; return x; };// string
		static const std::string& HeroCnfID1(){ static std::string x = "HeroCnfID1"; return x; };// string
		static const std::string& HeroCnfID2(){ static std::string x = "HeroCnfID2"; return x; };// string
		static const std::string& HeroCnfID3(){ static std::string x = "HeroCnfID3"; return x; };// string
		static const std::string& HeroID1(){ static std::string x = "HeroID1"; return x; };// object
		static const std::string& HeroID2(){ static std::string x = "HeroID2"; return x; };// object
		static const std::string& HeroID3(){ static std::string x = "HeroID3"; return x; };// object
		static const std::string& HeroStar1(){ static std::string x = "HeroStar1"; return x; };// int
		static const std::string& HeroStar2(){ static std::string x = "HeroStar2"; return x; };// int
		static const std::string& HeroStar3(){ static std::string x = "HeroStar3"; return x; };// int
		// Record
		class HeroValue
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "HeroValue"; return x; };
			static const int SUCKBLOOD = 0;//int
			static const int REFLECTDAMAGE = 1;//int
			static const int CRITICAL = 2;//int
			static const int MAXHP = 3;//int
			static const int MAXMP = 4;//int
			static const int MAXSP = 5;//int
			static const int HPREGEN = 6;//int
			static const int EVASION = 7;//int
			static const int MPREGEN = 8;//int
			static const int ATK_VALUE = 9;//int
			static const int DEF_VALUE = 10;//int
			static const int MOVE_SPEED = 11;//int
			static const int ATK_SPEED = 12;//int
			static const int ATK_FIRE = 13;//int
			static const int ATK_LIGHT = 14;//int
			static const int ATK_DARK = 15;//int
			static const int ATK_WIND = 16;//int
			static const int ATK_ICE = 17;//int
			static const int ATK_POISON = 18;//int
			static const int DEF_FIRE = 19;//int
			static const int DEF_LIGHT = 20;//int
			static const int DEF_DARK = 21;//int
			static const int DEF_WIND = 22;//int
			static const int DEF_ICE = 23;//int
			static const int DEF_POISON = 24;//int
			static const int DIZZY_GATE = 25;//int
			static const int MOVE_GATE = 26;//int
			static const int SKILL_GATE = 27;//int
			static const int PHYSICAL_GATE = 28;//int
			static const int MAGIC_GATE = 29;//int
			static const int BUFF_GATE = 30;//int
			static const int LUCK = 31;//int

		};

	};
	class PlayerPVP
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerPVP"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Commend(){ static std::string x = "Commend"; return x; };// int
		static const std::string& Cup(){ static std::string x = "Cup"; return x; };// int
		static const std::string& IsReconnect(){ static std::string x = "IsReconnect"; return x; };// int
		static const std::string& MatchPassTime(){ static std::string x = "MatchPassTime"; return x; };// int
		static const std::string& Report(){ static std::string x = "Report"; return x; };// int
		static const std::string& Streak(){ static std::string x = "Streak"; return x; };// int
		static const std::string& TreasureChest1(){ static std::string x = "TreasureChest1"; return x; };// string
		static const std::string& TreasureChest2(){ static std::string x = "TreasureChest2"; return x; };// string
		static const std::string& TreasureChest3(){ static std::string x = "TreasureChest3"; return x; };// string
		static const std::string& TreasureChestView1(){ static std::string x = "TreasureChestView1"; return x; };// string
		static const std::string& TreasureChestView2(){ static std::string x = "TreasureChestView2"; return x; };// string
		static const std::string& TreasureChestView3(){ static std::string x = "TreasureChestView3"; return x; };// string
		// Record
		class PVPMatch
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "PVPMatch"; return x; };
			static const int GUID = 0;//object
			static const int PVPMode = 1;//int
			static const int GameServerID = 2;//int
			static const int SceneID = 3;//int
			static const int GroupID = 4;//int
			static const int StartTime = 5;//int
			static const int Finished = 6;//int

		};
		class PlayingMatch
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "PlayingMatch"; return x; };
			static const int GUID = 0;//object
			static const int PVPMode = 1;//int
			static const int GameServerID = 2;//int
			static const int SceneID = 3;//int
			static const int GroupID = 4;//int
			static const int StartTime = 5;//int
			static const int Finished = 6;//int

		};

	};
	class PlayerTeamMember
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "PlayerTeamMember"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& TeamID(){ static std::string x = "TeamID"; return x; };// object
		// Record
		class TeamMemberList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "TeamMemberList"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int Leader = 2;//int
			static const int bp = 3;//int
			static const int GameID = 4;//int
			static const int Pending = 5;//int

		};

	};
	class Scene
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Scene"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& ActorID(){ static std::string x = "ActorID"; return x; };// int
		static const std::string& FilePath(){ static std::string x = "FilePath"; return x; };// string
		static const std::string& LoadingUI(){ static std::string x = "LoadingUI"; return x; };// string
		static const std::string& MaxGroup(){ static std::string x = "MaxGroup"; return x; };// int
		static const std::string& MaxGroupPlayers(){ static std::string x = "MaxGroupPlayers"; return x; };// int
		static const std::string& NavigationResPath(){ static std::string x = "NavigationResPath"; return x; };// string
		static const std::string& RelivePos(){ static std::string x = "RelivePos"; return x; };// string
		static const std::string& ResPath(){ static std::string x = "ResPath"; return x; };// string
		static const std::string& SceneName(){ static std::string x = "SceneName"; return x; };// string
		static const std::string& SceneShowName(){ static std::string x = "SceneShowName"; return x; };// string
		static const std::string& SoundList(){ static std::string x = "SoundList"; return x; };// string
		static const std::string& Type(){ static std::string x = "Type"; return x; };// int
		static const std::string& Width(){ static std::string x = "Width"; return x; };// int
		// Record

	};
	class Security
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Security"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& SecurityData(){ static std::string x = "SecurityData"; return x; };// string
		// Record

	};
	class Server
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Server"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Area(){ static std::string x = "Area"; return x; };// int
		static const std::string& CpuCount(){ static std::string x = "CpuCount"; return x; };// int
		static const std::string& IP(){ static std::string x = "IP"; return x; };// string
		static const std::string& MaxOnline(){ static std::string x = "MaxOnline"; return x; };// int
		static const std::string& Port(){ static std::string x = "Port"; return x; };// int
		static const std::string& ServerID(){ static std::string x = "ServerID"; return x; };// int
		static const std::string& Type(){ static std::string x = "Type"; return x; };// int
		static const std::string& UDPPort(){ static std::string x = "UDPPort"; return x; };// int
		static const std::string& WSPort(){ static std::string x = "WSPort"; return x; };// int
		static const std::string& WebPort(){ static std::string x = "WebPort"; return x; };// int
		// Record

	};
	class Shop
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Shop"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Count(){ static std::string x = "Count"; return x; };// int
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& ItemID(){ static std::string x = "ItemID"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& Steel(){ static std::string x = "Steel"; return x; };// int
		static const std::string& Stone(){ static std::string x = "Stone"; return x; };// int
		static const std::string& Type(){ static std::string x = "Type"; return x; };// int
		// Record

	};
	class Skill
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Skill"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& AnimaState(){ static std::string x = "AnimaState"; return x; };// string
		static const std::string& AtkDis(){ static std::string x = "AtkDis"; return x; };// float
		static const std::string& ConsumeProperty(){ static std::string x = "ConsumeProperty"; return x; };// string
		static const std::string& ConsumeType(){ static std::string x = "ConsumeType"; return x; };// int
		static const std::string& ConsumeValue(){ static std::string x = "ConsumeValue"; return x; };// int
		static const std::string& CoolDownTime(){ static std::string x = "CoolDownTime"; return x; };// float
		static const std::string& DamageProperty(){ static std::string x = "DamageProperty"; return x; };// string
		static const std::string& DamageType(){ static std::string x = "DamageType"; return x; };// int
		static const std::string& DamageValue(){ static std::string x = "DamageValue"; return x; };// int
		static const std::string& DefaultHitTime(){ static std::string x = "DefaultHitTime"; return x; };// string
		static const std::string& Desc(){ static std::string x = "Desc"; return x; };// string
		static const std::string& EffectObjType(){ static std::string x = "EffectObjType"; return x; };// int
		static const std::string& GetBuffList(){ static std::string x = "GetBuffList"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& NewObject(){ static std::string x = "NewObject"; return x; };// string
		static const std::string& ScriptObject(){ static std::string x = "ScriptObject"; return x; };// string
		static const std::string& SendBuffList(){ static std::string x = "SendBuffList"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SkillSubType(){ static std::string x = "SkillSubType"; return x; };// int
		static const std::string& SkillType(){ static std::string x = "SkillType"; return x; };// int
		static const std::string& SpriteFile(){ static std::string x = "SpriteFile"; return x; };// string
		// Record

	};
	class Talent
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Talent"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Desc(){ static std::string x = "Desc"; return x; };// string
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& NextID(){ static std::string x = "NextID"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SpriteFile(){ static std::string x = "SpriteFile"; return x; };// string
		// Record

	};
	class Task
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Task"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& ArgCnfID(){ static std::string x = "ArgCnfID"; return x; };// string
		static const std::string& ArgCount(){ static std::string x = "ArgCount"; return x; };// int
		static const std::string& AwardExp(){ static std::string x = "AwardExp"; return x; };// int
		static const std::string& AwardGold(){ static std::string x = "AwardGold"; return x; };// int
		static const std::string& AwardPack(){ static std::string x = "AwardPack"; return x; };// string
		static const std::string& DescID(){ static std::string x = "DescID"; return x; };// string
		static const std::string& LevelReq(){ static std::string x = "LevelReq"; return x; };// string
		static const std::string& NextTaskID(){ static std::string x = "NextTaskID"; return x; };// string
		static const std::string& Type(){ static std::string x = "Type"; return x; };// int
		// Record

	};
	class Team
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Team"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& Connection(){ static std::string x = "Connection"; return x; };// int
		static const std::string& Disable(){ static std::string x = "Disable"; return x; };// int
		static const std::string& GMMoveTo(){ static std::string x = "GMMoveTo"; return x; };// vector3
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		static const std::string& State(){ static std::string x = "State"; return x; };// int
		// Property
		static const std::string& Captain(){ static std::string x = "Captain"; return x; };// object
		static const std::string& PresidentName(){ static std::string x = "PresidentName"; return x; };// string
		// Record
		class ApplyList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ApplyList"; return x; };
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
			static const std::string& ThisName(){ static std::string x = "MemberList"; return x; };
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

}
#endif