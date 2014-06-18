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

namespace NFTCPClient
{
    public class ConstDefine
    {
        public static UInt32 NF_PACKET_HEAD_SIZE = 8;
        public static int MAX_PACKET_LEN = 655360;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class MsgHead
    {
        public MsgHead()
        {
            unMsgID = 0;
            unDataLen = 0;
        }
        public UInt32 unMsgID;
        public UInt32 unDataLen;
    };

    public class NFBinraryMessageEvent
    {		
        public NFBinraryMessageEvent(NFNet client)
        {
            net = client;
        }
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		private Hashtable mhtDelegation = new Hashtable();
        private NFNet net;
		public delegate void MsgDelegation(MsgHead head, MemoryStream stream);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		public bool RegisteredDelegation(NFMsg.EGameMsgID eMsg, MsgDelegation msgDelegate)
        {
			if(!mhtDelegation.ContainsKey(eMsg))
			{
				MsgDelegation myDelegationHandler = new MsgDelegation(msgDelegate);
				mhtDelegation.Add(eMsg, myDelegationHandler);
			}
			else
			{
				MsgDelegation myDelegationHandler = (MsgDelegation)mhtDelegation[eMsg];
				myDelegationHandler += new MsgDelegation(msgDelegate);
			}
			
			return true;
		}
		

		private bool DoDelegation(NFMsg.EGameMsgID eMsg, MsgHead head, MemoryStream stream)
        {
			if(mhtDelegation.ContainsKey(eMsg))
			{
				MsgDelegation myDelegationHandler = (MsgDelegation)mhtDelegation[eMsg];
				myDelegationHandler(head, stream);
					
				return true;
			}
			
			return false;
		}
			
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public void OnMessageEvent(MsgHead head, byte[] bytes)
        {
            if (head.unDataLen != bytes.Length)
            {
                return;
            }

            NFMsg.EGameMsgID eMsg = (NFMsg.EGameMsgID)head.unMsgID;

            string strTime = DateTime.Now.Hour + ":" + DateTime.Now.Minute + ":" + DateTime.Now.Second;
            string strData = "R:" + strTime + " MsgID:" + head.unMsgID + " Len:" + head.unDataLen;
            net.listener.aMsgList.Add(strData);


			DoDelegation(eMsg, head, new MemoryStream(bytes));            
        }
    }
}
