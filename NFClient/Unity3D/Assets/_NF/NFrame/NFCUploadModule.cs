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
            NFCKernelModule.Instance.RegisterClassCallBack("Player", OnClassHandler);
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

        public void OnClassHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
        {
            if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
            {
                NFIObject xObject = NFCKernelModule.Instance.GetObject(self);

                NFIPropertyManager xPropertyManager = xObject.GetPropertyManager();
                NFIDataList xPropertyNameList = xPropertyManager.GetPropertyList();
                for (int i = 0; i < xPropertyNameList.Count(); i++)
                {
                    string strPropertyName = xPropertyNameList.StringVal(i);
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (xProperty.GetUpload())
                    {
                        xProperty.RegisterCallback(OnPropertyHandler);
                    }
                }

                NFIRecordManager xRecordManager = xObject.GetRecordManager();
                NFIDataList xRecordNameList = xRecordManager.GetRecordList();
                for (int i = 0; i < xRecordNameList.Count(); i++)
                {
                    string strRecodeName = xRecordNameList.StringVal(i);
                    NFIRecord xRecord = xRecordManager.GetRecord(strRecodeName);
                    if(xRecord.GetUpload())
                    {
                        xRecord.RegisterCallback(OnRecordHandler);
                    }
                }
            }
        }

        public void OnPropertyHandler(NFGUID self, string strPropertyName, NFIDataList.TData oldVar, NFIDataList.TData newVar)
        {
            NFIObject xObject = NFCKernelModule.Instance.GetObject(self);
            NFIProperty xProperty = xObject.GetPropertyManager().GetProperty(strPropertyName);
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
                case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyVector2(self, strPropertyName, newVar);
                    }
                    break;
                case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                    {
                        NFStart.Instance.GetFocusSender().RequirePropertyVector3(self, strPropertyName, newVar);
                    }
                    break;
            }

        }

        public void OnRecordHandler(NFGUID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIDataList.TData oldVar, NFIDataList.TData newVar)
        {
            NFIObject xObject = NFCKernelModule.Instance.GetObject(self);
            NFIRecord xRecord = xObject.GetRecordManager().GetRecord(strRecordName);
            if (!xRecord.GetUpload())
            {
                return;
            }

            switch(eType)
            {
                case NFIRecord.eRecordOptype.Add:
                    {
                        NFStart.Instance.GetFocusSender().RequireAddRow(self, strRecordName, nRow);
                    }
                    break;
                case NFIRecord.eRecordOptype.Del:
                    {
                        NFStart.Instance.GetFocusSender().RequireRemoveRow(self, strRecordName, nRow);
                    }
                    break;
                case NFIRecord.eRecordOptype.Swap:
                    {
                        NFStart.Instance.GetFocusSender().RequireSwapRow(self, strRecordName, nRow, nCol);
                    }
                    break;
                case NFIRecord.eRecordOptype.Update:
                    {
                        switch(oldVar.GetType())
                        {
                            case NFIDataList.VARIANT_TYPE.VTYPE_INT:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordInt(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                            case NFIDataList.VARIANT_TYPE.VTYPE_FLOAT:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordFloat(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                            case NFIDataList.VARIANT_TYPE.VTYPE_STRING:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordString(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                            case NFIDataList.VARIANT_TYPE.VTYPE_OBJECT:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordObject(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                            case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordVector2(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                            case NFIDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                                {
                                    NFStart.Instance.GetFocusSender().RequireRecordVector3(self, strRecordName, nRow, nCol, newVar);
                                }
                                break;
                        }
                    }
                    break;
            }
        }
    }
}