using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	class NFCEvent : NFIEvent
	{
		public NFCEvent(NFIDENTID self, int nEventID, NFIValueList valueList)
		{
			mSelf = self;
			mnEventID = nEventID;
            mArgValueList = valueList;
		}

		public override void RegisterCallback(NFIEvent.EventHandler handler)
		{
			doHandlerDel += handler;
		}

		public override void DoEvent(NFIValueList valueList)
		{
			if (null != doHandlerDel)
			{
				doHandlerDel(mSelf, mnEventID, mArgValueList, valueList);
			}
		}


		NFIDENTID mSelf;
		int mnEventID;
		NFIValueList mArgValueList;

		NFIEvent.EventHandler doHandlerDel;
	}
}
