using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFCoreEx
{
	public class NFCKernel : NFIKernel
	{
// 		#region Instance
// 		private static NFIKernel _Instance = null;
// 		public static NFIKernel Instance
// 		{
// 			get
// 			{
// 				if (_Instance == null)
// 				{
// 					_Instance = new NFCKernel();
// 				}
// 				return _Instance;
// 			}
// 		}
// 		#endregion

		public NFCKernel()
		{
			mhtObject = new Hashtable();
			mhtClassHandleDel = new Hashtable();
		}

		~NFCKernel()
		{
			mhtObject = null;
		}

		public override bool AddHeartBeat(NFIDENTID self, string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler, float fTime, NFIValueList valueList)
		{
            NFIObject xGameObject = GetObject(self);
            if (null != xGameObject)
            {
                xGameObject.GetHeartBeatManager().AddHeartBeat(strHeartBeatName, fTime, valueList);
                xGameObject.GetHeartBeatManager().RegisterCallback(strHeartBeatName, handler);
            }
			return true;
		}

		public override void RegisterPropertyCallback(NFIDENTID self, string strPropertyName, NFIProperty.PropertyEventHandler handler)
		{
			NFIObject xGameObject = GetObject(self);
			if (null != xGameObject)
			{
				xGameObject.GetPropertyManager().RegisterCallback(strPropertyName, handler);
			}
		}

		public override void RegisterRecordCallback(NFIDENTID self, string strRecordName, NFIRecord.RecordEventHandler handler)
		{
			NFIObject xGameObject = GetObject(self);
			if (null != xGameObject)
			{
				xGameObject.GetRecordManager().RegisterCallback(strRecordName, handler);
			}
		}

		public override void RegisterClassCallBack(string strClassName, NFIObject.ClassEventHandler handler)
		{
			if(mhtClassHandleDel.ContainsKey(strClassName))
			{
				ClassHandleDel xHandleDel = (ClassHandleDel)mhtClassHandleDel[strClassName];
				xHandleDel.AddDel(handler);
				
			}
			else
			{
				ClassHandleDel xHandleDel = new ClassHandleDel();
				xHandleDel.AddDel(handler);
				mhtClassHandleDel[strClassName] = xHandleDel;
			}
		}

		public override void RegisterHeartBeatCallBack(NFIDENTID self, string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler)
		{
			NFIObject xGameObject = GetObject(self);
			if (null != xGameObject)
			{
				xGameObject.GetHeartBeatManager().RegisterCallback(strHeartBeatName, handler);
			}
		}

		public override void RegisterEventCallBack(NFIDENTID self, int nEventID, NFIEvent.EventHandler handler, NFIValueList valueList)
		{
			NFIObject xGameObject = GetObject(self);
			if (null != xGameObject)
			{
				xGameObject.GetEventManager().RegisterCallback(nEventID, handler, valueList);
			}
		}

		public override bool FindHeartBeat(NFIDENTID self, string strHeartBeatName)
		{
			NFIObject xGameObject = GetObject(self);
			if (null != xGameObject)
			{
				//gameObject.GetHeartBeatManager().AddHeartBeat()
			}

			return false;
		}

		public override bool RemoveHeartBeat(NFIDENTID self, string strHeartBeatName)
		{
			return true;
		}

        public override bool UpDate(float fTime)
        {
            foreach (System.Collections.DictionaryEntry gameObject in mhtObject)
            {
                NFIObject xGameObject = (NFIObject)gameObject.Value;
                xGameObject.GetHeartBeatManager().Update(fTime);
            }

            return true;
        }
		/////////////////////////////////////////////////////////////

		//public override bool AddRecordCallBack( NFIDENTID self, string strRecordName, RECORD_EVENT_FUNC cb );

		//public override bool AddPropertyCallBack( NFIDENTID self, string strCriticalName, PROPERTY_EVENT_FUNC cb );

		//     public override bool AddClassCallBack( string strClassName, CLASS_EVENT_FUNC cb );
		//
		//     public override bool RemoveClassCallBack( string strClassName, CLASS_EVENT_FUNC cb );

		/////////////////////////////////////////////////////////////////

		public override NFIObject GetObject(NFIDENTID ident)
		{
			if (mhtObject.ContainsKey(ident.nData64))
			{
				return (NFIObject)mhtObject[ident.nData64];
			}

			return null;
		}

		public override NFIObject CreateObject(NFIDENTID self, int nContainerID, int nGroupID, string strClassName, string strConfigIndex, NFIValueList arg)
		{
			if (!mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xNewObject = new NFCObject(self, nContainerID, nGroupID, strClassName, strConfigIndex);
				mhtObject.Add(self.nData64, xNewObject);

                NFCValueList varConfigID = new NFCValueList();
                varConfigID.AddString(strConfigIndex);
                xNewObject.GetPropertyManager().AddProperty("ConfigID", varConfigID);

                NFCValueList varConfigClass = new NFCValueList();
                varConfigClass.AddString(strClassName);
                xNewObject.GetPropertyManager().AddProperty("ClassName", varConfigClass);


				ClassHandleDel xHandleDel = (ClassHandleDel)mhtClassHandleDel[strClassName];
                if (null != xHandleDel && null != xHandleDel.GetHandler())
                {
					NFIObject.ClassEventHandler xHandlerList = xHandleDel.GetHandler();
                    xHandlerList(self, nContainerID, nGroupID, NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE, strClassName, strConfigIndex);
					xHandlerList(self, nContainerID, nGroupID, NFIObject.CLASS_EVENT_TYPE.OBJECT_LOADDATA, strClassName, strConfigIndex);
					xHandlerList(self, nContainerID, nGroupID, NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE_FINISH, strClassName, strConfigIndex);
				}
				return xNewObject;
			}

			return null;
		}


		public override bool DestroyObject(NFIDENTID self)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				string strClassName = xGameObject.ClassName();
				ClassHandleDel xHandleDel = (ClassHandleDel)mhtClassHandleDel[strClassName];
                if (null != xHandleDel && null != xHandleDel.GetHandler())
                {
					NFIObject.ClassEventHandler xHandlerList = xHandleDel.GetHandler();
                    xHandlerList(self, xGameObject.ContainerID(), xGameObject.GroupID(), NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY, xGameObject.ClassName(), xGameObject.ConfigIndex());
                }
				mhtObject.Remove(self.nData64);
				
				return true;
			}

			return false;
		}

		public override bool FindProperty(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.FindProperty(strPropertyName);
			}

			return false;
		}

		public override bool SetPropertyInt(NFIDENTID self, string strPropertyName, int nValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetPropertyInt(strPropertyName, nValue);
			}

			return false;
		}

		public override bool SetPropertyFloat(NFIDENTID self, string strPropertyName, float fValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetPropertyFloat(strPropertyName, fValue);
			}

			return false;
		}

		public override bool SetPropertyDouble(NFIDENTID self, string strPropertyName, double dValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetPropertyDouble(strPropertyName, dValue);
			}

			return false;
		}

		public override bool SetPropertyString(NFIDENTID self, string strPropertyName, string strValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetPropertyString(strPropertyName, strValue);
			}

			return false;
		}

		public override bool SetPropertyObject(NFIDENTID self, string strPropertyName, NFIDENTID objectValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetPropertyObject(strPropertyName, objectValue);
			}

			return false;
		}


		public override int QueryPropertyInt(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryPropertyInt(strPropertyName);
			}

			return 0;
		}

		public override float QueryPropertyFloat(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryPropertyFloat(strPropertyName);
			}

			return 0.0f;
		}

		public override double QueryPropertyDouble(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryPropertyDouble(strPropertyName);
			}

			return 0.0;
		}

		public override string QueryPropertyString(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryPropertyString(strPropertyName);
			}

			return "";
		}

		public override NFIDENTID QueryPropertyObject(NFIDENTID self, string strPropertyName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryPropertyObject(strPropertyName);
			}

			return new NFIDENTID();
		}


		public override NFIRecord FindRecord(NFIDENTID self, string strRecordName)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.GetRecordManager().GetRecord(strRecordName);
			}

			return null;
		}


		public override bool SetRecordInt(NFIDENTID self, string strRecordName, int nRow, int nCol, int nValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetRecordInt(strRecordName, nRow, nCol, nValue);
			}

			return false;
		}

		public override bool SetRecordFloat(NFIDENTID self, string strRecordName, int nRow, int nCol, float fValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetRecordFloat(strRecordName, nRow, nCol, fValue);
			}

			return false;
		}

		public override bool SetRecordDouble(NFIDENTID self, string strRecordName, int nRow, int nCol, double dwValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetRecordDouble(strRecordName, nRow, nCol, dwValue);
			}

			return false;
		}

		public override bool SetRecordString(NFIDENTID self, string strRecordName, int nRow, int nCol, string strValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetRecordString(strRecordName, nRow, nCol, strValue);
			}

			return false;
		}

		public override bool SetRecordObject(NFIDENTID self, string strRecordName, int nRow, int nCol, NFIDENTID objectValue)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.SetRecordObject(strRecordName, nRow, nCol, objectValue);
			}

			return false;
		}


		public override int QueryRecordInt(NFIDENTID self, string strRecordName, int nRow, int nCol)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryRecordInt(strRecordName, nRow, nCol);
			}

			return 0;
		}

		public override float QueryRecordFloat(NFIDENTID self, string strRecordName, int nRow, int nCol)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryRecordFloat(strRecordName, nRow, nCol);
			}

			return 0.0f;
		}

		public override double QueryRecordDouble(NFIDENTID self, string strRecordName, int nRow, int nCol)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryRecordDouble(strRecordName, nRow, nCol);
			}

			return 0.0;
		}

		public override string QueryRecordString(NFIDENTID self, string strRecordName, int nRow, int nCol)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryRecordString(strRecordName, nRow, nCol);
			}

			return "";
		}

		public override NFIDENTID QueryRecordObject(NFIDENTID self, string strRecordName, int nRow, int nCol)
		{
			if (mhtObject.ContainsKey(self.nData64))
			{
				NFIObject xGameObject = (NFIObject)mhtObject[self.nData64];
				return xGameObject.QueryRecordObject(strRecordName, nRow, nCol);
			}

			return new NFIDENTID();
		}
		
		public override NFIValueList GetObjectList()
		{
			NFIValueList varData = new NFCValueList();
			foreach( DictionaryEntry de in mhtObject) 
			{
				Int64 ident = (Int64)de.Key;
				varData.AddObject(new NFIDENTID(ident));				
			}
			
			return varData;
		}
		
		Hashtable mhtObject;
		Hashtable mhtClassHandleDel;
		
		class ClassHandleDel
		{
			public ClassHandleDel()
			{
				mhtHandleDelList = new Hashtable();
			}
			
			public void AddDel(NFIObject.ClassEventHandler handler)
			{
				if (!mhtHandleDelList.ContainsKey(handler))
				{
					mhtHandleDelList.Add(handler, handler.ToString());
					mHandleDel += handler;
				}
			}
			
			public NFIObject.ClassEventHandler GetHandler()
			{
				return mHandleDel;
			}
			
			private NFIObject.ClassEventHandler mHandleDel;
			Hashtable mhtHandleDelList;
		}
        
	}
}