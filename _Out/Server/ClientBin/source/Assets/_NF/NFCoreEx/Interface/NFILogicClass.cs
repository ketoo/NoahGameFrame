using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace NFCoreEx
{
    public abstract class NFILogicClass
	{
        public abstract NFIPropertyManager GetPropertyManager();
        public abstract ArrayList GetConfigNameList();
        public abstract bool AddConfigName(string strConfigName);

        public abstract string GetName();
        public abstract void SetName(string strConfigName);

        public abstract string GetPath();
        public abstract void SetPath(string strPath);

        public abstract string GetInstance();
        public abstract void SetInstance(string strInstancePath);
	}
}
