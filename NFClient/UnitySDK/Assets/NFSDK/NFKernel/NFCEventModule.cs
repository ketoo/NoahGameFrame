using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
	public class NFCEventModule : NFIEventModule
    {
        public override bool Awake() { return true; }
        public override bool Init() { return true; }
        public override bool AfterInit() { return true; }
        public override bool Execute() { return true; }
        public override bool BeforeShut() { return true; }
        public override bool Shut() { return true; }

        private static NFCEventModule _instance = null;
        public static NFCEventModule Instance()
        {
            return _instance;
        }

        public NFCEventModule(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
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