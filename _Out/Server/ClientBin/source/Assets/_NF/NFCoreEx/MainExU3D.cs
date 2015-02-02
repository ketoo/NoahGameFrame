using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

#if UNITY_EDITOR
//|| UNITY_IPHONE
//|| UNITY_ANDROID
//|| UNITY_STANDALONE_WIN
//|| UNITY_STANDALONE_LINUX

using UnityEngine;

namespace NFCoreEx
{
	
    class MainExU3D : MonoBehaviour 
    {
        NFIKernel xKernel = new NFCKernel();

		void Start ()
		{
			MainU3D();
		}
		
		void Update ()
		{
            xKernel.UpDate(Time.deltaTime);
		}
		
        static void HeartBeatEventHandler(NFIDENTID self, string strHeartBeat, float fTime, NFIValueList valueList)
        {
            Debug.Log(self);
            Debug.Log(" ");
            Debug.Log(strHeartBeat);
            Debug.Log(" ");
            Debug.Log(fTime.ToString());
            Debug.Log(" ");
        }

		static void OnRecordEventHandler(NFIDENTID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIValueList oldVar, NFIValueList newVar)
		{
            Debug.Log(self);
            Debug.Log(" ");
            Debug.Log(strRecordName);
            Debug.Log(" ");
            Debug.Log(eType.ToString());
            Debug.Log(" ");
            Debug.Log(nRow);
            Debug.Log(" ");
            Debug.Log(nCol);
            Debug.Log(" ");
            Debug.Log(oldVar.IntVal(0));
            Debug.Log(" ");
            Debug.Log(newVar.IntVal(0));
            Debug.Log(" ");
		}

		static void OnPropertydHandler(NFIDENTID self, string strProperty, NFIValueList oldVar, NFIValueList newVar)
		{
            Debug.Log(self);
            Debug.Log(" ");
            Debug.Log(strProperty);
            Debug.Log(" ");
            Debug.Log(oldVar.IntVal(0));
            Debug.Log(" ");
            Debug.Log(newVar.IntVal(0));
            Debug.Log(" ");
		}

        static void OnClassHandler(NFIDENTID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
        {
            Debug.Log(self);
            Debug.Log(" ");
            Debug.Log(eType.ToString());
            Debug.Log(" ");
            Debug.Log(strClassName);
            Debug.Log(" ");
            Debug.Log(strConfigIndex);
            Debug.Log(" ");
        }

        public void MainU3D()
        {
			Debug.Log("****************NFIValueList******************");

            NFIValueList var = new NFCValueList();

			for (int i = 0; i < 9; i +=3)
			{
				var.AddInt(i);
				var.AddFloat((float)i+1);
				var.AddString((i+2).ToString());
			}

			for (int i = 0; i < 9; i += 3)
			{
				Int64 n = var.IntVal(i);
				float f = var.FloatVal(i+1);
				string str = var.StringVal(i+2);
				Debug.Log(n);
				Debug.Log(f);
				Debug.Log(str);
			}


			Debug.Log("***************NFProperty*******************");

            NFIDENTID ident = new NFIDENTID(0, 1);
            NFIObject gameObject = xKernel.CreateObject(ident, 0, 0, "", "", new NFCValueList());

			NFIValueList valueProperty = new NFCValueList();
			valueProperty.AddInt(112221);
			gameObject.GetPropertyManager().AddProperty("111", valueProperty);
			Debug.Log(gameObject.QueryPropertyInt("111"));

			Debug.Log("***************NFRecord*******************");

			NFIValueList valueRecord = new NFCValueList();
			valueRecord.AddInt(0);
			valueRecord.AddFloat(0);
			valueRecord.AddString("");
			valueRecord.AddObject(ident);

			gameObject.GetRecordManager().AddRecord("testRecord", 10, valueRecord);

            xKernel.SetRecordInt(ident, "testRecord", 0, 0, 112221);
            xKernel.SetRecordFloat(ident, "testRecord", 0, 1, 1122210.0f);
            xKernel.SetRecordString(ident, "testRecord", 0, 2, ";;;;;;112221");
            xKernel.SetRecordObject(ident, "testRecord", 0, 3, ident);

			Debug.Log(gameObject.QueryRecordInt("testRecord", 0,0));
			Debug.Log(gameObject.QueryRecordFloat("testRecord", 0, 1));
			Debug.Log(gameObject.QueryRecordString("testRecord", 0, 2));
			Debug.Log(gameObject.QueryRecordObject("testRecord", 0, 3));

            Debug.Log(" ");
            Debug.Log("***************PropertyNFEvent*******************");

			//挂属性回调，挂表回调
            xKernel.RegisterPropertyCallback(ident, "111", OnPropertydHandler);
            xKernel.SetPropertyInt(ident, "111", 2456);

            Debug.Log(" ");
            Debug.Log("***************RecordNFEvent*******************");

            xKernel.RegisterRecordCallback(ident, "testRecord", OnRecordEventHandler);
            xKernel.SetRecordInt(ident, "testRecord", 0, 0, 1111111);

            Debug.Log(" ");
            Debug.Log("***************ClassNFEvent*******************");

            xKernel.RegisterClassCallBack("CLASSAAAAA", OnClassHandler);
            xKernel.CreateObject(new NFIDENTID(0, 2), 0, 0, "CLASSAAAAA", "CONFIGINDEX", new NFCValueList());
            xKernel.DestroyObject(new NFIDENTID(0, 2));


            Debug.Log(" ");
			Debug.Log("***************NFHeartBeat*******************");
            xKernel.AddHeartBeat(new NFIDENTID(0, 1), "TestHeartBeat", HeartBeatEventHandler, 5.0f, new NFCValueList());

        }
    }
}
#endif
