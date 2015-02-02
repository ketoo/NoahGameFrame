using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using ProtoBuf;
using UnityEngine;
using NFTCPClient;
using NFMsg;
using NFCoreEx;

class StructureTransform
{
    static bool bBig = false;//defalut little
    public static void Reverse(byte[] msg)
    {
        if (!bBig)
        {
            Array.Reverse(msg);
        }
    }

    public static void Reverse(byte[] msg, int nOffest, int nSize)
    {
        if (!bBig)
        {
            Array.Reverse(msg, nOffest, nSize);
        }
    }


    public static bool SetEndian(bool bIsBig)
    {
        bBig = bIsBig;
        return bBig;
    }

    public static void ByteArrayToStructureEndian(byte[] bytearray, ref object obj, int startoffset)
    {
        int len = Marshal.SizeOf(obj);
        IntPtr i = Marshal.AllocHGlobal(len);
        byte[] temparray = (byte[])bytearray.Clone();
        // ŽÓœá¹¹ÌåÖžÕë¹¹Ôìœá¹¹Ìå
        obj = Marshal.PtrToStructure(i, obj.GetType());
        // ×öŽó¶Ë×ª»»
        object thisBoxed = obj;
        Type test = thisBoxed.GetType();
        int reversestartoffset = startoffset;
        // ÁÐŸÙœá¹¹ÌåµÄÃ¿žö³ÉÔ±£¬²¢Reverse
        foreach (var field in test.GetFields())
        {
            object fieldValue = field.GetValue(thisBoxed); // Get value

            TypeCode typeCode = Type.GetTypeCode(fieldValue.GetType());  //Get Type
            if (typeCode != TypeCode.Object)  //Èç¹ûÎªÖµÀàÐÍ
            {
                Reverse(temparray, reversestartoffset, Marshal.SizeOf(fieldValue));
                reversestartoffset += Marshal.SizeOf(fieldValue);
            }
            else  //Èç¹ûÎªÒýÓÃÀàÐÍ
            {
                reversestartoffset += ((byte[])fieldValue).Length;
            }
        }
        try
        {
            //œ«×ÖœÚÊý×éžŽÖÆµœœá¹¹ÌåÖžÕë
            Marshal.Copy(temparray, startoffset, i, len);
        }
        catch (Exception ex) { Console.WriteLine("ByteArrayToStructure FAIL: error " + ex.ToString()); }
        obj = Marshal.PtrToStructure(i, obj.GetType());
        Marshal.FreeHGlobal(i);  //ÊÍ·ÅÄÚŽæ
    }

    public static byte[] StructureToByteArrayEndian(object obj)
    {
        object thisBoxed = obj;   //copy £¬œ« struct ×°Ïä
        Type test = thisBoxed.GetType();

        int offset = 0;
        byte[] data = new byte[Marshal.SizeOf(thisBoxed)];

        object fieldValue;
        TypeCode typeCode;
        byte[] temp;
        // ÁÐŸÙœá¹¹ÌåµÄÃ¿žö³ÉÔ±£¬²¢Reverse
        foreach (var field in test.GetFields())
        {
            fieldValue = field.GetValue(thisBoxed); // Get value

            typeCode = Type.GetTypeCode(fieldValue.GetType());  // get type

            switch (typeCode)
            {
                case TypeCode.Single: // float
                    {
                        temp = BitConverter.GetBytes((Single)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(Single));
                        break;
                    }
                case TypeCode.Int32:
                    {
                        temp = BitConverter.GetBytes((Int32)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(Int32));
                        break;
                    }
                case TypeCode.UInt32:
                    {
                        temp = BitConverter.GetBytes((UInt32)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(UInt32));
                        break;
                    }
                case TypeCode.Int16:
                    {
                        temp = BitConverter.GetBytes((Int16)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(Int16));
                        break;
                    }
                case TypeCode.UInt16:
                    {
                        temp = BitConverter.GetBytes((UInt16)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(UInt16));
                        break;
                    }
                case TypeCode.Int64:
                    {
                        temp = BitConverter.GetBytes((Int64)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(Int64));
                        break;
                    }
                case TypeCode.UInt64:
                    {
                        temp = BitConverter.GetBytes((UInt64)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(UInt64));
                        break;
                    }
                case TypeCode.Double:
                    {
                        temp = BitConverter.GetBytes((Double)fieldValue);
                        StructureTransform.Reverse(temp);
                        Array.Copy(temp, 0, data, offset, sizeof(Double));
                        break;
                    }
                case TypeCode.Byte:
                    {
                        data[offset] = (Byte)fieldValue;
                        break;
                    }
                default:
                    {
                        //System.Diagnostics.Debug.Fail("No conversion provided for this type : " + typeCode.ToString());
                        break;
                    }
            }; // switch
            if (typeCode == TypeCode.Object)
            {
                int length = ((byte[])fieldValue).Length;
                Array.Copy(((byte[])fieldValue), 0, data, offset, length);
                offset += length;
            }
            else
            {
                offset += Marshal.SizeOf(fieldValue);
            }
        } // foreach

        return data;
    } // Swap
};

