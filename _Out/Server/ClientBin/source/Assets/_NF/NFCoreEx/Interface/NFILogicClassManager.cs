using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
    public abstract class NFILogicClassManager
    {
        public abstract bool Clear();

        public abstract bool ExistElement(string strClassName);

        public abstract bool AddElement(string strClassName);

        public abstract NFILogicClass GetElement(string strClassName);

        public abstract Hashtable GetElementList();

    }
}