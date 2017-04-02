using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
    public abstract class NFILogicClassModule : NFIModule
    {
        public abstract void SetDataPath(string strDataPath);
        public abstract string GetDataPath();
        public abstract bool ExistElement(string strClassName);
        public abstract bool AddElement(string strClassName);

        public abstract NFILogicClass GetElement(string strClassName);
        public abstract Dictionary<string, NFILogicClass> GetElementList();
    }
}