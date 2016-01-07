// -------------------------------------------------------------------------
//    @FileName         :    NFProtocolDefine.cs
//    @Author           :    NFrame Studio
//    @Date             :    2016/01/07
//    @Module           :    NFProtocolDefine
// -------------------------------------------------------------------------

using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace NFrame
{

public class BB_Build
{
	public static readonly String Type = "Type";
	public static readonly String SubType = "SubType";
	public static readonly String Prefab = "Prefab";
	public static readonly String NormalStateFunc = "NormalStateFunc";
	public static readonly String UpStateFunc = "UpStateFunc";
	public static readonly String Icon = "Icon";
	public static readonly String ShowName = "ShowName";
	public static readonly String Desc = "Desc";

}

public class BB_Build
{

}

public class BB_Player
{
	public static readonly String RoleName = "RoleName";
	public static readonly String Sex = "Sex";
	public static readonly String Race = "Race";
	public static readonly String Camp = "Camp";
	public static readonly String LastContainerID = "LastContainerID";
	public static readonly String Level = "Level";
	public static readonly String ShowName = "ShowName";
	public static readonly String Gold = "Gold";
	public static readonly String Stone = "Stone";
	public static readonly String Steel = "Steel";
	public static readonly String Diamond = "Diamond";
	public static readonly String LoadPropertyFinish = "LoadPropertyFinish";

}

public class BB_Player
{
	public static readonly String BuildingList = "BuildingList";
	public static readonly String BuildingProduce = "BuildingProduce";

}

public class Buff
{
	public static readonly String EffectType = "EffectType";
	public static readonly String EffectValueType = "EffectValueType";
	public static readonly String EffectValueReferType = "EffectValueReferType";
	public static readonly String EffectTimeValue = "EffectTimeValue";
	public static readonly String EffectTimeInterval = "EffectTimeInterval";
	public static readonly String WashGroupID = "WashGroupID";
	public static readonly String ReverseReferType = "ReverseReferType";
	public static readonly String EffectClearOnDead = "EffectClearOnDead";
	public static readonly String DownSaveType = "DownSaveType";

}

public class Buff
{

}

public class ChatGroup
{
	public static readonly String Name = "Name";
	public static readonly String CreateObject = "CreateObject";

}

public class ChatGroup
{
	public static readonly String GroupMemberList = "GroupMemberList";
	public static readonly String ChatList = "ChatList";

}

public class ConsumeData
{
	public static readonly String VIPEXP = "VIPEXP";
	public static readonly String EXP = "EXP";
	public static readonly String HP = "HP";
	public static readonly String SP = "SP";
	public static readonly String MP = "MP";
	public static readonly String Gold = "Gold";
	public static readonly String Money = "Money";

}

public class ConsumeData
{

}

public class Cost
{
	public static readonly String CostMoney = "CostMoney";
	public static readonly String CostDiamond = "CostDiamond";
	public static readonly String CostVP = "CostVP";
	public static readonly String CostHonour = "CostHonour";

}

public class Cost
{

}

public class DescData
{
	public static readonly String ShowName = "ShowName";
	public static readonly String Icon = "Icon";
	public static readonly String Atlas = "Atlas";
	public static readonly String PrefabPath = "PrefabPath";
	public static readonly String PerformanceEffect = "PerformanceEffect";
	public static readonly String PerformanceSound = "PerformanceSound";
	public static readonly String DescText = "DescText";

}

public class DescData
{

}

public class EffectData
{
	public static readonly String MAXHP = "MAXHP";
	public static readonly String MAXMP = "MAXMP";
	public static readonly String MAXSP = "MAXSP";
	public static readonly String HPREGEN = "HPREGEN";
	public static readonly String SPREGEN = "SPREGEN";
	public static readonly String MPREGEN = "MPREGEN";
	public static readonly String ATK_VALUE = "ATK_VALUE";
	public static readonly String DEF_VALUE = "DEF_VALUE";
	public static readonly String MOVE_SPEED = "MOVE_SPEED";
	public static readonly String ATK_SPEED = "ATK_SPEED";
	public static readonly String ATK_FIRE = "ATK_FIRE";
	public static readonly String ATK_LIGHT = "ATK_LIGHT";
	public static readonly String ATK_ICE = "ATK_ICE";
	public static readonly String ATK_POISON = "ATK_POISON";
	public static readonly String DEF_FIRE = "DEF_FIRE";
	public static readonly String DEF_LIGHT = "DEF_LIGHT";
	public static readonly String DEF_ICE = "DEF_ICE";
	public static readonly String DEF_POISON = "DEF_POISON";
	public static readonly String DIZZY_GATE = "DIZZY_GATE";
	public static readonly String MOVE_GATE = "MOVE_GATE";
	public static readonly String SKILL_GATE = "SKILL_GATE";
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";
	public static readonly String MAGIC_GATE = "MAGIC_GATE";
	public static readonly String BUFF_GATE = "BUFF_GATE";

}

public class EffectData
{

}

public class Equip
{
	public static readonly String Sex = "Sex";
	public static readonly String IntensiveBuffList = "IntensiveBuffList";
	public static readonly String EnchantmentBuffList = "EnchantmentBuffList";
	public static readonly String SuitID = "SuitID";
	public static readonly String SuitBuffID = "SuitBuffID";

}

public class Equip
{

}

public class Guild
{
	public static readonly String Name = "Name";
	public static readonly String PresidentID = "PresidentID";
	public static readonly String PresidentName = "PresidentName";
	public static readonly String Rank = "Rank";
	public static readonly String GuildAD = "GuildAD";
	public static readonly String GuildDesc = "GuildDesc";
	public static readonly String GuildMoney = "GuildMoney";
	public static readonly String GuildLevel = "GuildLevel";
	public static readonly String GuildContinueDay = "GuildContinueDay";
	public static readonly String GuilID = "GuilID";
	public static readonly String GuilIDIcon = "GuilIDIcon";
	public static readonly String GuildMemeberCount = "GuildMemeberCount";
	public static readonly String GuildMemeberMaxCount = "GuildMemeberMaxCount";
	public static readonly String GuildHonor = "GuildHonor";
	public static readonly String GuildCreateTime = "GuildCreateTime";
	public static readonly String GuildCreateter = "GuildCreateter";
	public static readonly String GuildExp = "GuildExp";
	public static readonly String GuildStatus = "GuildStatus";
	public static readonly String DismissTime = "DismissTime";
	public static readonly String RecruitAD = "RecruitAD";
	public static readonly String RecruitLevel = "RecruitLevel";
	public static readonly String AutoRecruit = "AutoRecruit";

}

public class Guild
{
	public static readonly String GuildBoss = "GuildBoss";
	public static readonly String GuildMemberList = "GuildMemberList";
	public static readonly String GuildAppyList = "GuildAppyList";
	public static readonly String GuildEvent = "GuildEvent";
	public static readonly String GuildHouse = "GuildHouse";
	public static readonly String GuildSkill = "GuildSkill";

}

public class GuildConfig
{
	public static readonly String ConditionPlayerLevel = "ConditionPlayerLevel";
	public static readonly String ConditionPlayerVIP = "ConditionPlayerVIP";
	public static readonly String CostMoney = "CostMoney";
	public static readonly String DismissTime = "DismissTime";
	public static readonly String GuildLevel = "GuildLevel";
	public static readonly String MaxMember = "MaxMember";

}

public class GuildConfig
{

}

public class GuildJob
{
	public static readonly String GuildJob = "GuildJob";
	public static readonly String JobCount = "JobCount";
	public static readonly String Appoint = "Appoint";
	public static readonly String Fire = "Fire";
	public static readonly String Demise = "Demise";
	public static readonly String ApplyDismiss = "ApplyDismiss";
	public static readonly String StopDismiss = "StopDismiss";
	public static readonly String AcceptApply = "AcceptApply";
	public static readonly String DenyApply = "DenyApply";
	public static readonly String Kickout = "Kickout";
	public static readonly String SetRecruit = "SetRecruit";
	public static readonly String PublishRecruit = "PublishRecruit";
	public static readonly String EditAD = "EditAD";
	public static readonly String Leave = "Leave";
	public static readonly String LevelUp = "LevelUp";

}

public class GuildJob
{

}

public class GuildName
{
	public static readonly String GuildID = "GuildID";

}

public class GuildName
{

}

public class InitProperty
{
	public static readonly String Job = "Job";
	public static readonly String Level = "Level";
	public static readonly String EffectData = "EffectData";
	public static readonly String SkillIDRef = "SkillIDRef";
	public static readonly String ModelPtah = "ModelPtah";

}

public class InitProperty
{

}

public class IObject
{
	public static readonly String ID = "ID";
	public static readonly String ClassName = "ClassName";
	public static readonly String SceneID = "SceneID";
	public static readonly String GroupID = "GroupID";
	public static readonly String ConfigID = "ConfigID";

}

public class IObject
{

}

public class Item
{
	public static readonly String ItemType = "ItemType";
	public static readonly String ItemSubType = "ItemSubType";
	public static readonly String Level = "Level";
	public static readonly String Job = "Job";
	public static readonly String Quality = "Quality";
	public static readonly String DescID = "DescID";
	public static readonly String EffectData = "EffectData";
	public static readonly String ConsumeData = "ConsumeData";
	public static readonly String AwardData = "AwardData";
	public static readonly String CoolDownTime = "CoolDownTime";
	public static readonly String OverlayCount = "OverlayCount";
	public static readonly String ExpiredType = "ExpiredType";
	public static readonly String BuyPrice = "BuyPrice";
	public static readonly String SalePrice = "SalePrice";
	public static readonly String Script = "Script";
	public static readonly String Extend = "Extend";

}

public class Item
{

}

public class Language
{
	public static readonly String English = "English";
	public static readonly String Chinese = "Chinese";

}

public class Language
{

}

public class NPC
{
	public static readonly String SeedID = "SeedID";
	public static readonly String EXP = "EXP";
	public static readonly String HP = "HP";
	public static readonly String SP = "SP";
	public static readonly String MP = "MP";
	public static readonly String Money = "Money";
	public static readonly String LastAttacker = "LastAttacker";
	public static readonly String X = "X";
	public static readonly String Y = "Y";
	public static readonly String Z = "Z";
	public static readonly String TargetX = "TargetX";
	public static readonly String TargetY = "TargetY";
	public static readonly String TargetZ = "TargetZ";
	public static readonly String MAXHP = "MAXHP";
	public static readonly String MAXMP = "MAXMP";
	public static readonly String MAXSP = "MAXSP";
	public static readonly String HPREGEN = "HPREGEN";
	public static readonly String SPREGEN = "SPREGEN";
	public static readonly String MPREGEN = "MPREGEN";
	public static readonly String ATK_VALUE = "ATK_VALUE";
	public static readonly String DEF_VALUE = "DEF_VALUE";
	public static readonly String MOVE_SPEED = "MOVE_SPEED";
	public static readonly String ATK_SPEED = "ATK_SPEED";
	public static readonly String ATK_FIRE = "ATK_FIRE";
	public static readonly String ATK_LIGHT = "ATK_LIGHT";
	public static readonly String ATK_ICE = "ATK_ICE";
	public static readonly String ATK_POISON = "ATK_POISON";
	public static readonly String DEF_FIRE = "DEF_FIRE";
	public static readonly String DEF_LIGHT = "DEF_LIGHT";
	public static readonly String DEF_ICE = "DEF_ICE";
	public static readonly String DEF_POISON = "DEF_POISON";
	public static readonly String DIZZY_GATE = "DIZZY_GATE";
	public static readonly String MOVE_GATE = "MOVE_GATE";
	public static readonly String SKILL_GATE = "SKILL_GATE";
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";
	public static readonly String MAGIC_GATE = "MAGIC_GATE";
	public static readonly String BUFF_GATE = "BUFF_GATE";
	public static readonly String Prefab = "Prefab";
	public static readonly String MoveType = "MoveType";
	public static readonly String AtkDis = "AtkDis";
	public static readonly String DropPackList = "DropPackList";
	public static readonly String SkillIDRef = "SkillIDRef";
	public static readonly String Height = "Height";

}

public class NPC
{
	public static readonly String PosList = "PosList";

}

public class Player
{
	public static readonly String Name = "Name";
	public static readonly String Sex = "Sex";
	public static readonly String Race = "Race";
	public static readonly String Camp = "Camp";
	public static readonly String LastContainerID = "LastContainerID";
	public static readonly String Level = "Level";
	public static readonly String ShowName = "ShowName";
	public static readonly String PrefabPath = "PrefabPath";
	public static readonly String FirstTarget = "FirstTarget";
	public static readonly String State = "State";
	public static readonly String LogicState = "LogicState";
	public static readonly String CharType = "CharType";
	public static readonly String Job = "Job";
	public static readonly String VIPLevel = "VIPLevel";
	public static readonly String VIPEXP = "VIPEXP";
	public static readonly String EXP = "EXP";
	public static readonly String HP = "HP";
	public static readonly String SP = "SP";
	public static readonly String MP = "MP";
	public static readonly String Gold = "Gold";
	public static readonly String Money = "Money";
	public static readonly String Account = "Account";
	public static readonly String ConnectKey = "ConnectKey";
	public static readonly String MAXEXP = "MAXEXP";
	public static readonly String DEAD_COUNT = "DEAD_COUNT";
	public static readonly String RELIVE_SOUL = "RELIVE_SOUL";
	public static readonly String GUILD_NAME = "GUILD_NAME";
	public static readonly String SPOUSE_NAME = "SPOUSE_NAME";
	public static readonly String TEAM_CARD = "TEAM_CARD";
	public static readonly String PVP_ATK_CARD = "PVP_ATK_CARD";
	public static readonly String PVP_DEF_CARD = "PVP_DEF_CARD";
	public static readonly String LUCKLY_CARD = "LUCKLY_CARD";
	public static readonly String ATK_PVP = "ATK_PVP";
	public static readonly String DEF_PVP = "DEF_PVP";
	public static readonly String CreditValue = "CreditValue";
	public static readonly String SoulValue = "SoulValue";
	public static readonly String OnlineCount = "OnlineCount";
	public static readonly String TotalTime = "TotalTime";
	public static readonly String Citizenship = "Citizenship";
	public static readonly String LastOfflineTime = "LastOfflineTime";
	public static readonly String OnlineTime = "OnlineTime";
	public static readonly String TotalLineTime = "TotalLineTime";
	public static readonly String GMLevel = "GMLevel";
	public static readonly String LastAttacker = "LastAttacker";
	public static readonly String MAXHP = "MAXHP";
	public static readonly String MAXMP = "MAXMP";
	public static readonly String MAXSP = "MAXSP";
	public static readonly String HPREGEN = "HPREGEN";
	public static readonly String SPREGEN = "SPREGEN";
	public static readonly String MPREGEN = "MPREGEN";
	public static readonly String ATK_VALUE = "ATK_VALUE";
	public static readonly String DEF_VALUE = "DEF_VALUE";
	public static readonly String MOVE_SPEED = "MOVE_SPEED";
	public static readonly String ATK_SPEED = "ATK_SPEED";
	public static readonly String ATK_FIRE = "ATK_FIRE";
	public static readonly String ATK_LIGHT = "ATK_LIGHT";
	public static readonly String ATK_ICE = "ATK_ICE";
	public static readonly String ATK_POISON = "ATK_POISON";
	public static readonly String DEF_FIRE = "DEF_FIRE";
	public static readonly String DEF_LIGHT = "DEF_LIGHT";
	public static readonly String DEF_ICE = "DEF_ICE";
	public static readonly String DEF_POISON = "DEF_POISON";
	public static readonly String DIZZY_GATE = "DIZZY_GATE";
	public static readonly String MOVE_GATE = "MOVE_GATE";
	public static readonly String SKILL_GATE = "SKILL_GATE";
	public static readonly String PHYSICAL_GATE = "PHYSICAL_GATE";
	public static readonly String MAGIC_GATE = "MAGIC_GATE";
	public static readonly String BUFF_GATE = "BUFF_GATE";
	public static readonly String X = "X";
	public static readonly String Y = "Y";
	public static readonly String Z = "Z";
	public static readonly String TargetX = "TargetX";
	public static readonly String TargetY = "TargetY";
	public static readonly String TargetZ = "TargetZ";
	public static readonly String PathStep = "PathStep";
	public static readonly String LoadPropertyFinish = "LoadPropertyFinish";
	public static readonly String GameID = "GameID";
	public static readonly String GateID = "GateID";
	public static readonly String GuildID = "GuildID";

}

public class Player
{
	public static readonly String PlayerViewItem = "PlayerViewItem";
	public static readonly String PlayerHero = "PlayerHero";
	public static readonly String BagEquipList = "BagEquipList";
	public static readonly String BagItemList = "BagItemList";
	public static readonly String CommPropertyValue = "CommPropertyValue";
	public static readonly String EctypeList = "EctypeList";
	public static readonly String DropItemList = "DropItemList";
	public static readonly String SkillTable = "SkillTable";
	public static readonly String TaskMonsterList = "TaskMonsterList";
	public static readonly String TaskList = "TaskList";
	public static readonly String PVPList = "PVPList";
	public static readonly String ChatGroup = "ChatGroup";

}

public class Scene
{
	public static readonly String SceneName = "SceneName";
	public static readonly String SceneShowName = "SceneShowName";
	public static readonly String MaxGroup = "MaxGroup";
	public static readonly String MaxGroupPlayers = "MaxGroupPlayers";
	public static readonly String FilePath = "FilePath";
	public static readonly String RelivePos = "RelivePos";
	public static readonly String Width = "Width";
	public static readonly String SoundList = "SoundList";
	public static readonly String Share = "Share";
	public static readonly String CanClone = "CanClone";
	public static readonly String ActorID = "ActorID";
	public static readonly String LoadingUI = "LoadingUI";
	public static readonly String CamOffestPos = "CamOffestPos";
	public static readonly String CamOffestRot = "CamOffestRot";

}

public class Scene
{

}

public class Server
{
	public static readonly String ServerID = "ServerID";
	public static readonly String Name = "Name";
	public static readonly String MaxOnline = "MaxOnline";
	public static readonly String CpuCount = "CpuCount";
	public static readonly String IP = "IP";
	public static readonly String Port = "Port";
	public static readonly String Type = "Type";

}

public class Server
{

}

public class Shop
{
	public static readonly String Type = "Type";
	public static readonly String ItemID = "ItemID";
	public static readonly String Gold = "Gold";
	public static readonly String Steel = "Steel";
	public static readonly String Stone = "Stone";
	public static readonly String Diamond = "Diamond";
	public static readonly String Level = "Level";

}

public class Shop
{

}

public class Skill
{
	public static readonly String SkillType = "SkillType";
	public static readonly String AnimaState = "AnimaState";
	public static readonly String ShowName = "ShowName";
	public static readonly String Desc = "Desc";
	public static readonly String ConsumeProperty = "ConsumeProperty";
	public static readonly String ConsumeValue = "ConsumeValue";
	public static readonly String ConsumeType = "ConsumeType";
	public static readonly String DamageProperty = "DamageProperty";
	public static readonly String DamageValue = "DamageValue";
	public static readonly String DamageType = "DamageType";
	public static readonly String GetBuffList = "GetBuffList";
	public static readonly String SendBuffList = "SendBuffList";
	public static readonly String CoolDownTime = "CoolDownTime";
	public static readonly String RequireDistance = "RequireDistance";
	public static readonly String DamageDistance = "DamageDistance";
	public static readonly String TargetType = "TargetType";
	public static readonly String NewObject = "NewObject";
	public static readonly String Icon = "Icon";
	public static readonly String Atlas = "Atlas";
	public static readonly String UpLevel = "UpLevel";
	public static readonly String AfterUpSkillID = "AfterUpSkillID";
	public static readonly String PlayerSkill = "PlayerSkill";
	public static readonly String AtkDis = "AtkDis";
	public static readonly String NeedTar = "NeedTar";
	public static readonly String DefaultHitTime = "DefaultHitTime";

}

public class Skill
{

}

public class SkillRef
{
	public static readonly String NORMALATTACK1 = "NORMALATTACK1";
	public static readonly String NORMALATTACK2 = "NORMALATTACK2";
	public static readonly String NORMALATTACK3 = "NORMALATTACK3";
	public static readonly String NORMALTHUMP = "NORMALTHUMP";
	public static readonly String SKILL1 = "SKILL1";
	public static readonly String SKILL2 = "SKILL2";
	public static readonly String SKILL3 = "SKILL3";
	public static readonly String SKILL4 = "SKILL4";
	public static readonly String SKILL5 = "SKILL5";
	public static readonly String SKILL6 = "SKILL6";
	public static readonly String SKILL7 = "SKILL7";
	public static readonly String SKILL8 = "SKILL8";
	public static readonly String SKILL9 = "SKILL9";
	public static readonly String SKILL10 = "SKILL10";

}

public class SkillRef
{

}

public class SqlServer
{
	public static readonly String ServerID = "ServerID";
	public static readonly String IP = "IP";
	public static readonly String Port = "Port";
	public static readonly String SqlIP = "SqlIP";
	public static readonly String SqlPort = "SqlPort";
	public static readonly String SqlUser = "SqlUser";
	public static readonly String SqlPwd = "SqlPwd";
	public static readonly String SqlName = "SqlName";

}

public class SqlServer
{

}

public class StateFuncResources
{
	public static readonly String Atlas_ResID = "Atlas_ResID";

}

public class StateFuncResources
{

}

public class StateFunction
{
	public static readonly String EFT_INFO = "EFT_INFO";
	public static readonly String EFT_BOOOST = "EFT_BOOOST";
	public static readonly String EFT_LVLUP = "EFT_LVLUP";
	public static readonly String EFT_CREATE_SOLDER = "EFT_CREATE_SOLDER";
	public static readonly String EFT_CREATE_SPEEL = "EFT_CREATE_SPEEL";
	public static readonly String EFT_RESEARCH = "EFT_RESEARCH";
	public static readonly String EFT_COLLECT_GOLD = "EFT_COLLECT_GOLD";
	public static readonly String EFT_COLLECT_STONE = "EFT_COLLECT_STONE";
	public static readonly String EFT_COLLECT_STEEL = "EFT_COLLECT_STEEL";
	public static readonly String EFT_COLLECT_DIAMOND = "EFT_COLLECT_DIAMOND";
	public static readonly String EFT_SELL = "EFT_SELL";
	public static readonly String EFT_REPAIR = "EFT_REPAIR";
	public static readonly String EFT_CANCEL = "EFT_CANCEL";
	public static readonly String EFT_FINISH = "EFT_FINISH";

}

public class StateFunction
{

}

public class Task
{
	public static readonly String Type = "Type";
	public static readonly String NextTaskID = "NextTaskID";
	public static readonly String KillMonsterName = "KillMonsterName";
	public static readonly String KillCount = "KillCount";
	public static readonly String LevelReq = "LevelReq";
	public static readonly String AwardExp = "AwardExp";
	public static readonly String AwardGold = "AwardGold";
	public static readonly String AwardPack = "AwardPack";
	public static readonly String Desc = "Desc";

}

public class Task
{

}

}
