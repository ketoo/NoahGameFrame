#ifndef __NF_STRING_INFO_H__
#define __NF_STRING_INFO_H__

// desc:����NFED_ON_SHOW_STRING�¼�ʹ�õ��ִ�
// �ִ���ʽ���Դ���ʽ ��ʽȫ��ʹ��%s
// ���磺 %s��%s˵:���

// �����ִ�����ģ�����֣�ö��ID�Լ���������

enum ResultInfo
{
    EGEC_UNKOWN_ERROR                  = 0,       // δ֪����
    EGEC_SUCCESS                                = 1,       // �����ɹ�
    EGEC_SIGNATURE_SUCCESS            = 2,       // ǩ����֤�ɹ�
    EGEC_ACCOUNTPWD_INVALID   = 3,          // �˺Ų����ڻ����������
    EGEC_ACCOUNT_INVALID      = 4,              // ��֤ʱ��Ч�˺�
    EGEC_ACCOUNT_USING        = 5,                // �˺�����ʹ��
    EGEC_ACCOUNT_LOCKED       = 6,              // �˺��ѷ�ͣ
    EGEC_CHARACTER_EXIST      = 7,              // ������ɫʱ��ɫ���ظ�
    EGEC_CHARACTER_SUCCESS      = 8,          // ������ɫ�ɹ�
    EGEC_CHARACTER_FAILD      = 9,              // ������ɫʧ��
    EGEC_DELETECHAR_SUCCESS      = 10,          // ɾ����ɫ�ɹ�
    EGEC_DELETECHAR_FAILD      = 11,              // ɾ����ɫʧ��

    EGEC_SVRZONEID_INVALID    = 12,             // ������ɫʱ��Ϸ��������Ч
    EGEC_CHARACTER_NUMOUT     = 13,         // ������ɫʱ��ɫ���������趨ֵ��һ���˺���ཨ4����ɫ��
    EGEC_CHARACTER_INVALID    = 14,           // ��ɫ����Ч���Ƿ���ɫ����
    EGEC_CHARACTER_NOTEXIST   = 15,          // ��ɫ������
    EGEC_CHARACTER_USING      = 16,             // ��ɫ������Ϸ��
    EGEC_CHARACTER_LOCKED     = 17,           // ��ɫ�ѷ�ͣ

    EGEC_ENTERGAME_SUCCESS = 18,        //������Ϸ����ɹ�
    EGEC_ENTERGAME_FAILD = 19,              //������Ϸ����ʧ��

    EGEC_ACCPETTASK_SUCCESS = 20,        //������ɹ�
    EGEC_ACCPETTASK_FAILD = 21,              //������ʧ��

    EGEC_PULLDOWNTASK_SUCCESS = 22,        //������ɹ�
    EGEC_PULLDOWNTASK_FAILD = 23,              //������ʧ��

};

#endif // __NF_STRING_INFO_H__