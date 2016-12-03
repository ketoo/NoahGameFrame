//-----------------------------------------------------------------------
// <copyright file="NFCEvent.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
	class NFCEvent : NFIEvent
	{
		public NFCEvent(NFGUID self, int nEventID, NFIDataList valueList)
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


		NFGUID mSelf;
		int mnEventID;
		NFIDataList mArgValueList;

		NFIEvent.EventHandler mHandlerDel;
	}
}
