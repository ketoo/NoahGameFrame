#ifndef _ENCRYPT_RC4_
#define _ENCRYPT_RC4_

#include <string.h>

#define BOX_LEN 256

static void    swap_byte(unsigned char* a,          unsigned char* b);
int            GetKey(const unsigned char* pass, int pass_len, unsigned char* out);
int            RC4(unsigned char* data,       unsigned int nLen, const unsigned char* key, unsigned int key_len);

int            Encrypt(unsigned char* data,       unsigned int nLen, const char* key,          unsigned int key_len);      // ���ܣ����ؼ��ܽ��
int            Decrypt(unsigned char* data,       unsigned int nLen, const char* key,          unsigned int key_len);      // ���ܣ����ؽ��ܽ��

char*          ByteToHex(const unsigned char* vByte, const int vLen);                        // ���ֽ���pbBufferתΪʮ�������ַ��������㴫��
unsigned char* HexToByte(const char* szHex);                                                 // ��ʮ�������ַ���תΪ�ֽ���pbBuffer������

// sample
//int main(int argc,char *argv[])
//{
//    int size = 0;
//
//    char source[] = "testtest";
//    char pass[] = "123456";
//    char *result1 = NULL;
//    char *result2 = NULL;
//
//    result1 = Encrypt(source, pass);
//
//    printf("result1=%s\n", result1);
//
//    result2 = Decrypt(result1, pass);
//
//    printf("result2=%s\n", result2);
//
//    delete []result1;
//    delete []result2;
//
//    return 0;
//}

#endif // #ifndef _ENCRYPT_RC4_