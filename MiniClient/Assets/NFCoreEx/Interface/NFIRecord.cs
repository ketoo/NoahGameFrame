using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIRecord
    {
		
        public enum eRecordOptype
        {
            Add = 0,
            Del,
            Changed,
            Updata,
            Create,
        };

		public delegate void RecordEventHandler(NFIDENTID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIValueList oldVar, NFIValueList newVar);
		
		public abstract bool IsUsed(int nRow);
		public abstract int GetRows();
        public abstract int GetCols();
        public abstract NFIValueList.VARIANT_TYPE GetColType( int nCol);

        // add data
        public abstract int AddRow(int nRow);
        public abstract int AddRow(int nRow, NFIValueList var);

        // set data
        public abstract int SetValue(int nRow, NFIValueList var);

        public abstract bool SetInt(int nRow, int nCol, int value);
        public abstract bool SetFloat(int nRow, int nCol, float value);
        public abstract bool SetDouble(int nRow, int nCol, double value);
        public abstract bool SetString(int nRow, int nCol, string value);
        public abstract bool SetObject(int nRow, int nCol, NFIDENTID value);

        // query data
        public abstract NFIValueList QueryRow(int nRow);
        public abstract bool SwapRow(int nOriginRow, int nTargetRow);

        public abstract int QueryInt(int nRow, int nCol);
        public abstract float QueryFloat(int nRow, int nCol);
        public abstract double QueryDouble(int nRow, int nCol);
        public abstract string QueryString(int nRow, int nCol);
        public abstract NFIDENTID QueryObject(int nRow, int nCol);

        //public abstract int FindRow( int nRow );
        public abstract int FindColValue(int nCol, NFIValueList var);

        public abstract int FindInt(int nCol, int value);
        public abstract int FindFloat(int nCol, float value);
        public abstract int FindDouble(int nCol, double value);
        public abstract int FindString(int nCol, string value);
        public abstract int FindObject(int nCol, NFIDENTID value);

        public abstract bool Remove(int nRow);
        public abstract bool Clear();

		public abstract void RegisterCallback(RecordEventHandler handler);

        public abstract string GetName();
		public abstract void SetName(string strName);
    }
}