test_game_module = {}

local other_module = nil;
function test_game_module:reload()
	--other_module = script_module:find_module("other_module");
end

function test_game_module:awake()
	test_game_module.reload();
end

function test_game_module:init()
	print("test_game_module init!----");
	
	script_module:add_class_cb("Player", test_game_module, test_game_module.class_common_event);
end

function test_game_module:after_init()
	print("test_game_module after_init!----");

	local playerObject = script_module:create_object(NFGUID(), 1, 0, "Player", "", NFDataList());

	--property callback
	script_module:add_prop_cb(playerObject, "MAXHP", self, self.max_prop_cb);
	script_module:set_prop_int(playerObject,"MAXHP",100);

	--record callback
	script_module:add_record_cb(playerObject, "TaskList", self, self.task_list_cb);

	local varTask =  NFDataList();
	varTask:add_string("Task_From_Lua");
	varTask:add_int(1);
	varTask:add_int(1);

	script_module:add_row(playerObject, "TaskList", varTask);
	--script_module:set_record_int(playerObject, "TaskList", 0, 1, 3);
	--script_module:set_record_string(playerObject, "TaskList", 0, 0, "NewStr_Task_From_Lua");

	--event callback
	script_module:add_event_cb(playerObject, 1, self, self.event_cb);

	local dataList = NFDataList();
	dataList:add_int(21);
	dataList:add_float(22.5);
	dataList:add_string("str23");

	local ident = NFGUID();
	ident.head = 241;
	ident.data = 242;

	dataList:add_object(ident);

	script_module:do_event(playerObject, 1, dataList);

	--Hearback
	script_module:add_schedule(playerObject, "add_game_schedule", self, self.schedule, 5, 55555);

	script_module:add_module_schedule("add_game_module_schedule", self, self.module_schedule, 10, 55555);
end

function test_game_module:ready_execute()
	print("test_game_module ready_execute!");
end

function test_game_module:before_shut()
	print("test_game_module before_shut!");
end

function test_game_module:shut()
	print("test_game_module shut!");
end


function test_game_module:max_prop_cb(id, propertyName, oldVar, newVar)


	local oldValue = oldVar:int();
	local newValue = newVar:int();
	
	print("Hello Lua max_prop_cb oldVar:" .. tostring(oldValue) .. " newVar:" .. tostring(newValue));
	
end

function test_game_module:task_list_cb(id, recordName, nOpType, nRow, nCol, oldVar, newVar)
	print("Hello Lua task_list_cb ")
	if nOpType == RecordOptype.Add then
		print(" nOpType:".. tostring(nOpType) .. "");
	end

	if nOpType == RecordOptype.Update then
		if nCol == 0 then
			local nOldVar = oldVar:string();
			local nNewVar = newVar:string();

			print(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "");
		end
		if nCol == 1 then
		local nOldVar = oldVar:int();
		local nNewVar = newVar:int();

		print(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "");
		end
	end

end

function test_game_module:event_cb(id, nEventID, arg)
	local nValue = arg:int(0);
	local fValue = arg:float(1);
	local strValue = arg:string(2);

	local ident = arg:object(3);
	local head = ident.head;
	local data = ident.data;

	print("Hello Lua EventCallBack nEventID:".. nEventID .. "");
	print("\r\targ:nValue:".. tostring(nValue) .. " fValue:"..tostring(fValue).. " strValue:"..tostring(strValue).." ident:".. ident:tostring() .. "");
end

function test_game_module:schedule(id, strHeartBeat, fTime, nCount)
	local obj = NFDataList();
	--local s = os.clock()
	local s = script_module:time();
	if oldTime == nil then
		oldTime = s
	end
	print("Hello Lua HeartCallBac666666:".. strHeartBeat .. " Time:" .. (s-oldTime) .. "");
	oldTime = s;
end

function test_game_module:module_schedule(strHeartBeat, fTime, nCount)
	print("Hello Lua Module HeartCallBack666:".. strHeartBeat .. " Time:" .. fTime .. "");
	
end

function test_game_module:class_common_event(id, strClassName, eventID, varData)

	print("class_common_event, ClassName: " .. tostring(strClassName) .. " EventID: " .. tostring(eventID) .. "");
end