public class NFBinarySendLogic
{
    NFNet xNet;

    static public NFMsg.Ident NFToPB(NFCoreEx.NFIDENTID xID)
    {
        NFMsg.Ident xIdent = new NFMsg.Ident();
        xIdent.svrid = xID.nHead64;
        xIdent.index = xID.nData64;

        return xIdent;
    }


    public NFBinarySendLogic(NFNet clientnet)
    {
        xNet = clientnet;
    }

    public void SendMsg(NFCoreEx.NFIDENTID xID, NFMsg.EGameMsgID unMsgID, MemoryStream stream)
    {
        if (NFStart.Instance.bDebugMode)
        {
            return;
        }

        NFMsg.MsgBase xData = new NFMsg.MsgBase();
        xData.player_id = NFToPB(xID);
        xData.msg_data = stream.ToArray();

        MemoryStream body = new MemoryStream();
        Serializer.Serialize<NFMsg.MsgBase>(body, xData);

        MsgHead head = new MsgHead();
        head.unMsgID = (UInt16)unMsgID;
        head.unDataLen = (UInt32)body.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

        byte[] bodyByte = body.ToArray();
        byte[] headByte = StructureTransform.StructureToByteArrayEndian(head);


        byte[] sendBytes = new byte[head.unDataLen];
        headByte.CopyTo(sendBytes, 0);
        bodyByte.CopyTo(sendBytes, headByte.Length);

        xNet.mxClient.SendBytes(sendBytes);

        string strTime = DateTime.Now.Hour + ":" + DateTime.Now.Minute + ":" + DateTime.Now.Second;
        string strData = "S***:" + strTime + " MsgID:" + head.unMsgID + " Len:" + head.unDataLen;
        xNet.mxListener.aMsgList.Add(strData);
    }


