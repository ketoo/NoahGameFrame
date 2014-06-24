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
            byte[] bytes = eventParams.packet.bytes;
            int bytesCount = eventParams.packet.bytesCount;

            client.net.listener.Log("OnDataReceived:" + mnPacketSize + "|" + bytesCount);

            if (mnPacketSize + bytesCount < ConstDefine.MAX_PACKET_LEN)
            {
                Array.Copy(bytes, 0, mPacket, mnPacketSize, bytesCount);
                mnPacketSize += (UInt32)bytesCount;

                OnDataReceived();
            }
        }

        void OnDataReceived()
        {
            if (mnPacketSize >= ConstDefine.NF_PACKET_HEAD_SIZE)
            {
                object structType = new MsgHead();
                byte[] headBytes = new byte[Marshal.SizeOf(structType)];

                Array.Copy(mPacket, 0, headBytes, 0, Marshal.SizeOf(structType));
                StructureTransform.ByteArrayToStructureEndian(headBytes, ref structType, 0);
                MsgHead head = (MsgHead)structType;

                if (head.unDataLen == mnPacketSize)
                {
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
                    UInt32 nNewLen = mnPacketSize - head.unDataLen;
                    byte[] newpacket = new byte[ConstDefine.MAX_PACKET_LEN];
                    Array.Copy(mPacket, head.unDataLen, newpacket, 0, nNewLen);

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

        bool OnDataReceived(NFClientNet client, byte[] bytes, UInt32 bytesCount)
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
                    Array.Copy(bytes, ConstDefine.NF_PACKET_HEAD_SIZE, body, 0, nBodyLen);

                    client.net.binMsgEvent.OnMessageEvent(head, body);
                    return true;
                }
                else
                {
                    //space packet
                }
            }

            return false;
        }
    }
}