using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public class NFIDENTID : Object
    {
        public Int64 nHead64;
        public Int64 nData64;

        public NFIDENTID()
        {
            nHead64 = 0;
            nData64 = 0;
        }

        public NFIDENTID(NFIDENTID id)
        {
            nHead64 = id.nHead64;
            nData64 = id.nData64;
        }

        public NFIDENTID(Int64 nHead, Int64 nData)
        {
            nHead64 = nHead;
            nData64 = nData;
        }

		public static bool operator == (NFIDENTID ident, NFIDENTID other)
		{
            if (((object)ident == null) && ((object)other == null))
            {
                return true;
            }

            if (((object)ident == null) || ((object)other == null))
            {
                return false;
            }

            return ident.nHead64 == other.nHead64 && ident.nData64 == other.nData64;
		}

		public static bool operator != (NFIDENTID ident, NFIDENTID other)
		{
            return !(ident == other);
		}

        public override bool Equals(object other)
        {
            return this == (NFIDENTID)other;
        }

        public bool IsNull()
        {
            return 0 == nData64 && 0 == nHead64;
        }

        public override string ToString()
        {
            return nHead64.ToString() + "-" + nData64.ToString();
        }

        public bool Parse(string strData, out NFIDENTID id)
        {
            NFIDENTID xId = new NFIDENTID();
            id = xId;

            string[] strList = strData.Split('-');
            if (strList.Count() != 2)
            {
                return false;
            }

            Int64 nHead = 0;
            if (!Int64.TryParse(strList[0], out nHead))
            {
                return false;
            }

            Int64 nData = 0;
            if (!Int64.TryParse(strList[1], out nData))
            {
                return false;
            }

            id.nHead64 = nHead;
            id.nData64 = nData;

            return true;
        }

        public override int GetHashCode()
        {
            string str =this.ToString();
            return str.GetHashCode();
        }
    }
}