/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFWSModule.h"
#include "common/base64.hpp"
#include "common/sha1.hpp"
#include "common/string_ref.hpp"
#include "common/http_util.hpp"
#include "ws_error.hpp"
#include "NFComm/NFNetPlugin/NFINet.h"

enum ws_connection_state
{
    ws_init = 0,
    ws_handshaked,
};

enum class opcode : std::uint8_t
{
    incomplete = 0,
    text = 1,
    binary = 2,
    rsv3 = 3,
    rsv4 = 4,
    rsv5 = 5,
    rsv6 = 6,
    rsv7 = 7,
    close = 8,
    ping = 9,
    pong = 10,
    crsvb = 11,
    crsvc = 12,
    crsvd = 13,
    crsve = 14,
    crsvf = 15
};

struct frame_header
{
    bool fin;
    bool rsv1;
    bool rsv2;
    bool rsv3;
    bool mask;
    opcode op;//4bit
    uint8_t payload_len;//7 bit
    std::uint32_t key;
};

static constexpr size_t HANDSHAKE_MAX_SIZE = 8192;

static constexpr size_t PAYLOAD_MIN_LEN = 125;
static constexpr size_t PAYLOAD_MID_LEN = 126;
static constexpr size_t PAYLOAD_MAX_LEN = 127;
static constexpr size_t FIN_FRAME_FLAG = 0x80;// 1 0 0 0 0 0 0 0

NFWSModule::NFWSModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
    pPluginManager = p;

    mnBufferSize = 0;
	mLastTime = GetPluginManager()->GetNowTime();
    m_pNet = NULL;
}

NFWSModule::~NFWSModule()
{
    if (m_pNet)
    {
        m_pNet->Final();
    }

    delete m_pNet;
    m_pNet = NULL;
}

bool NFWSModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	return true;
}

bool NFWSModule::AfterInit()
{
	return true;
}

void NFWSModule::Initialization(const char* ip, const unsigned short nPort)
{
    m_pNet = NF_NEW NFNet(this, &NFWSModule::OnReceiveNetPack, &NFWSModule::OnSocketNetEvent, true);
    m_pNet->ExpandBufferSize(mnBufferSize);
    m_pNet->Initialization(ip, nPort);
}

int NFWSModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    m_pNet = NF_NEW NFNet(this, &NFWSModule::OnReceiveNetPack, &NFWSModule::OnSocketNetEvent, true);
    m_pNet->ExpandBufferSize(mnBufferSize);
    return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
}

unsigned int NFWSModule::ExpandBufferSize(const unsigned int size)
{
    if (size > 0)
    {
        mnBufferSize = size;
        if (m_pNet)
        {
            m_pNet->ExpandBufferSize(mnBufferSize);
        }
    }

    return mnBufferSize;
}

void NFWSModule::RemoveReceiveCallBack(const int msgID)
{
    std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(msgID);
    if (mxReceiveCallBack.end() != it)
    {
        mxReceiveCallBack.erase(it);
    }
}

bool NFWSModule::AddReceiveCallBack(const int msgID, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    if (mxReceiveCallBack.find(msgID) == mxReceiveCallBack.end())
    {
		std::list<NET_RECEIVE_FUNCTOR_PTR> xList;
		xList.push_back(cb);
		mxReceiveCallBack.insert(std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::value_type(msgID, xList));
        return true;
    }

	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(msgID);
	it->second.push_back(cb);

    return true;
}

bool NFWSModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    mxCallBackList.push_back(cb);

    return true;
}

bool NFWSModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
    mxEventCallBackList.push_back(cb);

    return true;
}

bool NFWSModule::Execute()
{
    if (!m_pNet)
    {
        return false;
    }


    KeepAlive();

    return m_pNet->Execute();
}

bool NFWSModule::SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex)
{
	NFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgData, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

		return false;
	}

	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFINetModule::NFToPB(NFGUID());

	std::string msg;
	if (!xMsg.SerializeToString(&msg))
	{
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

		return false;
	}
	SendMsgWithOutHead(msgID, msg.c_str(),msg.length(),sockIndex);

	return true;
}

