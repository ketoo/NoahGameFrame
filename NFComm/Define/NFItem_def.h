// -------------------------------------------------------------------------
//    �ļ���      :    NFItem_def.h
//    ������      :    LvSheng.Huang
//    ����ʱ��    :    2010-10-5 12:05
//    ��������    :    ��Ʒ�Ľṹ��
//
// -------------------------------------------------------------------------
#ifndef _NFC_ITEM_DEF_H_
#define _NFC_ITEM_DEF_H_

#pragma pack(push,1)

//��������
enum EGameItemType
{
    EGIT_None               = 0,
    EGIT_Item               = 1,
    EGIT_Equip              = 2,
    EGIT_Material           = 3,
    EGIT_Task               = 4,

    EGIT_Max                   ,

    //EGIT_NONE                 = 0,                // ��
    //EGIT_WEAPON_START                 = 1,                // ��
    //EGIT_WEAPON_KNIFT                 = 2,               // ��
    //EGIT_WEAPON_FIREGUN           = 3,                    // ��ǹ
    //EGIT_WEAPON_TREEGUN           = 4,                    // ľǹ
    //EGIT_WEAPON_SWORD             = 5,                    // ��
    //EGIT_WEAPON_BOW                   = 6,                // ��
    //EGIT_WEAPON_CROSSBOW      = 7,                // ��
    //EGIT_WEAPON_WAND                  = 8,                // ����
    //EGIT_WEAPON_DAGGER                = 9,                // ذ��
    //EGIT_WEAPON_AXE                       = 10,                // ��ͷ
    //EGIT_WEAPON_END,

    //EGIT_EQUIP_START,
    //   EGIT_EQUIP_HRID                   = 20,                // ͷ��
    //   EGIT_EQUIP_FACE                   = 21,                // ��
    //   EGIT_EQUIP_EARRINGS           = 22,                // ����
    //   EGIT_EQUIP_NECKLACE           = 23,                // ����
    //   EGIT_EQUIP_COAT                   = 24,                // ����
    //EGIT_EQUIP_PANT                   = 25,               // ����
    //   EGIT_EQUIP_SHAWL              = 26,                // ���
    //   EGIT_EQUIP_GLOVE                  = 27,               // ����(����)
    //   EGIT_EQUIP_RING                   = 28,               // ��ָ
    //   EGIT_EQUIP_SOCK                   = 29,               // ����==����D
    //   EGIT_EQUIP_BOOT                   = 30,               // ѥ��E
    //EGIT_EQUIP_WING                   = 31,                // ���
    //EGIT_EQUIP_FASHIONE           = 32,                // ʱװ
    //EGIT_EQUIP_END,

    //EGIT_EQUIP_BOOK_START = 50,               //װ���༼����
    //EGIT_EQUIP_BOOK1              = 51,            // ������1--��������ʵ��װ��һ����ǿ����ħ��������������װ��
    //EGIT_EQUIP_BOOK2          = 52,                // ������2
    //EGIT_EQUIP_BOOK3          = 53,                // ������3
    //EGIT_EQUIP_BOOK4          = 54,                // ������4
    //EGIT_EQUIP_BOOK5          = 55,                // ������5
    //EGIT_EQUIP_BOOK_END,
    //

    //EGIT_ITEM_START,
    //EGIT_ITEM_PROPERTY_CARD       = 100,                      // ��������ʯ��Ƭ���ϳ���Ƕ
    //EGIT_ITEM_INTENSIVE_CARD  = 101,                      // ǿ��ʯ��Ƭ
    //EGIT_ITEM_ENCHANTMENT_CARD        = 102,            // ��ħʯ��Ƭ
    //EGIT_ITEM_PVP_ATTACK_CARD     = 103,                // PVP������
    //EGIT_ITEM_PVP_DEFENSE_CARD        = 104,                // PVP������
    //EGIT_ITEM_SKILL_CARD      = 105,                               // ѧϰ�༼���飬��ͨ�ļ��ܶ�������ѧϰ��
    //EGIT_ITEM_GUILD_SKILL_CARD        = 106,               // ���Ἴ����
    //EGIT_ITEM_GUILD_DIAMOND       = 107,                       // ����ʥ��

    //EGIT_ITEM_POSITION            = 1000,               // �ƶ�λ��(���͵����ִ�)(���Ĵ���Ʒʱ�õ�ĳ�ִ���)
    //EGIT_ITEM_WORLD_HORN              = 1001,              // ��������(ʹ��ĳ�����ܵ�ʱ�����Ĵ���Ʒ)
    //EGIT_ITEM_RECOVER_POTION          = 1002,             // ��ҩѪҩ
    //EGIT_ITEM_GOLD_BOX            = 1004,               // ��Һ���
    //EGIT_ITEM_REBORN          = 1005,               // ��������(���Ĵ���Ʒʱ�õ�ĳ�ִ���)
    //EGIT_ITEM_EXP         = 1006,                         // ����ҩ
    //EGIT_ITEM_PACK            = 1007,                     // ���߰�--�����˼����飬�����BUFF��ʹ�ü��ܵĵ���GetXXXList

    //EGIT_ITEM_END,
};

