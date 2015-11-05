using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public class NFCRecordManager : NFIRecordManager
	{
		public NFCRecordManager(NFIDENTID ident)
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

		public override NFIRecord AddRecord(string strRecordName, int nRow, NFIDataList varData)
		{
			NFIRecord record = new NFCRecord(mSelf, strRecordName, nRow, varData);
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
		
		public override NFIDataList GetRecordList()
		{
			NFIDataList varData = new NFCDataList();
            foreach (KeyValuePair<string, NFIRecord> de in mhtRecord) 
			{
				varData.AddString(de.Key);				
			}
			
			return varData;
		}
		
		NFIDENTID mSelf;
        //Hashtable mhtRecord;
        Dictionary<string, NFIRecord> mhtRecord;
	}
}