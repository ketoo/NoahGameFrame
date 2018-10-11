//-----------------------------------------------------------------------
// <copyright file="NFIRecord.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFSDK
{
    public abstract class NFIRecord
    {

        public enum eRecordOptype
        {
            Add = 0,
            Del,
            Swap,
            Create,
            Update,
            Cleared,
            Sort,
            Cover,

        };

        public delegate void RecordEventHandler(NFGUID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFDataList.TData oldVar, NFDataList.TData newVar);

        public abstract void SetUpload(bool upload);
        public abstract bool GetUpload();
        public abstract bool IsUsed(int nRow);
        public abstract int GetRows();
        public abstract int GetCols();
		public abstract NFDataList.VARIANT_TYPE GetColType(int nCol);
        public abstract string GetColTag(int nCol);
		public abstract NFDataList GetColsData();
        public abstract NFDataList GetTagData();

        // add data
        public abstract int AddRow(int nRow);
        public abstract int AddRow(int nRow, NFDataList var);

        // set data
        public abstract int SetValue(int nRow, NFDataList var);

        public abstract bool SetInt(int nRow, int nCol, Int64 value);
        public abstract bool SetFloat(int nRow, int nCol, double value);
        public abstract bool SetString(int nRow, int nCol, string value);
        public abstract bool SetObject(int nRow, int nCol, NFGUID value);
        public abstract bool SetVector2(int nRow, int nCol, NFVector2 value);
        public abstract bool SetVector3(int nRow, int nCol, NFVector3 value);

        // query data
		public abstract NFDataList QueryRow(int nRow);
		public abstract NFDataList.TData QueryRowCol(int nRow, int nCol);
        public abstract bool SwapRow(int nOriginRow, int nTargetRow);

        public abstract Int64 QueryInt(int nRow, int nCol);
        public abstract double QueryFloat(int nRow, int nCol);
        public abstract string QueryString(int nRow, int nCol);
        public abstract NFGUID QueryObject(int nRow, int nCol);
        public abstract NFVector2 QueryVector2(int nRow, int nCol);
        public abstract NFVector3 QueryVector3(int nRow, int nCol);

        //public abstract int FindRow( int nRow );
        public abstract int FindColValue(int nCol, NFDataList var, ref NFDataList varResult);

        public abstract int FindInt(int nCol, Int64 value, ref NFDataList varResult);
        public abstract int FindFloat(int nCol, double value, ref NFDataList varResult);
        public abstract int FindString(int nCol, string value, ref NFDataList varResult);
        public abstract int FindObject(int nCol, NFGUID value, ref NFDataList varResult);
        public abstract int FindVector2(int nCol, NFVector2 value, ref NFDataList varResult);
        public abstract int FindVector3(int nCol, NFVector3 value, ref NFDataList varResult);

		public abstract int FindInt(int nCol, Int64 value);
		public abstract int FindFloat(int nCol, double value);
		public abstract int FindString(int nCol, string value);
		public abstract int FindObject(int nCol, NFGUID value);
		public abstract int FindVector2(int nCol, NFVector2 value);
		public abstract int FindVector3(int nCol, NFVector3 value);

        public abstract bool Remove(int nRow);
        public abstract bool Clear();

        public abstract void RegisterCallback(RecordEventHandler handler);

        public abstract string GetName();
        public abstract void SetName(string strName);
    }
}