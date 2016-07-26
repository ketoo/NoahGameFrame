using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public class NFCEventManager : NFIEventManager
    {
		public NFCEventManager(NFIDENTID self)
		{
			mSelf = self;
            mhtEvent = new Dictionary<int, NFIEvent>();
		}

		public override void RegisterCallback(int nEventID, NFIEvent.EventHandler handler, NFIDataList valueList)
		{
			if (!mhtEvent.ContainsKey(nEventID))
			{
				mhtEvent.Add(nEventID, new NFCEvent(mSelf, nEventID, valueList));
			}

			NFIEvent identEvent = (NFIEvent)mhtEvent[nEventID];
			identEvent.RegisterCallback(handler);

		}

		public override void DoEvent(int nEventID, NFIDataList valueList)
		{
			if (mhtEvent.ContainsKey(nEventID))
			{
				NFIEvent identEvent = (NFIEvent)mhtEvent[nEventID];
				identEvent.DoEvent(valueList);
			}
		}

		NFIDENTID mSelf;
        Dictionary<int, NFIEvent> mhtEvent;
    }
}