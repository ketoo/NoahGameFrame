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

-- GetPropertyInt(kernel, self, strPropertyName)
-- GetPropertyFloat(kernel, self, strPropertyName)
-- GetPropertyString(kernel, self, strPropertyName)
-- GetPropertyObject(kernel, self, strPropertyName)

-- AddRow(kernel, self, strRecordName, var)
-- SetRecordInt(kernel, self, strRecordName, nRow, nCol, nValue)
-- SetRecordFloat(kernel, self, strRecordName, nRow, nCol, fValue)
-- SetRecordString(kernel, self, strRecordName, nRow, nCol, strValue)
-- SetRecordObject(kernel, self, strRecordName, nRow, nCol, n64Value)

-- GetRecordInt(kernel, self, strRecordName, nRow, nCol)
-- GetRecordFloat(kernel, self, strRecordName, nRow, nCol)
-- GetRecordString(kernel, self, strRecordName, nRow, nCol)
-- GetRecordObject(kernel, self, strRecordName, nRow, nCol)

-- HasEventCallBack(kernel, self, nEventID)
-- RemoveEvent(kernel, nEventID)
-- RemoveEventCallBack(kernel, self, nEventID)
-- DoEvent(kernel, self, nEventID, arg)

-- FindHeartBeat(kernel, self, strHeartBeatName)
-- RemoveHeartBeat(kernel, self, strHeartBeatName)


-- ExistElement(kernel, strConfig)
-- GetPropertyInt(kernel, strConfig, strPropertyName)
-- GetPropertyFloat(kernel, strConfig, strPropertyName)
-- GetPropertyString(kernel, strConfig, strPropertyName)

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
-- Var:Int(index)
-- Var:Float(index)
-- Var:String(index)
-- Var:Object(index)

-- local n64Var = NFINT64();
-- n64Var:GetIdent()
-- n64Var:GetnSerial()
-- n64Var:SetIdent(nValue)
-- n64Var:SetnSerial(nValue)
isTest = false
local main={}

local array={"a","b","c","d","e","f"}

main.testFunction = function()
--    cpp_func(1,1)
    
--    for i,k in ipairs(array) do
--    end

    for i=1,10000 do
        if i == 1 then
        else
        end
    end
end

main.main = function()
    print("start time:",os.time())
    for i=1,10000 do
        main.testFunction()
    end
    print("end   time:",os.time())
end

if isTest then
    main.main()
end

package.path = '../../NFDataCfg/Ini/Components/?.lua;'



xPluginManager = nil;
function init_script_system(pPluginManager)
	xPluginManager = pPluginManager;
	
	io.write("\nHello Lua " .. tostring(pPluginManager) .. "\n\n");
	io.write();
	
end

function load_script_file(name)
	if package.loaded[name] then
		return 1;
	else
		local object = require(name);
		if nil == object then
			io.write("load_script_file " .. name .. " failed\n");		
			return 0;
		else
			io.write("load_script_file " .. name .. " successed\n");
			return 1;
		end
		
		object.name.pPluginManager = xPluginManager;
	end

end

function reload_script_file( name )
  io.write("reload_script_file " .. name .."\n");
  
  if package.loaded[name] then
	package.loaded[name] = nil
  end
  
  load_script_file( name )
end

function reload_script_list()

	reload_script_file("script_reload.lua")
	
	io.write("----Begin reload lua list----\n");
	
	for key, value in pairs(script_reload) do      
		reload_script_file(tostring(value))
	end
	
	io.write("----End reload lua list----\n");
end

function Init(...)
	Test:Init(...)
end

function AfterInit(...)
	Test:AfterInit(...)
end

function BeforeShut(...)
	Test:BeforeShut(...)

end

function Shut(...)
	Test:Shut(...)
end


function MaxPropertyCallBack(...)
	Test:MaxPropertyCallBack(...)
end

function TaskListCallBack(...)
	Test:TaskListCallBack(...)
end

function EventCallBack(...)
	Test:EventCallBack(...)

end

function HearCallBack(...)
	Test:HearCallBack(...)
end



