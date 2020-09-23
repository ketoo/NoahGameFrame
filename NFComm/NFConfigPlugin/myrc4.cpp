#include "myrc4.h"

#pragma warning ( push )
#pragma warning ( disable : 4267 )

int Encrypt(unsigned char* data, unsigned int len, const char* key, unsigned int key_len)
{
    if (data == NULL || key == NULL)
    {
        return -1;
    }

    int nResult = RC4(data, len, (unsigned char*)key, key_len);
    if (nResult == -1)
    {
        return -1;
    }

    //char* ret2 = ByteToHex(ret, ret_len);

    //delete[] ret;

    return nResult;
}

int Decrypt(unsigned char* data, unsigned int len, const char* key, unsigned int key_len)
{
    if (data == NULL || key == NULL)
    {
        return -1;
    }

    // unsigned char* src = HexToByte(szSource);
    // unsigned char* ret = new unsigned char[strlen(szSource) / 2 + 1];
    // int ret_len = 0;
    // memset(ret, strlen(szSource) / 2 + 1,0);

    int nResult = RC4(data, len, (unsigned char*)key, key_len);
    if (nResult == -1)
    {
        return -1;
    }

    //ret[ret_len] = '\0';

    return len;
}

int RC4(unsigned char* data, unsigned int len, const unsigned char* key, unsigned int key_len)
{
    if (data == NULL || key == NULL)
    {
        return -1;
    }

    unsigned char mBox[BOX_LEN] = {0};
    GetKey(key, key_len, mBox);

    int i = 0;
    int x = 0;
    int y = 0;

    for (unsigned int k = 0; k < len; ++k)
    {
        x = (x + 1) % BOX_LEN;
        y = (mBox[x] + y) % BOX_LEN;
        swap_byte(&mBox[x], &mBox[y]);
        data[k] = data[k] ^ mBox[(mBox[x] + mBox[y]) % BOX_LEN];
    }

    return len;
}

int GetKey(const unsigned char* pass, int pass_len, unsigned char* out)
{
    if (pass == NULL || out == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < BOX_LEN; ++i)
    {
        out[i] = i;
    }

    int j = 0;
    for (int i = 0; i < BOX_LEN; ++i)
    {
        j = (pass[i % pass_len] + out[i] + j) % BOX_LEN;
        swap_byte(&out[i], &out[j]);
    }

    return -1;
}

static void swap_byte(unsigned char* a, unsigned char* b)
{
    unsigned char swapByte = *a;
    *a = *b;
    *b = swapByte;
}


char* ByteToHex(const unsigned char* vByte, const int vLen)
{
    if (!vByte)
    {
        return NULL;
    }

    char* tmp = new char[vLen * 2 + 1]; 

    int tmp2;
    for (int i = 0; i < vLen; i++)
    {
        tmp2 = (int)(vByte[i]) / 16;
        tmp[i * 2] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
        tmp2 = (int)(vByte[i]) % 16;
        tmp[i * 2 + 1] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
    }

    tmp[vLen * 2] = '\0';
    return tmp;
}


unsigned char* HexToByte(const char* szHex)
{
    if (!szHex)
    {
        return NULL;
    }

    int iLen = strlen(szHex);

    if (iLen <= 0 || 0 != iLen % 2)
    {
        return NULL;
    }

    unsigned char* pbBuf = new unsigned char[iLen / 2]; 

    int tmp1, tmp2;
    for (int i = 0; i < iLen / 2; i++)
    {
        tmp1 = (int)szHex[i * 2] - (((int)szHex[i * 2] >= 'A') ? 'A' - 10 : '0');

        if (tmp1 >= 16)
        {
            return NULL;
        }

        tmp2 = (int)szHex[i * 2 + 1] - (((int)szHex[i * 2 + 1] >= 'A') ? 'A' - 10 : '0');

        if (tmp2 >= 16)
        {
            return NULL;
        }

        pbBuf[i] = (tmp1 * 16 + tmp2);
    }

    return pbBuf;
}

#pragma warning ( pop )
