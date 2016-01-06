// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.hpp
//    @Author           :    NFrame Studio
//    @Date             :    2016/01/06
//    @Module           :    NFProtocolDefine
// -------------------------------------------------------------------------

#ifndef NF_PR_NAME_HPP
#define NF_PR_NAME_HPP

namespace NFrame
{

class BB_Build
{
public:
	static const std::string& Type(){ static std::string xType = "Type"; return xType;}
	static const std::string& SubType(){ static std::string xSubType = "SubType"; return xSubType;}
	static const std::string& Prefab(){ static std::string xPrefab = "Prefab"; return xPrefab;}
	static const std::string& NormalStateFunc(){ static std::string xNormalStateFunc = "NormalStateFunc"; return xNormalStateFunc;}
	static const std::string& UpStateFunc(){ static std::string xUpStateFunc = "UpStateFunc"; return xUpStateFunc;}
	static const std::string& Icon(){ static std::string xIcon = "Icon"; return xIcon;}
	static const std::string& ShowName(){ static std::string xShowName = "ShowName"; return xShowName;}
	static const std::string& Desc(){ static std::string xDesc = "Desc"; return xDesc;}

};

class BB_Build
{
public:

};

class BB_Player
{
public:
	static const std::string& RoleName(){ static std::string xRoleName = "RoleName"; return xRoleName;}
	static const std::string& Sex(){ static std::string xSex = "Sex"; return xSex;}
	static const std::string& Race(){ static std::string xRace = "Race"; return xRace;}
	static const std::string& Camp(){ static std::string xCamp = "Camp"; return xCamp;}
	static const std::string& LastContainerID(){ static std::string xLastContainerID = "LastContainerID"; return xLastContainerID;}
	static const std::string& Level(){ static std::string xLevel = "Level"; return xLevel;}
	static const std::string& ShowName(){ static std::string xShowName = "ShowName"; return xShowName;}
	static const std::string& Gold(){ static std::string xGold = "Gold"; return xGold;}
	static const std::string& Stone(){ static std::string xStone = "Stone"; return xStone;}
	static const std::string& Steel(){ static std::string xSteel = "Steel"; return xSteel;}
	static const std::string& Diamond(){ static std::string xDiamond = "Diamond"; return xDiamond;}
	static const std::string& LoadPropertyFinish(){ static std::string xLoadPropertyFinish = "LoadPropertyFinish"; return xLoadPropertyFinish;}

};

class BB_Player
{
public:
	const static std::string BuildingList = "BuildingList";
	const static std::string BuildingProduce = "BuildingProduce";

};

class Buff
{
public:
	static const std::string& EffectType(){ static std::string xEffectType = "EffectType"; return xEffectType;}
	static const std::string& EffectValueType(){ static std::string xEffectValueType = "EffectValueType"; return xEffectValueType;}
	static const std::string& EffectValueReferType(){ static std::string xEffectValueReferType = "EffectValueReferType"; return xEffectValueReferType;}
	static const std::string& EffectTimeValue(){ static std::string xEffectTimeValue = "EffectTimeValue"; return xEffectTimeValue;}
	static const std::string& EffectTimeInterval(){ static std::string xEffectTimeInterval = "EffectTimeInterval"; return xEffectTimeInterval;}
	static const std::string& WashGroupID(){ static std::string xWashGroupID = "WashGroupID"; return xWashGroupID;}
	static const std::string& ReverseReferType(){ static std::string xReverseReferType = "ReverseReferType"; return xReverseReferType;}
	static const std::string& EffectClearOnDead(){ static std::string xEffectClearOnDead = "EffectClearOnDead"; return xEffectClearOnDead;}
	static const std::string& DownSaveType(){ static std::string xDownSaveType = "DownSaveType"; return xDownSaveType;}

};

class Buff
{
public:

};

class ChatGroup
{
public:
	static const std::string& Name(){ static std::string xName = "Name"; return xName;}
	static const std::string& CreateObject(){ static std::string xCreateObject = "CreateObject"; return xCreateObject;}

};

class ChatGroup
{
public:
	const static std::string GroupMemberList = "GroupMemberList";
	const static std::string ChatList = "ChatList";

};

class ConsumeData
{
public:
	static const std::string& VIPEXP(){ static std::string xVIPEXP = "VIPEXP"; return xVIPEXP;}
	static const std::string& EXP(){ static std::string xEXP = "EXP"; return xEXP;}
	static const std::string& HP(){ static std::string xHP = "HP"; return xHP;}
	static const std::string& SP(){ static std::string xSP = "SP"; return xSP;}
	static const std::string& MP(){ static std::string xMP = "MP"; return xMP;}
	static const std::string& Gold(){ static std::string xGold = "Gold"; return xGold;}
	static const std::string& Money(){ static std::string xMoney = "Money"; return xMoney;}

};

class ConsumeData
{
public:

};

class Cost
{
public:
	static const std::string& CostMoney(){ static std::string xCostMoney = "CostMoney"; return xCostMoney;}
	static const std::string& CostDiamond(){ static std::string xCostDiamond = "CostDiamond"; return xCostDiamond;}
	static const std::string& CostVP(){ static std::string xCostVP = "CostVP"; return xCostVP;}
	static const std::string& CostHonour(){ static std::string xCostHonour = "CostHonour"; return xCostHonour;}

};

class Cost
{
public:

};

class DescData
{
public:
	static const std::string& ShowName(){ static std::string xShowName = "ShowName"; return xShowName;}
	static const std::string& Icon(){ static std::string xIcon = "Icon"; return xIcon;}
	static const std::string& Atlas(){ static std::string xAtlas = "Atlas"; return xAtlas;}
	static const std::string& PrefabPath(){ static std::string xPrefabPath = "PrefabPath"; return xPrefabPath;}
	static const std::string& PerformanceEffect(){ static std::string xPerformanceEffect = "PerformanceEffect"; return xPerformanceEffect;}
	static const std::string& PerformanceSound(){ static std::string xPerformanceSound = "PerformanceSound"; return xPerformanceSound;}
	static const std::string& DescText(){ static std::string xDescText = "DescText"; return xDescText;}

};

class DescData
{
public:

};

class EffectData
{
public:
	static const std::string& MAXHP(){ static std::string xMAXHP = "MAXHP"; return xMAXHP;}
	static const std::string& MAXMP(){ static std::string xMAXMP = "MAXMP"; return xMAXMP;}
	static const std::string& MAXSP(){ static std::string xMAXSP = "MAXSP"; return xMAXSP;}
	static const std::string& HPREGEN(){ static std::string xHPREGEN = "HPREGEN"; return xHPREGEN;}
	static const std::string& SPREGEN(){ static std::string xSPREGEN = "SPREGEN"; return xSPREGEN;}
	static const std::string& MPREGEN(){ static std::string xMPREGEN = "MPREGEN"; return xMPREGEN;}
	static const std::string& ATK_VALUE(){ static std::string xATK_VALUE = "ATK_VALUE"; return xATK_VALUE;}
	static const std::string& DEF_VALUE(){ static std::string xDEF_VALUE = "DEF_VALUE"; return xDEF_VALUE;}
	static const std::string& MOVE_SPEED(){ static std::string xMOVE_SPEED = "MOVE_SPEED"; return xMOVE_SPEED;}
	static const std::string& ATK_SPEED(){ static std::string xATK_SPEED = "ATK_SPEED"; return xATK_SPEED;}
	static const std::string& ATK_FIRE(){ static std::string xATK_FIRE = "ATK_FIRE"; return xATK_FIRE;}
	static const std::string& ATK_LIGHT(){ static std::string xATK_LIGHT = "ATK_LIGHT"; return xATK_LIGHT;}
	static const std::string& ATK_ICE(){ static std::string xATK_ICE = "ATK_ICE"; return xATK_ICE;}
	static const std::string& ATK_POISON(){ static std::string xATK_POISON = "ATK_POISON"; return xATK_POISON;}
	static const std::string& DEF_FIRE(){ static std::string xDEF_FIRE = "DEF_FIRE"; return xDEF_FIRE;}
	static const std::string& DEF_LIGHT(){ static std::string xDEF_LIGHT = "DEF_LIGHT"; return xDEF_LIGHT;}
	static const std::string& DEF_ICE(){ static std::string xDEF_ICE = "DEF_ICE"; return xDEF_ICE;}
	static const std::string& DEF_POISON(){ static std::string xDEF_POISON = "DEF_POISON"; return xDEF_POISON;}
	static const std::string& DIZZY_GATE(){ static std::string xDIZZY_GATE = "DIZZY_GATE"; return xDIZZY_GATE;}
	static const std::string& MOVE_GATE(){ static std::string xMOVE_GATE = "MOVE_GATE"; return xMOVE_GATE;}
	static const std::string& SKILL_GATE(){ static std::string xSKILL_GATE = "SKILL_GATE"; return xSKILL_GATE;}
	static const std::string& PHYSICAL_GATE(){ static std::string xPHYSICAL_GATE = "PHYSICAL_GATE"; return xPHYSICAL_GATE;}
	static const std::string& MAGIC_GATE(){ static std::string xMAGIC_GATE = "MAGIC_GATE"; return xMAGIC_GATE;}
	static const std::string& BUFF_GATE(){ static std::string xBUFF_GATE = "BUFF_GATE"; return xBUFF_GATE;}

};

class EffectData
{
public:

};

class Equip
{
public:
	static const std::string& Sex(){ static std::string xSex = "Sex"; return xSex;}
	static const std::string& IntensiveBuffList(){ static std::string xIntensiveBuffList = "IntensiveBuffList"; return xIntensiveBuffList;}
	static const std::string& EnchantmentBuffList(){ static std::string xEnchantmentBuffList = "EnchantmentBuffList"; return xEnchantmentBuffList;}
	static const std::string& SuitID(){ static std::string xSuitID = "SuitID"; return xSuitID;}
	static const std::string& SuitBuffID(){ static std::string xSuitBuffID = "SuitBuffID"; return xSuitBuffID;}

};

class Equip
{
public:

};

class Guild
{
public:
	static const std::string& Name(){ static std::string xName = "Name"; return xName;}
	static const std::string& PresidentID(){ static std::string xPresidentID = "PresidentID"; return xPresidentID;}
	static const std::string& PresidentName(){ static std::string xPresidentName = "PresidentName"; return xPresidentName;}
	static const std::string& Rank(){ static std::string xRank = "Rank"; return xRank;}
	static const std::string& GuildAD(){ static std::string xGuildAD = "GuildAD"; return xGuildAD;}
	static const std::string& GuildDesc(){ static std::string xGuildDesc = "GuildDesc"; return xGuildDesc;}
	static const std::string& GuildMoney(){ static std::string xGuildMoney = "GuildMoney"; return xGuildMoney;}
	static const std::string& GuildLevel(){ static std::string xGuildLevel = "GuildLevel"; return xGuildLevel;}
	static const std::string& GuildContinueDay(){ static std::string xGuildContinueDay = "GuildContinueDay"; return xGuildContinueDay;}
	static const std::string& GuilID(){ static std::string xGuilID = "GuilID"; return xGuilID;}
	static const std::string& GuilIDIcon(){ static std::string xGuilIDIcon = "GuilIDIcon"; return xGuilIDIcon;}
	static const std::string& GuildMemeberCount(){ static std::string xGuildMemeberCount = "GuildMemeberCount"; return xGuildMemeberCount;}
	static const std::string& GuildMemeberMaxCount(){ static std::string xGuildMemeberMaxCount = "GuildMemeberMaxCount"; return xGuildMemeberMaxCount;}
	static const std::string& GuildHonor(){ static std::string xGuildHonor = "GuildHonor"; return xGuildHonor;}
	static const std::string& GuildCreateTime(){ static std::string xGuildCreateTime = "GuildCreateTime"; return xGuildCreateTime;}
	static const std::string& GuildCreateter(){ static std::string xGuildCreateter = "GuildCreateter"; return xGuildCreateter;}
	static const std::string& GuildExp(){ static std::string xGuildExp = "GuildExp"; return xGuildExp;}
	static const std::string& GuildStatus(){ static std::string xGuildStatus = "GuildStatus"; return xGuildStatus;}
	static const std::string& DismissTime(){ static std::string xDismissTime = "DismissTime"; return xDismissTime;}
	static const std::string& RecruitAD(){ static std::string xRecruitAD = "RecruitAD"; return xRecruitAD;}
	static const std::string& RecruitLevel(){ static std::string xRecruitLevel = "RecruitLevel"; return xRecruitLevel;}
	static const std::string& AutoRecruit(){ static std::string xAutoRecruit = "AutoRecruit"; return xAutoRecruit;}

};

class Guild
{
public:
	const static std::string GuildBoss = "GuildBoss";
	const static std::string GuildMemberList = "GuildMemberList";
	const static std::string GuildAppyList = "GuildAppyList";
	const static std::string GuildEvent = "GuildEvent";
	const static std::string GuildHouse = "GuildHouse";
	const static std::string GuildSkill = "GuildSkill";

};

class GuildConfig
{
public:
	static const std::string& ConditionPlayerLevel(){ static std::string xConditionPlayerLevel = "ConditionPlayerLevel"; return xConditionPlayerLevel;}
	static const std::string& ConditionPlayerVIP(){ static std::string xConditionPlayerVIP = "ConditionPlayerVIP"; return xConditionPlayerVIP;}
	static const std::string& CostMoney(){ static std::string xCostMoney = "CostMoney"; return xCostMoney;}
	static const std::string& DismissTime(){ static std::string xDismissTime = "DismissTime"; return xDismissTime;}
	static const std::string& GuildLevel(){ static std::string xGuildLevel = "GuildLevel"; return xGuildLevel;}
	static const std::string& MaxMember(){ static std::string xMaxMember = "MaxMember"; return xMaxMember;}

};

class GuildConfig
{
public:

};

class GuildJob
{
public:
	static const std::string& GuildJob(){ static std::string xGuildJob = "GuildJob"; return xGuildJob;}
	static const std::string& JobCount(){ static std::string xJobCount = "JobCount"; return xJobCount;}
	static const std::string& Appoint(){ static std::string xAppoint = "Appoint"; return xAppoint;}
	static const std::string& Fire(){ static std::string xFire = "Fire"; return xFire;}
	static const std::string& Demise(){ static std::string xDemise = "Demise"; return xDemise;}
	static const std::string& ApplyDismiss(){ static std::string xApplyDismiss = "ApplyDismiss"; return xApplyDismiss;}
	static const std::string& StopDismiss(){ static std::string xStopDismiss = "StopDismiss"; return xStopDismiss;}
	static const std::string& AcceptApply(){ static std::string xAcceptApply = "AcceptApply"; return xAcceptApply;}
	static const std::string& DenyApply(){ static std::string xDenyApply = "DenyApply"; return xDenyApply;}
	static const std::string& Kickout(){ static std::string xKickout = "Kickout"; return xKickout;}
	static const std::string& SetRecruit(){ static std::string xSetRecruit = "SetRecruit"; return xSetRecruit;}
	static const std::string& PublishRecruit(){ static std::string xPublishRecruit = "PublishRecruit"; return xPublishRecruit;}
	static const std::string& EditAD(){ static std::string xEditAD = "EditAD"; return xEditAD;}
	static const std::string& Leave(){ static std::string xLeave = "Leave"; return xLeave;}
	static const std::string& LevelUp(){ static std::string xLevelUp = "LevelUp"; return xLevelUp;}

};

class GuildJob
{
public:

};

class GuildName
{
public:
	static const std::string& GuildID(){ static std::string xGuildID = "GuildID"; return xGuildID;}

};

class GuildName
{
public:

};

class InitProperty
{
public:
	static const std::string& Job(){ static std::string xJob = "Job"; return xJob;}
	static const std::string& Level(){ static std::string xLevel = "Level"; return xLevel;}
	static const std::string& EffectData(){ static std::string xEffectData = "EffectData"; return xEffectData;}
	static const std::string& SkillIDRef(){ static std::string xSkillIDRef = "SkillIDRef"; return xSkillIDRef;}
	static const std::string& ModelPtah(){ static std::string xModelPtah = "ModelPtah"; return xModelPtah;}

};

class InitProperty
{
public:

};

class IObject
{
public:
	static const std::string& ID(){ static std::string xID = "ID"; return xID;}
	static const std::string& ClassName(){ static std::string xClassName = "ClassName"; return xClassName;}
	static const std::string& SceneID(){ static std::string xSceneID = "SceneID"; return xSceneID;}
	static const std::string& GroupID(){ static std::string xGroupID = "GroupID"; return xGroupID;}
	static const std::string& ConfigID(){ static std::string xConfigID = "ConfigID"; return xConfigID;}

};

class IObject
{
public:

};

class Item
{
public:
	static const std::string& ItemType(){ static std::string xItemType = "ItemType"; return xItemType;}
	static const std::string& ItemSubType(){ static std::string xItemSubType = "ItemSubType"; return xItemSubType;}
	static const std::string& Level(){ static std::string xLevel = "Level"; return xLevel;}
	static const std::string& Job(){ static std::string xJob = "Job"; return xJob;}
	static const std::string& Quality(){ static std::string xQuality = "Quality"; return xQuality;}
	static const std::string& DescID(){ static std::string xDescID = "DescID"; return xDescID;}
	static const std::string& EffectData(){ static std::string xEffectData = "EffectData"; return xEffectData;}
	static const std::string& ConsumeData(){ static std::string xConsumeData = "ConsumeData"; return xConsumeData;}
	static const std::string& AwardData(){ static std::string xAwardData = "AwardData"; return xAwardData;}
	static const std::string& CoolDownTime(){ static std::string xCoolDownTime = "CoolDownTime"; return xCoolDownTime;}
	static const std::string& OverlayCount(){ static std::string xOverlayCount = "OverlayCount"; return xOverlayCount;}
	static const std::string& ExpiredType(){ static std::string xExpiredType = "ExpiredType"; return xExpiredType;}
	static const std::string& BuyPrice(){ static std::string xBuyPrice = "BuyPrice"; return xBuyPrice;}
	static const std::string& SalePrice(){ static std::string xSalePrice = "SalePrice"; return xSalePrice;}
	static const std::string& Script(){ static std::string xScript = "Script"; return xScript;}
	static const std::string& Extend(){ static std::string xExtend = "Extend"; return xExtend;}

};

class Item
{
public:

};

class Language
{
public:
	static const std::string& English(){ static std::string xEnglish = "English"; return xEnglish;}
	static const std::string& Chinese(){ static std::string xChinese = "Chinese"; return xChinese;}

};

class Language
{
public:

};

class NPC
{
public:
	static const std::string& SeedID(){ static std::string xSeedID = "SeedID"; return xSeedID;}
	static const std::string& EXP(){ static std::string xEXP = "EXP"; return xEXP;}
	static const std::string& HP(){ static std::string xHP = "HP"; return xHP;}
	static const std::string& SP(){ static std::string xSP = "SP"; return xSP;}
	static const std::string& MP(){ static std::string xMP = "MP"; return xMP;}
	static const std::string& Money(){ static std::string xMoney = "Money"; return xMoney;}
	static const std::string& LastAttacker(){ static std::string xLastAttacker = "LastAttacker"; return xLastAttacker;}
	static const std::string& X(){ static std::string xX = "X"; return xX;}
	static const std::string& Y(){ static std::string xY = "Y"; return xY;}
	static const std::string& Z(){ static std::string xZ = "Z"; return xZ;}
	static const std::string& TargetX(){ static std::string xTargetX = "TargetX"; return xTargetX;}
	static const std::string& TargetY(){ static std::string xTargetY = "TargetY"; return xTargetY;}
	static const std::string& TargetZ(){ static std::string xTargetZ = "TargetZ"; return xTargetZ;}
	static const std::string& MAXHP(){ static std::string xMAXHP = "MAXHP"; return xMAXHP;}
	static const std::string& MAXMP(){ static std::string xMAXMP = "MAXMP"; return xMAXMP;}
	static const std::string& MAXSP(){ static std::string xMAXSP = "MAXSP"; return xMAXSP;}
	static const std::string& HPREGEN(){ static std::string xHPREGEN = "HPREGEN"; return xHPREGEN;}
	static const std::string& SPREGEN(){ static std::string xSPREGEN = "SPREGEN"; return xSPREGEN;}
	static const std::string& MPREGEN(){ static std::string xMPREGEN = "MPREGEN"; return xMPREGEN;}
	static const std::string& ATK_VALUE(){ static std::string xATK_VALUE = "ATK_VALUE"; return xATK_VALUE;}
	static const std::string& DEF_VALUE(){ static std::string xDEF_VALUE = "DEF_VALUE"; return xDEF_VALUE;}
	static const std::string& MOVE_SPEED(){ static std::string xMOVE_SPEED = "MOVE_SPEED"; return xMOVE_SPEED;}
	static const std::string& ATK_SPEED(){ static std::string xATK_SPEED = "ATK_SPEED"; return xATK_SPEED;}
	static const std::string& ATK_FIRE(){ static std::string xATK_FIRE = "ATK_FIRE"; return xATK_FIRE;}
	static const std::string& ATK_LIGHT(){ static std::string xATK_LIGHT = "ATK_LIGHT"; return xATK_LIGHT;}
	static const std::string& ATK_ICE(){ static std::string xATK_ICE = "ATK_ICE"; return xATK_ICE;}
	static const std::string& ATK_POISON(){ static std::string xATK_POISON = "ATK_POISON"; return xATK_POISON;}
	static const std::string& DEF_FIRE(){ static std::string xDEF_FIRE = "DEF_FIRE"; return xDEF_FIRE;}
	static const std::string& DEF_LIGHT(){ static std::string xDEF_LIGHT = "DEF_LIGHT"; return xDEF_LIGHT;}
	static const std::string& DEF_ICE(){ static std::string xDEF_ICE = "DEF_ICE"; return xDEF_ICE;}
	static const std::string& DEF_POISON(){ static std::string xDEF_POISON = "DEF_POISON"; return xDEF_POISON;}
	static const std::string& DIZZY_GATE(){ static std::string xDIZZY_GATE = "DIZZY_GATE"; return xDIZZY_GATE;}
	static const std::string& MOVE_GATE(){ static std::string xMOVE_GATE = "MOVE_GATE"; return xMOVE_GATE;}
	static const std::string& SKILL_GATE(){ static std::string xSKILL_GATE = "SKILL_GATE"; return xSKILL_GATE;}
	static const std::string& PHYSICAL_GATE(){ static std::string xPHYSICAL_GATE = "PHYSICAL_GATE"; return xPHYSICAL_GATE;}
	static const std::string& MAGIC_GATE(){ static std::string xMAGIC_GATE = "MAGIC_GATE"; return xMAGIC_GATE;}
	static const std::string& BUFF_GATE(){ static std::string xBUFF_GATE = "BUFF_GATE"; return xBUFF_GATE;}
	static const std::string& Prefab(){ static std::string xPrefab = "Prefab"; return xPrefab;}
	static const std::string& MoveType(){ static std::string xMoveType = "MoveType"; return xMoveType;}
	static const std::string& AtkDis(){ static std::string xAtkDis = "AtkDis"; return xAtkDis;}
	static const std::string& DropPackList(){ static std::string xDropPackList = "DropPackList"; return xDropPackList;}
	static const std::string& SkillIDRef(){ static std::string xSkillIDRef = "SkillIDRef"; return xSkillIDRef;}
	static const std::string& Height(){ static std::string xHeight = "Height"; return xHeight;}

};

class NPC
{
public:
	const static std::string PosList = "PosList";

};

class Player
{
public:
	static const std::string& Name(){ static std::string xName = "Name"; return xName;}
	static const std::string& Sex(){ static std::string xSex = "Sex"; return xSex;}
	static const std::string& Race(){ static std::string xRace = "Race"; return xRace;}
	static const std::string& Camp(){ static std::string xCamp = "Camp"; return xCamp;}
	static const std::string& LastContainerID(){ static std::string xLastContainerID = "LastContainerID"; return xLastContainerID;}
	static const std::string& Level(){ static std::string xLevel = "Level"; return xLevel;}
	static const std::string& ShowName(){ static std::string xShowName = "ShowName"; return xShowName;}
	static const std::string& PrefabPath(){ static std::string xPrefabPath = "PrefabPath"; return xPrefabPath;}
	static const std::string& FirstTarget(){ static std::string xFirstTarget = "FirstTarget"; return xFirstTarget;}
	static const std::string& State(){ static std::string xState = "State"; return xState;}
	static const std::string& LogicState(){ static std::string xLogicState = "LogicState"; return xLogicState;}
	static const std::string& CharType(){ static std::string xCharType = "CharType"; return xCharType;}
	static const std::string& Job(){ static std::string xJob = "Job"; return xJob;}
	static const std::string& VIPLevel(){ static std::string xVIPLevel = "VIPLevel"; return xVIPLevel;}
	static const std::string& VIPEXP(){ static std::string xVIPEXP = "VIPEXP"; return xVIPEXP;}
	static const std::string& EXP(){ static std::string xEXP = "EXP"; return xEXP;}
	static const std::string& HP(){ static std::string xHP = "HP"; return xHP;}
	static const std::string& SP(){ static std::string xSP = "SP"; return xSP;}
	static const std::string& MP(){ static std::string xMP = "MP"; return xMP;}
	static const std::string& Gold(){ static std::string xGold = "Gold"; return xGold;}
	static const std::string& Money(){ static std::string xMoney = "Money"; return xMoney;}
	static const std::string& Account(){ static std::string xAccount = "Account"; return xAccount;}
	static const std::string& ConnectKey(){ static std::string xConnectKey = "ConnectKey"; return xConnectKey;}
	static const std::string& MAXEXP(){ static std::string xMAXEXP = "MAXEXP"; return xMAXEXP;}
	static const std::string& DEAD_COUNT(){ static std::string xDEAD_COUNT = "DEAD_COUNT"; return xDEAD_COUNT;}
	static const std::string& RELIVE_SOUL(){ static std::string xRELIVE_SOUL = "RELIVE_SOUL"; return xRELIVE_SOUL;}
	static const std::string& GUILD_NAME(){ static std::string xGUILD_NAME = "GUILD_NAME"; return xGUILD_NAME;}
	static const std::string& SPOUSE_NAME(){ static std::string xSPOUSE_NAME = "SPOUSE_NAME"; return xSPOUSE_NAME;}
	static const std::string& TEAM_CARD(){ static std::string xTEAM_CARD = "TEAM_CARD"; return xTEAM_CARD;}
	static const std::string& PVP_ATK_CARD(){ static std::string xPVP_ATK_CARD = "PVP_ATK_CARD"; return xPVP_ATK_CARD;}
	static const std::string& PVP_DEF_CARD(){ static std::string xPVP_DEF_CARD = "PVP_DEF_CARD"; return xPVP_DEF_CARD;}
	static const std::string& LUCKLY_CARD(){ static std::string xLUCKLY_CARD = "LUCKLY_CARD"; return xLUCKLY_CARD;}
	static const std::string& ATK_PVP(){ static std::string xATK_PVP = "ATK_PVP"; return xATK_PVP;}
	static const std::string& DEF_PVP(){ static std::string xDEF_PVP = "DEF_PVP"; return xDEF_PVP;}
	static const std::string& CreditValue(){ static std::string xCreditValue = "CreditValue"; return xCreditValue;}
	static const std::string& SoulValue(){ static std::string xSoulValue = "SoulValue"; return xSoulValue;}
	static const std::string& OnlineCount(){ static std::string xOnlineCount = "OnlineCount"; return xOnlineCount;}
	static const std::string& TotalTime(){ static std::string xTotalTime = "TotalTime"; return xTotalTime;}
	static const std::string& Citizenship(){ static std::string xCitizenship = "Citizenship"; return xCitizenship;}
	static const std::string& LastOfflineTime(){ static std::string xLastOfflineTime = "LastOfflineTime"; return xLastOfflineTime;}
	static const std::string& OnlineTime(){ static std::string xOnlineTime = "OnlineTime"; return xOnlineTime;}
	static const std::string& TotalLineTime(){ static std::string xTotalLineTime = "TotalLineTime"; return xTotalLineTime;}
	static const std::string& GMLevel(){ static std::string xGMLevel = "GMLevel"; return xGMLevel;}
	static const std::string& LastAttacker(){ static std::string xLastAttacker = "LastAttacker"; return xLastAttacker;}
	static const std::string& MAXHP(){ static std::string xMAXHP = "MAXHP"; return xMAXHP;}
	static const std::string& MAXMP(){ static std::string xMAXMP = "MAXMP"; return xMAXMP;}
	static const std::string& MAXSP(){ static std::string xMAXSP = "MAXSP"; return xMAXSP;}
	static const std::string& HPREGEN(){ static std::string xHPREGEN = "HPREGEN"; return xHPREGEN;}
	static const std::string& SPREGEN(){ static std::string xSPREGEN = "SPREGEN"; return xSPREGEN;}
	static const std::string& MPREGEN(){ static std::string xMPREGEN = "MPREGEN"; return xMPREGEN;}
	static const std::string& ATK_VALUE(){ static std::string xATK_VALUE = "ATK_VALUE"; return xATK_VALUE;}
	static const std::string& DEF_VALUE(){ static std::string xDEF_VALUE = "DEF_VALUE"; return xDEF_VALUE;}
	static const std::string& MOVE_SPEED(){ static std::string xMOVE_SPEED = "MOVE_SPEED"; return xMOVE_SPEED;}
	static const std::string& ATK_SPEED(){ static std::string xATK_SPEED = "ATK_SPEED"; return xATK_SPEED;}
	static const std::string& ATK_FIRE(){ static std::string xATK_FIRE = "ATK_FIRE"; return xATK_FIRE;}
	static const std::string& ATK_LIGHT(){ static std::string xATK_LIGHT = "ATK_LIGHT"; return xATK_LIGHT;}
	static const std::string& ATK_ICE(){ static std::string xATK_ICE = "ATK_ICE"; return xATK_ICE;}
	static const std::string& ATK_POISON(){ static std::string xATK_POISON = "ATK_POISON"; return xATK_POISON;}
	static const std::string& DEF_FIRE(){ static std::string xDEF_FIRE = "DEF_FIRE"; return xDEF_FIRE;}
	static const std::string& DEF_LIGHT(){ static std::string xDEF_LIGHT = "DEF_LIGHT"; return xDEF_LIGHT;}
	static const std::string& DEF_ICE(){ static std::string xDEF_ICE = "DEF_ICE"; return xDEF_ICE;}
	static const std::string& DEF_POISON(){ static std::string xDEF_POISON = "DEF_POISON"; return xDEF_POISON;}
	static const std::string& DIZZY_GATE(){ static std::string xDIZZY_GATE = "DIZZY_GATE"; return xDIZZY_GATE;}
	static const std::string& MOVE_GATE(){ static std::string xMOVE_GATE = "MOVE_GATE"; return xMOVE_GATE;}
	static const std::string& SKILL_GATE(){ static std::string xSKILL_GATE = "SKILL_GATE"; return xSKILL_GATE;}
	static const std::string& PHYSICAL_GATE(){ static std::string xPHYSICAL_GATE = "PHYSICAL_GATE"; return xPHYSICAL_GATE;}
	static const std::string& MAGIC_GATE(){ static std::string xMAGIC_GATE = "MAGIC_GATE"; return xMAGIC_GATE;}
	static const std::string& BUFF_GATE(){ static std::string xBUFF_GATE = "BUFF_GATE"; return xBUFF_GATE;}
	static const std::string& X(){ static std::string xX = "X"; return xX;}
	static const std::string& Y(){ static std::string xY = "Y"; return xY;}
	static const std::string& Z(){ static std::string xZ = "Z"; return xZ;}
	static const std::string& TargetX(){ static std::string xTargetX = "TargetX"; return xTargetX;}
	static const std::string& TargetY(){ static std::string xTargetY = "TargetY"; return xTargetY;}
	static const std::string& TargetZ(){ static std::string xTargetZ = "TargetZ"; return xTargetZ;}
	static const std::string& PathStep(){ static std::string xPathStep = "PathStep"; return xPathStep;}
	static const std::string& LoadPropertyFinish(){ static std::string xLoadPropertyFinish = "LoadPropertyFinish"; return xLoadPropertyFinish;}
	static const std::string& GameID(){ static std::string xGameID = "GameID"; return xGameID;}
	static const std::string& GateID(){ static std::string xGateID = "GateID"; return xGateID;}
	static const std::string& GuildID(){ static std::string xGuildID = "GuildID"; return xGuildID;}

};

class Player
{
public:
	const static std::string PlayerViewItem = "PlayerViewItem";
	const static std::string PlayerHero = "PlayerHero";
	const static std::string BagEquipList = "BagEquipList";
	const static std::string BagItemList = "BagItemList";
	const static std::string CommPropertyValue = "CommPropertyValue";
	const static std::string EctypeList = "EctypeList";
	const static std::string DropItemList = "DropItemList";
	const static std::string SkillTable = "SkillTable";
	const static std::string TaskMonsterList = "TaskMonsterList";
	const static std::string TaskList = "TaskList";
	const static std::string PVPList = "PVPList";
	const static std::string ChatGroup = "ChatGroup";

};

class Scene
{
public:
	static const std::string& SceneName(){ static std::string xSceneName = "SceneName"; return xSceneName;}
	static const std::string& SceneShowName(){ static std::string xSceneShowName = "SceneShowName"; return xSceneShowName;}
	static const std::string& MaxGroup(){ static std::string xMaxGroup = "MaxGroup"; return xMaxGroup;}
	static const std::string& MaxGroupPlayers(){ static std::string xMaxGroupPlayers = "MaxGroupPlayers"; return xMaxGroupPlayers;}
	static const std::string& FilePath(){ static std::string xFilePath = "FilePath"; return xFilePath;}
	static const std::string& RelivePos(){ static std::string xRelivePos = "RelivePos"; return xRelivePos;}
	static const std::string& Width(){ static std::string xWidth = "Width"; return xWidth;}
	static const std::string& SoundList(){ static std::string xSoundList = "SoundList"; return xSoundList;}
	static const std::string& Share(){ static std::string xShare = "Share"; return xShare;}
	static const std::string& CanClone(){ static std::string xCanClone = "CanClone"; return xCanClone;}
	static const std::string& ActorID(){ static std::string xActorID = "ActorID"; return xActorID;}
	static const std::string& LoadingUI(){ static std::string xLoadingUI = "LoadingUI"; return xLoadingUI;}
	static const std::string& CamOffestPos(){ static std::string xCamOffestPos = "CamOffestPos"; return xCamOffestPos;}
	static const std::string& CamOffestRot(){ static std::string xCamOffestRot = "CamOffestRot"; return xCamOffestRot;}

};

class Scene
{
public:

};

class Server
{
public:
	static const std::string& ServerID(){ static std::string xServerID = "ServerID"; return xServerID;}
	static const std::string& Name(){ static std::string xName = "Name"; return xName;}
	static const std::string& MaxOnline(){ static std::string xMaxOnline = "MaxOnline"; return xMaxOnline;}
	static const std::string& CpuCount(){ static std::string xCpuCount = "CpuCount"; return xCpuCount;}
	static const std::string& IP(){ static std::string xIP = "IP"; return xIP;}
	static const std::string& Port(){ static std::string xPort = "Port"; return xPort;}
	static const std::string& Type(){ static std::string xType = "Type"; return xType;}

};

class Server
{
public:

};

class Shop
{
public:
	static const std::string& Type(){ static std::string xType = "Type"; return xType;}
	static const std::string& ItemID(){ static std::string xItemID = "ItemID"; return xItemID;}
	static const std::string& Gold(){ static std::string xGold = "Gold"; return xGold;}
	static const std::string& Steel(){ static std::string xSteel = "Steel"; return xSteel;}
	static const std::string& Stone(){ static std::string xStone = "Stone"; return xStone;}
	static const std::string& Diamond(){ static std::string xDiamond = "Diamond"; return xDiamond;}
	static const std::string& Level(){ static std::string xLevel = "Level"; return xLevel;}

};

class Shop
{
public:

};

class Skill
{
public:
	static const std::string& SkillType(){ static std::string xSkillType = "SkillType"; return xSkillType;}
	static const std::string& AnimaState(){ static std::string xAnimaState = "AnimaState"; return xAnimaState;}
	static const std::string& ShowName(){ static std::string xShowName = "ShowName"; return xShowName;}
	static const std::string& Desc(){ static std::string xDesc = "Desc"; return xDesc;}
	static const std::string& ConsumeProperty(){ static std::string xConsumeProperty = "ConsumeProperty"; return xConsumeProperty;}
	static const std::string& ConsumeValue(){ static std::string xConsumeValue = "ConsumeValue"; return xConsumeValue;}
	static const std::string& ConsumeType(){ static std::string xConsumeType = "ConsumeType"; return xConsumeType;}
	static const std::string& DamageProperty(){ static std::string xDamageProperty = "DamageProperty"; return xDamageProperty;}
	static const std::string& DamageValue(){ static std::string xDamageValue = "DamageValue"; return xDamageValue;}
	static const std::string& DamageType(){ static std::string xDamageType = "DamageType"; return xDamageType;}
	static const std::string& GetBuffList(){ static std::string xGetBuffList = "GetBuffList"; return xGetBuffList;}
	static const std::string& SendBuffList(){ static std::string xSendBuffList = "SendBuffList"; return xSendBuffList;}
	static const std::string& CoolDownTime(){ static std::string xCoolDownTime = "CoolDownTime"; return xCoolDownTime;}
	static const std::string& RequireDistance(){ static std::string xRequireDistance = "RequireDistance"; return xRequireDistance;}
	static const std::string& DamageDistance(){ static std::string xDamageDistance = "DamageDistance"; return xDamageDistance;}
	static const std::string& TargetType(){ static std::string xTargetType = "TargetType"; return xTargetType;}
	static const std::string& NewObject(){ static std::string xNewObject = "NewObject"; return xNewObject;}
	static const std::string& Icon(){ static std::string xIcon = "Icon"; return xIcon;}
	static const std::string& Atlas(){ static std::string xAtlas = "Atlas"; return xAtlas;}
	static const std::string& UpLevel(){ static std::string xUpLevel = "UpLevel"; return xUpLevel;}
	static const std::string& AfterUpSkillID(){ static std::string xAfterUpSkillID = "AfterUpSkillID"; return xAfterUpSkillID;}
	static const std::string& PlayerSkill(){ static std::string xPlayerSkill = "PlayerSkill"; return xPlayerSkill;}
	static const std::string& AtkDis(){ static std::string xAtkDis = "AtkDis"; return xAtkDis;}
	static const std::string& NeedTar(){ static std::string xNeedTar = "NeedTar"; return xNeedTar;}
	static const std::string& DefaultHitTime(){ static std::string xDefaultHitTime = "DefaultHitTime"; return xDefaultHitTime;}

};

class Skill
{
public:

};

class SkillRef
{
public:
	static const std::string& NORMALATTACK1(){ static std::string xNORMALATTACK1 = "NORMALATTACK1"; return xNORMALATTACK1;}
	static const std::string& NORMALATTACK2(){ static std::string xNORMALATTACK2 = "NORMALATTACK2"; return xNORMALATTACK2;}
	static const std::string& NORMALATTACK3(){ static std::string xNORMALATTACK3 = "NORMALATTACK3"; return xNORMALATTACK3;}
	static const std::string& NORMALTHUMP(){ static std::string xNORMALTHUMP = "NORMALTHUMP"; return xNORMALTHUMP;}
	static const std::string& SKILL1(){ static std::string xSKILL1 = "SKILL1"; return xSKILL1;}
	static const std::string& SKILL2(){ static std::string xSKILL2 = "SKILL2"; return xSKILL2;}
	static const std::string& SKILL3(){ static std::string xSKILL3 = "SKILL3"; return xSKILL3;}
	static const std::string& SKILL4(){ static std::string xSKILL4 = "SKILL4"; return xSKILL4;}
	static const std::string& SKILL5(){ static std::string xSKILL5 = "SKILL5"; return xSKILL5;}
	static const std::string& SKILL6(){ static std::string xSKILL6 = "SKILL6"; return xSKILL6;}
	static const std::string& SKILL7(){ static std::string xSKILL7 = "SKILL7"; return xSKILL7;}
	static const std::string& SKILL8(){ static std::string xSKILL8 = "SKILL8"; return xSKILL8;}
	static const std::string& SKILL9(){ static std::string xSKILL9 = "SKILL9"; return xSKILL9;}
	static const std::string& SKILL10(){ static std::string xSKILL10 = "SKILL10"; return xSKILL10;}

};

class SkillRef
{
public:

};

class SqlServer
{
public:
	static const std::string& ServerID(){ static std::string xServerID = "ServerID"; return xServerID;}
	static const std::string& IP(){ static std::string xIP = "IP"; return xIP;}
	static const std::string& Port(){ static std::string xPort = "Port"; return xPort;}
	static const std::string& SqlIP(){ static std::string xSqlIP = "SqlIP"; return xSqlIP;}
	static const std::string& SqlPort(){ static std::string xSqlPort = "SqlPort"; return xSqlPort;}
	static const std::string& SqlUser(){ static std::string xSqlUser = "SqlUser"; return xSqlUser;}
	static const std::string& SqlPwd(){ static std::string xSqlPwd = "SqlPwd"; return xSqlPwd;}
	static const std::string& SqlName(){ static std::string xSqlName = "SqlName"; return xSqlName;}

};

class SqlServer
{
public:

};

class StateFuncResources
{
public:
	static const std::string& Atlas_ResID(){ static std::string xAtlas_ResID = "Atlas_ResID"; return xAtlas_ResID;}

};

class StateFuncResources
{
public:

};

class StateFunction
{
public:
	static const std::string& EFT_INFO(){ static std::string xEFT_INFO = "EFT_INFO"; return xEFT_INFO;}
	static const std::string& EFT_BOOOST(){ static std::string xEFT_BOOOST = "EFT_BOOOST"; return xEFT_BOOOST;}
	static const std::string& EFT_LVLUP(){ static std::string xEFT_LVLUP = "EFT_LVLUP"; return xEFT_LVLUP;}
	static const std::string& EFT_CREATE_SOLDER(){ static std::string xEFT_CREATE_SOLDER = "EFT_CREATE_SOLDER"; return xEFT_CREATE_SOLDER;}
	static const std::string& EFT_CREATE_SPEEL(){ static std::string xEFT_CREATE_SPEEL = "EFT_CREATE_SPEEL"; return xEFT_CREATE_SPEEL;}
	static const std::string& EFT_RESEARCH(){ static std::string xEFT_RESEARCH = "EFT_RESEARCH"; return xEFT_RESEARCH;}
	static const std::string& EFT_COLLECT_GOLD(){ static std::string xEFT_COLLECT_GOLD = "EFT_COLLECT_GOLD"; return xEFT_COLLECT_GOLD;}
	static const std::string& EFT_COLLECT_STONE(){ static std::string xEFT_COLLECT_STONE = "EFT_COLLECT_STONE"; return xEFT_COLLECT_STONE;}
	static const std::string& EFT_COLLECT_STEEL(){ static std::string xEFT_COLLECT_STEEL = "EFT_COLLECT_STEEL"; return xEFT_COLLECT_STEEL;}
	static const std::string& EFT_COLLECT_DIAMOND(){ static std::string xEFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND"; return xEFT_COLLECT_DIAMOND;}
	static const std::string& EFT_SELL(){ static std::string xEFT_SELL = "EFT_SELL"; return xEFT_SELL;}
	static const std::string& EFT_REPAIR(){ static std::string xEFT_REPAIR = "EFT_REPAIR"; return xEFT_REPAIR;}
	static const std::string& EFT_CANCEL(){ static std::string xEFT_CANCEL = "EFT_CANCEL"; return xEFT_CANCEL;}
	static const std::string& EFT_FINISH(){ static std::string xEFT_FINISH = "EFT_FINISH"; return xEFT_FINISH;}

};

class StateFunction
{
public:

};

class Task
{
public:
	static const std::string& Type(){ static std::string xType = "Type"; return xType;}
	static const std::string& NextTaskID(){ static std::string xNextTaskID = "NextTaskID"; return xNextTaskID;}
	static const std::string& KillMonsterName(){ static std::string xKillMonsterName = "KillMonsterName"; return xKillMonsterName;}
	static const std::string& KillCount(){ static std::string xKillCount = "KillCount"; return xKillCount;}
	static const std::string& LevelReq(){ static std::string xLevelReq = "LevelReq"; return xLevelReq;}
	static const std::string& AwardExp(){ static std::string xAwardExp = "AwardExp"; return xAwardExp;}
	static const std::string& AwardGold(){ static std::string xAwardGold = "AwardGold"; return xAwardGold;}
	static const std::string& AwardPack(){ static std::string xAwardPack = "AwardPack"; return xAwardPack;}
	static const std::string& Desc(){ static std::string xDesc = "Desc"; return xDesc;}

};

class Task
{
public:

};

} // !@NFrame

#endif NF_PR_NAME_HPP // !NF_PR_NAME_HPP
