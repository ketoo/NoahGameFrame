using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public abstract class NFIRecordManager
    {
        public abstract NFIRecord AddRecord(string strRecordName,  int nRow, NFIDataList varData);
		public abstract NFIRecord GetRecord(string strRecordName);
		public abstract NFIDataList GetRecordList();
		
		public abstract void RegisterCallback(string strRecordName, NFIRecord.RecordEventHandler handler);
    }
}