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
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
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
	class ConsumeData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "ConsumeData"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
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
	class DescData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "DescData"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& Atlas(){ static std::string x = "Atlas"; return x; };// string
		static const std::string& DescText(){ static std::string x = "DescText"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& PerformanceEffect(){ static std::string x = "PerformanceEffect"; return x; };// string
		static const std::string& PerformanceSound(){ static std::string x = "PerformanceSound"; return x; };// string
		static const std::string& PrefabPath(){ static std::string x = "PrefabPath"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		// Record

	};
	class EffectData
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "EffectData"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
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
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& LUCK(){ static std::string x = "LUCK"; return x; };// int
		static const std::string& MAGIC_GATE(){ static std::string x = "MAGIC_GATE"; return x; };// int
		static const std::string& MAXHP(){ static std::string x = "MAXHP"; return x; };// int
		static const std::string& MAXMP(){ static std::string x = "MAXMP"; return x; };// int
		static const std::string& MAXSP(){ static std::string x = "MAXSP"; return x; };// int
		static const std::string& MOVE_GATE(){ static std::string x = "MOVE_GATE"; return x; };// int
		static const std::string& MOVE_SPEED(){ static std::string x = "MOVE_SPEED"; return x; };// int
		static const std::string& MP(){ static std::string x = "MP"; return x; };// int
		static const std::string& MPREGEN(){ static std::string x = "MPREGEN"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SPREGEN(){ static std::string x = "SPREGEN"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		// Record

	};
	class Equip
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Equip"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& BuyPrice(){ static std::string x = "BuyPrice"; return x; };// int
		static const std::string& Desc(){ static std::string x = "Desc"; return x; };// string
		static const std::string& DropPrePath(){ static std::string x = "DropPrePath"; return x; };// string
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& EnchantmentBuffList(){ static std::string x = "EnchantmentBuffList"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& IntensiveBuffList(){ static std::string x = "IntensiveBuffList"; return x; };// string
		static const std::string& ItemSubType(){ static std::string x = "ItemSubType"; return x; };// int
		static const std::string& ItemType(){ static std::string x = "ItemType"; return x; };// int
		static const std::string& Job(){ static std::string x = "Job"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& PrefabPath(){ static std::string x = "PrefabPath"; return x; };// string
		static const std::string& Quality(){ static std::string x = "Quality"; return x; };// int
		static const std::string& SalePrice(){ static std::string x = "SalePrice"; return x; };// int
		static const std::string& Sex(){ static std::string x = "Sex"; return x; };// int
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SuitBuffID(){ static std::string x = "SuitBuffID"; return x; };// string
		static const std::string& SuitID(){ static std::string x = "SuitID"; return x; };// int
		// Record

	};
	class GM
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "GM"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		// Record

	};
	class Group
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Group"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& MatchGambleDiamond(){ static std::string x = "MatchGambleDiamond"; return x; };// int
		static const std::string& MatchID(){ static std::string x = "MatchID"; return x; };// object
		static const std::string& MatchOpponentCup(){ static std::string x = "MatchOpponentCup"; return x; };// int
		static const std::string& MatchOpponentD(){ static std::string x = "MatchOpponentD"; return x; };// int
		static const std::string& MatchOpponentDiamond(){ static std::string x = "MatchOpponentDiamond"; return x; };// int
		static const std::string& MatchOpponentGold(){ static std::string x = "MatchOpponentGold"; return x; };// int
		static const std::string& MatchOpponentHead(){ static std::string x = "MatchOpponentHead"; return x; };// string
		static const std::string& MatchOpponentHeroCnf1(){ static std::string x = "MatchOpponentHeroCnf1"; return x; };// string
		static const std::string& MatchOpponentHeroCnf2(){ static std::string x = "MatchOpponentHeroCnf2"; return x; };// string
		static const std::string& MatchOpponentHeroCnf3(){ static std::string x = "MatchOpponentHeroCnf3"; return x; };// string
		static const std::string& MatchOpponentHeroHP1(){ static std::string x = "MatchOpponentHeroHP1"; return x; };// int
		static const std::string& MatchOpponentHeroHP2(){ static std::string x = "MatchOpponentHeroHP2"; return x; };// int
		static const std::string& MatchOpponentHeroHP3(){ static std::string x = "MatchOpponentHeroHP3"; return x; };// int
		static const std::string& MatchOpponentHeroID1(){ static std::string x = "MatchOpponentHeroID1"; return x; };// object
		static const std::string& MatchOpponentHeroID2(){ static std::string x = "MatchOpponentHeroID2"; return x; };// object
		static const std::string& MatchOpponentHeroID3(){ static std::string x = "MatchOpponentHeroID3"; return x; };// object
		static const std::string& MatchOpponentHeroStar1(){ static std::string x = "MatchOpponentHeroStar1"; return x; };// int
		static const std::string& MatchOpponentHeroStar2(){ static std::string x = "MatchOpponentHeroStar2"; return x; };// int
		static const std::string& MatchOpponentHeroStar3(){ static std::string x = "MatchOpponentHeroStar3"; return x; };// int
		static const std::string& MatchOpponentID(){ static std::string x = "MatchOpponentID"; return x; };// object
		static const std::string& MatchOpponentK(){ static std::string x = "MatchOpponentK"; return x; };// int
		static const std::string& MatchOpponentLevel(){ static std::string x = "MatchOpponentLevel"; return x; };// int
		static const std::string& MatchOpponentName(){ static std::string x = "MatchOpponentName"; return x; };// string
		static const std::string& MatchStar(){ static std::string x = "MatchStar"; return x; };// int
		static const std::string& MatchTeamID(){ static std::string x = "MatchTeamID"; return x; };// object
		// Record
		class MatchMember
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "MatchMember"; return x; };
			static const int GUID = 0;//object
			static const int Name = 1;//string
			static const int K = 2;//int
			static const int D = 3;//int
			static const int A = 4;//int
			static const int Diamond = 5;//int
			static const int Cup = 6;//int
			static const int MVP = 7;//int
			static const int Streak = 8;//int
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

	};
	class IObject
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "IObject"; return x; };		// Property
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Record

	};
	class InitProperty
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "InitProperty"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& Job(){ static std::string x = "Job"; return x; };// int
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& ModelPtah(){ static std::string x = "ModelPtah"; return x; };// string
		static const std::string& SkillIDRef(){ static std::string x = "SkillIDRef"; return x; };// string
		// Record

	};
	class Item
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Item"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& AwardData(){ static std::string x = "AwardData"; return x; };// string
		static const std::string& AwardProperty(){ static std::string x = "AwardProperty"; return x; };// int
		static const std::string& BuyPrice(){ static std::string x = "BuyPrice"; return x; };// int
		static const std::string& ConsumeData(){ static std::string x = "ConsumeData"; return x; };// string
		static const std::string& CoolDownTime(){ static std::string x = "CoolDownTime"; return x; };// float
		static const std::string& DescID(){ static std::string x = "DescID"; return x; };// string
		static const std::string& EffectData(){ static std::string x = "EffectData"; return x; };// string
		static const std::string& ExpiredType(){ static std::string x = "ExpiredType"; return x; };// int
		static const std::string& Extend(){ static std::string x = "Extend"; return x; };// string
		static const std::string& HeroType(){ static std::string x = "HeroType"; return x; };// int
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& ItemSubType(){ static std::string x = "ItemSubType"; return x; };// int
		static const std::string& ItemType(){ static std::string x = "ItemType"; return x; };// int
		static const std::string& Job(){ static std::string x = "Job"; return x; };// string
		static const std::string& Level(){ static std::string x = "Level"; return x; };// int
		static const std::string& OverlayCount(){ static std::string x = "OverlayCount"; return x; };// int
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
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& Chinese(){ static std::string x = "Chinese"; return x; };// string
		static const std::string& English(){ static std::string x = "English"; return x; };// string
		static const std::string& French(){ static std::string x = "French"; return x; };// string
		static const std::string& Spanish(){ static std::string x = "Spanish"; return x; };// string
		// Record

	};
	class Map
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Map"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& InComeDiamond(){ static std::string x = "InComeDiamond"; return x; };// int
		static const std::string& InComeGold(){ static std::string x = "InComeGold"; return x; };// int
		static const std::string& InComeOil(){ static std::string x = "InComeOil"; return x; };// int
		static const std::string& MapLevel(){ static std::string x = "MapLevel"; return x; };// int
		static const std::string& MaxCount(){ static std::string x = "MaxCount"; return x; };// int
		// Record
		class Station
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Station"; return x; };
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
		static const std::string& ThisName(){ static std::string x = "NPC"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& AIOwnerID(){ static std::string x = "AIOwnerID"; return x; };// object
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
		static const std::string& Camp(){ static std::string x = "Camp"; return x; };// object
		static const std::string& Climb(){ static std::string x = "Climb"; return x; };// int
		static const std::string& ConsumeData(){ static std::string x = "ConsumeData"; return x; };// string
		static const std::string& CrisisDis(){ static std::string x = "CrisisDis"; return x; };// float
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
		static const std::string& MasterID(){ static std::string x = "MasterID"; return x; };// object
		static const std::string& MasterName(){ static std::string x = "MasterName"; return x; };// string
		static const std::string& MeleeType(){ static std::string x = "MeleeType"; return x; };// int
		static const std::string& NPCSubType(){ static std::string x = "NPCSubType"; return x; };// int
		static const std::string& NPCType(){ static std::string x = "NPCType"; return x; };// int
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& Prefab(){ static std::string x = "Prefab"; return x; };// string
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SPREGEN(){ static std::string x = "SPREGEN"; return x; };// int
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
		static const std::string& VIPEXP(){ static std::string x = "VIPEXP"; return x; };// int
		static const std::string& Width(){ static std::string x = "Width"; return x; };// float
		// Record
		class Cooldown
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Cooldown"; return x; };
			static const int SkillID = 0;//string
			static const int Time = 1;//int

		};

	};
	class NoSqlServer
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "NoSqlServer"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
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
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& ATK_DARK(){ static std::string x = "ATK_DARK"; return x; };// int
		static const std::string& ATK_FIRE(){ static std::string x = "ATK_FIRE"; return x; };// int
		static const std::string& ATK_ICE(){ static std::string x = "ATK_ICE"; return x; };// int
		static const std::string& ATK_LIGHT(){ static std::string x = "ATK_LIGHT"; return x; };// int
		static const std::string& ATK_POISON(){ static std::string x = "ATK_POISON"; return x; };// int
		static const std::string& ATK_PVP(){ static std::string x = "ATK_PVP"; return x; };// int
		static const std::string& ATK_SPEED(){ static std::string x = "ATK_SPEED"; return x; };// int
		static const std::string& ATK_VALUE(){ static std::string x = "ATK_VALUE"; return x; };// int
		static const std::string& ATK_WIND(){ static std::string x = "ATK_WIND"; return x; };// int
		static const std::string& Account(){ static std::string x = "Account"; return x; };// string
		static const std::string& BUFF_GATE(){ static std::string x = "BUFF_GATE"; return x; };// int
		static const std::string& CRITICAL(){ static std::string x = "CRITICAL"; return x; };// int
		static const std::string& Camp(){ static std::string x = "Camp"; return x; };// object
		static const std::string& Clan_AD(){ static std::string x = "Clan_AD"; return x; };// string
		static const std::string& Clan_ContinueDay(){ static std::string x = "Clan_ContinueDay"; return x; };// int
		static const std::string& Clan_CreateTime(){ static std::string x = "Clan_CreateTime"; return x; };// int
		static const std::string& Clan_Desc(){ static std::string x = "Clan_Desc"; return x; };// string
		static const std::string& Clan_EctypID(){ static std::string x = "Clan_EctypID"; return x; };// int
		static const std::string& Clan_Exp(){ static std::string x = "Clan_Exp"; return x; };// int
		static const std::string& Clan_Gold(){ static std::string x = "Clan_Gold"; return x; };// int
		static const std::string& Clan_Honor(){ static std::string x = "Clan_Honor"; return x; };// int
		static const std::string& Clan_ID(){ static std::string x = "Clan_ID"; return x; };// object
		static const std::string& Clan_Icon(){ static std::string x = "Clan_Icon"; return x; };// string
		static const std::string& Clan_Level(){ static std::string x = "Clan_Level"; return x; };// int
		static const std::string& Clan_MemeberMaxCount(){ static std::string x = "Clan_MemeberMaxCount"; return x; };// int
		static const std::string& Clan_Name(){ static std::string x = "Clan_Name"; return x; };// string
		static const std::string& Clan_PresidentID(){ static std::string x = "Clan_PresidentID"; return x; };// object
		static const std::string& Clan_PresidentName(){ static std::string x = "Clan_PresidentName"; return x; };// string
		static const std::string& Cland_MemeberCount(){ static std::string x = "Cland_MemeberCount"; return x; };// int
		static const std::string& Cland_Rank(){ static std::string x = "Cland_Rank"; return x; };// int
		static const std::string& Commend(){ static std::string x = "Commend"; return x; };// int
		static const std::string& ConnectKey(){ static std::string x = "ConnectKey"; return x; };// string
		static const std::string& Cup(){ static std::string x = "Cup"; return x; };// int
		static const std::string& DEF_DARK(){ static std::string x = "DEF_DARK"; return x; };// int
		static const std::string& DEF_FIRE(){ static std::string x = "DEF_FIRE"; return x; };// int
		static const std::string& DEF_ICE(){ static std::string x = "DEF_ICE"; return x; };// int
		static const std::string& DEF_LIGHT(){ static std::string x = "DEF_LIGHT"; return x; };// int
		static const std::string& DEF_POISON(){ static std::string x = "DEF_POISON"; return x; };// int
		static const std::string& DEF_PVP(){ static std::string x = "DEF_PVP"; return x; };// int
		static const std::string& DEF_VALUE(){ static std::string x = "DEF_VALUE"; return x; };// int
		static const std::string& DEF_WIND(){ static std::string x = "DEF_WIND"; return x; };// int
		static const std::string& DIZZY_GATE(){ static std::string x = "DIZZY_GATE"; return x; };// int
		static const std::string& Diamond(){ static std::string x = "Diamond"; return x; };// int
		static const std::string& EXP(){ static std::string x = "EXP"; return x; };// int
		static const std::string& FightHeroCnfID(){ static std::string x = "FightHeroCnfID"; return x; };// string
		static const std::string& FightHeroID(){ static std::string x = "FightHeroID"; return x; };// object
		static const std::string& FirstTarget(){ static std::string x = "FirstTarget"; return x; };// object
		static const std::string& GMLevel(){ static std::string x = "GMLevel"; return x; };// int
		static const std::string& GameID(){ static std::string x = "GameID"; return x; };// int
		static const std::string& GateID(){ static std::string x = "GateID"; return x; };// int
		static const std::string& Gold(){ static std::string x = "Gold"; return x; };// int
		static const std::string& GuildID(){ static std::string x = "GuildID"; return x; };// object
		static const std::string& HP(){ static std::string x = "HP"; return x; };// int
		static const std::string& HPREGEN(){ static std::string x = "HPREGEN"; return x; };// int
		static const std::string& Head(){ static std::string x = "Head"; return x; };// string
		static const std::string& HeroCnfID1(){ static std::string x = "HeroCnfID1"; return x; };// string
		static const std::string& HeroCnfID2(){ static std::string x = "HeroCnfID2"; return x; };// string
		static const std::string& HeroCnfID3(){ static std::string x = "HeroCnfID3"; return x; };// string
		static const std::string& HeroID1(){ static std::string x = "HeroID1"; return x; };// object
		static const std::string& HeroID2(){ static std::string x = "HeroID2"; return x; };// object
		static const std::string& HeroID3(){ static std::string x = "HeroID3"; return x; };// object
		static const std::string& HeroStar1(){ static std::string x = "HeroStar1"; return x; };// int
		static const std::string& HeroStar2(){ static std::string x = "HeroStar2"; return x; };// int
		static const std::string& HeroStar3(){ static std::string x = "HeroStar3"; return x; };// int
		static const std::string& HomeCellPos(){ static std::string x = "HomeCellPos"; return x; };// vector3
		static const std::string& HomeSceneID(){ static std::string x = "HomeSceneID"; return x; };// int
		static const std::string& Item1(){ static std::string x = "Item1"; return x; };// string
		static const std::string& Item1Count(){ static std::string x = "Item1Count"; return x; };// int
		static const std::string& Item2(){ static std::string x = "Item2"; return x; };// string
		static const std::string& Item2Count(){ static std::string x = "Item2Count"; return x; };// int
		static const std::string& Item3(){ static std::string x = "Item3"; return x; };// string
		static const std::string& Item3Count(){ static std::string x = "Item3Count"; return x; };// int
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
		static const std::string& OnlineCount(){ static std::string x = "OnlineCount"; return x; };// int
		static const std::string& OnlineTime(){ static std::string x = "OnlineTime"; return x; };// object
		static const std::string& PHYSICAL_GATE(){ static std::string x = "PHYSICAL_GATE"; return x; };// int
		static const std::string& PrefabPath(){ static std::string x = "PrefabPath"; return x; };// string
		static const std::string& REFLECTDAMAGE(){ static std::string x = "REFLECTDAMAGE"; return x; };// int
		static const std::string& RELIVE_SOUL(){ static std::string x = "RELIVE_SOUL"; return x; };// int
		static const std::string& Race(){ static std::string x = "Race"; return x; };// int
		static const std::string& Report(){ static std::string x = "Report"; return x; };// int
		static const std::string& SKILL_GATE(){ static std::string x = "SKILL_GATE"; return x; };// int
		static const std::string& SP(){ static std::string x = "SP"; return x; };// int
		static const std::string& SPREGEN(){ static std::string x = "SPREGEN"; return x; };// int
		static const std::string& SUCKBLOOD(){ static std::string x = "SUCKBLOOD"; return x; };// int
		static const std::string& Sex(){ static std::string x = "Sex"; return x; };// int
		static const std::string& SkillNormal(){ static std::string x = "SkillNormal"; return x; };// string
		static const std::string& SkillSpecial1(){ static std::string x = "SkillSpecial1"; return x; };// string
		static const std::string& SkillSpecial2(){ static std::string x = "SkillSpecial2"; return x; };// string
		static const std::string& SkillTHUMP(){ static std::string x = "SkillTHUMP"; return x; };// string
		static const std::string& Streak(){ static std::string x = "Streak"; return x; };// int
		static const std::string& TeamID(){ static std::string x = "TeamID"; return x; };// object
		static const std::string& TotalLineTime(){ static std::string x = "TotalLineTime"; return x; };// object
		static const std::string& TotalTime(){ static std::string x = "TotalTime"; return x; };// int
		static const std::string& VIPEXP(){ static std::string x = "VIPEXP"; return x; };// int
		static const std::string& VIPLevel(){ static std::string x = "VIPLevel"; return x; };// int
		// Record
		class BagEquipList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BagEquipList"; return x; };
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
			static const std::string& ThisName(){ static std::string x = "BagItemList"; return x; };
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int
			static const int Bound = 2;//int
			static const int ExpiredType = 3;//int
			static const int Date = 4;//int

		};
		class BlockList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "BlockList"; return x; };
			static const int GUID = 0;//object

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
			static const int VIP = 6;//int
			static const int Online = 7;//int
			static const int Power = 8;//int
			static const int Title = 9;//int
			static const int GameID = 10;//int
			static const int JoinTime = 11;//int
			static const int Contribution = 12;//int
			static const int LastTime = 13;//string
			static const int AllContribution = 14;//int

		};
		class Clan_WarList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "Clan_WarList"; return x; };
			static const int GUID = 0;//object
			static const int Level = 1;//int

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
			static const int SPREGEN = 7;//int
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
			static const int SkillID = 0;//string
			static const int Time = 1;//int

		};
		class DropItemList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "DropItemList"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int ItemCount = 2;//int
			static const int Postion = 3;//vector3

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
			static const int SPREGEN = 7;//int
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
		class PlayerHero
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "PlayerHero"; return x; };
			static const int GUID = 0;//object
			static const int ConfigID = 1;//string
			static const int Activated = 2;//int
			static const int Level = 3;//int
			static const int Exp = 4;//int
			static const int Star = 5;//int
			static const int HP = 6;//int
			static const int Ice_Talent = 7;//int
			static const int Fire_Talent = 8;//int
			static const int Light_Talent = 9;//int
			static const int Wind_Talent = 10;//int
			static const int Poison_Talent = 11;//int
			static const int Dark_Talent = 12;//int

		};
		class TaskList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "TaskList"; return x; };
			static const int TaskID = 0;//string
			static const int TaskStatus = 1;//int
			static const int ArgCnfID = 2;//string
			static const int ArgCount = 3;//int

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
		class TempItemList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "TempItemList"; return x; };
			static const int ConfigID = 0;//string
			static const int ItemCount = 1;//int

		};
		class WarList
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "WarList"; return x; };
			static const int WarID = 0;//object
			static const int AttackerID = 1;//object
			static const int BeAttackerID = 2;//object
			static const int AttackerName = 3;//string
			static const int BeAttackerName = 4;//string
			static const int AttackerLevel = 5;//int
			static const int BeAttackerLevel = 6;//int
			static const int AttackerCup = 7;//int
			static const int BeAttackerCup = 8;//int
			static const int AttackerHeroList = 9;//string
			static const int AttackerHeroStarList = 10;//int
			static const int BeAttackerHeroList = 11;//string
			static const int BeAttackerHeroStarList = 12;//int
			static const int Gold = 13;//int
			static const int Diamond = 14;//int
			static const int Winner = 15;//object
			static const int WarStar = 16;//int
			static const int EventTime = 17;//int
			static const int CostTime = 18;//int
			static const int ItemList = 19;//string

		};
		class WarObject
		{
		public:
			//Class name
			static const std::string& ThisName(){ static std::string x = "WarObject"; return x; };
			static const int OjectID = 0;//object
			static const int CreateTime = 1;//int

		};

	};
	class Scene
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Scene"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
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
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& SecurityData(){ static std::string x = "SecurityData"; return x; };// string
		// Record

	};
	class Server
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Server"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& Area(){ static std::string x = "Area"; return x; };// int
		static const std::string& Cell(){ static std::string x = "Cell"; return x; };// int
		static const std::string& CpuCount(){ static std::string x = "CpuCount"; return x; };// int
		static const std::string& IP(){ static std::string x = "IP"; return x; };// string
		static const std::string& MaxOnline(){ static std::string x = "MaxOnline"; return x; };// int
		static const std::string& Port(){ static std::string x = "Port"; return x; };// int
		static const std::string& ServerID(){ static std::string x = "ServerID"; return x; };// int
		static const std::string& Type(){ static std::string x = "Type"; return x; };// int
		static const std::string& WSPort(){ static std::string x = "WSPort"; return x; };// int
		static const std::string& WebPort(){ static std::string x = "WebPort"; return x; };// int
		// Record

	};
	class Skill
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "Skill"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& AnimaState(){ static std::string x = "AnimaState"; return x; };// string
		static const std::string& AtkDis(){ static std::string x = "AtkDis"; return x; };// float
		static const std::string& ConsumeProperty(){ static std::string x = "ConsumeProperty"; return x; };// string
		static const std::string& ConsumeType(){ static std::string x = "ConsumeType"; return x; };// int
		static const std::string& ConsumeValue(){ static std::string x = "ConsumeValue"; return x; };// string
		static const std::string& CoolDownTime(){ static std::string x = "CoolDownTime"; return x; };// float
		static const std::string& DamageCount(){ static std::string x = "DamageCount"; return x; };// int
		static const std::string& DamageDistance(){ static std::string x = "DamageDistance"; return x; };// float
		static const std::string& DamageIntervalTime(){ static std::string x = "DamageIntervalTime"; return x; };// float
		static const std::string& DamageProperty(){ static std::string x = "DamageProperty"; return x; };// string
		static const std::string& DamageType(){ static std::string x = "DamageType"; return x; };// int
		static const std::string& DamageValue(){ static std::string x = "DamageValue"; return x; };// int
		static const std::string& DefaultHitTime(){ static std::string x = "DefaultHitTime"; return x; };// string
		static const std::string& Desc(){ static std::string x = "Desc"; return x; };// string
		static const std::string& EffectObjType(){ static std::string x = "EffectObjType"; return x; };// int
		static const std::string& GetBuffList(){ static std::string x = "GetBuffList"; return x; };// string
		static const std::string& Icon(){ static std::string x = "Icon"; return x; };// string
		static const std::string& Melee(){ static std::string x = "Melee"; return x; };// int
		static const std::string& NewObject(){ static std::string x = "NewObject"; return x; };// string
		static const std::string& NextID(){ static std::string x = "NextID"; return x; };// string
		static const std::string& PlayerSkill(){ static std::string x = "PlayerSkill"; return x; };// int
		static const std::string& SendBuffList(){ static std::string x = "SendBuffList"; return x; };// string
		static const std::string& ShowName(){ static std::string x = "ShowName"; return x; };// string
		static const std::string& SkillType(){ static std::string x = "SkillType"; return x; };// int
		static const std::string& SpriteFile(){ static std::string x = "SpriteFile"; return x; };// string
		// Record

	};
	class SqlServer
	{
	public:
		//Class name
		static const std::string& ThisName(){ static std::string x = "SqlServer"; return x; };		// IObject
		static const std::string& ClassName(){ static std::string x = "ClassName"; return x; };// string
		static const std::string& ConfigID(){ static std::string x = "ConfigID"; return x; };// string
		static const std::string& GroupID(){ static std::string x = "GroupID"; return x; };// int
		static const std::string& ID(){ static std::string x = "ID"; return x; };// string
		static const std::string& MoveTo(){ static std::string x = "MoveTo"; return x; };// vector3
		static const std::string& Name(){ static std::string x = "Name"; return x; };// string
		static const std::string& Position(){ static std::string x = "Position"; return x; };// vector3
		static const std::string& SceneID(){ static std::string x = "SceneID"; return x; };// int
		// Property
		static const std::string& IP(){ static std::string x = "IP"; return x; };// string
		static const std::string& Port(){ static std::string x = "Port"; return x; };// int
		static const std::string& ServerID(){ static std::string x = "ServerID"; return x; };// int
		static const std::string& SqlIP(){ static std::string x = "SqlIP"; return x; };// string
		static const std::string& SqlName(){ static std::string x = "SqlName"; return x; };// string
		static const std::string& SqlPort(){ static std::string x = "SqlPort"; return x; };// int
		static const std::string& SqlPwd(){ static std::string x = "SqlPwd"; return x; };// string
		static const std::string& SqlUser(){ static std::string x = "SqlUser"; return x; };// string
		// Record

	};

}
#endif