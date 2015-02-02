// -------------------------------------------------------------------------
//    �ļ���      :    NFSkill_def.h
//    ������      :    LvSheng.Huang
//    ����ʱ��    :    2010-10-5 12:05
//    ��������    :    ���ܵĽṹ��
//
// -------------------------------------------------------------------------

#ifndef _NFC_SKILL_DEF_H_
#define _NFC_SKILL_DEF_H_

#pragma pack(push,1)


enum EGameSkillType
{
    EGST_JOBSKILL_BRIEF = 0,//��ͨ����(��Я��BUFF)
    EGST_JOBSKILL_BULLET = 1,//��ͨ�ӵ�(��Я��BUFF)
    EGST_JOBSKILL_FLASH = 2,//��ͨ��˸(��Я��BUFF)
    EGST_JOBSKILL_NEWOBJECT = 3,//��ͨ�ٻ�(����)(��Я��BUFF)
};

enum EGameSkillConsumeType
{
    ConsumeType_Value           = 0, // ����ֵ
    ConsumeType_CurrentPercent  = 1, // ��ǰ�ٷֱ�
    ConsumeType_MaxPercent      = 2, // ���ٷֱ�
};

//�����������
enum EGameSkillResultsType
{
    EART_MISS = 0,        //MISS
    EART_NORMAL = 1,        //��ͨ��������
    EART_THUMP_BACK = 2,     //�ػ�(����)
    EART_THUMP_DOWN = 3,      //�ػ�(����)
    EART_RESIST = 4,        //��
    EART_SUBSECTION2 = 5,   //�ֳ�2������
    EART_KILLEDMISS = 6,        //���ﱻ��û�ˡ�����ֻ���ڹ���������¾���������²���,�ͻ���������
    EART_KILLEDNORMAL = 7        //���ﱻ����
};

//����ʹ�ü��ܷ���(����������׷�������ͷ�-AI)
enum NFSkillTestSkillResult
{
    //����0:���� 1:���벻�� 2:CD����
    NFSTSR_OK,
    NFSTSR_DISTANCE,
    NFSTSR_CD,
};

//���ܱ�colֵ
enum EGameSkillStoreType
{
    EGSST_TYPE_SKILL_CONFIGID,//����CONFIGID
    EGSST_TYPE_SKILL_LEVEL,//���ܵȼ�
    EGSST_TYPE_SKILL_GEM_CONFIGID,//���걦ʯ
    EGSST_TYPE_SKILL_GEM_LEVEL,//����ȼ�
};

#pragma pack(pop)
#endif