using UnityEngine;
using System.Collections;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System;
using NFCoreEx;
using NFTCPClient;
using System.Collections.Generic;

public class NFCRenderInterface
{
    #region Instance
    private static NFCRenderInterface _Instance = null;
    private static readonly object _syncLock = new object();
    public static NFCRenderInterface Instance
    {
        get
        {
            lock (_syncLock)
            {
                if (_Instance == null)
                {
                    _Instance = new NFCRenderInterface();
                    _Instance.Init();
                }
                return _Instance;
            }
        }
    }
    #endregion

    public void Init()
    {
        NFCKernel.Instance.RegisterClassCallBack("Player", OnClassPlayerEventHandler);
        NFCKernel.Instance.RegisterClassCallBack("NPC", OnClassNPCEventHandler);
    }

    private void OnClassPlayerEventHandler(NFIDENTID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        {
            string strConfigID = NFCKernel.Instance.QueryPropertyString(self, "ConfigID");
            Vector3 vec = new Vector3();
            vec.x = NFCKernel.Instance.QueryPropertyFloat(self, "X");
            vec.y = NFCKernel.Instance.QueryPropertyFloat(self, "Y");
            vec.z = NFCKernel.Instance.QueryPropertyFloat(self, "Z");

            string strPrefabPath = "";
            if (strConfigID.Length <= 0)
            {
                strPrefabPath = NFCElementManager.Instance.QueryPropertyString("Player", "Prefab");
            }
            else
            {
                strPrefabPath = NFCElementManager.Instance.QueryPropertyString(strConfigID, "Prefab");
            }

            //CreateObject(self, strPrefabPath, vec, strClassName);
        }
        else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        {
            //DestroyObject(self);
        }
        
    }

    private void OnClassNPCEventHandler(NFIDENTID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        {
            string strConfigID = NFCKernel.Instance.QueryPropertyString(self, "ConfigID");
            Vector3 vec = new Vector3();
            vec.x = NFCKernel.Instance.QueryPropertyFloat(self, "X");
            vec.y = NFCKernel.Instance.QueryPropertyFloat(self, "Y");
            vec.z = NFCKernel.Instance.QueryPropertyFloat(self, "Z");

            string strPrefabPath = "";
            if (strConfigID.Length <= 0)
            {
                strPrefabPath = NFCElementManager.Instance.QueryPropertyString("Player", "Prefab");
            }
            else
            {
                strPrefabPath = NFCElementManager.Instance.QueryPropertyString(strConfigID, "Prefab");
            }

            //CreateObject(self, strPrefabPath, vec, strClassName);
        }
        else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        {
            //DestroyObject(self);
        }
        
    }
    public enum RecordBuildingList
    {
        BuildingID = 0,
        BuildingGUID = 1,
    }

    /////////////////////////////////////////////////////////////////
    private void OnBuildRecordEventHandler(NFIDENTID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIValueList oldVar, NFIValueList newVar)
    {
 
    }

   
    ///////////////////////////////////////////////////////////////////////////////////////
    Dictionary<NFIDENTID, GameObject> mhtObject = new Dictionary<NFIDENTID, GameObject>();
}