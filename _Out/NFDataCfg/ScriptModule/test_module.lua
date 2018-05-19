test_module = {}
register_module(test_module,"test_module");

function test_module.awake()
end

function test_module.init()
	io.write("test_module init!\n");

	io.write("Addr of pPluginManager " .. tostring(pPluginManager) .. "\n");

	local pKernelModule = pPluginManager:FindKernelModule("NFCKernelModule");
	io.write("Addr of NFCKernelModule " .. tostring(pKernelModule) .. "\n");

	local pLogicClassModule = pPluginManager:FindLogicClassModule("NFCLogicClassModule");
	io.write("Addr of NFCLogicClassModule " .. tostring(pLogicClassModule) .. "\n");

	local pElementInfoModule = pPluginManager:FindElementInfoModule("NFCElementInfoModule");
	io.write("Addr of NFCElementInfoModule " .. tostring(pElementInfoModule) .. "\n");

	pLuaScriptModule:AddClassCallBack("Player", "TestModule.OnClassCommonEvent");

end

function test_module.after_init()
	io.write("test_module after_init!" .. tostring(pLuaScriptModule) .. "\n");
--[[
	local pKernelModule = pPluginManager:FindKernelModule("NFCKernelModule");
	pKernelModule:CreateScene(1);

	local pObject = pKernelModule:CreateObject(NFGUID(), 1, 0, "Player", "", NFCDataList());
	local OID = pObject:Self();

	--property callback
	pLuaScriptModule:AddPropertyCallBack(OID, "MAXHP", "TestModule.MaxPropertyCallBack");
	pKernelModule:SetPropertyInt(OID,"MAXHP",100);

	--record callback
	pLuaScriptModule:AddRecordCallBack(OID, "TaskList", "TestModule.TaskListCallBack");
	local varTask =  NFCDataList();
	varTask:AddString("Task_From_Lua");
	varTask:AddInt(1);
	varTask:AddInt(1);

	pLuaScriptModule:AddRow(OID, "TaskList", varTask);
	pKernelModule:SetRecordInt(OID, "TaskList", 0, 1, 3);
	pKernelModule:SetRecordString(OID, "TaskList", 0, 0, "NewStr_Task_From_Lua");

	--event callback
	pLuaScriptModule:AddEventCallBack(OID, 1, "TestModule.EventCallBack");

	local obj = NFCDataList();
	obj:AddInt(21);
	obj:AddFloat(22.5);
	obj:AddString("str23");

	local ident = NFGUID();
	ident:SetHead(241);
	ident:SetData(242);

	obj:AddObject(ident);

	pKernelModule:DoEvent(OID, 1, obj);

	--Hearback
	pLuaScriptModule:AddHeartBeat(OID, "strHeartBeatName", "test_module.HearCallBack", 2, 55555);
	]]
end

function test_module.ready_execute()
	io.write("test_module execute!\n");
end

function test_module.execute()
	io.write("test_module execute!\n");
end

function test_module.before_shut()
	io.write("test_module before_shut!\n");
end

function test_module.shut()
	io.write("test_module shut!\n");
end


function test_module.MaxPropertyCallBack(self, propertyName, oldVar, newVar)
	local nOldVar = oldVar:GetInt();
	local nNewVar = newVar:GetInt();

	local obj = NFCDataList();
	io.write("Hello Lua MaxPropertyCallBack oldVar:" .. tostring(nOldVar) .. " newVar:" .. tostring(nNewVar) .. "\n");
end

function test_module.TaskListCallBack(self, recordName, nOpType, nRow, nCol, oldVar, newVar)
	io.write("Hello Lua TaskListCallBack ")
	if nCol == 0 then
		local nOldVar = oldVar:GetString();
		local nNewVar = newVar:GetString();

		io.write(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "\n");
	end

	if nCol == 1 then
		local nOldVar = oldVar:GetInt();
		local nNewVar = newVar:GetInt();

		io.write(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "\n");
	end

end

function test_module.EventCallBack(self, nEventID, arg)
	local nValue = arg:Int(0);
	local fValue = arg:Float(1);
	local strValue = arg:String(2);


	local ident = arg:Object(3);
	local head = ident:GetHead();
	local data = ident:GetData();

	io.write("Hello Lua EventCallBack nEventID:".. nEventID .. "\n");
	io.write("\r\targ:nValue:".. tostring(nValue) .. " fValue:"..tostring(fValue).. " strValue:"..tostring(strValue).." head:"..tostring(head).." data:"..tostring(data).."\n");
end

function test_module.HearCallBack(self, strHeartBeat, fTime, nCount)
	local obj = NFCDataList();
	--local s = os.clock()
	local s = pPluginManager:GetNowTime();
	if oldTime == nil then
		oldTime = s
	end
	io.write("Hello Lua HearCallBack :".. strHeartBeat .. " Time:" .. (s-oldTime) .. "\n");
	oldTime = s;
end

function test_module.OnClassCommonEvent(self, strClassName, eventID, varData)
	io.write("onClassCommonEvent, ClassName: " .. tostring(strClassName) .. " EventID: " .. tostring(eventID) .. "\n");
end

function test_module.OnRecordCommonEvent(self, recordName, nOpType, nRow, nCol, oldVar, newVar)
	io.write("OnRecordCommonEvent, self: " .. tostring(self) .. " nOpType: " .. tostring(nOpType) .. " oldVar: " .. tostring(oldVar) .. " newVar: " .. tostring(newVar) .. "\n");
end

function test_module.OnPropertyCommEvent(self, strPropertyName, oldVar, newVar)
	io.write("OnPropertyCommEvent, self: " .. tostring(self) .. " strPropertyName: " .. tostring(strPropertyName) .. " oldVar: " .. tostring(oldVar) .. " newVar: " .. tostring(newVar) .. "\n");
end
