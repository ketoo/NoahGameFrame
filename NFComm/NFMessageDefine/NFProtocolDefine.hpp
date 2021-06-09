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
	class BigMapCity
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "BigMapCity"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& CityLevel(){ static std::string x = "CityLevel"; return x; };// int
		static const std::string& Country(){ static std::string x = "Country"; return x; };// string
		static const std::string& Element(){ static std::string x = "Element"; return x; };// int
		static const std::string& IsCapital(){ static std::string x = "IsCapital"; return x; };// int
		static const std::string& IsHumanCity(){ static std::string x = "IsHumanCity"; return x; };// int
		static const std::string& X(){ static std::string x = "X"; return x; };// int
		static const std::string& Z(){ static std::string x = "Z"; return x; };// int
		// Record

	};
	class BigMapTile
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "BigMapTile"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& Active(){ static std::string x = "Active"; return x; };// int
		static const std::string& CityName(){ static std::string x = "CityName"; return x; };// string
		static const std::string& CountryLevel(){ static std::string x = "CountryLevel"; return x; };// int
		static const std::string& CountrySceneID(){ static std::string x = "CountrySceneID"; return x; };// int
		static const std::string& Element(){ static std::string x = "Element"; return x; };// int
		static const std::string& SeedID(){ static std::string x = "SeedID"; return x; };// int
		static const std::string& TileBuildingCnfID(){ static std::string x = "TileBuildingCnfID"; return x; };// string
		static const std::string& TileBuildingPos(){ static std::string x = "TileBuildingPos"; return x; };// string
		static const std::string& TileMineCnfID(){ static std::string x = "TileMineCnfID"; return x; };// string
		static const std::string& TileMinePos(){ static std::string x = "TileMinePos"; return x; };// string
		static const std::string& TileOutputGemCnfID(){ static std::string x = "TileOutputGemCnfID"; return x; };// string
		static const std::string& TileOutputGemCount(){ static std::string x = "TileOutputGemCount"; return x; };// int
		// Record

	};
	class Buff
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Buff"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& BuffGroupID(){ static std::string x = "BuffGroupID"; return x; };// int
		static const std::string& BuffType(){ static std::string x = "BuffType"; return x; };// int
		static const std::string& CleanOnDead(){ static std::string x = "CleanOnDead"; return x; };// int
		static const std::string& EffectPropertyID(){ static std::string x = "EffectPropertyID"; return x; };// string
		static const std::string& EffectPropertyType(){ static std::string x = "EffectPropertyType"; return x; };// int
		static const std::string& EffectPropertyValue(){ static std::string x = "EffectPropertyValue"; return x; };// int
		static const std::string& EffectReferPropertyID(){ static std::string x = "EffectReferPropertyID"; return x; };// string
		static const std::string& EffectTriggerCount(){ static std::string x = "EffectTriggerCount"; return x; };// int
		static const std::string& EffectTriggerInterval(){ static std::string x = "EffectTriggerInterval"; return x; };// float
		static const std::string& OnlyForHero(){ static std::string x = "OnlyForHero"; return x; };// int
		static const std::string& RemoveOnSwapScene(){ static std::string x = "RemoveOnSwapScene"; return x; };// int
		static const std::string& SaveOnOffline(){ static std::string x = "SaveOnOffline"; return x; };// int
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
			static const int ConfigID = 1;//string
			static const int Level = 2;//int
			static const int Pos = 3;//vector3

		};
		class Clan_DefNPC
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_DefNPC"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Level = 2;//int
			static const int Pos = 3;//vector3

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
			static const int LastTime = 4;//string
			static const int JoinTime = 5;//int
			static const int BP = 6;//int
			static const int Online = 7;//int
			static const int Title = 8;//int
			static const int TotalContribution = 9;//int
			static const int Contribution = 10;//int

		};
		class Clan_Property
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_Property"; return x; };
			static const int GUID = 0;//object
			static const int Clan_Name = 1;//string
			static const int Clan_PresidentID = 2;//object
			static const int Clan_PresidentName = 3;//string
			static const int Clan_Rank = 4;//int
			static const int Clan_AD = 5;//string
			static const int Clan_Desc = 6;//string
			static const int Clan_Gold = 7;//int
			static const int Clan_Level = 8;//int
			static const int Clan_Icon = 9;//string
			static const int Clan_CreateTime = 10;//int
			static const int Clan_MapTileID = 11;//string
			static const int Clan_GameID = 12;//int
			static const int Steel = 13;//int
			static const int Stone = 14;//int
			static const int Wood = 15;//int
			static const int Crystal = 16;//int
			static const int SourceOfLife = 17;//int
			static const int GemWind = 18;//int
			static const int GemFire = 19;//int
			static const int GemLight = 20;//int
			static const int GemPoison = 21;//int
			static const int GemDark = 22;//int
			static const int GemIce = 23;//int

		};
		class Clan_Recommendation
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_Recommendation"; return x; };
			static const int ClanID = 0;//object
			static const int ClanName = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Level = 4;//object
			static const int MemberCount = 5;//int
			static const int MaxMemberCount = 6;//int

		};
		class Clan_SearchList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_SearchList"; return x; };
			static const int ClanID = 0;//object
			static const int ClanName = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Level = 4;//int
			static const int MemberCount = 5;//int
			static const int MaxMemberCount = 6;//int

		};
		class Clan_WarApplyList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarApplyList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int DefenderClan = 4;//object
			static const int DefenderClanName = 5;//string
			static const int MapTileID = 6;//string

		};
		class Clan_WarList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int Status = 4;//int
			static const int AttackerWin = 5;//int
			static const int DefenderClan = 6;//object
			static const int DefenderClanName = 7;//string
			static const int SetupTime = 8;//int
			static const int FinishTime = 9;//int
			static const int MapTileID = 10;//string

		};
		class Clan_WarNPC
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarNPC"; return x; };
			static const int NPCID = 0;//object
			static const int NPCConfigID = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Pos = 4;//vector3

		};
		class Clan_WarReceiveList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarReceiveList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int DefenderClan = 4;//object
			static const int DefenderClanName = 5;//string
			static const int MapTileID = 6;//string

		};
		class Clan_WarResource
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarResource"; return x; };
			static const int PlayerID = 0;//object
			static const int Leader = 1;//object
			static const int LeaderName = 2;//string
			static const int Steel = 3;//int
			static const int Wood = 4;//int
			static const int Stone = 5;//int
			static const int Crystal = 6;//int

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
	class DungeonReward
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "DungeonReward"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& AwardPackage(){ static std::string x = "AwardPackage"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		// Record

	};
	class EffectData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "EffectData"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		class RuntimeBuff
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "RuntimeBuff"; return x; };
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
	class EventResult
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "EventResult"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& EventResultId(){ static std::string x = "EventResultId"; return x; };// int
		static const std::string& MsgContentID(){ static std::string x = "MsgContentID"; return x; };// string
		static const std::string& MsgContentType(){ static std::string x = "MsgContentType"; return x; };// int
		// Record

	};
	class Exchange
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Exchange"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& ContributionPoint(){ static std::string x = "ContributionPoint"; return x; };// int
		static const std::string& Crystal(){ static std::string x = "Crystal"; return x; };// int
		static const std::string& ItemID(){ static std::string x = "ItemID"; return x; };// string
		static const std::string& Steel(){ static std::string x = "Steel"; return x; };// int
		static const std::string& Stone(){ static std::string x = "Stone"; return x; };// int
		static const std::string& Wood(){ static std::string x = "Wood"; return x; };// int
		// Record

	};
	class GM
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "GM"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& HoldTimeToWin(){ static std::string x = "HoldTimeToWin"; return x; };// int
		static const std::string& MatchID(){ static std::string x = "MatchID"; return x; };// object
		static const std::string& MatchMaxTime(){ static std::string x = "MatchMaxTime"; return x; };// int
		static const std::string& MatchPassTime(){ static std::string x = "MatchPassTime"; return x; };// int
		// Record
		class MatchItem
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchItem"; return x; };
			static const int PlayerID = 0;//object
			static const int Pos = 1;//vector3
			static const int ConfigID = 2;//string

		};
		class MatchMember
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchMember"; return x; };
			static const int PlayerID = 0;//object
			static const int Name = 1;//string
			static const int TeamID = 2;//object
			static const int K = 3;//int
			static const int D = 4;//int
			static const int A = 5;//int
			static const int Diamond = 6;//int
			static const int BetDiamond = 7;//int
			static const int BetBattlePoint = 8;//int
			static const int BattlePointIfLose = 9;//int
			static const int WinBattlePoint = 10;//int
			static const int MVP = 11;//int
			static const int BuyBackCount = 12;//int
			static const int FightHeroCnf = 13;//string
			static const int HeroCnf1 = 14;//string
			static const int HeroCnf2 = 15;//string
			static const int HeroCnf3 = 16;//string
			static const int HeroStar1 = 17;//int
			static const int HeroStar2 = 18;//int
			static const int HeroStar3 = 19;//int

		};
		class MatchTeam
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchTeam"; return x; };
			static const int TeamID = 0;//object
			static const int LeaderID = 1;//object
			static const int Diamond = 2;//int
			static const int Win = 3;//int
			static const int DiamondTime = 4;//int
			static const int AvgBattlePoint = 5;//int

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
	class InitProperty
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "InitProperty"; return x; };		// IObject
		static const std::string& CampID(){ static std::string x = "CampID"; return x; };// object
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
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
		static const std::string& CommonCD(){ static std::string x = "CommonCD"; return x; };// float
		static const std::string& ConsumeData(){ static std::string x = "ConsumeData"; return x; };// string
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& DescID(){ static std::string x = "DescID"; return x; };// string
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& DropPackList(){ static std::string x = "DropPackList"; return x; };// string
		static const std::string& DropProbability(){ static std::string x = "DropProbability"; return x; };// int
		static const std::string& EVASION(){ static std::string x = "EVASION"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& FearOfFire(){ static std::string x = "FearOfFire"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& Height(){ static std::string x = "Height"; return x; };// float
		static const std::string& HeroStar(){ static std::string x = "HeroStar"; return x; };// int
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& LastAttacker(){ static std::string x = "LastAttacker"; return x; };// object
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& MeleeType(){ static std::string x = "MeleeType"; return x; };// int
		static const std::string& NPCSubType(){ static std::string x = "NPCSubType"; return x; };// int
		static const std::string& NPCType(){ static std::string x = "NPCType"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& Prefab(){ static std::string x = "Prefab"; return x; };// string
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& Refresh(){ static std::string x = "Refresh"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
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
		// Include Property, come from CooldownRecord 
		// Include Property, come from BuffRecord 
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
		class RuntimeBuff
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "RuntimeBuff"; return x; };
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
		class BuffRecord
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuffRecord"; return x; };
			static const int ConfigID = 0;//string
			static const int Releaser = 1;//object
			static const int ReleasedTime = 2;//int
			static const int EffectTriggerCount = 3;//int
			static const int EffectTriggerred = 4;//int
			static const int BuffGroupID = 5;//int

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
		static const std::string& Account(){ static std::string x = "Account"; return x; };// string
		static const std::string& BUFF_GATE(){ static std::string x = "BUFF_GATE"; return x; };// int
		static const std::string& BattlePoint(){ static std::string x = "BattlePoint"; return x; };// int
		static const std::string& CRITICAL(){ static std::string x = "CRITICAL"; return x; };// int
		static const std::string& ConnectKey(){ static std::string x = "ConnectKey"; return x; };// string
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& EVASION(){ static std::string x = "EVASION"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& GMLevel(){ static std::string x = "GMLevel"; return x; };// int
		static const std::string& GameID(){ static std::string x = "GameID"; return x; };// int
		static const std::string& GateID(){ static std::string x = "GateID"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& Head(){ static std::string x = "Head"; return x; };// string
		static const std::string& Job(){ static std::string x = "Job"; return x; };// int
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& LastOfflineTime(){ static std::string x = "LastOfflineTime"; return x; };// object
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXEXP(){ static std::string x = "MAXEXP"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& NoticeID(){ static std::string x = "NoticeID"; return x; };// int
		static const std::string& OnlineCount(){ static std::string x = "OnlineCount"; return x; };// int
		static const std::string& OnlineTime(){ static std::string x = "OnlineTime"; return x; };// object
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& Race(){ static std::string x = "Race"; return x; };// int
		static const std::string& ReconnectReason(){ static std::string x = "ReconnectReason"; return x; };// int
		static const std::string& Reconnected(){ static std::string x = "Reconnected"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		static const std::string& ScenarioProgress(){ static std::string x = "ScenarioProgress"; return x; };// int
		static const std::string& Sex(){ static std::string x = "Sex"; return x; };// int
		static const std::string& SkillNormal(){ static std::string x = "SkillNormal"; return x; };// string
		static const std::string& SkillSpecial1(){ static std::string x = "SkillSpecial1"; return x; };// string
		static const std::string& SkillSpecial2(){ static std::string x = "SkillSpecial2"; return x; };// string
		static const std::string& SkillTHUMP(){ static std::string x = "SkillTHUMP"; return x; };// string
		static const std::string& TeamID(){ static std::string x = "TeamID"; return x; };// object
		static const std::string& TotalTime(){ static std::string x = "TotalTime"; return x; };// int
		// Include Property, come from CooldownRecord 
		// Include Property, come from BuffRecord 
		// Include Property, come from PlayerHero 
		static const std::string& FightHeroCnfID(){ static std::string x = "FightHeroCnfID"; return x; };// string
		static const std::string& FightHeroID(){ static std::string x = "FightHeroID"; return x; };// object
		static const std::string& ForgeEquipID(){ static std::string x = "ForgeEquipID"; return x; };// object
		static const std::string& ForgeEquipLuckyItem(){ static std::string x = "ForgeEquipLuckyItem"; return x; };// string
		static const std::string& ForgeEquipRate(){ static std::string x = "ForgeEquipRate"; return x; };// int
		static const std::string& ForgeOrInlay(){ static std::string x = "ForgeOrInlay"; return x; };// int
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
		static const std::string& InlayEquipID(){ static std::string x = "InlayEquipID"; return x; };// object
		static const std::string& InlayEquipLuckyItem(){ static std::string x = "InlayEquipLuckyItem"; return x; };// string
		static const std::string& InlayEquipRate(){ static std::string x = "InlayEquipRate"; return x; };// int
		static const std::string& InlayStoneID(){ static std::string x = "InlayStoneID"; return x; };// string
		// Include Property, come from PlayerBuilding 
		static const std::string& ClanPickableCrystal(){ static std::string x = "ClanPickableCrystal"; return x; };// int
		static const std::string& ClanPickableSourceOfLife(){ static std::string x = "ClanPickableSourceOfLife"; return x; };// int
		static const std::string& ClanPickableSteel(){ static std::string x = "ClanPickableSteel"; return x; };// int
		static const std::string& ClanPickableStone(){ static std::string x = "ClanPickableStone"; return x; };// int
		static const std::string& ClanPickableWood(){ static std::string x = "ClanPickableWood"; return x; };// int
		static const std::string& LastProduceTime(){ static std::string x = "LastProduceTime"; return x; };// int
		static const std::string& PickableSteel(){ static std::string x = "PickableSteel"; return x; };// int
		static const std::string& PickableStone(){ static std::string x = "PickableStone"; return x; };// int
		static const std::string& PickableWood(){ static std::string x = "PickableWood"; return x; };// int
		// Include Property, come from PlayerPVP 
		static const std::string& BetBattlePoint(){ static std::string x = "BetBattlePoint"; return x; };// int
		static const std::string& BetDiamond(){ static std::string x = "BetDiamond"; return x; };// int
		static const std::string& Commend(){ static std::string x = "Commend"; return x; };// int
		static const std::string& LastTeleportTime(){ static std::string x = "LastTeleportTime"; return x; };// int
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
		// Include Property, come from PlayerFutures 
		// Include Property, come from PlayerShopBenefit 
		// Include Property, come from PlayerLotto 
		static const std::string& LottoBonus(){ static std::string x = "LottoBonus"; return x; };// int
		static const std::string& LottoCredit(){ static std::string x = "LottoCredit"; return x; };// int
		// Include Property, come from PlayerScenario 
		static const std::string& ScenarioEquipId(){ static std::string x = "ScenarioEquipId"; return x; };// object
		static const std::string& ScenarioId(){ static std::string x = "ScenarioId"; return x; };// int
		static const std::string& ScenarioStatusId(){ static std::string x = "ScenarioStatusId"; return x; };// int
		static const std::string& ScenarioStepId(){ static std::string x = "ScenarioStepId"; return x; };// int
		// Include Property, come from PlayerDungeon 
		static const std::string& DungeonFinished(){ static std::string x = "DungeonFinished"; return x; };// int
		static const std::string& DungeonKilledAll(){ static std::string x = "DungeonKilledAll"; return x; };// int
		static const std::string& DungeonKilledBoss(){ static std::string x = "DungeonKilledBoss"; return x; };// int
		static const std::string& DungeonKilledElite(){ static std::string x = "DungeonKilledElite"; return x; };// int
		static const std::string& DungeonLevel(){ static std::string x = "DungeonLevel"; return x; };// int
		static const std::string& DungeonNextCost(){ static std::string x = "DungeonNextCost"; return x; };// int
		static const std::string& DungeonWin(){ static std::string x = "DungeonWin"; return x; };// int
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
			static const int ConfigID = 1;//string
			static const int Level = 2;//int
			static const int Pos = 3;//vector3

		};
		class Clan_DefNPC
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_DefNPC"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Level = 2;//int
			static const int Pos = 3;//vector3

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
			static const int LastTime = 4;//string
			static const int JoinTime = 5;//int
			static const int BP = 6;//int
			static const int Online = 7;//int
			static const int Title = 8;//int
			static const int TotalContribution = 9;//int
			static const int Contribution = 10;//int

		};
		class Clan_Property
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_Property"; return x; };
			static const int GUID = 0;//object
			static const int Clan_Name = 1;//string
			static const int Clan_PresidentID = 2;//object
			static const int Clan_PresidentName = 3;//string
			static const int Clan_Rank = 4;//int
			static const int Clan_AD = 5;//string
			static const int Clan_Desc = 6;//string
			static const int Clan_Gold = 7;//int
			static const int Clan_Level = 8;//int
			static const int Clan_Icon = 9;//string
			static const int Clan_CreateTime = 10;//int
			static const int Clan_MapTileID = 11;//string
			static const int Clan_GameID = 12;//int
			static const int Steel = 13;//int
			static const int Stone = 14;//int
			static const int Wood = 15;//int
			static const int Crystal = 16;//int
			static const int SourceOfLife = 17;//int
			static const int GemWind = 18;//int
			static const int GemFire = 19;//int
			static const int GemLight = 20;//int
			static const int GemPoison = 21;//int
			static const int GemDark = 22;//int
			static const int GemIce = 23;//int

		};
		class Clan_Recommendation
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_Recommendation"; return x; };
			static const int ClanID = 0;//object
			static const int ClanName = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Level = 4;//object
			static const int MemberCount = 5;//int
			static const int MaxMemberCount = 6;//int

		};
		class Clan_SearchList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_SearchList"; return x; };
			static const int ClanID = 0;//object
			static const int ClanName = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Level = 4;//int
			static const int MemberCount = 5;//int
			static const int MaxMemberCount = 6;//int

		};
		class Clan_WarApplyList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarApplyList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int DefenderClan = 4;//object
			static const int DefenderClanName = 5;//string
			static const int MapTileID = 6;//string

		};
		class Clan_WarList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int Status = 4;//int
			static const int AttackerWin = 5;//int
			static const int DefenderClan = 6;//object
			static const int DefenderClanName = 7;//string
			static const int SetupTime = 8;//int
			static const int FinishTime = 9;//int
			static const int MapTileID = 10;//string

		};
		class Clan_WarNPC
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarNPC"; return x; };
			static const int NPCID = 0;//object
			static const int NPCConfigID = 1;//string
			static const int Leader = 2;//object
			static const int LeaderName = 3;//string
			static const int Pos = 4;//vector3

		};
		class Clan_WarReceiveList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarReceiveList"; return x; };
			static const int GUID = 0;//object
			static const int AttackerClan = 1;//object
			static const int AttackerClanName = 2;//string
			static const int Money = 3;//int
			static const int DefenderClan = 4;//object
			static const int DefenderClanName = 5;//string
			static const int MapTileID = 6;//string

		};
		class Clan_WarResource
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarResource"; return x; };
			static const int PlayerID = 0;//object
			static const int Leader = 1;//object
			static const int LeaderName = 2;//string
			static const int Steel = 3;//int
			static const int Wood = 4;//int
			static const int Stone = 5;//int
			static const int Crystal = 6;//int

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
			static const int Stone1 = 7;//string
			static const int Stone2 = 8;//string
			static const int Stone3 = 9;//string
			static const int Stone4 = 10;//string
			static const int Lock = 11;//int
			static const int Future = 12;//int
			static const int UserData = 13;//string

		};
		class RuntimeBuff
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "RuntimeBuff"; return x; };
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
		class BuffRecord
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuffRecord"; return x; };
			static const int ConfigID = 0;//string
			static const int Releaser = 1;//object
			static const int ReleasedTime = 2;//int
			static const int EffectTriggerCount = 3;//int
			static const int EffectTriggerred = 4;//int
			static const int BuffGroupID = 5;//int

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
		class FuturesAccepted
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FuturesAccepted"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int RandPropertyID = 2;//string
			static const int RandPropertyValue = 3;//int
			static const int NowIntensifyLevel = 4;//int
			static const int FutureIntensifyLevel = 5;//int
			static const int NowStone1 = 6;//string
			static const int NowStone2 = 7;//string
			static const int NowStone3 = 8;//string
			static const int NowStone4 = 9;//string
			static const int FutureStone1 = 10;//string
			static const int FutureStone2 = 11;//string
			static const int FutureStone3 = 12;//string
			static const int FutureStone4 = 13;//string
			static const int Owner = 14;//object
			static const int OwnerName = 15;//string
			static const int Price = 16;//int
			static const int Date = 17;//int
			static const int Deliverable = 18;//int

		};
		class FuturesReleased
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FuturesReleased"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int RandPropertyID = 2;//string
			static const int RandPropertyValue = 3;//int
			static const int NowIntensifyLevel = 4;//int
			static const int FutureIntensifyLevel = 5;//int
			static const int NowStone1 = 6;//string
			static const int NowStone2 = 7;//string
			static const int NowStone3 = 8;//string
			static const int NowStone4 = 9;//string
			static const int FutureStone1 = 10;//string
			static const int FutureStone2 = 11;//string
			static const int FutureStone3 = 12;//string
			static const int FutureStone4 = 13;//string
			static const int Date = 14;//int
			static const int Price = 15;//int

		};
		class FuturesVacant
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "FuturesVacant"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int RandPropertyID = 2;//string
			static const int RandPropertyValue = 3;//int
			static const int NowIntensifyLevel = 4;//int
			static const int FutureIntensifyLevel = 5;//int
			static const int NowStone1 = 6;//string
			static const int NowStone2 = 7;//string
			static const int NowStone3 = 8;//string
			static const int NowStone4 = 9;//string
			static const int FutureStone1 = 10;//string
			static const int FutureStone2 = 11;//string
			static const int FutureStone3 = 12;//string
			static const int FutureStone4 = 13;//string
			static const int Owner = 14;//object
			static const int OwnerName = 15;//string
			static const int Price = 16;//int
			static const int Date = 17;//int
			static const int Acceptable = 18;//int

		};
		class BuyRecord
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BuyRecord"; return x; };
			static const int ShopID = 0;//string
			static const int Diamond = 1;//int
			static const int Time = 2;//int

		};
		class ShopBenefit
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "ShopBenefit"; return x; };
			static const int ShopID = 0;//string
			static const int Diamond = 1;//int
			static const int PickTime = 2;//int

		};
		class LottoTickets
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "LottoTickets"; return x; };
			static const int GUID = 0;//object
			static const int Ticket = 1;//int

		};
		class DraftInventory
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "DraftInventory"; return x; };
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int

		};
		class NextLevelInventory
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "NextLevelInventory"; return x; };
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int

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
		static const std::string& RelivePosEx(){ static std::string x = "RelivePosEx"; return x; };// string
		static const std::string& ResPath(){ static std::string x = "ResPath"; return x; };// string
		static const std::string& ResourcePos(){ static std::string x = "ResourcePos"; return x; };// string
		static const std::string& SceneName(){ static std::string x = "SceneName"; return x; };// string
		static const std::string& SceneShowName(){ static std::string x = "SceneShowName"; return x; };// string
		static const std::string& SoundList(){ static std::string x = "SoundList"; return x; };// string
		static const std::string& SubType(){ static std::string x = "SubType"; return x; };// int
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
		static const std::string& Best(){ static std::string x = "Best"; return x; };// int
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& Dollar(){ static std::string x = "Dollar"; return x; };// float
		static const std::string& ExtraDay(){ static std::string x = "ExtraDay"; return x; };// int
		static const std::string& ExtraDiamondPerDay(){ static std::string x = "ExtraDiamondPerDay"; return x; };// int
		static const std::string& ItemCount(){ static std::string x = "ItemCount"; return x; };// int
		static const std::string& ItemID(){ static std::string x = "ItemID"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& LimitTimePerWeek(){ static std::string x = "LimitTimePerWeek"; return x; };// int
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