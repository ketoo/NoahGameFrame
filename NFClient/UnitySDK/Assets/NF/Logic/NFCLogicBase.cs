using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace NFSDK
{
	public class NFCLogicBase: NFIModule
    {
        public delegate bool EventHandler(NFIDataList valueList);
        public override bool Awake() { return true; }
        public override bool AfterInit() { return true; }
        public override bool Execute() { return true; }
        public override bool BeforeShut() { return true; }
        public override bool Shut() { return true; }

        public NFCLogicBase()
        {
            mhtEvent = new Dictionary<int, ArrayList>();
		}

        public override bool Init() 
        { 
            return true;
        }

        public void RegisterCallback(int nEventID, EventHandler handler)
        {
            ArrayList events;
            if (!mhtEvent.TryGetValue(nEventID, out events))
            {
                events = new ArrayList();
                mhtEvent.Add(nEventID, events);
            }
            events.Add(handler);
		}

        public void DoEvent(int nEventID, NFIDataList valueList = null)
        {
            ArrayList events;
            if (!mhtEvent.TryGetValue(nEventID, out events))
            {
                return;
            }

            foreach (EventHandler handle in events)
            {
                if (!handle(valueList))
                    break;
            }
        }

        static public NFMsg.Ident NFToPB(NFGUID xID)
        {
            NFMsg.Ident xIdent = new NFMsg.Ident();
            xIdent.svrid = xID.nHead64;
            xIdent.index = xID.nData64;
            return xIdent;
        }

        static public NFGUID PBToNF(NFMsg.Ident xID)
        {
            NFGUID xIdent = new NFGUID();
            xIdent.nHead64 = xID.svrid;
            xIdent.nData64 = xID.index;

            return xIdent;
        }

        static public NFMsg.Vector2 NFToPB(NFVector2 value)
        {
            NFMsg.Vector2 vector = new NFMsg.Vector2();
            vector.x = value.X();
            vector.y = value.Y();

            return vector;
        }
        static public NFVector2 PBToNF(NFMsg.Vector2 xVector)
        {
            NFVector2 xData = new NFVector2(xVector.x, xVector.y);

            return xData;
        }

        static public NFMsg.Vector3 NFToPB(NFVector3 value)
        {
            NFMsg.Vector3 vector = new NFMsg.Vector3();
            vector.x = value.X();
            vector.y = value.Y();
            vector.z = value.Z();

            return vector;
        }
        static public NFVector3 PBToNF(NFMsg.Position xVector)
        {
            NFVector3 xData = new NFVector3(xVector.x, xVector.y, xVector.z);

            return xData;
        }

        Dictionary<int, ArrayList> mhtEvent;
    }
}