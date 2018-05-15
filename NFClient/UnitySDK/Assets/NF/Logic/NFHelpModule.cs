using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using NFrame;
using NFSDK;
using UnityEngine;

namespace NFSDK
{
	public class NFHelpModule : NFIModule
	{
		public NFHelpModule(NFIPluginManager pluginManager)
		{
			mPluginManager = pluginManager;
		}

        public NFMsg.Ident NFToPB(NFGUID xID)
        {
            NFMsg.Ident xIdent = new NFMsg.Ident();
            xIdent.svrid = xID.nHead64;
            xIdent.index = xID.nData64;
            return xIdent;
        }

        public NFGUID PBToNF(NFMsg.Ident xID)
        {
            NFGUID xIdent = new NFGUID();
            xIdent.nHead64 = xID.svrid;
            xIdent.nData64 = xID.index;

            return xIdent;
        }

        public NFMsg.Vector2 NFToPB(NFVector2 value)
        {
            NFMsg.Vector2 vector = new NFMsg.Vector2();
            vector.x = value.X();
            vector.y = value.Y();

            return vector;
        }
        public NFVector2 PBToNF(NFMsg.Vector2 xVector)
        {
            NFVector2 xData = new NFVector2(xVector.x, xVector.y);

            return xData;
        }

        public NFMsg.Vector3 NFToPB(NFVector3 value)
        {
            NFMsg.Vector3 vector = new NFMsg.Vector3();
            vector.x = value.X();
            vector.y = value.Y();
            vector.z = value.Z();

            return vector;
        }

        public NFVector3 PBToNF(NFMsg.Vector3 xVector)
        {
            NFVector3 xData = new NFVector3(xVector.x, xVector.y, xVector.z);

            return xData;
        }

		public override bool Awake()
		{
			return true;
		}

		public override bool Init()
		{
            return true;
		}

		public override bool AfterInit()
		{
            return true;
		}

		public override bool Execute()
		{
            return true;
		}

		public override bool BeforeShut()
		{
            return true;
		}

		public override bool Shut()
		{
            return true;
		}
	}

}