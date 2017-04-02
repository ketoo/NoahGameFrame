using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFSDK
{
	public abstract class NFIEvent
	{
		public delegate void EventHandler(NFGUID self, int nEventID, NFIDataList initValueList, NFIDataList valueList);

		public abstract void RegisterCallback(NFIEvent.EventHandler handler);
		public abstract void DoEvent(NFIDataList valueList);
	}
}
