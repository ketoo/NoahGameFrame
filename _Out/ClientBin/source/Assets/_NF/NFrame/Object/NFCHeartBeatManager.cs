//-----------------------------------------------------------------------
// <copyright file="NFCHeartBeatManager.cs">
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
	public class NFCHeartBeatManager : NFIHeartBeatManager
    {
		public NFCHeartBeatManager(NFGUID self)
		{
			mSelf = self;
            mhtHeartBeat = new Dictionary<string, NFIHeartBeat>();
		}

        public override void AddHeartBeat(string strHeartBeatName, float fTime, int nCount, NFIHeartBeat.HeartBeatEventHandler handler)
		{
			if (!mhtHeartBeat.ContainsKey(strHeartBeatName))
			{
                NFIHeartBeat xHeartBeat = new NFCHeartBeat(mSelf, strHeartBeatName, fTime, nCount);
                mhtHeartBeat.Add(strHeartBeatName, xHeartBeat);
                xHeartBeat.RegisterCallback(handler);
			}
		}

        public override bool FindHeartBeat(string strHeartBeatName)
        {
            if (!mhtHeartBeat.ContainsKey(strHeartBeatName))
            {
                return true;
            }

            return false;
        }
        
        public override void RemoveHeartBeat(string strHeartBeatName)
        {
            if (!mhtHeartBeat.ContainsKey(strHeartBeatName))
            {
                mhtHeartBeat.Remove(strHeartBeatName);
            }
        }

		public override void Update(float fPassTime)
		{

            NFIDataList xRemoveKeyList = new NFCDataList(); ;

            foreach (KeyValuePair<string, NFIHeartBeat> kv in mhtHeartBeat)
            {
                NFIHeartBeat xHeartBeat = (NFIHeartBeat)kv.Value;
                if (xHeartBeat.NeedRemove())
                {
                    xRemoveKeyList.AddString((string)kv.Key);
                }
                else
                {
                    xHeartBeat.Update(fPassTime);
                }
            }

            for (int i = 0; i < xRemoveKeyList.Count(); i++)
            {
                mhtHeartBeat.Remove(xRemoveKeyList.StringVal(i));
            }
		}

		NFGUID mSelf;
        Dictionary<string, NFIHeartBeat> mhtHeartBeat;
    }
}