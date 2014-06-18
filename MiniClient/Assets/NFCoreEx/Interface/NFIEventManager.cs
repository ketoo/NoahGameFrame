using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	public abstract class NFIEventManager
	{
		public abstract void RegisterCallback(int nEventID, NFIEvent.EventHandler handler, NFIValueList valueList);
		public abstract void DoEvent(int nEventID, NFIValueList valueList);
	}
}