//����������(��������)
enum EGameEquipType
{
    EquipType_Weapon            = 0,    // ����
    EquipType_Armor             = 1,    // �ؼ�
    EquipType_Necklace          = 2,    // ����
    EquipType_Ring              = 3,    // ��ָ
    EquipType_Earrings          = 4,    // ����
    EquipType_Badge             = 5,    // ����
    EquipType_Helmet            = 6,    // ͷ��
    EquipType_Shawl             = 7,    // ���
    EquipType_Glove             = 8,    // ����
    EquipType_Wrist             = 9,    // ����
    EquipType_Belt              = 10,   // ����
    EquipType_Pant              = 11,   // ����
    EquipType_Boot              = 12,   // ѥ��
    EquipType_Wing              = 13,   // ���
    EquipType_Cloak             = 14,   // ����
    EquipType_Fashion           = 15,   // ʱװ

    EquipType_Max,
};

enum EGameItemSubType
{
    EGIT_ITEM_START             = 0,    // ��
    EGIT_ITEM_PROPERTY_CARD     = 1,    // ��������ʯ���ϳ���Ƕ
    EGIT_ITEM_INTENSIVE_CARD    = 2,    // ǿ��ʯ
    EGIT_ITEM_ENCHANTMENT_CARD  = 3,    // ��ħʯ
    EGIT_ITEM_PVP_ATTACK_CARD   = 4,    // PVP����
    EGIT_ITEM_PVP_DEFENSE_CARD  = 5,    // PVP����
    EGIT_ITEM_SKILL_CARD        = 6,    // ѧϰ�༼���飬��ͨ�ļ��ܶ�������ѧϰ��
    EGIT_ITEM_GUILD_SKILL_CARD  = 7,    // ���Ἴ����
    EGIT_ITEM_GUILD_DIAMOND     = 8,    // ����ʥ��
    EGIT_ITEM_POSITION          = 9,    // �ƶ�λ��(���͵����ִ�)(���Ĵ���Ʒʱ�õ�ĳ�ִ���)
    EGIT_ITEM_WORLD_HORN        = 10,   // ��������(ʹ��ĳ�����ܵ�ʱ�����Ĵ���Ʒ)
    EGIT_ITEM_RECOVER_POTION    = 11,   // ��ҩѪҩ����������
    EGIT_ITEM_GOLD_BOX          = 12,   // ��Һ���
    EGIT_ITEM_REBORN            = 13,   // ��������(���Ĵ���Ʒʱ�õ�ĳ�ִ���)
    EGIT_ITEM_EXP               = 14,   // ����ҩ
    EGIT_ITEM_PACK              = 15,   // ���߰�--�����˼����飬�����BUFF��ʹ�ü��ܵĵ���GetXXXList
};

//����ʱ������
enum EGameItemExpiredType
{
    EGIET_NONE = 0,               // ������������
    EGIET_USE_TIMES,              // ʹ�ô���, ����ħ��,��0����Ҫ��ʧ(��ͨ��Ʒ����)
    EGIET_REAL_TIME,              // ��ʵ��ʱ������(��),�ʺ�ʱװ,���,��Ʒ(д��1��1��)
    EGIET_END_INDEX,
};

////////////////////////////////////////////////////////////////////////////////
//���ӵ��������б�BagItemList
enum EGameItemStoreType
{
    EGIT_TYPE_GUID,//"װ�����߾ֲ�����ID--װ���󲻿��޸�
    EGIT_TYPE_CONFIGID,//"��Ʒ����ID--װ���󲻿��޸�
    EGIT_TYPE_ITEMCOUNT,//"��Ʒ����"/>
    EGIT_TYPE_ITEMLOCK,//"�Ƿ��"/>
    EGIT_TYPE_TIMETYPE,//"����ʱ������--װ���󲻿��޸�
    EGIT_TYPE_SOLTCOUNT,//"������-װ����������Ч"/>
    EGIT_TYPE_INLAY_CARD1,//"��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INLAY_CARD2,//��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INLAY_CARD3,//��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INLAY_CARD4,//��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INLAY_CARD5,//��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INLAY_CARD6,//��Ƕ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_INTENSIVE_LEVEL,//"ǿ���ȼ�-װ��,��������Ч"/>
    EGIT_TYPE_ENCHANTMENT_LEVEL,//"��ħ�ȼ�-װ��,��������Ч"/>
    EGIT_TYPE_ENCHANTMENT_CARD,//"��ħ��ʯID-װ��,��������Ч--װ���󲻿��޸�
    EGIT_TYPE_CREATE_TIME,//"�õ�����Ʒ������"/>
};

//�������װ��PlayerViewItem
enum EGamePlayerViewType
{
    EGPVT_TYPE_CONFIGID,//"��Ʒ����ID--װ���󲻿��޸�
    EGPVT_TYPE_INTENSIVE_LEVEL,//"ǿ���ȼ�-װ��,��������Ч"/>
    EGPVT_TYPE_ENCHANTMENT_LEVEL,//"��ħ�ȼ�-װ��,��������Ч"/>
    EGPVT_TYPE_ENCHANTMENT_CARD,//"��ħ��ʯID-װ��,��������Ч--װ���󲻿��޸�

};

#pragma pack(pop)

#endif