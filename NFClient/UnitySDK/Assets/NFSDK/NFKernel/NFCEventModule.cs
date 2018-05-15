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
            mhtEvent = new Dictionary<int, NFIEvent>();
		}
  
  
        public override void RegisterCallback(int nEventID, NFIEvent.EventHandler handler)
        {
            if (!mhtEvent.ContainsKey(nEventID))
            {
				mhtEvent.Add(nEventID, new NFCEvent(nEventID, new NFCDataList()));
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

        public override void DoEvent(int nEventID)
        {
			NFIDataList valueList = new NFCDataList();
			if (mhtEvent.ContainsKey(nEventID))
            {
				NFIEvent identEvent = (NFIEvent)mhtEvent[nEventID];
                identEvent.DoEvent(valueList);
            }
        }

        Dictionary<int, NFIEvent> mhtEvent;
    }
}