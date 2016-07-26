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
            Swap,
            Updata,
            Create,
        };

		public delegate void RecordEventHandler(NFIDENTID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIDataList oldVar, NFIDataList newVar);
		
		public abstract bool IsUsed(int nRow);
		public abstract int GetRows();
        public abstract int GetCols();
        public abstract NFIDataList.VARIANT_TYPE GetColType( int nCol);
        public abstract NFIDataList GetColsData();

        // add data
        public abstract int AddRow(int nRow);
        public abstract int AddRow(int nRow, NFIDataList var);

        // set data
        public abstract int SetValue(int nRow, NFIDataList var);

        public abstract bool SetInt(int nRow, int nCol, Int64 value);
        public abstract bool SetFloat(int nRow, int nCol, float value);
        public abstract bool SetDouble(int nRow, int nCol, double value);
        public abstract bool SetString(int nRow, int nCol, string value);
        public abstract bool SetObject(int nRow, int nCol, NFIDENTID value);

        // query data
        public abstract NFIDataList QueryRow(int nRow);
        public abstract bool SwapRow(int nOriginRow, int nTargetRow);

        public abstract Int64 QueryInt(int nRow, int nCol);
        public abstract float QueryFloat(int nRow, int nCol);
        public abstract double QueryDouble(int nRow, int nCol);
        public abstract string QueryString(int nRow, int nCol);
        public abstract NFIDENTID QueryObject(int nRow, int nCol);

        //public abstract int FindRow( int nRow );
        public abstract int FindColValue(int nCol, NFIDataList var);

        public abstract int FindInt(int nCol, Int64 value);
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