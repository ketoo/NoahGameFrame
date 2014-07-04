Test = {}


function Test:Init(kernel, self, arg) 
	io.write("Hello Lua Init!\n");
end	

function Test:AfterInit(kernel, self, arg)
	io.write("Hello Lua AfterInit!\n");
	
	local nMaxhp = QueryPropertyInt(kernel, self, "MAXHP");
	io.write("Hello Lua MAXHP:" ..nMaxhp .. "\n");
	
	AddPropertyCallBack(kernel, self, "MAXHP", "Test", "MaxPropertyCallBack");
	SetPropertyInt(kernel, self, "MAXHP", 400);	
	
	--record callback
	AddRecordCallBack(kernel, self, "TaskList","Test", "TaskListCallBack");	
	
	--event callback
	AddEventCallBack(kernel, self, 1, "Test", "EventCallBack");
	
	local obj = NFCVarList();
	DoEvent(kernel, self, 1, obj);
	
	--event Hearback
	AddHeartBeat(kernel, self, "strHeartBeatName","Test", "HearCallBack", 5, 5);	
	

end	

function Test:BeforeShut(kernel, self, arg)
	io.write("Hello Lua BeforeShut\n");
end	

function Test:Shut(kernel, self, arg)
	io.write("Hello Lua Shut\n");
end

function Test:MaxPropertyCallBack(kernel, self, propertyName, oldVar, newVar, arg)
	local nOldVar = oldVar:IntVal(0);
	local nNewVar = newVar:IntVal(0);

	local obj = NFCVarList();
	io.write("Hello Lua MaxPropertyCallBack oldVar:".. nOldVar .."newVar:" .. nNewVar .. "\n");
end	

function Test:TaskListCallBack(kernel, self, recordName, nOpType, nRow, nCol,oldVar, newVar, arg)
	local nOldVar = oldVar:StringVal(0);
	local nNewVar = newVar:StringVal(0);

	io.write("Hello Lua TaskListCallBack nOpType:".. nOpType.. " oldVar:".. nOldVar .." newVar:" .. nNewVar .. "\n");
end

function Test:EventCallBack(kernel, self, nEventID, arg)

	local obj = NFCVarList();
	io.write("Hello Lua EventCallBack nEventID:".. nEventID .. "\n");
end	

function Test:HearCallBack(kernel, self, strHeartBeat, fTime, nCount, arg)

	local obj = NFCVarList();
	io.write("Hello Lua HearCallBack :".. strHeartBeat .. "\n");
end	




