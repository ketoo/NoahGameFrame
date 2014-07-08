using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIElementManager
    {
        public abstract bool Load();
        public abstract bool Clear();

        public abstract bool ExistElement(string strConfigName);
        public abstract bool AddElement(string strName);
        public abstract NFIElement GetElement(string strConfigName);

        public abstract int QueryPropertyInt(string strConfigName, string strPropertyName);
        public abstract float QueryPropertyFloat(string strConfigName, string strPropertyName);
        public abstract double QueryPropertyDouble(string strConfigName, string strPropertyName);
        public abstract string QueryPropertyString(string strConfigName, string strPropertyName);
    }
}