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
        mNet.mxBinMsgEvent.RegisteredResultCodeDelegation(NFMsg.EGameEventCode.EGEC_UNKOWN_ERROR, EGEC_UNKOWN_ERROR);
        mNet.mxBinMsgEvent.RegisteredResultCodeDelegation(NFMsg.EGameEventCode.EGEC_ACCOUNT_SUCCESS, EGEC_ACCOUNT_SUCCESS);
    }

    private void EGEC_UNKOWN_ERROR(NFMsg.EGameEventCode eCode)
    {

    }

    private void EGEC_ACCOUNT_SUCCESS(NFMsg.EGameEventCode eCode)
    {
        mNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN;
    }
}