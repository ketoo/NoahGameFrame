using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public class NFCLogicClassManager : NFILogicClassManager
    {
        public override bool Load()
        {
            return false;
        }

        public override bool Clear()
        {
            return false;
        }

        public override bool ExistElement(string strClassName)
        {
            if (mhtObject.ContainsKey(strClassName))
            {
                return true;
            }

            return false;
        }
        
        public override bool AddElement(string strName)
        {
            if (!mhtObject.ContainsKey(strName))
            {
                NFILogicClass xElement = new NFCLogicClass();
                mhtObject.Add(strName, xElement);

                return true;
            }

            return false;
        }

        public override NFILogicClass GetElement(string strClassName)
        {
            if (mhtObject.ContainsKey(strClassName))
            {
                return (NFILogicClass)mhtObject[strClassName];
            }

            return null;
        }

        /////////////////////////////////////////
        private Hashtable mhtObject;
    }
}