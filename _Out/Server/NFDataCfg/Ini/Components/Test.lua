Test = {}


function Test:Init(kernel, self)
	io.write("Hello Lua Init!\n");
end

function Test:AfterInit(kernel, self, arg)
	io.write("Hello Lua AfterInit!\n");

	if kernel == nil then
		io.write("kernel is nil");
	end

	if self == nil then
		io.write("self is nil");
	end

	local nMaxhp = GetPropertyInt(kernel, self, "MAXHP");
	io.write("Hello Lua MAXHP:" ..nMaxhp .. "\n");

	--property callback
	AddPropertyCallBack(kernel, self, "MAXHP", "Test", "MaxPropertyCallBack");
	local maxHP = 400;
	SetPropertyInt(kernel, self, "MAXHP", maxHP);

	nMaxhp = GetPropertyInt(kernel, self, "MAXHP");
	io.write("Hello Lua GetPropertyInt MAXHP:" ..nMaxhp .. "\n");

	--record callback
	AddRecordCallBack(kernel, self, "TaskList","Test", "TaskListCallBack");

	local varTask =  NFCVarList();
	varTask:AddString("Task_From_Lua");
	varTask:AddInt(1);

	AddRow(kernel, self, "TaskList", varTask);
	SetRecordInt(kernel, self, "TaskList", 0, 1, 3);

	--event callback
	AddEventCallBack(kernel, self, 1, "Test", "EventCallBack");

	local obj = NFCVarList();
	obj:AddInt(21);
	obj:AddFloat(22.5);
	obj:AddString("str23");

	local ident = NFIDENTID();
	ident:SetHead(241);
	ident:SetData(242);

	obj:AddObject(ident);

	io.write("aaaaaaaaaaaaaaaaaa\n");
	DoEvent(kernel, self, 1, obj);

	--Hearback
	AddHeartBeat(kernel, self, "strHeartBeatName","Test", "HearCallBack", 5, 5);
end

function Test:BeforeShut(kernel, self, arg)
	io.write("Hello Lua BeforeShut\n");
end

function Test:Shut(kernel, self, arg)
	io.write("Hello Lua Shut\n");
end


function Test:MaxPropertyCallBack(kernel, self, propertyName, oldVar, newVar, arg)
	local nOldVar = oldVar:Int(0);
	local nNewVar = newVar:Int(0);

	local obj = NFCVarList();
	io.write("Hello Lua MaxPropertyCallBack oldVar:".. nOldVar .." newVar:" .. nNewVar .. "\n");
end

function Test:TaskListCallBack(kernel, self, recordName, nOpType, nRow, nCol, oldVar, newVar, arg)
	if nCol == 0 then
		local nOldVar = oldVar:String(0);
		local nNewVar = newVar:String(0);

		io.write("Hello Lua TaskListCallBack nOpType:".. nOpType.. " oldVar:".. nOldVar .." newVar:" .. nNewVar .. "\n");
	end

	if nCol == 1 then
		local nOldVar = oldVar:Int(0);
		local nNewVar = newVar:Int(0);

		io.write("Hello Lua TaskListCallBack nOpType:".. nOpType.. " oldVar:".. nOldVar .." newVar:" .. nNewVar .. "\n");
	end

end

function Test:EventCallBack(kernel, self, nEventID, arg)
	local nValue = arg:Int(0);
	local fValue = arg:Float(1);
	local strValue = arg:String(2);


	local ident = arg:Object(3);
	local head = ident:GetHead();
	local data = ident:GetData();

	io.write("Hello Lua EventCallBack nEventID:".. nEventID .. "\n");
	io.write("\r\targ:nValue£º".. nValue .. " fValue:"..fValue.. " strValue:"..strValue.." head:"..head.." data:"..data.."\n");
end

function Test:HearCallBack(kernel, self, strHeartBeat, fTime, nCount, arg)

	local obj = NFCVarList();
	io.write("Hello Lua HearCallBack :".. strHeartBeat .. "\n");
end
