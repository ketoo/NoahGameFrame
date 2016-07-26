using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public abstract class NFIHeartBeatManager
    {
        public abstract void AddHeartBeat(string strHeartBeatName, float fTime, NFIHeartBeat.HeartBeatEventHandler handler, NFIDataList valueList);
		public abstract void Update(float fPassTime);
    }
}