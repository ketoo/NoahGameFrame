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

        public override void AddHeartBeat(string strHeartBeatName, float fTime, NFIValueList valueList)
		{
			if (!mhtHeartBeat.ContainsKey(strHeartBeatName))
			{
                NFIHeartBeat xHeartBeat = new NFCHeartBeat(mSelf, strHeartBeatName, fTime, valueList);
                mhtHeartBeat.Add(strHeartBeatName, xHeartBeat);
			}
		}

		public override void RegisterCallback(string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler)
		{
			if (mhtHeartBeat.ContainsKey(strHeartBeatName))
			{
				NFIHeartBeat xHeartBeat = (NFIHeartBeat)mhtHeartBeat[strHeartBeatName];
				xHeartBeat.RegisterCallback(handler);
			}
		}

		public override void Update(float fPassTime)
		{

            NFCValueList keyList = new NFCValueList();

            foreach (System.Collections.DictionaryEntry heartObject in mhtHeartBeat)
            {
                NFIHeartBeat heartBeat = (NFIHeartBeat)heartObject.Value;
                if (heartBeat.Update(fPassTime))
                {
                    keyList.AddString((string)heartObject.Key);
                }

//                 Console.WriteLine(heartObject.Key.ToString());
//                 Console.WriteLine(heartObject.Value.ToString());
            }

            for (int i = 0; i < keyList.Count(); i++ )
            {
                mhtHeartBeat.Remove(keyList.StringVal(i));
            }

		}

		NFIDENTID mSelf;
		Hashtable mhtHeartBeat;
    }
}