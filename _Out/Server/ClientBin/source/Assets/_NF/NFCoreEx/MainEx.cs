using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace NFCoreEx
{
	
    class MainEx
    {
        static void HeartBeatEventHandler(NFIDENTID self, string strHeartBeat, float fTime, NFIValueList valueList)
        {
            Console.Write(self);
            Console.Write(" ");
            Console.Write(strHeartBeat);
            Console.Write(" ");
            Console.Write(fTime.ToString());
            Console.WriteLine(" ");
        }

		static void OnRecordEventHandler(NFIDENTID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIValueList oldVar, NFIValueList newVar)
		{
            Console.Write(self);
            Console.Write(" ");
            Console.Write(strRecordName);
            Console.Write(" ");
            Console.Write(eType.ToString());
            Console.Write(" ");
            Console.Write(nRow);
            Console.Write(" ");
            Console.Write(nCol);
            Console.Write(" ");
            Console.Write(oldVar.IntVal(0));
            Console.Write(" ");
            Console.Write(newVar.IntVal(0));
            Console.WriteLine(" ");
		}

		static void OnPropertydHandler(NFIDENTID self, string strProperty, NFIValueList oldVar, NFIValueList newVar)
		{
            Console.Write(self);
            Console.Write(" ");
            Console.Write(strProperty);
            Console.Write(" ");
            Console.Write(oldVar.IntVal(0));
            Console.Write(" ");
            Console.Write(newVar.IntVal(0));
            Console.WriteLine(" ");
		}

        static void OnClassHandler(NFIDENTID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
        {
            Console.Write(self);
            Console.Write(" ");
            Console.Write(eType.ToString());
            Console.Write(" ");
            Console.Write(strClassName);
            Console.Write(" ");
            Console.Write(strConfigIndex);
            Console.WriteLine(" ");
        }

        public static void Main()
        {
            NFIKernel kernel = new NFCKernel();

			Console.WriteLine("****************NFIValueList******************");

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
				Console.WriteLine(n);
				Console.WriteLine(f);
				Console.WriteLine(str);
			}


			Console.WriteLine("***************NFProperty*******************");

            NFIDENTID ident = new NFIDENTID(0, 1);
            NFIObject gameObject = kernel.CreateObject(ident, 0, 0, "", "", new NFCValueList());

			NFIValueList valueProperty = new NFCValueList();
			valueProperty.AddInt(112221);
			gameObject.GetPropertyManager().AddProperty("111", valueProperty);
			Console.WriteLine(gameObject.QueryPropertyInt("111"));

			Console.WriteLine("***************NFRecord*******************");

			NFIValueList valueRecord = new NFCValueList();
			valueRecord.AddInt(0);
			valueRecord.AddFloat(0);
			valueRecord.AddString("");
			valueRecord.AddObject(ident);

			gameObject.GetRecordManager().AddRecord("testRecord", 10, valueRecord);

            kernel.SetRecordInt(ident, "testRecord", 0, 0, 112221);
            kernel.SetRecordFloat(ident, "testRecord", 0, 1, 1122210.0f);
            kernel.SetRecordString(ident, "testRecord", 0, 2, ";;;;;;112221");
            kernel.SetRecordObject(ident, "testRecord", 0, 3, ident);

			Console.WriteLine(gameObject.QueryRecordInt("testRecord", 0,0));
			Console.WriteLine(gameObject.QueryRecordFloat("testRecord", 0, 1));
			Console.WriteLine(gameObject.QueryRecordString("testRecord", 0, 2));
			Console.WriteLine(gameObject.QueryRecordObject("testRecord", 0, 3));

            Console.WriteLine(" ");
            Console.WriteLine("***************PropertyNFEvent*******************");

			//挂属性回调，挂表回调
            kernel.RegisterPropertyCallback(ident, "111", OnPropertydHandler);
            kernel.SetPropertyInt(ident, "111", 2456);

            Console.WriteLine(" ");
            Console.WriteLine("***************RecordNFEvent*******************");

            kernel.RegisterRecordCallback(ident, "testRecord", OnRecordEventHandler);
            kernel.SetRecordInt(ident, "testRecord", 0, 0, 1111111);

            Console.WriteLine(" ");
            Console.WriteLine("***************ClassNFEvent*******************");

            kernel.RegisterClassCallBack("CLASSAAAAA", OnClassHandler);
            kernel.CreateObject(new NFIDENTID(0, 2), 0, 0, "CLASSAAAAA", "CONFIGINDEX", new NFCValueList());
            kernel.DestroyObject(new NFIDENTID(0, 2));


            Console.WriteLine(" ");
			Console.WriteLine("***************NFHeartBeat*******************");
            kernel.AddHeartBeat(new NFIDENTID(0, 1), "TestHeartBeat", HeartBeatEventHandler, 5.0f, new NFCValueList());

            while (true)
            {
                System.Threading.Thread.Sleep(1000);
                kernel.UpDate(1.0f);
            }
        }
    }
}
