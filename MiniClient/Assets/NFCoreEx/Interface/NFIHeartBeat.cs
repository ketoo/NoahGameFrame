using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public abstract class NFIHeartBeat
	{
		public delegate void HeartBeatEventHandler(NFIDENTID self, string strHeartBeat, float fTime, NFIValueList valueList);

		public abstract void RegisterCallback(NFIHeartBeat.HeartBeatEventHandler handler);
		public abstract bool Update(float fPassTime);
	}
}