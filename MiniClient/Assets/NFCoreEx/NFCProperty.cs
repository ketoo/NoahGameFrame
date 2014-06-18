using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public class NFCProperty : NFIProperty
    {
        public NFCProperty( NFIDENTID self, string strPropertyName, NFIValueList varData)
        {
            mSelf = self;
            msPropertyName = strPropertyName;
            mVarProperty = varData;
        }

        public override void SetValue(NFIValueList varData)
        {
            mVarProperty = varData;
        }

        public override NFIValueList GetValue()
        {
            return mVarProperty;
        }

        public override string GetKey()
        {
            return msPropertyName;
        }
		
		public override NFIValueList.VARIANT_TYPE GetType()
		{
			return mVarProperty.GetType(0);
		}
		
        public override int QueryInt()
        {
            return mVarProperty.IntVal(0);
        }

        public override float QueryFloat()
        {
            return mVarProperty.FloatVal(0);
        }

        public override double QueryDouble()
        {
            return mVarProperty.DoubleVal(0);
        }

        public override string QueryString()
        {
            return mVarProperty.StringVal(0);
        }

        public override NFIDENTID QueryObject()
        {
            return mVarProperty.ObjectVal(0);
        }

		public override bool SetInt(int value)
		{
			if (mVarProperty.IntVal(0) != value)
			{
				NFCValueList oldValue = new NFCValueList(mVarProperty);
				
				mVarProperty.SetInt(0, value);

				NFCValueList newValue = new NFCValueList(mVarProperty);

                if (null != doHandleDel)
                {
                    doHandleDel(mSelf, msPropertyName, oldValue, newValue);
                }
				
			}

			return true;
		}

		public override bool SetFloat(float value)
		{
			if (mVarProperty.FloatVal(0) - value > 0.01f
				|| mVarProperty.FloatVal(0) - value < -0.01f)
			{
				NFCValueList oldValue = new NFCValueList(mVarProperty);

				mVarProperty.SetFloat(0, value);

				NFCValueList newValue = new NFCValueList(mVarProperty);

                if (null != doHandleDel)
                {
                    doHandleDel(mSelf, msPropertyName, oldValue, newValue);
                }
			}

			return true;
		}

		public override bool SetDouble(double value)
		{
            if (mVarProperty.DoubleVal(0) - value > 0.01f
                || mVarProperty.DoubleVal(0) - value < -0.01f)
            {
                NFCValueList oldValue = new NFCValueList(mVarProperty);

                mVarProperty.SetDouble(0, value);

                NFCValueList newValue = new NFCValueList(mVarProperty);

                if (null != doHandleDel)
                {
                    doHandleDel(mSelf, msPropertyName, oldValue, newValue);
                }
            }

			return true;
		}

		public override bool SetString(string value)
		{
            if (mVarProperty.StringVal(0) != value)
            {
                NFCValueList oldValue = new NFCValueList(mVarProperty);

                mVarProperty.SetString(0, value);

                NFCValueList newValue = new NFCValueList(mVarProperty);

                if (null != doHandleDel)
                {
                    doHandleDel(mSelf, msPropertyName, oldValue, newValue);
                }
            }

			return true;
		}

		public override bool SetObject(NFIDENTID value)
		{

            if (mVarProperty.ObjectVal(0).nData64 != value.nData64)
            {
                NFCValueList oldValue = new NFCValueList(mVarProperty);

                mVarProperty.SetObject(0, value);

                NFCValueList newValue = new NFCValueList(mVarProperty);

                if (null != doHandleDel)
                {
                    doHandleDel(mSelf, msPropertyName, oldValue, newValue);
                }
            }

			return true;
		}

		public override void RegisterCallback(PropertyEventHandler handler)
		{
			doHandleDel += handler;
		}



		PropertyEventHandler doHandleDel;

		NFIDENTID mSelf;
		string msPropertyName;
		NFIValueList mVarProperty;
    }
}