bool NFWSModule::SendMsgWithOutHead(const int16_t msgID, const char* msg, const size_t len, const NFSOCK sockIndex /*= 0*/)
{
    std::string strOutData;
    int nAllLen = EnCode(msgID, msg, len, strOutData);
    if (nAllLen == len + NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        auto frame = EncodeFrame(strOutData.data(), strOutData.size(), false);
        return SendRawMsg(frame, sockIndex);
    }

    return false;
}

int NFWSModule::EnCode(const uint16_t umsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData)
{
    NFMsgHead xHead;
    xHead.SetMsgID(umsgID);
    xHead.SetBodyLength(unDataLen);

    char szHead[NFIMsgHead::NF_Head::NF_HEAD_LENGTH] = { 0 };
    xHead.EnCode(szHead);

    strOutData.clear();
    strOutData.append(szHead, NFIMsgHead::NF_Head::NF_HEAD_LENGTH);
    strOutData.append(strData, unDataLen);

    return xHead.GetBodyLength() + NFIMsgHead::NF_Head::NF_HEAD_LENGTH;
}

bool NFWSModule::SendMsg(const std::string& msg, const NFSOCK sockIndex, const bool text)
{
    auto frame = EncodeFrame(msg.data(), msg.size(), text);
    return SendRawMsg(frame, sockIndex);
}

bool NFWSModule::SendMsgToAllClient(const std::string& msg, const bool text)
{
    auto frame = EncodeFrame(msg.data(), msg.size(), text);
	bool bRet = m_pNet->SendMsgToAllClient(frame.c_str(), (uint32_t)frame.length());
	if (!bRet)
	{
		std::ostringstream stream;
		stream << " SendMsgToAllClient failed";
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
	}

	return bRet;
}

NFINet* NFWSModule::GetNet()
{
    return m_pNet;
}

void NFWSModule::OnError(const NFSOCK sockIndex, const std::error_code & e)
{
    // may write/print error log
    // then close socket
#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_TRY
#endif
    for (auto& cb : mxEventCallBackList)
    {
        NET_EVENT_FUNCTOR_PTR& pFunPtr = cb;
        NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();

        pFunc->operator()(sockIndex, NF_NET_EVENT::NF_NET_EVENT_ERROR, m_pNet);
    }

#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_END
#endif

	std::ostringstream stream;
    stream << "WebSocket error: ";
    stream << e.value();
    stream << " ";
    stream << e.message();
    m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
    m_pNet->CloseNetObject(sockIndex);
}

bool NFWSModule::SendRawMsg(const std::string & msg, const NFSOCK sockIndex)
{
    bool bRet = m_pNet->SendMsg(msg.c_str(), (uint32_t)msg.length(), sockIndex);
    if (!bRet)
    {
        std::ostringstream stream;
        stream << " SendMsg failed fd " << sockIndex;
        m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
    }

    return bRet;
}

