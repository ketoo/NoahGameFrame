-- --NFLuaAPI:

-- SetComponentEnable(kernel, self, strComponentName, bEnable)
-- QueryComponentEnable(kernel, self, strComponentName)

-- CreateContainer(kernel, nContainerIndex, strSceneConfigID)
-- ExistContainer(kernel, nContainerIndex)

-- CreateObject(kernel, self, nContainerID, nGroupID, strClassName, strConfigIndex, arg)
-- DestroyObject(kernel, self)

-- FindProperty(kernel, self, strPropertyName)
-- AddProperty(kernel, self, strPropertyName, varType, bPublic, bPrivate, bSave, nIndex, strScriptFunction)

-- SetPropertyInt(kernel, self, strPropertyName, nValue)
-- SetPropertyFloat(kernel, self, strPropertyName, fValue)
-- SetPropertyString(kernel, self, strPropertyName, strValue)
-- SetPropertyObject(kernel, self, strPropertyName, n64Value)

-- QueryPropertyInt(kernel, self, strPropertyName)
-- QueryPropertyFloat(kernel, self, strPropertyName)
-- QueryPropertyString(kernel, self, strPropertyName)
-- QueryPropertyObject(kernel, self, strPropertyName)

-- AddRow(kernel, self, strRecordName, var)
-- SetRecordInt(kernel, self, strRecordName, nRow, nCol, nValue)
-- SetRecordFloat(kernel, self, strRecordName, nRow, nCol, fValue)
-- SetRecordString(kernel, self, strRecordName, nRow, nCol, strValue)
-- SetRecordObject(kernel, self, strRecordName, nRow, nCol, n64Value)

-- QueryRecordInt(kernel, self, strRecordName, nRow, nCol)
-- QueryRecordFloat(kernel, self, strRecordName, nRow, nCol)
-- QueryRecordString(kernel, self, strRecordName, nRow, nCol)
-- QueryRecordObject(kernel, self, strRecordName, nRow, nCol)

-- HasEventCallBack(kernel, self, nEventID)
-- RemoveEvent(kernel, nEventID)
-- RemoveEventCallBack(kernel, self, nEventID)
-- DoEvent(kernel, self, nEventID, arg)

-- FindHeartBeat(kernel, self, strHeartBeatName)
-- RemoveHeartBeat(kernel, self, strHeartBeatName)


-- ExistElement(kernel, strConfig)
-- QueryPropertyInt(kernel, strConfig, strPropertyName)
-- QueryPropertyFloat(kernel, strConfig, strPropertyName)
-- QueryPropertyString(kernel, strConfig, strPropertyName)

-- AddPropertyCallBack(kernel, self, strPropertyName, strComponentName, strFunction)
-- -- eg:look at "Test:MaxPropertyCallBack(kernel, self, propertyName, oldVar, newVar, arg)" in Test.lua

-- AddRecordCallBack(kernel, self, strRecordName, strComponentName, strFunction)
-- -- eg:look at "Test:TaskListCallBack(kernel, self, recordName, nOpType, nRow, nCol,oldVar, newVar, arg)" in Test.lua
	
-- AddEventCallBack(kernel, self, nEventID, strComponentName, strFunction)
-- -- eg:look at "Test:EventCallBack(kernel, self, nEventID, arg)" in Test.lua
	
-- AddHeartBeat(kernel, self, strHeartBeatName, strComponentName, strFunction, fTime, nCount)
-- -- eg:look at "Test:HearCallBack(kernel, self, strHeartBeat, fTime, nCount, arg)" in Test.lua

-- -- NFLuaClass

-- local Var = NFCVarList();
-- Var:IsEmpty()
-- Var:GetCount()
-- Var:Type(index)
-- Var:AddInt(nValue)
-- Var:AddFloat(fValue)
-- Var:AddString(strValue)
-- Var:AddObject(n64Value)
-- Var:SetInt(index, nValue)
-- Var:SetFloat(index, fValue)
-- Var:SetString(index, strValue)
-- Var:SetObject(index, n64Value)
-- Var:IntVal(index)
-- Var:FloatVal(index)
-- Var:StringVal(index)
-- Var:ObjectVal(index)

-- local n64Var = NFINT64();
-- n64Var:GetIdent()
-- n64Var:GetnSerial()
-- n64Var:SetIdent(nValue)
-- n64Var:SetnSerial(nValue)

package.path = '../../NFDataCfg/Ini/Components/?.lua;'

function load_script_file(name)
	local object = require(name);
	if nil == object then
		return 0;
	else
		return 1;
	end
end
