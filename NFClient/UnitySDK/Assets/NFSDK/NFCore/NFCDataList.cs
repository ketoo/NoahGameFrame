using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
    public class NFCDataList : NFIDataList
    {
        private Dictionary<int, TData> mValueObject = new Dictionary<int, TData>();

        //==============================================

        public NFCDataList(string str, char c)
        {
            string[] strSub = str.Split(c);
            foreach (string strDest in strSub)
            {
                AddString(strDest);
            }
        }

        public NFCDataList(NFIDataList src)
        {
			for (int i = 0; i < src.Count(); i++ )
			{
				switch (src.GetType(i))
				{
					case VARIANT_TYPE.VTYPE_INT:
						AddInt(src.IntVal(i));
					break;
					case VARIANT_TYPE.VTYPE_FLOAT:
						AddFloat(src.FloatVal(i));
					break;
					case VARIANT_TYPE.VTYPE_STRING:
						AddString(src.StringVal(i));
					break;
					case VARIANT_TYPE.VTYPE_OBJECT:
						AddObject(src.ObjectVal(i));
					break;
                    case VARIANT_TYPE.VTYPE_VECTOR2:
                        AddVector2(src.Vector2Val(i));
                        break;
                    case VARIANT_TYPE.VTYPE_VECTOR3:
                        AddVector3(src.Vector3Val(i));
                        break;
                    default:
					break;
				}
			}

        }

        public NFCDataList()
        {
        }

        public override bool AddInt(Int64 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_INT);
            data.Set(value);

			return AddDataObject(ref data);
        }

        public override bool AddFloat(double value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_FLOAT);
            data.Set(value);

			return AddDataObject(ref data);
        }

        public override bool AddString(string value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_STRING);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public override bool AddObject(NFGUID value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_OBJECT);
            data.Set(value);

			return AddDataObject(ref data);
        }

        public override bool AddVector2(NFVector2 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_VECTOR2);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public override bool AddVector3(NFVector3 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_VECTOR3);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public override bool SetInt(int index, Int64 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_INT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override bool SetFloat(int index, double value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_FLOAT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override bool SetString(int index, string value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_STRING)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override bool SetObject(int index, NFGUID value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_OBJECT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override bool SetVector2(int index, NFVector2 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR2)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override bool SetVector3(int index, NFVector3 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public override Int64 IntVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_INT)
            {
                return data.IntVal();
            }

            return NFIDataList.NULL_INT;
        }

        public override double FloatVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_FLOAT)
            {
                return (double)data.FloatVal();
            }

            return (float)NFIDataList.NULL_DOUBLE;
        }

        public override string StringVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_STRING)
            {
                return data.StringVal();
            }

            return NFIDataList.NULL_STRING;
        }

        public override NFGUID ObjectVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_OBJECT)
            {
                return data.ObjectVal();
            }

            return NFIDataList.NULL_OBJECT;
        }

        public override NFVector2 Vector2Val(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                return data.Vector2Val();
            }

            return NFIDataList.NULL_VECTOR2;
        }

        public override NFVector3 Vector3Val(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                return data.Vector3Val();
            }

            return NFIDataList.NULL_VECTOR3;
        }

        public override int Count()
		{
			return mValueObject.Count;
		}

		public override void Clear()
		{
			mValueObject.Clear();
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
        protected bool AddDataObject(ref TData data)
        {
            int nCount = mValueObject.Count;
            mValueObject.Add(nCount, data);

            return true;
        }

		public override VARIANT_TYPE GetType(int index)
        {
			if (mValueObject.Count > index)
			{
				TData data = (TData)mValueObject[index];

				return data.GetType();
			}

			return VARIANT_TYPE.VTYPE_UNKNOWN;
        }

        public override TData GetData(int index)
        {
            if (mValueObject.ContainsKey(index))
            {
                return (TData)mValueObject[index];
            }

            return null;
        }
    }
}
