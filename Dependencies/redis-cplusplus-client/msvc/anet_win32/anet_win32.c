/**
 *  Implement the 3 functions used by redis-cpp boost impl.
 *  Use winsock2 as backend.
 *                                   hklo.tw@gmail.com
 */

#include <winsock2.h>
#include <stdio.h>

#include "anet_win32.h"

int anetTcpNoDelay(char *err, int fd)
{
    int yes = 1;
    SOCKET sock = (SOCKET)fd;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&yes, sizeof(yes)) == -1)
    {
        if (err) {
            sprintf_s(err, ANET_ERR_LEN, "%s", "Failed to do setsockopt(TCP_NODELAY).");
        }
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetTcpConnect(char *err, char *addr, int port)
{
    int retry;
    SOCKET sock;
    SOCKADDR_IN sockaddr;
    struct hostent *ht;

    // Assume client side has already initiated winsock2.
    // Do initialize if this assumption failed.
    retry = 1;
do_retry:

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if ( INVALID_SOCKET == sock && WSANOTINITIALISED == WSAGetLastError()) {
        if (retry) {
            WSADATA wsaData;
            WSAStartup(MAKEWORD(1, 0), &wsaData);
            retry = 0;
            goto do_retry;
        } else {
            if (err) {
                sprintf_s(err, ANET_ERR_LEN, "%s", "Failed to initialize WinSock2.");
            }
            return ANET_ERR;
        }
    }
    
    ht = gethostbyname(addr);
    if (!ht) {
        if (err) {
            sprintf_s(err, ANET_ERR_LEN, "%s", "Failed to resolve target host.");
        }
        return ANET_ERR;
    }

    sockaddr.sin_family = AF_INET;
    memcpy(&sockaddr.sin_addr.s_addr, ht->h_addr, ht->h_length);
    sockaddr.sin_port = htons(port);

    if (0 != connect(sock, (SOCKADDR*)&sockaddr, sizeof(sockaddr)))
    {
        if (err) {
            sprintf_s(err, ANET_ERR_LEN, "%s", "Failed to connect.");
        }
        return ANET_ERR;
    }

    return (int)sock;
}

int anetWrite(int fd, char *buf, int count)
{
    SOCKET sock = (SOCKET)fd;
    int nwritten, totlen = 0;
    while(totlen != count) {
        nwritten = send(sock, buf, count-totlen, 0);
        if (nwritten == 0) return totlen;
        if (nwritten == SOCKET_ERROR) return -1;
        totlen += nwritten;
        buf += nwritten;
    }
    return totlen;
}
