using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public abstract class NFIHeartBeatManager
    {
        public abstract void AddHeartBeat(string strHeartBeatName, float fTime, NFIValueList valueList);
		public abstract void RegisterCallback(string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler);
		public abstract void Update(float fPassTime);
    }
}