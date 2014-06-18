using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public class NFIDENTID
    {
        public Int64 nData64;
//         public Int64 nIdent;
//         public Int64 nSerial;


        public NFIDENTID()
        {
            nData64 = 0;
        }

        public NFIDENTID(Int64 nData)
        {
            nData64 = nData;
        }

// 		public static operator = (NFIDENTID ident, Int64 nValue)
// 		{
// 			ident.nData64 = nValue;
// 		}

		public static bool operator == (NFIDENTID ident, NFIDENTID other)
		{
			return other.nData64 == ident.nData64;
		}

		public static bool operator != (NFIDENTID ident, NFIDENTID other)
		{
			return other.nData64 != ident.nData64;
		}

		public static bool operator < (NFIDENTID ident, NFIDENTID other)
		{
			return other.nData64 < ident.nData64;
		}

		public static bool operator > (NFIDENTID ident, NFIDENTID other)
		{
			return other.nData64 > ident.nData64;
		}

        public bool IsNull()
        {
            return 0 == nData64;
        }
    }
}
