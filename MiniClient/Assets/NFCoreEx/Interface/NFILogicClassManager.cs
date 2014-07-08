using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFILogicClassManager
    {
        public abstract bool Load();

        public abstract bool Clear();

        public abstract bool ExistElement(string strClassName);

        public abstract bool AddElement(string strClassName);

        public abstract NFILogicClass GetElement(string strClassName);
    }
}