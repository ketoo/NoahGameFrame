using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIValueList
    {
        public enum VARIANT_TYPE
        {
            VTYPE_UNKNOWN,  // 未知
            VTYPE_INT,              // 32位整数
            VTYPE_FLOAT,            // 单精度浮点数
            VTYPE_DOUBLE,       // 双精度浮点数
            VTYPE_STRING,       // 字符串
            VTYPE_OBJECT,       // 对象ID
            VTYPE_MAX,
        };

        public abstract bool AddInt(int value);
        public abstract bool AddFloat(float value);
        public abstract bool AddDouble(double value);
        public abstract bool AddString(string value);
        public abstract bool AddObject(NFIDENTID value);

        public abstract bool SetInt(int index, int value);
        public abstract bool SetFloat(int index, float value);
        public abstract bool SetDouble(int index, double value);
        public abstract bool SetString(int index, string value);
        public abstract bool SetObject(int index, NFIDENTID value);

        public abstract int IntVal(int index);
        public abstract float FloatVal(int index);
        public abstract double DoubleVal(int index);
        public abstract string StringVal(int index);
        public abstract NFIDENTID ObjectVal(int index);

		public abstract int Count();
		public abstract void Clear();
		public abstract VARIANT_TYPE GetType(int index);
    }
}

