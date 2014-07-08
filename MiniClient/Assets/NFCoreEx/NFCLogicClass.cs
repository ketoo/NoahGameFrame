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
    public class NFCLogicClass : NFILogicClass
	{
        public NFCLogicClass()
        {
            mxPropertyManager = new NFCPropertyManager(new NFIDENTID(0));
        }

        public override NFIPropertyManager GetPropertyManager()
        {
            return mxPropertyManager;
        }

        public override ArrayList GetConfigNameList()
        {
            return mxIntansList;
        }

        public override bool AddConfigName(string strConfigName)
        {
            mxIntansList.Add(strConfigName);

            return true;
        }
        /////////////////////////////////////
        private NFIPropertyManager mxPropertyManager;
        private ArrayList mxIntansList = new ArrayList();
	}
}