    public void LoginPB(string strAccount, string strPassword, string strSessionKey)
    {
        if (NFStart.Instance.bDebugMode)
        {
            xNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST;
            //NFCRenderInterface.Instance.LoadScene("SelectScene");
        }
        else
        {
            NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
            xData.account = System.Text.Encoding.Default.GetBytes(strAccount);
            xData.password = System.Text.Encoding.Default.GetBytes(strPassword);
            xData.security_code = System.Text.Encoding.Default.GetBytes(strSessionKey);
            xData.signBuff = System.Text.Encoding.Default.GetBytes("");
            xData.clientVersion = 1;
            xData.loginMode = 0;
            xData.clientIP = 0;
            xData.clientMAC = 0;
            xData.device_info = System.Text.Encoding.Default.GetBytes("");
            xData.extra_info = System.Text.Encoding.Default.GetBytes("");

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqAccountLogin>(stream, xData);

            SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_LOGIN, stream);
        }
    }

    public void RequireWorldList()
    {
        NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
        xData.type = NFMsg.ReqServerListType.RSLT_WORLD_SERVER;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
    }

    public void RequireConnectWorld(int nWorldID)
    {
        NFMsg.ReqConnectWorld xData = new NFMsg.ReqConnectWorld();
        xData.world_id = nWorldID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqConnectWorld>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_CONNECT_WORLD, stream);
    }

    public void RequireVerifyWorldKey(string strAccount, string strKey)
    {
        NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
        xData.account = System.Text.Encoding.Default.GetBytes(strAccount);
        xData.password = System.Text.Encoding.Default.GetBytes("");
        xData.security_code = System.Text.Encoding.Default.GetBytes(strKey);
        xData.signBuff = System.Text.Encoding.Default.GetBytes("");
        xData.clientVersion = 1;
        xData.loginMode = 0;
        xData.clientIP = 0;
        xData.clientMAC = 0;
        xData.device_info = System.Text.Encoding.Default.GetBytes("");
        xData.extra_info = System.Text.Encoding.Default.GetBytes("");

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAccountLogin>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_CONNECT_KEY, stream);
    }

    public void RequireServerList()
    {
        NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
        xData.type = NFMsg.ReqServerListType.RSLT_GAMES_ERVER;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
    }

    public void RequireSelectServer(int nServerID)
    {
        NFMsg.ReqSelectServer xData = new NFMsg.ReqSelectServer();
        xData.world_id = nServerID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqSelectServer>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_SELECT_SERVER, stream);
    }

    public void RequireRoleList(string strAccount, int nGameID)
    {
        NFMsg.ReqRoleList xData = new NFMsg.ReqRoleList();
        xData.game_id = nGameID;
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqRoleList>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_ROLE_LIST, stream);
    }

    public void RequireCreateRole(string strAccount, string strRoleName, int byCareer, int bySex, int nGameID)
    {
        if (strRoleName.Length >= 20 || strRoleName.Length < 1)
        {
            return;
        }

        NFMsg.ReqCreateRole xData = new NFMsg.ReqCreateRole();
        xData.career = byCareer;
        xData.sex = bySex;
        xData.noob_name = UnicodeEncoding.Default.GetBytes(strRoleName);
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);
        xData.race = 0;
        xData.game_id = nGameID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqCreateRole>(stream, xData);

        SendMsg(new NFCoreEx.NFIDENTID(), NFMsg.EGameMsgID.EGMI_REQ_CREATE_ROLE, stream);

        if (NFStart.Instance.bDebugMode)
        {
            NFMsg.AckRoleLiteInfoList xAckBodyData = new NFMsg.AckRoleLiteInfoList();
            NFMsg.RoleLiteInfo info = new NFMsg.RoleLiteInfo();

            info.career = byCareer;
            info.sex = bySex;
            info.noob_name = xData.noob_name;
            info.race = xData.race;
            info.noob_name = xData.noob_name;
            info.id = new NFMsg.Ident();
            info.game_id = 1;
            info.role_level = 1;
            info.view_record = xData.account = UnicodeEncoding.Default.GetBytes("");
            info.delete_time = 1;
            info.reg_time = 1;
            info.last_offline_time = 1;
            info.last_offline_ip = 1;
            xAckBodyData.char_data.Add(info);

            MemoryStream xAckBodyStream = new MemoryStream();
            Serializer.Serialize<NFMsg.AckRoleLiteInfoList>(xAckBodyStream, xAckBodyData);

            NFMsg.MsgBase xAckData = new NFMsg.MsgBase();
            xAckData.player_id = info.id;
            xAckData.msg_data = xAckBodyStream.ToArray();

            MemoryStream xAckAllStream = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckAllStream, xAckData);

            MsgHead head = new MsgHead();
            head.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_ROLE_LIST;
            head.unDataLen = (UInt32)xAckAllStream.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(head, xAckAllStream.ToArray());
        }
    }

    public void RequireDelRole(NFCoreEx.NFIDENTID objectID, string strAccount, string strRoleName, int nGameID)
    {
        NFMsg.ReqDeleteRole xData = new NFMsg.ReqDeleteRole();
        xData.name = UnicodeEncoding.Default.GetBytes(strRoleName);
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);
        xData.game_id = nGameID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqDeleteRole>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_DELETE_ROLE, stream);
    }

    public void RequireEnterGameServer(NFCoreEx.NFIDENTID objectID, string strAccount, string strRoleName, int nServerID)
    {
        NFMsg.ReqEnterGameServer xData = new NFMsg.ReqEnterGameServer();
        xData.name = UnicodeEncoding.Default.GetBytes(strRoleName);
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);
        xData.game_id = nServerID;
        xData.id = NFToPB(objectID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqEnterGameServer>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_ENTER_GAME, stream);

        if (NFStart.Instance.bDebugMode)
        {
            //EGMI_ACK_OBJECT_ENTRY
            //property
            //EGMI_ACK_SWAP_SCENE
            //EGMI_ACK_OBJECT_ENTRY
            //property
            float fX = 0.0f;
            float fY = 0.0f;
            float fZ = 0.0f;
            NFIElement xElement = NFCElementManager.Instance.GetElement("1");
            if (null != xElement)
            {
                string strRelivePos = xElement.QueryString("RelivePos");
                string[] sArray = strRelivePos.Split(';');
                if (sArray.Length > 0)
                {
                    sArray = sArray[0].Split(',');
                }

                if (sArray.Length == 3)
                {
                    fX = float.Parse(sArray[0]);
                    fY = float.Parse(sArray[1]);
                    fZ = float.Parse(sArray[2]);
                }
            }
            /////////////////////////////////////////////
            //mainplayer
            NFMsg.AckPlayerEntryList xAckMainBodyData = new NFMsg.AckPlayerEntryList();
            NFMsg.PlayerEntryInfo xInfo = new NFMsg.PlayerEntryInfo();
            NFMsg.Ident xID = new NFMsg.Ident();
            xInfo.object_guid = xID;
            xInfo.x = fX;
            xInfo.y = fY;
            xInfo.z = fZ;
            xInfo.career_type = 1;
            xInfo.player_state = 1;
            xInfo.config_id = UnicodeEncoding.Default.GetBytes("");
            xInfo.scene_id = 1;
            xInfo.class_id = UnicodeEncoding.Default.GetBytes("Player");

            xAckMainBodyData.object_list.Add(xInfo);

            MemoryStream xAckMianPlayerBodyStream = new MemoryStream();
            Serializer.Serialize<NFMsg.AckPlayerEntryList>(xAckMianPlayerBodyStream, xAckMainBodyData);

            NFMsg.MsgBase xAckMianPlayerData = new NFMsg.MsgBase();
            xAckMianPlayerData.player_id = xID;
            xAckMianPlayerData.msg_data = xAckMianPlayerBodyStream.ToArray();

            MemoryStream xAckAllStream = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckAllStream, xAckMianPlayerData);

            MsgHead head = new MsgHead();
            head.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY;
            head.unDataLen = (UInt32)xAckAllStream.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(head, xAckAllStream.ToArray());
            /////////////////////////////////////////////
            //property

			NFMsg.ObjectPropertyInt propertyData = new NFMsg.ObjectPropertyInt();

            PropertyInt xPropertyInt = new PropertyInt();
            xPropertyInt.property_name = UnicodeEncoding.Default.GetBytes("MOVE_SPEED");
            xPropertyInt.data = 50000;
            propertyData.property_list.Add(xPropertyInt);
            propertyData.player_id = xID;

            MemoryStream xAckPropertyIntStream = new MemoryStream();
            Serializer.Serialize<NFMsg.ObjectPropertyInt>(xAckPropertyIntStream, propertyData);

            NFMsg.MsgBase xPropertyIntMsg = new NFMsg.MsgBase();
            xPropertyIntMsg.player_id = xID;
            xPropertyIntMsg.msg_data = xAckPropertyIntStream.ToArray();

            MemoryStream xAckPropertyIntAllStream = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckPropertyIntAllStream, xPropertyIntMsg);

            MsgHead xAckPropertyhead = new MsgHead();
            xAckPropertyhead.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_INT;
            xAckPropertyhead.unDataLen = (UInt32)xAckPropertyIntAllStream.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(xAckPropertyhead, xAckPropertyIntAllStream.ToArray());

            /////////////////////////////////////////////
            xNet.mPlayerState = NFNet.PLAYER_STATE.E_PLAYER_GAMEING;
            //NFCRenderInterface.Instance.LoadScene(1, fX, fY, fZ);
            /////////////////////////////////////////////

            //npc
            NFMsg.AckPlayerEntryList xAckNPCBodyData = new NFMsg.AckPlayerEntryList();
            for (int i = 0; i < 5; ++i)
            {
                NFMsg.PlayerEntryInfo xNPCInfo = new NFMsg.PlayerEntryInfo();

                NFMsg.Ident xNPCID = new NFMsg.Ident();
                xNPCID.index = i + 10000;
                xNPCInfo.object_guid = xNPCID;
                xNPCInfo.x = fX + i;
                xNPCInfo.y = fY;
                xNPCInfo.z = fZ + i;
                xNPCInfo.career_type = 1;
                xNPCInfo.player_state = 1;
                xNPCInfo.config_id = UnicodeEncoding.Default.GetBytes("");
                xNPCInfo.scene_id = 1;
                xNPCInfo.class_id = UnicodeEncoding.Default.GetBytes("Player");

                xAckNPCBodyData.object_list.Add(xNPCInfo);
            }

            MemoryStream xAckNPCBodyStream = new MemoryStream();
            Serializer.Serialize<NFMsg.AckPlayerEntryList>(xAckNPCBodyStream, xAckNPCBodyData);

            NFMsg.MsgBase xAckNPCrData = new NFMsg.MsgBase();
            xAckNPCrData.player_id = xID;
            xAckNPCrData.msg_data = xAckNPCBodyStream.ToArray();

            MemoryStream xAckAllNPCStream = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckAllNPCStream, xAckNPCrData);

            MsgHead xNPCHead = new MsgHead();
            xNPCHead.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY;
            xNPCHead.unDataLen = (UInt32)xAckAllNPCStream.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(xNPCHead, xAckAllNPCStream.ToArray());
            //////////////////////////////////////////////
        }
    }

    public void RequireHeartBeat(NFCoreEx.NFIDENTID objectID)
    {
        NFMsg.ReqHeartBeat xData = new NFMsg.ReqHeartBeat();

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqHeartBeat>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_STS_HEART_BEAT, stream);
    }

    //有可能是他副本的NPC移动,因此增加64对象ID
    public void RequireMove(NFCoreEx.NFIDENTID objectID, float fX, float fZ)
    {
        NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
        xData.mover = NFToPB(objectID);
        xData.moveType = 0;

        NFMsg.Position xTargetPos = new NFMsg.Position();
        xTargetPos.x = fX;
        xTargetPos.z = fZ;
        xData.target_pos.Add(xTargetPos);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckPlayerMove>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_MOVE, stream);

        if (NFStart.Instance.bDebugMode)
        {
            NFMsg.MsgBase xAckData = new NFMsg.MsgBase();
            xAckData.player_id = xData.mover;
            xAckData.msg_data = stream.ToArray();

            MemoryStream xAckBody = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckBody, xAckData);

            MsgHead head = new MsgHead();
            head.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_MOVE;
            head.unDataLen = (UInt32)xAckBody.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(head, xAckBody.ToArray());

        }
    }

    public void RequireMoveImmune(NFCoreEx.NFIDENTID objectID, float fX, float fZ)
    {
        NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
        xData.mover = NFToPB(objectID);
        xData.moveType = 0;
        NFMsg.Position xTargetPos = new NFMsg.Position();
        xTargetPos.x = fX;
        xTargetPos.z = fZ;
        xData.target_pos.Add(xTargetPos);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckPlayerMove>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_MOVE_IMMUNE, stream);

        if (NFStart.Instance.bDebugMode)
        {
            NFMsg.MsgBase xAckData = new NFMsg.MsgBase();
            xAckData.player_id = xData.mover;
            xAckData.msg_data = stream.ToArray();

            MemoryStream xAckBody = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(xAckBody, xAckData);

            MsgHead head = new MsgHead();
            head.unMsgID = (UInt16)NFMsg.EGameMsgID.EGMI_ACK_MOVE_IMMUNE;
            head.unDataLen = (UInt32)xAckBody.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;

            xNet.mxBinMsgEvent.OnMessageEvent(head, xAckBody.ToArray());

        }
    }

    //有可能是他副本的NPC移动,因此增加64对象ID
    public void RequireUseSkill(NFCoreEx.NFIDENTID objectID, string strKillID, NFCoreEx.NFIDENTID nTargetID, float fNowX, float fNowZ, float fTarX, float fTarZ)
    {
        //Debug.Log("RequireUseSkill:" + strKillID);

        NFMsg.Position xNowPos = new NFMsg.Position();
        NFMsg.Position xTarPos = new NFMsg.Position();

        xNowPos.x = fNowX;
        xNowPos.y = 0.0f;
        xNowPos.z = fNowZ;
        xTarPos.x = fTarX;
        xTarPos.y = 0.0f;
        xTarPos.z = fTarZ;

        NFMsg.ReqAckUseSkill xData = new NFMsg.ReqAckUseSkill();
        xData.user = NFToPB(objectID);
        xData.skill_id = UnicodeEncoding.Default.GetBytes(strKillID);
        xData.tar_pos = xTarPos;
        xData.now_pos = xNowPos;

        if (!nTargetID.IsNull())
        {
            xData.effect_ident.Add(NFToPB(nTargetID));
            xData.effect_value.Add(0);
            xData.effect_rlt.Add(0);
        }

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckUseSkill>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_SKILL_OBJECTX, stream);
    }

    public void RequireUseItem(NFCoreEx.NFIDENTID objectID, NFCoreEx.NFIDENTID nGuid, NFCoreEx.NFIDENTID nTargetID)
    {
        NFMsg.ReqAckUseItem xData = new NFMsg.ReqAckUseItem();
        xData.item_guid = NFToPB(nGuid);
        xData.effect_ident.Add(NFToPB(nTargetID));
        xData.effect_value.Add(0);
        xData.effect_rlt.Add(0);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckUseItem>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_ITEM_OBJECT, stream);
    }

    public void RequireChat(NFCoreEx.NFIDENTID objectID, NFCoreEx.NFIDENTID targetID, int nType, string strData)
    {
        NFMsg.ReqAckPlayerChat xData = new NFMsg.ReqAckPlayerChat();
        xData.chat_id = NFToPB(targetID);
        xData.chat_type = (NFMsg.ReqAckPlayerChat.EGameChatType)nType;
        xData.chat_info = UnicodeEncoding.Default.GetBytes(strData);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckPlayerChat>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_CHAT, stream);

    }

    public void RequireSwapScene(NFCoreEx.NFIDENTID objectID, int nTransferType, int nSceneID, int nLineIndex)
    {
        NFMsg.ReqAckSwapScene xData = new NFMsg.ReqAckSwapScene();
        xData.transfer_type = (NFMsg.ReqAckSwapScene.EGameSwapType)nTransferType;
        xData.scene_id = nSceneID;
        xData.line_id = nLineIndex;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckSwapScene>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_SWAP_SCENE, stream);
    }

    public void RequireProperty(NFCoreEx.NFIDENTID objectID, string strPropertyName, int nValue)
    {
        NFMsg.ReqCommand xData = new NFMsg.ReqCommand();
        xData.control_id = NFToPB(objectID);
        xData.command_id = ReqCommand.EGameCommandType.EGCT_MODIY_PROPERTY;
        xData.command_str_value = UnicodeEncoding.Default.GetBytes(strPropertyName);
        xData.command_value = nValue;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqCommand>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_COMMAND, stream);
    }

    public void RequireItem(NFCoreEx.NFIDENTID objectID, string strItemName, int nCount)
    {
        NFMsg.ReqCommand xData = new NFMsg.ReqCommand();
        xData.control_id = NFToPB(objectID);
        xData.command_id = ReqCommand.EGameCommandType.EGCT_MODIY_ITEM;
        xData.command_str_value = UnicodeEncoding.Default.GetBytes(strItemName);
        xData.command_value = nCount;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqCommand>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_COMMAND, stream);
    }

    public void RequireAcceptTask(NFCoreEx.NFIDENTID objectID, string strTaskID)
    {
        NFMsg.ReqAcceptTask xData = new NFMsg.ReqAcceptTask();
        xData.task_id = UnicodeEncoding.Default.GetBytes(strTaskID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAcceptTask>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_ACCEPT_TASK, stream);
    }

    public void RequireCompeleteTask(NFCoreEx.NFIDENTID objectID, string strTaskID)
    {
        NFMsg.ReqCompeleteTask xData = new NFMsg.ReqCompeleteTask();
        xData.task_id = UnicodeEncoding.Default.GetBytes(strTaskID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqCompeleteTask>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_COMPELETE_TASK, stream);
    }

    public void RequirePickUpItem(NFCoreEx.NFIDENTID objectID, NFCoreEx.NFIDENTID nItemID)
    {
        NFMsg.ReqPickDropItem xData = new NFMsg.ReqPickDropItem();
        xData.item_guid = NFToPB(nItemID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqPickDropItem>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_PICK_ITEM, stream);
    }
}