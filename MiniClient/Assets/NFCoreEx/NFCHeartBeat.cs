using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public class NFCHeartBeat : NFIHeartBeat
	{

		public NFCHeartBeat(NFIDENTID self, string strHeartBeatName, float fTime, NFIValueList valueList)
		{
			mSelf = self;
			mstrHeartBeatName = strHeartBeatName;
			mfTime = fTime;
			mfOldTime = fTime;
			mArgValueList = valueList;
		}

		public override void RegisterCallback(NFIHeartBeat.HeartBeatEventHandler handler)
		{
			doHandlerDel += handler;
		}

		public override bool Update(float fPassTime)
		{
			mfTime -= fPassTime;
			if (mfTime < 0.0f)
			{
				if (null != doHandlerDel)
				{
					doHandlerDel(mSelf, mstrHeartBeatName, mfOldTime, mArgValueList);
				}
				return true;
			}

			return false;
		}

		NFIDENTID mSelf;
		string mstrHeartBeatName;
		float mfTime;
		float mfOldTime;
		NFIValueList mArgValueList;

		HeartBeatEventHandler doHandlerDel;
    }
}