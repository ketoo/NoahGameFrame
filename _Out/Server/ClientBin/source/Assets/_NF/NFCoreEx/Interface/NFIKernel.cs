using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIKernel
    {
		public abstract bool AddHeartBeat(NFIDENTID self, string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler, float fTime, NFIValueList valueList);

        public abstract bool FindHeartBeat(NFIDENTID self, string strHeartBeatName);

        public abstract bool RemoveHeartBeat(NFIDENTID self, string strHeartBeatName);

        public abstract bool UpDate(float fTime);
		
		public abstract NFIValueList GetObjectList();
			
        /////////////////////////////////////////////////////////////
		public abstract void RegisterPropertyCallback(NFIDENTID self, string strPropertyName, NFIProperty.PropertyEventHandler handler);
		
        public abstract void RegisterRecordCallback(NFIDENTID self, string strRecordName, NFIRecord.RecordEventHandler handler);

        public abstract void RegisterClassCallBack(string strClassName, NFIObject.ClassEventHandler handler);

		public abstract void RegisterHeartBeatCallBack(NFIDENTID self, string strHeartBeatName, NFIHeartBeat.HeartBeatEventHandler handler);

		public abstract void RegisterEventCallBack(NFIDENTID self, int nEventID, NFIEvent.EventHandler handler, NFIValueList valueList);
        /////////////////////////////////////////////////////////////////

        public abstract NFIObject GetObject(NFIDENTID ident);

        public abstract NFIObject CreateObject(NFIDENTID self, int nContainerID, int nGroupID, string strClassName, string strConfigIndex, NFIValueList arg);

        public abstract bool DestroyObject(NFIDENTID self);

        public abstract bool FindProperty(NFIDENTID self, string strPropertyName);

        public abstract bool SetPropertyInt(NFIDENTID self, string strPropertyName, Int64 nValue);
        public abstract bool SetPropertyFloat(NFIDENTID self, string strPropertyName, float fValue);
        public abstract bool SetPropertyDouble(NFIDENTID self, string strPropertyName, double dValue);
        public abstract bool SetPropertyString(NFIDENTID self, string strPropertyName, string strValue);
        public abstract bool SetPropertyObject(NFIDENTID self, string strPropertyName, NFIDENTID objectValue);

        public abstract Int64 QueryPropertyInt(NFIDENTID self, string strPropertyName);
        public abstract float QueryPropertyFloat(NFIDENTID self, string strPropertyName);
        public abstract double QueryPropertyDouble(NFIDENTID self, string strPropertyName);
        public abstract string QueryPropertyString(NFIDENTID self, string strPropertyName);
        public abstract NFIDENTID QueryPropertyObject(NFIDENTID self, string strPropertyName);

        public abstract NFIRecord FindRecord(NFIDENTID self, string strRecordName);

        public abstract bool SetRecordInt(NFIDENTID self, string strRecordName, int nRow, int nCol, Int64 nValue);
        public abstract bool SetRecordFloat(NFIDENTID self, string strRecordName, int nRow, int nCol, float fValue);
        public abstract bool SetRecordDouble(NFIDENTID self, string strRecordName, int nRow, int nCol, double dwValue);
        public abstract bool SetRecordString(NFIDENTID self, string strRecordName, int nRow, int nCol, string strValue);
        public abstract bool SetRecordObject(NFIDENTID self, string strRecordName, int nRow, int nCol, NFIDENTID objectValue);

        public abstract Int64 QueryRecordInt(NFIDENTID self, string strRecordName, int nRow, int nCol);
        public abstract float QueryRecordFloat(NFIDENTID self, string strRecordName, int nRow, int nCol);
        public abstract double QueryRecordDouble(NFIDENTID self, string strRecordName, int nRow, int nCol);
        public abstract string QueryRecordString(NFIDENTID self, string strRecordName, int nRow, int nCol);
        public abstract NFIDENTID QueryRecordObject(NFIDENTID self, string strRecordName, int nRow, int nCol);

        public abstract int FindRecordRow(NFIDENTID self, string strRecordName, int nCol, int nValue);
        public abstract int FindRecordRow(NFIDENTID self, string strRecordName, int nCol, float fValue);
        public abstract int FindRecordRow(NFIDENTID self, string strRecordName, int nCol, double fnValue);
        public abstract int FindRecordRow(NFIDENTID self, string strRecordName, int nCol, string strValue);
        public abstract int FindRecordRow(NFIDENTID self, string strRecordName, int nCol, NFIDENTID nValue);
    }
}