//-----------------------------------------------------------------------
// <copyright file="NFCEventModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFrame
{
	public class NFCEventModule : NFIEventModule
    {
		public NFCEventModule()
		{
            mhtEvent = new Dictionary<NFGUID, Dictionary<int, NFIEvent>>();
		}

		public override void RegisterCallback(NFGUID self, int nEventID, NFIEvent.EventHandler handler, NFIDataList valueList)
		{
            Dictionary<int, NFIEvent> xData = mhtEvent[self];
			if (!mhtEvent.ContainsKey(self) || null == xData)
			{
                xData = new Dictionary<int, NFIEvent>();
                xData.Add(nEventID, new NFCEvent(self, nEventID, valueList));
                
                mhtEvent.Add(self, xData);
                return;
			}

            if (!xData.ContainsKey(nEventID))
            {
                xData.Add(nEventID, new NFCEvent(self, nEventID, valueList));
                return ;
            }

			NFIEvent identEvent = (NFIEvent)mhtEvent[self][nEventID];
			identEvent.RegisterCallback(handler);
		}

        public override void DoEvent(NFGUID self, int nEventID, NFIDataList valueList)
        {
            Dictionary<int, NFIEvent> xData = mhtEvent[self];
            if (null != xData)
            {
                if (xData.ContainsKey(nEventID))
                {
                    NFIEvent identEvent = (NFIEvent)xData[nEventID];
                    identEvent.DoEvent(valueList);
                }
            }
        }

        Dictionary<NFGUID, Dictionary<int, NFIEvent>> mhtEvent;
    }
}