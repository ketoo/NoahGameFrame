using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections;
using System.IO;
using ProtoBuf;

namespace NFrame
{
    public class NFCUplaoaModule : NFIUploadModule
    {
        public override void Init()
        {
            NFCKernelModule.Instance.RegisterCommonPropertyEvent(OnPropertydHandler);
        }

        public override void AfterInit()
        {
        }

        public override void BeforeShut() 
        {
        }

        public override void Shut()
        {
        }


        public override void Execute()
        {

        }

        static void OnPropertydHandler(NFGUID self, string strPropertyName, NFIDataList.TData oldVar, NFIDataList.TData newVar)
        {
            //judge is/not upload
            NFIObject go = NFCKernelModule.Instance.GetObject(self);
            NFIProperty xProperty = go.GetPropertyManager().GetProperty(strPropertyName);
            if (!xProperty.GetUpload())
            {
                return;
            }

            switch (oldVar.GetType())
            {
                case NFIDataList.VARIANT_TYPE.VTYPE_INT:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyInt(self, strPropertyName, newVar);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_FLOAT:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyFloat(self, strPropertyName, newVar);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_STRING:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyString(self, strPropertyName, newVar);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_OBJECT:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyObject(self, strPropertyName, newVar);
                    }
                    break;
            }

        }
    }
}