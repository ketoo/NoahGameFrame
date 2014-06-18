using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;
using UnityEngine;
using NFMsg;
using ProtoBuf;
using NFTCPClient;

public class NFCResultLogic
{
    NFNet mNet = null;
    public NFCResultLogic(NFNet net)
    {
        mNet = net;
        mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_EVENT_RESULT, EGMI_EVENT_RESULT);

    }

    private void EGMI_EVENT_RESULT(MsgHead head, MemoryStream stream)
    {
        //OnResultMsg
        NFMsg.AckEventResult resultCode = new NFMsg.AckEventResult();
        resultCode = Serializer.Deserialize<NFMsg.AckEventResult>(stream);
    }
}