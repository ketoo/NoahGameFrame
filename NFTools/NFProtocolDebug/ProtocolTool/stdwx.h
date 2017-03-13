#ifndef stdwx_h__
#define stdwx_h__

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include "wx/wx.h"

struct NetMsg
{
	unsigned int nMsgID;
	std::string strMsgData;
};

#define MAKE_MSG_ID(msg_type,msg_id)      (((msg_type)<<8)|(msg_id))
#define GET_MSG_TYPE(raw_val)			  (((raw_val)>>8)&0x0000ffff)
#define GET_MSG_ID(raw_val)				  ((raw_val)&0x000000ff)

#endif // stdwx_h__
