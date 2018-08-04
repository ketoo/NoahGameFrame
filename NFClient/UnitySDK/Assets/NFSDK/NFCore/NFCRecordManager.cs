//-----------------------------------------------------------------------
// <copyright file="NFCRecordManager.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
	public class NFCRecordManager : NFIRecordManager
	{
		public NFCRecordManager(NFGUID ident)
		{
			mSelf = ident;
            mhtRecord = new Dictionary<string, NFIRecord>();
		}

		public override void RegisterCallback(string strRecordName, NFIRecord.RecordEventHandler handler)
		{
			if (mhtRecord.ContainsKey(strRecordName))
			{
				NFIRecord record = (NFIRecord)mhtRecord[strRecordName];
				record.RegisterCallback(handler);
			}
		}

		public override NFIRecord AddRecord(string strRecordName, int nRow, NFDataList varData, NFDataList varTag)
		{
			NFIRecord record = new NFCRecord (mSelf, strRecordName, nRow, varData, varTag);
			mhtRecord.Add(strRecordName, record);

			return record;
		}

		public override NFIRecord GetRecord(string strPropertyName)
		{
			NFIRecord record = null;

			if (mhtRecord.ContainsKey(strPropertyName))
			{
				record = (NFIRecord)mhtRecord[strPropertyName];
			}

			return record;
		}
		
		public override NFDataList GetRecordList()
		{
			NFDataList varData = new NFDataList();
            foreach (KeyValuePair<string, NFIRecord> de in mhtRecord) 
			{
				varData.AddString(de.Key);				
			}
			
			return varData;
		}
		
		NFGUID mSelf;
        //Hashtable mhtRecord;
        Dictionary<string, NFIRecord> mhtRecord;
	}
}