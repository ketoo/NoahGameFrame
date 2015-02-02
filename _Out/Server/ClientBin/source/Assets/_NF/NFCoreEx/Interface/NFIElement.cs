using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	public abstract class NFIElement
	{
        public abstract NFIPropertyManager GetPropertyManager();

        public abstract Int64 QueryInt(string strName);
        public abstract float QueryFloat(string strName);
        public abstract double QueryDouble(string strName);
        public abstract string QueryString(string strName);
        public abstract NFIDENTID QueryObject(string strName);

	}
}
