using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	public abstract class NFIEvent
	{
		public delegate void EventHandler(NFIDENTID self, int nEventID, NFIValueList initValueList, NFIValueList valueList);

		public abstract void RegisterCallback(NFIEvent.EventHandler handler);
		public abstract void DoEvent(NFIValueList valueList);
	}
}