void NFWSModule::OnReceiveNetPack(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
    if (msgID < 0)
    {
        NetObject* pNetObject = m_pNet->GetNetObject(sockIndex);
        if (nullptr != pNetObject)
        {
            switch (pNetObject->GetConnectKeyState())
            {
            case ws_init:
            {
                string_view_t data(pNetObject->GetBuff(), pNetObject->GetBuffLen());
                auto pos = data.find("\r\n\r\n");
                if (pos != string_view_t::npos)
                {
                    auto ec = HandShake(sockIndex, data.data(), pos);
                    if (ec)
                    {
                        //mark need send then close here:
                        SendRawMsg("HTTP/1.1 400 Bad Request\r\n\r\n", sockIndex);
                        //log ec.message()
                        //OnError(sockIndex, ec);
                        return;
                    }
                    pNetObject->RemoveBuff(0, pos+4);
                    pNetObject->SetConnectKeyState(ws_handshaked);
                    //may have more data, check it
                    ec = DecodeFrame(sockIndex, pNetObject);
                    if (ec)
                    {
                        OnError(sockIndex, ec);
                        return;
                    }
                }
                else if (data.size() > HANDSHAKE_MAX_SIZE)
                {
                    OnError(sockIndex, websocket::make_error_code(websocket::error::buffer_overflow));
                    return;
                }
                break;
            }
            case ws_handshaked:
            {
                auto ec = DecodeFrame(sockIndex, pNetObject);
                if (ec)
                {
                    OnError(sockIndex, ec);
                    return;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    else
    {
        m_pLogModule->LogInfo("OnReceiveNetPack " + std::to_string(msgID), __FUNCTION__, __LINE__);
#if NF_PLATFORM != NF_PLATFORM_WIN
		NF_CRASH_TRY
#endif
		auto it = mxReceiveCallBack.find(msgID);
        if (mxReceiveCallBack.end() != it)
        {
			auto& xFunList = it->second;
            for (auto itList = xFunList.begin(); itList != xFunList.end(); ++itList)
            {
				auto& pFunPtr = *itList;
				auto pFunc = pFunPtr.get();

                pFunc->operator()(sockIndex, msgID, msg, len);
            }
        } 
        else
        {
            for (auto itList = mxCallBackList.begin(); itList != mxCallBackList.end(); ++itList)
            {
				auto& pFunPtr = *itList;
				auto pFunc = pFunPtr.get();

                pFunc->operator()(sockIndex, msgID, msg, len);
            }
        }
#if NF_PLATFORM != NF_PLATFORM_WIN
        NF_CRASH_END
#endif
    }
}

void NFWSModule::OnSocketNetEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    for (auto it = mxEventCallBackList.begin();
         it != mxEventCallBackList.end(); ++it)
    {
		auto& pFunPtr = *it;
		auto pFunc = pFunPtr.get();
        pFunc->operator()(sockIndex, eEvent, pNet);
    }
}

void NFWSModule::KeepAlive()
{
    if (!m_pNet)
    {
        return;
    }

    if (m_pNet->IsServer())
    {
        return;
    }

    if (mLastTime + 10 > GetPluginManager()->GetNowTime())
    {
        return;
    }

	mLastTime = GetPluginManager()->GetNowTime();
}

std::error_code NFWSModule::HandShake(const NFSOCK sockIndex, const char * msg, const uint32_t len)
{
    string_view_t data{ msg,len };
    string_view_t method;
    string_view_t ignore;
    string_view_t version;
    http::util::case_insensitive_multimap_view header;
    if (!http::util::request_parser::parse(data, method
        , ignore
        , ignore
        , version
        , header))
    {
        return websocket::make_error_code(websocket::error::ws_bad_http_header);
    }

    if (version<"1.0" || version>"1.1")
    {
        return make_error_code(websocket::error::ws_bad_http_version);
    }

    if (method != "GET")
        return make_error_code(websocket::error::ws_bad_method);

    string_view_t connection;
    if (!http::util::try_get_header(header, "connection", connection))
        return make_error_code(websocket::error::ws_no_connection);

    string_view_t upgrade;
    if (!http::util::try_get_header(header, "upgrade", upgrade))
        return make_error_code(websocket::error::ws_no_upgrade);

    if (!http::util::iequal_string(connection, string_view_t{ "upgrade" }))
        return make_error_code(websocket::error::ws_no_connection_upgrade);

    if (!http::util::iequal_string(upgrade, string_view_t{ "websocket" }))
        return make_error_code(websocket::error::ws_no_upgrade_websocket);

    string_view_t sec_ws_key;
    if (!http::util::try_get_header(header, string_view_t{ "sec-websocket-key" }, sec_ws_key))
        return make_error_code(websocket::error::ws_no_sec_key);

    if (base64_decode(std::string{ sec_ws_key.data(), sec_ws_key.size() }).size() != 16)
        return make_error_code(websocket::error::ws_bad_sec_key);

    string_view_t sec_ws_version;
    if (!http::util::try_get_header(header, "sec-websocket-version", sec_ws_version))
        return make_error_code(websocket::error::ws_no_sec_version);

    if (sec_ws_version != "13")
    {
        return make_error_code(websocket::error::ws_bad_sec_version);
    }

    string_view_t protocol;
    http::util::try_get_header(header, "sec-websocket-protocol", protocol);

    std::string response;
    response.append("HTTP/1.1 101 Switching Protocols\r\n");
    response.append("Upgrade: WebSocket\r\n");
    response.append("Connection: Upgrade\r\n");
    response.append("Sec-WebSocket-Accept: ");
    response.append(HashKey(sec_ws_key.data(), sec_ws_key.size()));
    response.append("\r\n", 2);
    if (!protocol.empty())
    {
        response.append("Sec-WebSocket-Protocol: ");
        response.append(protocol.data(), protocol.size());
        response.append("\r\n", 2);
    }
    response.append("\r\n", 2);
    SendRawMsg(response, sockIndex);

    return std::error_code();
}

std::error_code NFWSModule::DecodeFrame(const NFSOCK sockIndex, NetObject* pNetObject)
{
    const char* data = pNetObject->GetBuff();
    size_t size = pNetObject->GetBuffLen();
    const uint8_t* tmp = (const uint8_t*)(data);

    if (size < 3)
    {
        return std::error_code();
    }

    size_t need = 2;
    frame_header fh;

    fh.payload_len = tmp[1] & 0x7F;
    switch (fh.payload_len)
    {
    case PAYLOAD_MID_LEN: need += 2; break;
    case PAYLOAD_MAX_LEN: need += 8; break;
    default:
        break;
    }

    fh.mask = (tmp[1] & 0x80) != 0;
    //message client to server must masked.
    if (!fh.mask)
    {
        return make_error_code(websocket::error::ws_bad_unmasked_frame);
    }

    if (fh.mask)
    {
        need += 4;
    }

    //need more data
    if (size < need)
    {
        return std::error_code();
    }

    fh.op = static_cast<opcode>(tmp[0] & 0x0F);
    fh.fin = (tmp[0] & 0x80) != 0;
    fh.rsv1 = (tmp[0] & 0x40) != 0;
    fh.rsv2 = (tmp[0] & 0x20) != 0;
    fh.rsv3 = (tmp[0] & 0x10) != 0;

    switch (fh.op)
    {
    case opcode::text:
    case opcode::binary:
        if (fh.rsv1 || fh.rsv2 || fh.rsv3)
        {
            // reserved bits not cleared
            return make_error_code(websocket::error::ws_bad_reserved_bits);
        }
        break;
    case opcode::incomplete:
    {
        //not support continuation frame
        return make_error_code(websocket::error::ws_bad_continuation);
        break;
    }
    default:
        if (!fh.fin)
        {
            //not support fragmented control message
            return make_error_code(websocket::error::ws_bad_control_fragment);
        }
        if (fh.payload_len > PAYLOAD_MIN_LEN)
        {
            // invalid length for control message
            return make_error_code(websocket::error::ws_bad_control_size);
        }
        if (fh.rsv1 || fh.rsv2 || fh.rsv3)
        {
            // reserved bits not cleared
            return make_error_code(websocket::error::ws_bad_reserved_bits);
        }
        break;
    }

    uint64_t reallen = 0;
    switch (fh.payload_len)
    {
    case PAYLOAD_MID_LEN:
    {
        auto n = *(uint16_t*)(&tmp[2]);
        reallen = NFIMsgHead::NF_NTOHS(n);
        if (reallen < PAYLOAD_MID_LEN)
        {
            // length not canonical
            return make_error_code(websocket::error::ws_bad_size);
        }
        break;
    }
    case PAYLOAD_MAX_LEN:
    {
        //unsupport 64bit len data frame
        //game server 64K is enough for client to server
        return make_error_code(websocket::error::ws_bad_size);
        // reallen = *(uint64_t*)(&tmp[2]);
        // reallen = NFIMsgHead::NF_NTOHLL(reallen);
        // if (reallen < 65536)
        // {
        //     // length not canonical
        //     return make_error_code(websocket::error::ws_bad_size);
        // }
        // break;
    }
    default:
        reallen = fh.payload_len;
        break;
    }

    if (size < need + reallen)
    {
        //need more data
        return std::error_code();
    }

    if (fh.mask)
    {
        fh.key = *((uint32_t*)(tmp + (need - sizeof(fh.key))));
        // unmask data:
        uint8_t* d = (uint8_t*)(tmp + need);
        for (uint64_t i = 0; i < reallen; i++)
        {
            d[i] = d[i] ^ ((uint8_t*)(&fh.key))[i % 4];
        }
    }

    if (fh.op == opcode::close)
    {
        //mark: may have error msg
        return websocket::make_error_code(websocket::error::ws_closed);
    }

    // mark:
    // need : control frame len
    // reallen : payload  len
    // write on message callback here
    // callback(data+need,reallen)

	if (fh.op == opcode::binary)
	{
		const char* pbData = data + need;
		NFMsgHead xHead;
		int nMsgBodyLength = DeCode(pbData, reallen, xHead);
		if (nMsgBodyLength > 0 && xHead.GetMsgID() > 0)
		{
			OnReceiveNetPack(sockIndex, xHead.GetMsgID(), pbData + NFIMsgHead::NF_Head::NF_HEAD_LENGTH, nMsgBodyLength);
		}
	}
	else if (fh.op == opcode::text)
	{
		const char* pbData = data + need;
		OnReceiveNetPack(sockIndex, 0, pbData, reallen);
	}

    //remove control frame
    size_t offset = need + reallen;
    pNetObject->RemoveBuff(0, offset);

    return DecodeFrame(sockIndex,pNetObject);
}

int NFWSModule::DeCode(const char* strData, const uint32_t unAllLen, NFMsgHead& xHead)
{ 
    if (unAllLen < NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    { 
        return -1;
    }
    if (NFIMsgHead::NF_Head::NF_HEAD_LENGTH != xHead.DeCode(strData))
    {  
        return -2;
    }
    if (xHead.GetBodyLength() > (unAllLen - NFIMsgHead::NF_Head::NF_HEAD_LENGTH))
    {   
        return -3;
    }

    return xHead.GetBodyLength();
}

std::string NFWSModule::EncodeFrame(const char * data, size_t size_, bool text)
{
    //may write a buffer with headreserved space
    std::string res;
    res.reserve(size_ + 10);

    std::string sizebuf;
    uint64_t size = size_;
    
    uint8_t payload_len = 0;
    if (size <= PAYLOAD_MIN_LEN)
    {
        payload_len = static_cast<uint8_t>(size);
    }
    else if (size <= UINT16_MAX)
    {
        payload_len = static_cast<uint8_t>(PAYLOAD_MID_LEN);
        uint16_t n = (uint16_t)size;
        n = NFIMsgHead::NF_HTONS(n);
        sizebuf.append(reinterpret_cast<const char*>(&n), sizeof(n));
    }
    else
    {
        payload_len = static_cast<uint8_t>(PAYLOAD_MAX_LEN);
        size = NFIMsgHead::NF_HTONLL(size);
        sizebuf.append(reinterpret_cast<const char*>(&size), sizeof(size));
    }

    uint8_t ocode = FIN_FRAME_FLAG | static_cast<uint8_t>(opcode::binary);
    if (text)
    {
        ocode = FIN_FRAME_FLAG | static_cast<uint8_t>(opcode::text);
    }

    res.append(reinterpret_cast<const char*>(&ocode), sizeof(opcode));
    res.append(reinterpret_cast<const char*>(&payload_len), sizeof(payload_len));
    if(!sizebuf.empty())
        res.append(sizebuf);

    res.append(data,size);
    return res;
}

std::string NFWSModule::HashKey(const char * key, size_t len)
{
    uint8_t keybuf[60] = {0};
    std::memcpy(keybuf, key, len);
    std::memcpy(keybuf + len, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36);

    uint8_t shakey[sha1::sha1_context::digest_size] = { 0 };
    sha1::sha1_context ctx;
    sha1::init(ctx);
    sha1::update(ctx, keybuf, sizeof(keybuf));
    sha1::finish(ctx, shakey);
    return base64_encode(shakey, sizeof(shakey));
}
