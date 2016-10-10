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
using NFrame;

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

    static public NFMsg.Ident NFToPB(NFrame.NFGUID xID)
    {
        NFMsg.Ident xIdent = new NFMsg.Ident();
        xIdent.svrid = xID.nHead64;
        xIdent.index = xID.nData64;

        return xIdent;
    }

    static public NFMsg.Vector2 NFToPB(NFVector2 value)
    {
        NFMsg.Vector2 vector = new NFMsg.Vector2();
        vector.x = value.X();
        vector.y = value.Y();

        return vector;
    }

    static public NFMsg.Vector3 NFToPB(NFVector3 value)
    {
        NFMsg.Vector3 vector = new NFMsg.Vector3();
        vector.x = value.X();
        vector.y = value.Y();
        vector.z = value.Z();

        return vector;
    }


    public NFBinarySendLogic(NFNet clientnet)
    {
        xNet = clientnet;
    }

    public void SendMsg(NFrame.NFGUID xID, NFMsg.EGameMsgID unMsgID, MemoryStream stream)
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

            SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_LOGIN, stream);
        }
    }

    public void RequireWorldList()
    {
        NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
        xData.type = NFMsg.ReqServerListType.RSLT_WORLD_SERVER;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
    }

    public void RequireConnectWorld(int nWorldID)
    {
        NFMsg.ReqConnectWorld xData = new NFMsg.ReqConnectWorld();
        xData.world_id = nWorldID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqConnectWorld>(stream, xData);

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_CONNECT_WORLD, stream);
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

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_CONNECT_KEY, stream);
    }

    public void RequireServerList()
    {
        NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
        xData.type = NFMsg.ReqServerListType.RSLT_GAMES_ERVER;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
    }

    public void RequireSelectServer(int nServerID)
    {
        NFMsg.ReqSelectServer xData = new NFMsg.ReqSelectServer();
        xData.world_id = nServerID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqSelectServer>(stream, xData);

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_SELECT_SERVER, stream);
    }

    public void RequireRoleList(string strAccount, int nGameID)
    {
        NFMsg.ReqRoleList xData = new NFMsg.ReqRoleList();
        xData.game_id = nGameID;
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqRoleList>(stream, xData);

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_ROLE_LIST, stream);
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

        SendMsg(new NFrame.NFGUID(), NFMsg.EGameMsgID.EGMI_REQ_CREATE_ROLE, stream);

    }

    public void RequireDelRole(NFrame.NFGUID objectID, string strAccount, string strRoleName, int nGameID)
    {
        NFMsg.ReqDeleteRole xData = new NFMsg.ReqDeleteRole();
        xData.name = UnicodeEncoding.Default.GetBytes(strRoleName);
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);
        xData.game_id = nGameID;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqDeleteRole>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_DELETE_ROLE, stream);
    }

    public void RequireEnterGameServer(NFrame.NFGUID objectID, string strAccount, string strRoleName, int nServerID)
    {
        NFMsg.ReqEnterGameServer xData = new NFMsg.ReqEnterGameServer();
        xData.name = UnicodeEncoding.Default.GetBytes(strRoleName);
        xData.account = UnicodeEncoding.Default.GetBytes(strAccount);
        xData.game_id = nServerID;
        xData.id = NFToPB(objectID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqEnterGameServer>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_ENTER_GAME, stream);
    }

    public void RequireHeartBeat(NFrame.NFGUID objectID)
    {
        NFMsg.ReqHeartBeat xData = new NFMsg.ReqHeartBeat();

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqHeartBeat>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_STS_HEART_BEAT, stream);
    }

    //有可能是他副本的NPC移动,因此增加64对象ID
    public void RequireMove(NFrame.NFGUID objectID, float fX, float fZ)
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
    }

    public void RequireMoveImmune(NFrame.NFGUID objectID, float fX, float fZ)
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
    }

    //有可能是他副本的NPC移动,因此增加64对象ID
    public void RequireUseSkill(NFrame.NFGUID objectID, string strKillID, NFrame.NFGUID nTargetID, float fNowX, float fNowZ, float fTarX, float fTarZ)
    {
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
        xData.skill_id = System.Text.Encoding.Default.GetBytes(strKillID);
        xData.tar_pos = xTarPos;
        xData.now_pos = xNowPos;

        NFMsg.EffectData xEffData = new NFMsg.EffectData();
        xEffData.effect_ident = (NFToPB(nTargetID));
        xEffData.effect_value = 0;
        xEffData.effect_rlt = 0;

        xData.effect_data.Add(xEffData);


        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckUseSkill>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_SKILL_OBJECTX, stream);
    }

    public void RequireChat(NFrame.NFGUID objectID, NFrame.NFGUID targetID, int nType, string strData)
    {
        NFMsg.ReqAckPlayerChat xData = new NFMsg.ReqAckPlayerChat();
        xData.chat_id = NFToPB(targetID);
        xData.chat_name = UnicodeEncoding.Default.GetBytes(xNet.strRoleName);
        xData.chat_type = (NFMsg.ReqAckPlayerChat.EGameChatType)nType;
        xData.chat_info = UnicodeEncoding.Default.GetBytes(strData);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckPlayerChat>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_CHAT, stream);
    }

    public void RequireSwapScene(NFrame.NFGUID objectID, int nTransferType, int nSceneID, int nLineIndex)
    {
        NFMsg.ReqAckSwapScene xData = new NFMsg.ReqAckSwapScene();
        xData.transfer_type = (NFMsg.ReqAckSwapScene.EGameSwapType)nTransferType;
        xData.scene_id = nSceneID;
        xData.line_id = nLineIndex;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAckSwapScene>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_SWAP_SCENE, stream);
    }


    public void RequireAcceptTask(NFrame.NFGUID objectID, string strTaskID)
    {
        NFMsg.ReqAcceptTask xData = new NFMsg.ReqAcceptTask();
        xData.task_id = UnicodeEncoding.Default.GetBytes(strTaskID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqAcceptTask>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_ACCEPT_TASK, stream);
    }

    public void RequireCompeleteTask(NFrame.NFGUID objectID, string strTaskID)
    {
        NFMsg.ReqCompeleteTask xData = new NFMsg.ReqCompeleteTask();
        xData.task_id = UnicodeEncoding.Default.GetBytes(strTaskID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqCompeleteTask>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_COMPELETE_TASK, stream);
    }

    public void RequirePickUpItem(NFrame.NFGUID objectID, NFrame.NFGUID nItemID)
    {
        NFMsg.ReqPickDropItem xData = new NFMsg.ReqPickDropItem();
        xData.item_guid = NFToPB(nItemID);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ReqPickDropItem>(stream, xData);

        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_REQ_PICK_ITEM, stream);
    }

    public void RequirePropertyInt(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyInt xData = new NFMsg.ObjectPropertyInt();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyInt xPropertyInt = new NFMsg.PropertyInt();
        xPropertyInt.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xPropertyInt.data = newVar.IntVal();
        xData.property_list.Add(xPropertyInt);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyInt>(stream, xData);
        Debug.Log("send upload int");
        SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_INT, stream);
    }

    public void RequirePropertyFloat(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyFloat xData = new NFMsg.ObjectPropertyFloat();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyFloat xPropertyFloat = new NFMsg.PropertyFloat();
        xPropertyFloat.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xPropertyFloat.data = (float)newVar.FloatVal();
        xData.property_list.Add(xPropertyFloat);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyFloat>(stream, xData);
        Debug.Log("send upload Float");
        NFStart.Instance.GetFocusSender().SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_FLOAT, stream);
    }

    public void RequirePropertyString(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyString xData = new NFMsg.ObjectPropertyString();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyString xPropertyString = new NFMsg.PropertyString();
        xPropertyString.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xPropertyString.data = System.Text.Encoding.Default.GetBytes(newVar.StringVal());
        xData.property_list.Add(xPropertyString);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyString>(stream, xData);
        Debug.Log("send upload String");
        NFStart.Instance.GetFocusSender().SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_STRING, stream);
    }

    public void RequirePropertyObject(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyObject xData = new NFMsg.ObjectPropertyObject();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyObject xPropertyObject = new NFMsg.PropertyObject();
        xPropertyObject.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xPropertyObject.data = NFBinarySendLogic.NFToPB(newVar.ObjectVal());
        xData.property_list.Add(xPropertyObject);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyObject>(stream, xData);
        Debug.Log("send upload Object");
        NFStart.Instance.GetFocusSender().SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_OBJECT, stream);
    }

    public void RequirePropertyVector2(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyVector2 xData = new NFMsg.ObjectPropertyVector2();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyVector2 xProperty = new NFMsg.PropertyVector2();
        xProperty.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xProperty.data = NFBinarySendLogic.NFToPB(newVar.Vector2Val());
        xData.property_list.Add(xProperty);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyVector2>(stream, xData);

        NFStart.Instance.GetFocusSender().SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_VECTOR2, stream);
    }

    public void RequirePropertyVector3(NFrame.NFGUID objectID, string strPropertyName, NFIDataList.TData newVar)
    {
        NFMsg.ObjectPropertyVector3 xData = new NFMsg.ObjectPropertyVector3();
        xData.player_id = NFBinarySendLogic.NFToPB(objectID);

        NFMsg.PropertyVector3 xProperty = new NFMsg.PropertyVector3();
        xProperty.property_name = System.Text.Encoding.Default.GetBytes(strPropertyName);
        xProperty.data = NFBinarySendLogic.NFToPB(newVar.Vector3Val());
        xData.property_list.Add(xProperty);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectPropertyVector3>(stream, xData);

        NFStart.Instance.GetFocusSender().SendMsg(objectID, NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_VECTOR3, stream);
    }

    public void RequireAddRow(NFrame.NFGUID self, string strRecordName, int nRow)
    {
        NFMsg.ObjectRecordAddRow xData = new NFMsg.ObjectRecordAddRow();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordAddRowStruct xRecordAddRowStruct = new NFMsg.RecordAddRowStruct();
        xData.row_data.Add(xRecordAddRowStruct);
        xRecordAddRowStruct.row = nRow;

        NFIObject xObject = NFCKernelModule.Instance.GetObject(self);
        NFIRecord xRecord = xObject.GetRecordManager().GetRecord(strRecordName);
        NFIDataList xRowData = xRecord.QueryRow(nRow);
        for(int i = 0;i<xRowData.Count();i++)
        {
            switch(xRowData.GetType(i))
            {
                case NFIDataList.VARIANT_TYPE.VTYPE_INT:
                    {
                        NFMsg.RecordInt xRecordInt = new NFMsg.RecordInt();
                        xRecordInt.row = nRow;
                        xRecordInt.col = i;
                        xRecordInt.data = xRowData.IntVal(i);
                        xRecordAddRowStruct.record_int_list.Add(xRecordInt);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_FLOAT:
                    {
                        NFMsg.RecordFloat xRecordFloat = new NFMsg.RecordFloat();
                        xRecordFloat.row = nRow;
                        xRecordFloat.col = i;
                        xRecordFloat.data = (float)xRowData.FloatVal(i);
                        xRecordAddRowStruct.record_float_list.Add(xRecordFloat);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_STRING:
                    {
                        NFMsg.RecordString xRecordString = new NFMsg.RecordString();
                        xRecordString.row = nRow;
                        xRecordString.col = i;
                        xRecordString.data = System.Text.Encoding.Default.GetBytes(xRowData.StringVal(i));
                        xRecordAddRowStruct.record_string_list.Add(xRecordString);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_OBJECT:
                    {
                        NFMsg.RecordObject xRecordObject = new NFMsg.RecordObject();
                        xRecordObject.row = nRow;
                        xRecordObject.col = i;
                        xRecordObject.data = NFBinarySendLogic.NFToPB(xRowData.ObjectVal(i));
                        xRecordAddRowStruct.record_object_list.Add(xRecordObject);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                    {
                        NFMsg.RecordVector2 xRecordVector = new NFMsg.RecordVector2();
                        xRecordVector.row = nRow;
                        xRecordVector.col = i;
                        xRecordVector.data = NFBinarySendLogic.NFToPB(xRowData.Vector2Val(i));
                        xRecordAddRowStruct.record_vector2_list.Add(xRecordVector);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                    {
                        NFMsg.RecordVector3 xRecordVector = new NFMsg.RecordVector3();
                        xRecordVector.row = nRow;
                        xRecordVector.col = i;
                        xRecordVector.data = NFBinarySendLogic.NFToPB(xRowData.Vector3Val(i));
                        xRecordAddRowStruct.record_vector3_list.Add(xRecordVector);
                    }
                    break;

            }
        }

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordAddRow>(stream, xData);
        Debug.Log("send upload record addRow");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_ADD_ROW, stream);
    }

    public void RequireRemoveRow(NFrame.NFGUID self, string strRecordName, int nRow)
    {
        NFMsg.ObjectRecordRemove xData = new NFMsg.ObjectRecordRemove();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);
        xData.remove_row.Add(nRow);

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordRemove>(stream, xData);
        Debug.Log("send upload record removeRow");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_REMOVE_ROW, stream);
    }

    public void RequireSwapRow(NFrame.NFGUID self, string strRecordName, int nOriginRow, int nTargetRow)
    {
        NFMsg.ObjectRecordSwap xData = new NFMsg.ObjectRecordSwap();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.origin_record_name = System.Text.Encoding.Default.GetBytes(strRecordName);
        xData.target_record_name = System.Text.Encoding.Default.GetBytes(strRecordName);
        xData.row_origin = nOriginRow;
        xData.row_target = nTargetRow;

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordSwap>(stream, xData);
        Debug.Log("send upload record swapRow");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_SWAP_ROW, stream);
    }

    public void RequireRecordInt(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordInt xData = new NFMsg.ObjectRecordInt();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordInt xRecordInt = new NFMsg.RecordInt();
        xData.property_list.Add(xRecordInt);
        xRecordInt.row = nRow;
        xRecordInt.col = nCol;
        xRecordInt.data = newVar.IntVal();

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordInt>(stream, xData);
        Debug.Log("send upload record int");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_INT, stream);
    }

    public void RequireRecordFloat(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordFloat xData = new NFMsg.ObjectRecordFloat();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordFloat xRecordFloat = new NFMsg.RecordFloat();
        xData.property_list.Add(xRecordFloat);
        xRecordFloat.row = nRow;
        xRecordFloat.col = nCol;
        xRecordFloat.data = (float)newVar.FloatVal();

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordFloat>(stream, xData);
        Debug.Log("send upload record float");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_FLOAT, stream);
    }

    public void RequireRecordString(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordString xData = new NFMsg.ObjectRecordString();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordString xRecordString = new NFMsg.RecordString();
        xData.property_list.Add(xRecordString);
        xRecordString.row = nRow;
        xRecordString.col = nCol;
        xRecordString.data = System.Text.Encoding.Default.GetBytes(newVar.StringVal());

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordString>(stream, xData);
        Debug.Log("send upload record string");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_STRING, stream);
    }

    public void RequireRecordObject(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordObject xData = new NFMsg.ObjectRecordObject();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordObject xRecordObject = new NFMsg.RecordObject();
        xData.property_list.Add(xRecordObject);
        xRecordObject.row = nRow;
        xRecordObject.col = nCol;
        xRecordObject.data = NFBinarySendLogic.NFToPB(newVar.ObjectVal());

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordObject>(stream, xData);
        Debug.Log("send upload record object");
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_OBJECT, stream);
    }

    public void RequireRecordVector2(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordVector2 xData = new NFMsg.ObjectRecordVector2();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordVector2 xRecordVector = new NFMsg.RecordVector2();
        xRecordVector.row = nRow;
        xRecordVector.col = nCol;
        xRecordVector.data = NFBinarySendLogic.NFToPB(newVar.Vector2Val());

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordVector2>(stream, xData);
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_VECTOR2, stream);
    }

    public void RequireRecordVector3(NFrame.NFGUID self, string strRecordName, int nRow, int nCol, NFIDataList.TData newVar)
    {
        NFMsg.ObjectRecordVector3 xData = new NFMsg.ObjectRecordVector3();
        xData.player_id = NFBinarySendLogic.NFToPB(self);
        xData.record_name = System.Text.Encoding.Default.GetBytes(strRecordName);

        NFMsg.RecordVector3 xRecordVector = new NFMsg.RecordVector3();
        xRecordVector.row = nRow;
        xRecordVector.col = nCol;
        xRecordVector.data = NFBinarySendLogic.NFToPB(newVar.Vector3Val());

        MemoryStream stream = new MemoryStream();
        Serializer.Serialize<NFMsg.ObjectRecordVector3>(stream, xData);
        SendMsg(self, NFMsg.EGameMsgID.EGMI_ACK_RECORD_VECTOR3, stream);
    }
}