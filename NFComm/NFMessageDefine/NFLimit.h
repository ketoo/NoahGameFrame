// -------------------------------------------------------------------------
//    @FileName      :    NFLimit.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2010-10-5 12:05
//    @Module    :    ���޶���ֵ
//
// -------------------------------------------------------------------------

#ifndef _NF_LIMIT_H_
#define _NF_LIMIT_H_

#include "NFMsgBase.pb.h"
#include "NFComm/NFCore/NFIdentID.h"

#pragma pack(push,1)

//���弫��ֵ
//#define PT_MAX_GC_SIZE          (0x8000)        // ���ص��ͻ��˵����ķ����С32K
//#define PT_MAX_CG_SIZE          (0x2000)        // �ͻ��˵����ص����ķ����С8K

#define NF_NAME_MAXLEN          32              // �������ֳ���
#define NF_BROAD_MAXLEN         64             // ���㲥����
#define NF_PATH_MAXLEN          128            // ����������·���ȳ���
#define NF_ACCOUNT_MAXROLE      4               //һ���˺�������༸����ɫ
#define NF_ROLE_MAXPKG          150             //һ����ɫ������༸����������
#define NF_ROLE_DEFAULTPKG      60              //��������Ĭ�ϸ��豳��������

#define NF_VIEWGRID_UNIT        200             //һ�����ӣ��ж��ٸ���׼��λ��10����׼��λΪ1�ף�
#define NF_VIEWSIZE_MAXLEN      9               //��Զ����,���Լ�Ϊ���ģ����Կ����������ڵ���Ϣ������ͼ,ԭ�����Լ���λ�ã��ڿ��ʾ���ӷ�Χ

//#define NF_SCENE_LEFTLENPOS       -32000      //��������߾���
//#define NF_SCENE_RIGHTPOS     32000           //�������ұ߾���

//#define NF_MAX_ONLINE         3000            //�����������

/*
������
���ѡ�
������
*/

#pragma pack(pop)

#endif