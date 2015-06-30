using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public class NFCHeartBeatManager : NFIHeartBeatManager
    {
		public NFCHeartBeatManager(NFIDENTID self)
		{
			mSelf = self;
			mhtHeartBeat = new Hashtable();
		}

        public override void AddHeartBeat(string strHeartBeatName, float fTime, NFIHeartBeat.HeartBeatEventHandler handler, NFIDataList valueList)
		{
			if (!mhtHeartBeat.ContainsKey(strHeartBeatName))
			{
                NFIHeartBeat xHeartBeat = new NFCHeartBeat(mSelf, strHeartBeatName, fTime, valueList);
                mhtHeartBeat.Add(strHeartBeatName, xHeartBeat);
                xHeartBeat.RegisterCallback(handler);
			}
		}

		public override void Update(float fPassTime)
		{

            NFCDataList keyList = null;

            foreach (System.Collections.DictionaryEntry heartObject in mhtHeartBeat)
            {
                NFIHeartBeat heartBeat = (NFIHeartBeat)heartObject.Value;
                if (heartBeat.Update(fPassTime))
                {
                    if (null == keyList)
                    {
                        keyList = new NFCDataList();
                    }

                    keyList.AddString((string)heartObject.Key);
                }
            }

            if (null != keyList)
            {
                for (int i = 0; i < keyList.Count(); i++)
                {
                    mhtHeartBeat.Remove(keyList.StringVal(i));
                }
            }
		}

		NFIDENTID mSelf;
		Hashtable mhtHeartBeat;
    }
}