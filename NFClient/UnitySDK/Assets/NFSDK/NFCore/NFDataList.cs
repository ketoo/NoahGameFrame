//-----------------------------------------------------------------------
// <copyright file="NFDataList.cs">
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
    public class NFDataList
    {
        private Dictionary<int, TData> mValueObject = new Dictionary<int, TData>();

        //==============================================
        public enum VARIANT_TYPE
        {
            VTYPE_UNKNOWN,  // 未知
            VTYPE_INT,              // 64
            VTYPE_FLOAT,            // 单精度浮点数
            VTYPE_STRING,       // 字符串
            VTYPE_OBJECT,       // 对象ID
            VTYPE_VECTOR2,
            VTYPE_VECTOR3,
            VTYPE_MAX,
        };

        public class TData
        {
            public TData(TData x)
            {
                nType = x.nType;
                mData = x.mData;
            }

            public TData()
            {
                mData = new Object();
                nType = VARIANT_TYPE.VTYPE_UNKNOWN;
            }

            public TData(VARIANT_TYPE eType)
            {
                mData = new Object();
                nType = eType;
            }

            public VARIANT_TYPE GetType()
            {
                return nType;
            }

            public bool Set(Int64 value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_INT;
                }

                if (nType == VARIANT_TYPE.VTYPE_INT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(double value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_FLOAT;
                }

                if (nType == VARIANT_TYPE.VTYPE_FLOAT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(string value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_STRING;
                }

                if (nType == VARIANT_TYPE.VTYPE_STRING)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFGUID value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_OBJECT;
                }

                if (nType == VARIANT_TYPE.VTYPE_OBJECT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFVector2 value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_VECTOR2;
                }

                if (nType == VARIANT_TYPE.VTYPE_VECTOR2)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFVector3 value)
            {
                if (nType == VARIANT_TYPE.VTYPE_UNKNOWN)
                {
                    nType = VARIANT_TYPE.VTYPE_VECTOR3;
                }

                if (nType == VARIANT_TYPE.VTYPE_VECTOR3)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public Int64 IntVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_INT)
                {

                    return (Int64)mData;
                }

                return NFDataList.NULL_INT;
            }

            public double FloatVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_FLOAT)
                {

                    return (double)mData;
                }

                return NFDataList.NULL_DOUBLE;
            }

            public string StringVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_STRING)
                {

                    return (string)mData;
                }

                return NFDataList.NULL_STRING;
            }

            public NFGUID ObjectVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_OBJECT)
                {

                    return (NFGUID)mData;
                }

                return NFDataList.NULL_OBJECT;
            }

            public NFVector2 Vector2Val()
            {
                if (nType == VARIANT_TYPE.VTYPE_VECTOR2)
                {

                    return (NFVector2)mData;
                }

                return NFDataList.NULL_VECTOR2;
            }

            public NFVector3 Vector3Val()
            {
                if (nType == VARIANT_TYPE.VTYPE_VECTOR3)
                {

                    return (NFVector3)mData;
                }

                return NFDataList.NULL_VECTOR3;
            }

            public string ToString()
            {
                switch (nType)
                {
                    case VARIANT_TYPE.VTYPE_INT:
                        return IntVal().ToString();
                        break;
                    case VARIANT_TYPE.VTYPE_FLOAT:
                        return FloatVal().ToString();
                        break;
                    case VARIANT_TYPE.VTYPE_OBJECT:
                        return ObjectVal().ToString();
                        break;
                    case VARIANT_TYPE.VTYPE_STRING:
                        return StringVal();
                        break;
                    case VARIANT_TYPE.VTYPE_VECTOR2:
                        return Vector2Val().ToString();
                        break;
                    case VARIANT_TYPE.VTYPE_VECTOR3:
                        return Vector3Val().ToString();
                        break;
                    default:
                        break;
                }

                return NFDataList.NULL_STRING;
            }

            private VARIANT_TYPE nType;
            private Object mData;
        }

        public static readonly Int64 NULL_INT = 0;
        public static readonly double NULL_DOUBLE = 0.0;
        public static readonly string NULL_STRING = "";
        public static readonly NFGUID NULL_OBJECT = new NFGUID();
        public static readonly NFVector2 NULL_VECTOR2 = new NFVector2();
        public static readonly NFVector3 NULL_VECTOR3 = new NFVector3();
        public static readonly double EPS_DOUBLE = 0.0001;
        public static readonly TData NULL_TDATA = new TData();

        public NFDataList(string str, char c)
        {
            string[] strSub = str.Split(c);
            foreach (string strDest in strSub)
            {
                AddString(strDest);
            }
        }

        public NFDataList(NFDataList src)
        {
            for (int i = 0; i < src.Count(); i++)
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

        public NFDataList()
        {
        }

        public bool AddInt(Int64 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_INT);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool AddFloat(double value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_FLOAT);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool AddString(string value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_STRING);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool AddObject(NFGUID value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_OBJECT);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool AddVector2(NFVector2 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_VECTOR2);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool AddVector3(NFVector3 value)
        {
            TData data = new TData(VARIANT_TYPE.VTYPE_VECTOR3);
            data.Set(value);

            return AddDataObject(ref data);
        }

        public bool SetInt(int index, Int64 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_INT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public bool SetFloat(int index, double value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_FLOAT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public bool SetString(int index, string value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_STRING)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public bool SetObject(int index, NFGUID value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_OBJECT)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public bool SetVector2(int index, NFVector2 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR2)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public bool SetVector3(int index, NFVector3 value)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                data.Set(value);

                return true;
            }

            return false;
        }

        public Int64 IntVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_INT)
            {
                return data.IntVal();
            }

            return NFDataList.NULL_INT;
        }

        public double FloatVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_FLOAT)
            {
                return (double)data.FloatVal();
            }

            return (float)NFDataList.NULL_DOUBLE;
        }

        public string StringVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_STRING)
            {
                return data.StringVal();
            }

            return NFDataList.NULL_STRING;
        }

        public NFGUID ObjectVal(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_OBJECT)
            {
                return data.ObjectVal();
            }

            return NFDataList.NULL_OBJECT;
        }

        public NFVector2 Vector2Val(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                return data.Vector2Val();
            }

            return NFDataList.NULL_VECTOR2;
        }

        public NFVector3 Vector3Val(int index)
        {
            TData data = GetData(index);
            if (data != null && data.GetType() == VARIANT_TYPE.VTYPE_VECTOR3)
            {
                return data.Vector3Val();
            }

            return NFDataList.NULL_VECTOR3;
        }

        public int Count()
        {
            return mValueObject.Count;
        }

        public void Clear()
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

        public VARIANT_TYPE GetType(int index)
        {
            if (mValueObject.Count > index)
            {
                TData data = (TData)mValueObject[index];

                return data.GetType();
            }

            return VARIANT_TYPE.VTYPE_UNKNOWN;
        }

        public TData GetData(int index)
        {
            if (mValueObject.ContainsKey(index))
            {
                return (TData)mValueObject[index];
            }

            return null;
        }
    }
}
