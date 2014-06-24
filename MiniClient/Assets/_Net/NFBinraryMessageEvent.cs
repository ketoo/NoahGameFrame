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
        public static UInt32 NF_PACKET_HEAD_SIZE = 6;
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
        public UInt16 unMsgID;
        public UInt32 unDataLen;
    };

    public class NFBinraryMessageEvent
    {		
        public NFBinraryMessageEvent(NFNet client)
        {
            net = client;
        }
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        private Hashtable mhtMsgDelegation = new Hashtable();
        private Hashtable mhtEventDelegation = new Hashtable();
        private NFNet net;
        public delegate void MsgDelegation(MsgHead head, MemoryStream stream);
        public delegate void ResultCodeDelegation(NFMsg.EGameEventCode eCode);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		public bool RegisteredDelegation(NFMsg.EGameMsgID eMsg, MsgDelegation msgDelegate)
        {
			if(!mhtMsgDelegation.ContainsKey(eMsg))
			{
				MsgDelegation myDelegationHandler = new MsgDelegation(msgDelegate);
				mhtMsgDelegation.Add(eMsg, myDelegationHandler);
			}
			else
			{
				MsgDelegation myDelegationHandler = (MsgDelegation)mhtMsgDelegation[eMsg];
				myDelegationHandler += new MsgDelegation(msgDelegate);
			}
			
			return true;
		}

        public bool RegisteredResultCodeDelegation(NFMsg.EGameEventCode eCode, ResultCodeDelegation msgDelegate)
        {
            if (!mhtEventDelegation.ContainsKey(eCode))
            {
                ResultCodeDelegation myDelegationHandler = new ResultCodeDelegation(msgDelegate);
                mhtEventDelegation.Add(eCode, myDelegationHandler);
            }
            else
            {
                ResultCodeDelegation myDelegationHandler = (ResultCodeDelegation)mhtMsgDelegation[eCode];
                myDelegationHandler += new ResultCodeDelegation(msgDelegate);
            }

            return true;
        }

        public bool DoResultCodeDelegation(NFMsg.EGameEventCode eCode)
        {
            if (mhtEventDelegation.ContainsKey(eCode))
            {
                ResultCodeDelegation myDelegationHandler = (ResultCodeDelegation)mhtEventDelegation[eCode];
                myDelegationHandler(eCode);

                return true;
            }

            return false;
        }

		private bool DoDelegation(NFMsg.EGameMsgID eMsg, MsgHead head, MemoryStream stream)
        {
			if(mhtMsgDelegation.ContainsKey(eMsg))
			{
				MsgDelegation myDelegationHandler = (MsgDelegation)mhtMsgDelegation[eMsg];
				myDelegationHandler(head, stream);
					
				return true;
			}
			
			return false;
		}
			
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public void OnMessageEvent(MsgHead head, byte[] bytes)
        {
            if (head.unDataLen != bytes.Length + ConstDefine.NF_PACKET_HEAD_SIZE)
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
