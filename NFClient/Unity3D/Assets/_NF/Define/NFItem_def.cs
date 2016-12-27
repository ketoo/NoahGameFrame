using System;


public class NFItem_def
{
	public NFItem_def ()
	{
	}
	
	//道具类型
	
	public enum EGameItemType
	{
	    EGIT_NONE					= 0,
		EGIT_WEAPON_START					= 1,                // 无
		EGIT_WEAPON_KNIFT					= 2,               // 刀
		EGIT_WEAPON_FIREGUN			= 3,					// 火枪
		EGIT_WEAPON_TREEGUN			= 4,					// 木枪
		EGIT_WEAPON_SWORD				= 5,					// 剑
		EGIT_WEAPON_BOW					= 6,                // 弓
		EGIT_WEAPON_CROSSBOW		= 7,                // 弩
		EGIT_WEAPON_WAND					= 8,                // 法杖
		EGIT_WEAPON_DAGGER				= 9,                // 匕首
		EGIT_WEAPON_AXE						= 10,                // 斧头
		EGIT_WEAPON_END,
	
		EGIT_EQUIP_START,
	    EGIT_EQUIP_HRID					= 20,                // 头发
	    EGIT_EQUIP_FACE					= 21,                // 脸
	    EGIT_EQUIP_EARRINGS			= 22,                // 耳环
	    EGIT_EQUIP_NECKLACE			= 23,                // 项链
	    EGIT_EQUIP_COAT					= 24,                // 大衣
		EGIT_EQUIP_PANT					= 25,               // 裤子
	    EGIT_EQUIP_SHAWL				= 26,                // 肩甲
	    EGIT_EQUIP_GLOVE					= 27,               // 护腕(手套)
	    EGIT_EQUIP_RING					= 28,				// 戒指
	    EGIT_EQUIP_SOCK					= 29,               // 袜子==护腿D
	    EGIT_EQUIP_BOOT					= 30,               // 靴子E
		EGIT_EQUIP_WING					= 31,                // 翅膀
		EGIT_EQUIP_FASHIONE			= 32,                // 时装
	
		EGIT_BOOK_SKILL1			= 33,                // 技能书1--技能书其实和装备一样能强化附魔，本身就是特殊的装备
		EGIT_BOOK_SKILL2			= 33,                // 技能书2
		EGIT_BOOK_SKILL3			= 33,                // 技能书3
		EGIT_BOOK_SKILL4			= 33,                // 技能书4
		EGIT_BOOK_SKILL5			= 33,                // 技能书5
	
		EGIT_EQUIP_END,
	
		EGIT_ITEM_START,
		EGIT_ITEM_PROPERTY_CARD		= 100,						// 基础属性石卡片，合成镶嵌
		EGIT_ITEM_INTENSIVE_CARD	= 101,						// 强化石卡片
		EGIT_ITEM_ENCHANTMENT_CARD		= 102,            // 附魔石卡片
		EGIT_ITEM_PVP_ATTACK_CARD		= 103,                // PVP攻击卡
		EGIT_ITEM_PVP_DEFENSE_CARD		= 104,                // PVP防御卡
		EGIT_ITEM_SKILL_CARD		= 105,								 // 技能书
		EGIT_ITEM_GUILD_SKILL_CARD		= 106,				 // 工会技能书
		EGIT_ITEM_GUILD_DIAMOND		= 107,						 // 工会圣宝
	
		EGIT_ITEM_POSITION			= 1000,               // 移动位置(传送到新手村)
		EGIT_ITEM_WORLD_HORN				= 1001,               // 世界喇叭
		EGIT_ITEM_RECOVER_MP			= 1002,               // 蓝药
		EGIT_ITEM_RECOVER_HP					= 1003,				// 血药
		EGIT_ITEM_GUILD_BOX			= 1004,               // 金币盒子
		EGIT_ITEM_REBORN			= 1005,               // 重生道具
	
	
		EGIT_ITEM_END,
	};
	
	//道具子类型(归属类型)
	public enum EGameItemSubType
	{
		
	};
	
	//道具时间类型
	public enum EGameItemExpiredType
	{
	 EGIET_NONE = 0,               // 永不过期类型
	 EGIET_USE_TIMES,              // 使用次数, 变形魔杖,变0道具要消失(普通物品数量)
	 EGIET_REAL_TIME,              // 真实的时间流逝(秒),适合时装,礼包,饰品(写死1天1周)
	 EGIET_END_INDEX,
	};
	////////////////////////////////////////////////////////////////////////////////
	//格子道具属性列表BagItemList
	public enum EGameItemStoreType
	{
		EGIT_TYPE_AUTOID,//"装备道具局部自增ID--装备后不可修改
		
		EGIT_TYPE_CONFIGID,//"物品配置ID--装备后不可修改
		EGIT_TYPE_ITEMCOUNT,//"物品数量"/>
		EGIT_TYPE_ITEMLOCK,//"是否绑定"/>
		EGIT_TYPE_TIMETYPE,//"到期时间类型--装备后不可修改
		EGIT_TYPE_SOLTCOUNT,//"洞数量-装备技能书有效"/>
		EGIT_TYPE_INLAY_CARD1,//"镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INLAY_CARD2,//镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INLAY_CARD3,//镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INLAY_CARD4,//镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INLAY_CARD5,//镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INLAY_CARD6,//镶嵌宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_INTENSIVE_LEVEL,//"强化等级-装备,技能书有效"/>
		EGIT_TYPE_ENCHANTMENT_LEVEL,//"附魔等级-装备,技能书有效"/>
		EGIT_TYPE_ENCHANTMENT_CARD,//"附魔宝石ID-装备,技能书有效--装备后不可修改
		EGIT_TYPE_CREATE_TIME,//"得到此物品的日期"/>	 
	};
	
	//互相可视装备PlayerViewItem
	public enum EGamePlayerViewType
	{
		EGPVT_TYPE_CONFIGID,//"物品配置ID--装备后不可修改
		EGPVT_TYPE_INTENSIVE_LEVEL,//"强化等级-装备,技能书有效"/>
		EGPVT_TYPE_ENCHANTMENT_LEVEL,//"附魔等级-装备,技能书有效"/>
		EGPVT_TYPE_ENCHANTMENT_CARD,//"附魔宝石ID-装备,技能书有效--装备后不可修改
	
	};

}

