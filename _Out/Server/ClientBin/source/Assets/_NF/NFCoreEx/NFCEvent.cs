using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	class NFCEvent : NFIEvent
	{
		public NFCEvent(NFIDENTID self, int nEventID, NFIDataList valueList)
		{
			mSelf = self;
			mnEventID = nEventID;
            mArgValueList = valueList;
		}

		public override void RegisterCallback(NFIEvent.EventHandler handler)
		{
			mHandlerDel += handler;
		}

		public override void DoEvent(NFIDataList valueList)
		{
			if (null != mHandlerDel)
			{
				mHandlerDel(mSelf, mnEventID, mArgValueList, valueList);
			}
		}


		NFIDENTID mSelf;
		int mnEventID;
		NFIDataList mArgValueList;

		NFIEvent.EventHandler mHandlerDel;
	}
}
