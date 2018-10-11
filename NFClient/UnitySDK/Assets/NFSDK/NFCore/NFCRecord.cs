//-----------------------------------------------------------------------
// <copyright file="NFCRecord.cs">
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
    public class NFCRecord : NFIRecord
    {

		public NFCRecord(NFGUID self, string strRecordName, int nRow, NFDataList varData, NFDataList varTag)
		{
			mSelf = self;
			mnRow = nRow;
			mstrRecordName = strRecordName;
            mVarRecordType = new NFDataList(varData);
			mxTag = new NFDataList(varTag);
		}

        public override void SetUpload(bool upload)
        {
            mbUpload = upload;
        }

        public override bool GetUpload()
        {
            return mbUpload;
        }

        //==============================================
        public override bool IsUsed(int nRow)
		{
			if (mhtRecordVec.ContainsKey(nRow))
			{
				return true;
			}
			
			return false;
		}
		
		public override int GetRows()
        {
			return mnRow;
        }
		
        public override int GetCols()
        {
			return mVarRecordType.Count();
        }

        public override NFDataList.VARIANT_TYPE GetColType(int nCol)
        {
			return mVarRecordType.GetType(nCol);
        }

        public override NFDataList GetColsData()
        {
            return mVarRecordType;
        }

		public override NFDataList GetTagData()
		{
			return mxTag;
		}
		public override string GetColTag(int nCol)
		{
			return mxTag.StringVal (nCol);
		}

        // add data
        public override int AddRow(int nRow)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				return AddRow(nRow, mVarRecordType);
			}
			
			return -1;
        }

        public override int AddRow(int nRow, NFDataList var)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					mhtRecordVec[nRow] = new NFDataList(var);

                    if (null != doHandleDel)
                    {
                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Add, nRow, 0, NFDataList.NULL_TDATA, NFDataList.NULL_TDATA);
                    }
					return nRow;
				}
			}
			
			
            return -1;
        }

        // set data
        public override int SetValue(int nRow, NFDataList var)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				
				mhtRecordVec[nRow] = var;
				return nRow;
			}
            return -1;
        }

        public override bool SetInt(int nRow, int nCol, Int64 value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_INT)
				{
					if (valueList.IntVal(nCol) != value)
					{
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_INT);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_INT);

                        oldValue.Set(valueList.IntVal(nCol));
                        newValue.Set(value);

                        valueList.SetInt(nCol, value);
	                   
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
	                    }
	                }
				}
				return true;
				
			}
            return false;
        }

        public override bool SetFloat(int nRow, int nCol, double value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_FLOAT)
				{
					if (valueList.FloatVal(nCol) - value > NFDataList.EPS_DOUBLE
						|| valueList.FloatVal(nCol) - value < -NFDataList.EPS_DOUBLE)
					{
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_FLOAT);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_FLOAT);

                        oldValue.Set(valueList.FloatVal(nCol));
                        newValue.Set(value);

	                    valueList.SetFloat(nCol, value);
	
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
	                    }
	                }
	
				}
	
				return true;
			}
			return false;
        }

        public override bool SetString(int nRow, int nCol, string value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_STRING)
				{
					if (valueList.StringVal(nCol) != value)
					{
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_STRING);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_STRING);

                        oldValue.Set(valueList.StringVal(nCol));
                        newValue.Set(value);

                        valueList.SetString(nCol, value);
	
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
	                    }
	                }
				}
	
				return true;
			}

			return false;
        }

        public override bool SetObject(int nRow, int nCol, NFGUID value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_OBJECT)
				{
					if (valueList.ObjectVal(nCol) != value)
					{
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_OBJECT);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_OBJECT);

                        oldValue.Set(valueList.ObjectVal(nCol));
                        newValue.Set(value);

                        valueList.SetObject(nCol, value);

                        if (null != doHandleDel)
                        {
                            doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
                        }
                    }
				}

				return true;
			}

			return false;
        }

        public override bool SetVector2(int nRow, int nCol, NFVector2 value)
        {
            if (nRow >= 0 && nRow < mnRow)
            {
                if (!mhtRecordVec.ContainsKey(nRow))
                {
                    AddRow(nRow);
                }
                NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
                if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_VECTOR2)
                {
                    if (valueList.Vector2Val(nCol) != value)
                    {
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_VECTOR2);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_VECTOR2);

                        oldValue.Set(valueList.Vector2Val(nCol));
                        newValue.Set(value);

                        valueList.SetVector2(nCol, value);

                        if (null != doHandleDel)
                        {
                            doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
                        }
                    }
                }

                return true;
            }

            return false;
        }

        public override bool SetVector3(int nRow, int nCol, NFVector3 value)
        {
            if (nRow >= 0 && nRow < mnRow)
            {
                if (!mhtRecordVec.ContainsKey(nRow))
                {
                    AddRow(nRow);
                }
                NFDataList valueList = (NFDataList)mhtRecordVec[nRow];
                if (valueList.GetType(nCol) == NFDataList.VARIANT_TYPE.VTYPE_VECTOR3)
                {
                    if (valueList.Vector3Val(nCol) != value)
                    {
                        NFDataList.TData oldValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_VECTOR3);
                        NFDataList.TData newValue = new NFDataList.TData(NFDataList.VARIANT_TYPE.VTYPE_VECTOR3);

                        oldValue.Set(valueList.Vector3Val(nCol));
                        newValue.Set(value);

                        valueList.SetVector3(nCol, value);

                        if (null != doHandleDel)
                        {
                            doHandleDel(mSelf, mstrRecordName, eRecordOptype.Update, nRow, nCol, oldValue, newValue);
                        }
                    }
                }

                return true;
            }

            return false;
        }

        // query data
        public override NFDataList QueryRow(int nRow)
        {
			if (mhtRecordVec.ContainsKey(nRow))
			{
				return  (NFDataList)mhtRecordVec[nRow];
			}

            return null;
        }

		public override NFDataList.TData QueryRowCol(int nRow, int nCol)
		{
			if (mhtRecordVec.ContainsKey(nRow))
			{
				NFDataList dataList =  (NFDataList)mhtRecordVec[nRow];
				if (dataList != null)
				{
					return dataList.GetData (nCol);
				}
			}

			return null;
		}

        public override bool SwapRow(int nOriginRow, int nTargetRow)
        {
			if(nOriginRow >= 0 && nOriginRow < mnRow && nTargetRow >= 0 && nTargetRow < mnRow)
			{
	            NFDataList valueOriginList = null;
	            NFDataList valueTargetList = null;
	           
	            if (mhtRecordVec.ContainsKey(nOriginRow))
	            {
	                valueOriginList = (NFDataList)mhtRecordVec[nOriginRow];
	            }
	            if (mhtRecordVec.ContainsKey(nTargetRow))
	            {
	                valueTargetList = (NFDataList)mhtRecordVec[nOriginRow];
	            }
	
	            if (null == valueTargetList)
	            {
	                if (mhtRecordVec.ContainsKey(nOriginRow))
	                {
	                    mhtRecordVec.Remove(nOriginRow);
	                }
	            }
	            else
	            {
	               mhtRecordVec[nOriginRow] = valueTargetList;
	            }
	            
	            if (null == valueOriginList)
	            {
	                if (mhtRecordVec.ContainsKey(nTargetRow))
	                {
	                    mhtRecordVec.Remove(nTargetRow);
	                }
	            }
	            else
	            {
	                mhtRecordVec[nTargetRow] = valueOriginList;
	            }
	           
	            if (null != doHandleDel)
	             {
	                 doHandleDel(mSelf, mstrRecordName, eRecordOptype.Swap, nOriginRow, nTargetRow, NFDataList.NULL_TDATA, NFDataList.NULL_TDATA);
	             }
	            return true;
			}
			return false;
        }

        public override Int64 QueryInt(int nRow, int nCol)
        {
			NFDataList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.IntVal(nCol);
			}

			return 0;
        }

        public override double QueryFloat(int nRow, int nCol)
        {
			NFDataList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.FloatVal(nCol);
			}

            return 0.0;
        }

        public override string QueryString(int nRow, int nCol)
        {
			NFDataList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.StringVal(nCol);
			}

            return NFDataList.NULL_STRING;
        }

        public override NFGUID QueryObject(int nRow, int nCol)
        {
			NFDataList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.ObjectVal(nCol);
			}

            return NFDataList.NULL_OBJECT;
        }

        public override NFVector2 QueryVector2(int nRow, int nCol)
        {
            NFDataList valueList = QueryRow(nRow);
            if (null != valueList)
            {
                return valueList.Vector2Val(nCol);
            }

            return NFDataList.NULL_VECTOR2;
        }

        public override NFVector3 QueryVector3(int nRow, int nCol)
        {
            NFDataList valueList = QueryRow(nRow);
            if (null != valueList)
            {
                return valueList.Vector3Val(nCol);
            }

            return NFDataList.NULL_VECTOR3;
        }

        //public override int FindRow( int nRow );
        public override int FindColValue(int nCol, NFDataList var, ref NFDataList varResult)
        {
			for (int i = 0; i < mhtRecordVec.Count; i++ )
			{
				NFDataList valueList = (NFDataList)mhtRecordVec[i];
				switch (valueList.GetType(0))
				{
					case NFDataList.VARIANT_TYPE.VTYPE_INT:
						return FindInt(nCol, var.IntVal(0), ref varResult);

					case NFDataList.VARIANT_TYPE.VTYPE_FLOAT:
						return FindInt(nCol, var.IntVal(0), ref varResult);

					case NFDataList.VARIANT_TYPE.VTYPE_STRING:
						return FindInt(nCol, var.IntVal(0), ref varResult);

					case NFDataList.VARIANT_TYPE.VTYPE_OBJECT:
						return FindObject(nCol, var.ObjectVal(0), ref varResult);

                    case NFDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                        return FindVector2(nCol, var.Vector2Val(0), ref varResult);

                    case NFDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                        return FindVector3(nCol, var.Vector3Val(0), ref varResult);
                    default:
					break;
				}
			}


            return -1;
        }

        public override int FindInt(int nCol, Int64 value, ref NFDataList varResult)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFDataList valueList = (NFDataList)mhtRecordVec[i];
				if (valueList.IntVal(nCol) == value)
				{
                    varResult.AddInt(i);
                }
            }

            return varResult.Count();
        }

        public override int FindFloat(int nCol, double value, ref NFDataList varResult)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFDataList valueList = (NFDataList)mhtRecordVec[i];
				if (valueList.FloatVal(nCol) == value)
				{
                    varResult.AddInt(i);
                }
            }

            return varResult.Count();
        }

        public override int FindString(int nCol, string value, ref NFDataList varResult)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFDataList valueList = (NFDataList)mhtRecordVec[i];
				if (valueList.StringVal(nCol) == value)
				{
                    varResult.AddInt(i);
                }
            }

            return varResult.Count();
        }

        public override int FindObject(int nCol, NFGUID value, ref NFDataList varResult)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFDataList valueList = (NFDataList)mhtRecordVec[i];
				if (valueList.ObjectVal(nCol) == value)
				{
                    varResult.AddInt(i);
                }
			}

            return varResult.Count();
        }

        public override int FindVector2(int nCol, NFVector2 value, ref NFDataList varResult)
        {
            foreach (int i in mhtRecordVec.Keys)
            {
                NFDataList valueList = (NFDataList)mhtRecordVec[i];
                if (valueList.Vector2Val(nCol) == value)
                {
                    varResult.AddInt(i);
                }
            }

            return varResult.Count();
        }

        public override int FindVector3(int nCol, NFVector3 value, ref NFDataList varResult)
        {
            foreach (int i in mhtRecordVec.Keys)
            {
                NFDataList valueList = (NFDataList)mhtRecordVec[i];
                if (valueList.Vector3Val(nCol) == value)
                {
                    varResult.AddInt(i);
                }
            }

            return varResult.Count();
        }
		public override int FindInt(int nCol, Int64 value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindInt (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

		public override int FindFloat(int nCol, double value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindFloat (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

		public override int FindString(int nCol, string value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindString (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

		public override int FindObject(int nCol, NFGUID value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindObject (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

		public override int FindVector2(int nCol, NFVector2 value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindVector2 (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

		public override int FindVector3(int nCol, NFVector3 value)
		{
			NFDataList varResult = new NFDataList ();
			int nCount = FindVector3 (nCol, value, ref varResult);
			if (nCount > 0 && varResult.Count() > 0)
			{
				return (int)varResult.IntVal (0);
			}

			return -1;
		}

        public override bool Remove(int nRow)
        {
			if (mhtRecordVec.Contains(nRow))
            {
				if (null != doHandleDel)
                {
                    doHandleDel(mSelf, mstrRecordName, eRecordOptype.Del, nRow, 0, NFDataList.NULL_TDATA, NFDataList.NULL_TDATA);
                }
				mhtRecordVec.Remove(nRow);
				return true;
            }

            return false;
        }

        public override bool Clear()
        {
            for (int i = 0; i < mhtUseState.Count; ++i )
            {
                if (IsUsed(i))
                {
                    Remove(i);

                    mhtUseState[i] = 0;
                }
            }

			mhtRecordVec.Clear();

            return true;
        }

		public override void RegisterCallback(RecordEventHandler handler)
		{
			doHandleDel += handler;
		}

        public override string GetName()
        {
            return mstrRecordName;
        }

        public override void SetName(string strName)
        {
            mstrRecordName = strName;
        }

		RecordEventHandler doHandleDel;

		NFDataList mVarRecordType;
        Hashtable mhtRecordVec = new Hashtable();
        Dictionary<int, int> mhtUseState = new Dictionary<int, int>();
		NFDataList mxTag;

		NFGUID mSelf;
		string mstrRecordName;
		int mnRow;
        bool mbUpload;
    }
}