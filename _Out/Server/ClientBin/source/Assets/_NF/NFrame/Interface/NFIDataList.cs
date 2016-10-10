//-----------------------------------------------------------------------
// <copyright file="NFIDataList.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
    public abstract class NFIDataList
    {
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
                if (nType == VARIANT_TYPE.VTYPE_INT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(double value)
            {
                if (nType == VARIANT_TYPE.VTYPE_FLOAT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(string value)
            {
                if (nType == VARIANT_TYPE.VTYPE_STRING)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFGUID value)
            {
                if (nType == VARIANT_TYPE.VTYPE_OBJECT)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFVector2 value)
            {
                if (nType == VARIANT_TYPE.VTYPE_VECTOR2)
                {
                    mData = value;
                    return true;
                }

                return false;
            }

            public bool Set(NFVector3 value)
            {
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

                return NFIDataList.NULL_INT;
            }

            public double FloatVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_FLOAT)
                {

                    return (double)mData;
                }

                return NFIDataList.NULL_DOUBLE;
            }

            public string StringVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_STRING)
                {

                    return (string)mData;
                }

                return NFIDataList.NULL_STRING;
            }

            public NFGUID ObjectVal()
            {
                if (nType == VARIANT_TYPE.VTYPE_OBJECT)
                {

                    return (NFGUID)mData;
                }

                return NFIDataList.NULL_OBJECT;
            }

            public NFVector2 Vector2Val()
            {
                if (nType == VARIANT_TYPE.VTYPE_VECTOR2)
                {

                    return (NFVector2)mData;
                }

                return NFIDataList.NULL_VECTOR2;
            }

            public NFVector3 Vector3Val()
            {
                if (nType == VARIANT_TYPE.VTYPE_VECTOR3)
                {

                    return (NFVector3)mData;
                }

                return NFIDataList.NULL_VECTOR3;
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


        public abstract bool AddInt(Int64 value);
        public abstract bool AddFloat(double value);
        public abstract bool AddString(string value);
        public abstract bool AddObject(NFGUID value);
        public abstract bool AddVector2(NFVector2 value);
        public abstract bool AddVector3(NFVector3 value);

        public abstract bool SetInt(int index, Int64 value);
        public abstract bool SetFloat(int index, double value);
        public abstract bool SetString(int index, string value);
        public abstract bool SetObject(int index, NFGUID value);
        public abstract bool SetVector2(int index, NFVector2 value);
        public abstract bool SetVector3(int index, NFVector3 value);


        public abstract Int64 IntVal(int index);
        public abstract double FloatVal(int index);
        public abstract string StringVal(int index);
        public abstract NFGUID ObjectVal(int index);
        public abstract NFVector2 Vector2Val(int index);
        public abstract NFVector3 Vector3Val(int index);

        public abstract int Count();
        public abstract void Clear();
        public abstract VARIANT_TYPE GetType(int index);
        public abstract TData GetData(int index);
    }
}

