using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public class NFCRecord : NFIRecord
    {

		public NFCRecord(NFIDENTID self, string strRecordName, int nRow, NFIValueList varData)
		{
			mSelf = self;
			mnRow = nRow;
			mstrRecordName = strRecordName;
			mVarRecordType = varData;

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

        public override NFIValueList.VARIANT_TYPE GetColType(int nCol)
        {
			return mVarRecordType.GetType(nCol);
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

        public override int AddRow(int nRow, NFIValueList var)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					mhtRecordVec[nRow] = new NFCValueList(var);
					return nRow;
				}
			}
			
			
            return -1;
        }

        // set data
        public override int SetValue(int nRow, NFIValueList var)
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
				NFIValueList valueList = (NFIValueList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFIValueList.VARIANT_TYPE.VTYPE_INT)
				{
					if (valueList.IntVal(nCol) != value)
					{
						NFCValueList oldValue = new NFCValueList();
						oldValue.AddInt(valueList.IntVal(nCol));
	
						valueList.SetInt(nCol, value);
	
						NFCValueList newValue = new NFCValueList();
						newValue.AddInt(valueList.IntVal(nCol));
	                   
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Updata, nRow, nCol, oldValue, newValue);
	                    }
	                }
				}
				return true;
				
			}
            return false;
        }

        public override bool SetFloat(int nRow, int nCol, float value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFIValueList valueList = (NFIValueList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFIValueList.VARIANT_TYPE.VTYPE_FLOAT)
				{
					if (valueList.FloatVal(nCol) - value > 0.01f
						|| valueList.FloatVal(nCol) - value < -0.01f)
					{
						NFCValueList oldValue = new NFCValueList();
						oldValue.AddFloat(valueList.FloatVal(nCol));
	
						valueList.SetFloat(nCol, value);
	
						NFCValueList newValue = new NFCValueList();
						newValue.AddFloat(valueList.FloatVal(nCol));
	
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Updata, nRow, nCol, oldValue, newValue);
	                    }
	                }
	
				}
	
				return true;
			}
			return false;
        }

        public override bool SetDouble(int nRow, int nCol, double value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				
				NFIValueList valueList = (NFIValueList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFIValueList.VARIANT_TYPE.VTYPE_DOUBLE)
				{
					if (valueList.DoubleVal(nCol) - value > 0.01f
						|| valueList.DoubleVal(nCol) - value < -0.01f)
					{
						NFCValueList oldValue = new NFCValueList();
						oldValue.AddDouble(valueList.DoubleVal(nCol));
	
						valueList.SetDouble(nCol, value);
	
						NFCValueList newValue = new NFCValueList();
						newValue.AddDouble(valueList.DoubleVal(nCol));
	
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Updata, nRow, nCol, oldValue, newValue);
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
				NFIValueList valueList = (NFIValueList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFIValueList.VARIANT_TYPE.VTYPE_STRING)
				{
					if (valueList.StringVal(nCol) != value)
					{
						NFCValueList oldValue = new NFCValueList();
						oldValue.AddString(valueList.StringVal(nCol));
	
						valueList.SetString(nCol, value);
	
						NFCValueList newValue = new NFCValueList();
						newValue.AddString(valueList.StringVal(nCol));
	
	                    if (null != doHandleDel)
	                    {
	                        doHandleDel(mSelf, mstrRecordName, eRecordOptype.Updata, nRow, nCol, oldValue, newValue);
	                    }
	                }
				}
	
				return true;
			}

			return false;
        }

        public override bool SetObject(int nRow, int nCol, NFIDENTID value)
        {
			if(nRow >= 0 && nRow < mnRow)
			{
				if (!mhtRecordVec.ContainsKey(nRow))
				{
					AddRow(nRow);
				}
				NFIValueList valueList = (NFIValueList)mhtRecordVec[nRow];
				if (valueList.GetType(nCol) == NFIValueList.VARIANT_TYPE.VTYPE_OBJECT)
				{
					if (valueList.ObjectVal(nCol) != value)
					{
						NFCValueList oldValue = new NFCValueList();
						oldValue.AddObject(valueList.ObjectVal(nCol));

						valueList.SetObject(nCol, value);

						NFCValueList newValue = new NFCValueList();
						newValue.AddObject(valueList.ObjectVal(nCol));

                        if (null != doHandleDel)
                        {
                            doHandleDel(mSelf, mstrRecordName, eRecordOptype.Updata, nRow, nCol, oldValue, newValue);
                        }
                    }
				}

				return true;
			}

			return false;
        }

        // query data
        public override NFIValueList QueryRow(int nRow)
        {
			if (mhtRecordVec.ContainsKey(nRow))
			{
				return  (NFIValueList)mhtRecordVec[nRow];
			}

            return null;
        }

        public override bool SwapRow(int nOriginRow, int nTargetRow)
        {
			if(nOriginRow >= 0 && nOriginRow < mnRow && nTargetRow >= 0 && nTargetRow < mnRow)
			{
	            NFIValueList valueOriginList = null;
	            NFIValueList valueTargetList = null;
	           
	            if (mhtRecordVec.ContainsKey(nOriginRow))
	            {
	                valueOriginList = (NFIValueList)mhtRecordVec[nOriginRow];
	            }
	            if (mhtRecordVec.ContainsKey(nTargetRow))
	            {
	                valueTargetList = (NFIValueList)mhtRecordVec[nOriginRow];
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
	                 doHandleDel(mSelf, mstrRecordName, eRecordOptype.Changed, nOriginRow, nTargetRow, new NFCValueList(), new NFCValueList());
	             }
	            return true;
			}
			return false;
        }

        public override Int64 QueryInt(int nRow, int nCol)
        {
			NFIValueList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.IntVal(nCol);
			}

			return 0;
        }

        public override float QueryFloat(int nRow, int nCol)
        {
			NFIValueList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.FloatVal(nCol);
			}

            return 0.0f;
        }

        public override double QueryDouble(int nRow, int nCol)
        {
			NFIValueList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.DoubleVal(nCol);
			}

            return 0.0;
        }

        public override string QueryString(int nRow, int nCol)
        {
			NFIValueList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.StringVal(nCol);
			}

            return "";
        }

        public override NFIDENTID QueryObject(int nRow, int nCol)
        {
			NFIValueList valueList = QueryRow(nRow);
			if (null != valueList)
			{
				return valueList.ObjectVal(nCol);
			}

            return new NFIDENTID();
        }

        //public override int FindRow( int nRow );
        public override int FindColValue(int nCol, NFIValueList var)
        {
			for (int i = 0; i < mhtRecordVec.Count; i++ )
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				switch (valueList.GetType(0))
				{
					case NFIValueList.VARIANT_TYPE.VTYPE_INT:
						return FindInt(nCol, var.IntVal(0));

					case NFIValueList.VARIANT_TYPE.VTYPE_FLOAT:
						return FindInt(nCol, var.IntVal(0));

					case NFIValueList.VARIANT_TYPE.VTYPE_DOUBLE:
						return FindInt(nCol, var.IntVal(0));

					case NFIValueList.VARIANT_TYPE.VTYPE_STRING:
						return FindInt(nCol, var.IntVal(0));

					case NFIValueList.VARIANT_TYPE.VTYPE_OBJECT:
						return FindObject(nCol, var.ObjectVal(0));

					default:
					break;
				}
			}


            return -1;
        }

        public override int FindInt(int nCol, Int64 value)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				if (valueList.IntVal(nCol) == value)
				{
					return i;
				}
			}
            return -1;
        }

        public override int FindFloat(int nCol, float value)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				if (valueList.FloatVal(nCol) == value)
				{
					return i;
				}
			}
            return -1;
        }

        public override int FindDouble(int nCol, double value)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				if (valueList.DoubleVal(nCol) == value)
				{
					return i;
				}
			}
            return -1;
        }

        public override int FindString(int nCol, string value)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				if (valueList.StringVal(nCol) == value)
				{
					return i;
				}
			}

            return -1;
        }

        public override int FindObject(int nCol, NFIDENTID value)
        {
			foreach (int i in mhtRecordVec.Keys)
			{
				NFIValueList valueList = (NFIValueList)mhtRecordVec[i];
				if (valueList.ObjectVal(nCol) == value)
				{
					return i;
				}
			}

            return -1;
        }

        public override bool Remove(int nRow)
        {
			if (mhtRecordVec.Contains(nRow))
            {
				if (null != doHandleDel)
                {
                    doHandleDel(mSelf, mstrRecordName, eRecordOptype.Del, nRow, 0, (NFIValueList)mhtRecordVec[nRow], (NFIValueList)mhtRecordVec[nRow]);
                }
				mhtRecordVec.Remove(nRow);
				return true;
            }

            return false;
        }

        public override bool Clear()
        {
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

		NFIValueList mVarRecordType;
		Hashtable mhtRecordVec = new Hashtable();

		NFIDENTID mSelf;
		string mstrRecordName;
		int mnRow;
    }
}