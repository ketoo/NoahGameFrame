using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

namespace NFSDK
{
    public class NFCMessageDispatcher
    {
        public delegate void MessageHandler(UInt16 id, MemoryStream stream);
        public void AddReceiveCallBack(UInt16 id, MessageHandler netHandler)
        {
            if (m_msgHandles.ContainsKey(id))
            {
                Debug.LogFormat("Cannot register exist func {0}!", id);
                return;
            }
            m_msgHandles.Add(id, netHandler);
        }
        public void clearregisterMsgFunc()
        {
            m_msgHandles.Clear();
        }
        public void dispatchMessage(UInt16 id, MemoryStream stream)
        {
            Debug.LogFormat("MessageDispatcher::dispatchMessage {0}", id);
            MessageHandler handler;
            if (!m_msgHandles.TryGetValue(id, out handler))
            {
                Debug.LogFormat("Cannot handle this message id: {0}", id);
                if (id == 0)
                {
                    Debug.Log("FUCK!!!");
                }
                return;
            }

            handler(id, stream);
        }
        public Dictionary<UInt16, MessageHandler> m_msgHandles = new Dictionary<ushort, MessageHandler>();
        ArrayList mCatchingMsg = new ArrayList();
    };

}