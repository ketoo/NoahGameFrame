using System;
using System.Collections;
using System.Runtime.InteropServices;
using UnityEngine;

namespace NFTCPClient
{
    public class NFBinaryClientListener
    {
        public NFBinaryClientListener(NFClientNet c)
        {
            client = c;
        }

        private NFClientNet client;
        private UInt32 mnPacketSize = 0;
        private byte[] mPacket = new byte[ConstDefine.MAX_PACKET_LEN];

        public void OnClientConnect(NFTCPEventParams eventParams)
        {
            Debug.Log("Client connected");
        }

        public void OnClientDisconnect(NFTCPEventParams eventParams)
        {
            if(client.IsConnected())
            {
                client.Disconnect();
            }
            client.net.mPlayerState = NFNet.PLAYER_STATE.E_DISCOUNT;

            Debug.Log("Client disconnected");
        }

        public void OnClientConnectionRefused(NFTCPEventParams eventParams)
        {
            Debug.Log("Client refused");
            client.net.listener.Log("Client refused");

            client = null;
        }

        public void OnDataReceived(NFTCPEventParams eventParams)
        {

            //stick packet
            byte[] bytes = eventParams.packet.bytes;
            int bytesCount = eventParams.packet.bytesCount;

            client.net.listener.Log("OnDataReceived:" + mnPacketSize + "|" + bytesCount);

            if (mnPacketSize + bytesCount < ConstDefine.MAX_PACKET_LEN)
            {
                //先缓存
                Array.Copy(bytes, 0, mPacket, mnPacketSize, bytesCount);
                mnPacketSize += (UInt32)bytesCount;

                OnDataReceived();
            }
            else
            {
                //把包头打印
                object structType = new MsgHead();
                StructureTransform.ByteArrayToStructureEndian(bytes, ref structType, 0);
                MsgHead head = (MsgHead)structType;
                client.net.listener.Log("-----ID SIZE---:" + head.unMsgID + "|" + head.unDataLen);
            }
        }

        static bool OnDataReceived(NFClientNet client, byte[] bytes, UInt32 bytesCount)
        {
            if (bytes.Length == bytesCount)
            {
                object structType = new MsgHead();
                StructureTransform.ByteArrayToStructureEndian(bytes, ref structType, 0);
                MsgHead head = (MsgHead)structType;

                Int32 nBodyLen = (Int32)bytesCount - (Int32)ConstDefine.NF_PACKET_HEAD_SIZE;
                if (nBodyLen > 0)
                {
                    byte[] body = new byte[nBodyLen];
                    Array.Copy(bytes, 10, body, 0, nBodyLen);

                    client.net.binMsgEvent.OnMessageEvent(head, body);
                    return true;
                }
            }

            return false;
        }

        public void OnDataReceived()
        {
            //包头内部的那个长度，包括了包头的10字节
            if (mnPacketSize >= ConstDefine.NF_PACKET_HEAD_SIZE)
            {
                object structType = new MsgHead();
                byte[] headBytes = new byte[Marshal.SizeOf(structType)];

                Array.Copy(mPacket, 0, headBytes, 0, Marshal.SizeOf(structType));
                StructureTransform.ByteArrayToStructureEndian(mPacket, ref structType, 0);
                MsgHead head = (MsgHead)structType;

                //UnityEngine.Debug.Log("MsgType=" + head.usMsgID.ToString());
                //此长度要看是否包含包头,不包含就要加上包头长度,目前是包含
                if (head.unDataLen == mnPacketSize)
                {
                    //包刚合适
                    byte[] body_head = new byte[head.unDataLen];
                    Array.Copy(mPacket, 0, body_head, 0, head.unDataLen);
                    mnPacketSize = 0;

                    if (false == OnDataReceived(client, body_head, head.unDataLen))
                    {
                        OnClientDisconnect(new NFTCPEventParams());
                    }
                }
                else if (mnPacketSize > head.unDataLen)
                {
                    //超过一个包
                    //剩余的包内容弄出来
                    UInt32 nNewLen = mnPacketSize - head.unDataLen;
                    byte[] newpacket = new byte[ConstDefine.MAX_PACKET_LEN];
                    Array.Copy(mPacket, head.unDataLen, newpacket, 0, nNewLen);

                    //消息内容
                    byte[] body_head = new byte[head.unDataLen];
                    Array.Copy(mPacket, 0, body_head, 0, head.unDataLen);
                    mnPacketSize = nNewLen;
                    mPacket = newpacket;

                    if (false == OnDataReceived(client, body_head, head.unDataLen))
                    {
                        OnClientDisconnect(new NFTCPEventParams());
                    }

                    OnDataReceived();
                }
            }
        }
    }
}