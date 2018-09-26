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
	print(self.class_common_event);
	for k,v in pairs(self) do
		if v == self.class_common_event then
		print(k);
		print(v);
		end
	end
	script_module:add_class_cb("Player", self, self.class_common_event);
	--script_module:add_class_cb("Player", test_game_module.class_common_event);

	print("Hello game script_module");

	local app_id = script_module:app_id();
	local app_type = script_module:app_type();

	
end

function test_game_module:after_init()
	print("test_game_module after_init!----");

	local playerObject = script_module:create_object(NFGUID(), 1, 0, "Player", "", NFDataList());

	print("test_game_module after_init!11111----");
	print("create_object" .. playerObject:tostring());

	--property callback
	script_module:add_prop_cb(playerObject, "MAXHP", "test_game_module.max_prop_cb");
	script_module:set_prop_int(playerObject,"MAXHP",100);

	--record callback
	script_module:add_record_cb(playerObject, "TaskList", "test_game_module.task_list_cb");

	local varTask =  NFDataList();
	varTask:add_string("Task_From_Lua");
	varTask:add_int(1);
	varTask:add_int(1);

	script_module:add_row(playerObject, "TaskList", varTask);
	--script_module:set_record_int(playerObject, "TaskList", 0, 1, 3);
	--script_module:set_record_string(playerObject, "TaskList", 0, 0, "NewStr_Task_From_Lua");

	--event callback
	script_module:add_event_cb(playerObject, 1, "test_game_module.event_cb");

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
	script_module:add_schedule(playerObject, "add_game_schedule", "test_game_module.schedule", 5, 55555);
	script_module:add_module_schedule("add_game_module_schedule", "test_game_module.module_schedule", 10, 55555);
end

function test_game_module:ready_execute()
	io.write("test_game_module ready_execute!\n");
end

function test_game_module:before_shut()
	io.write("test_game_module before_shut!\n");
end

function test_game_module:shut()
	io.write("test_game_module shut!\n");
end


function test_game_module:max_prop_cb(self, propertyName, oldVar, newVar)
	local oldVar = oldVar:int();
	local newVar = newVar:int();

	io.write("Hello Lua max_prop_cb oldVar:" .. tostring(oldVar) .. " newVar:" .. tostring(newVar) .. "\n");
end

function test_game_module:task_list_cb(self, recordName, nOpType, nRow, nCol, oldVar, newVar)
	io.write("Hello Lua task_list_cb ")
	if nOpType == RecordOptype.Add then
		io.write(" nOpType:".. tostring(nOpType) .. "\n");
	end

	if nOpType == RecordOptype.Update then
		if nCol == 0 then
			local nOldVar = oldVar:string();
			local nNewVar = newVar:string();

			io.write(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "\n");
		end
		if nCol == 1 then
		local nOldVar = oldVar:int();
		local nNewVar = newVar:int();

		io.write(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "\n");
		end
	end

end

function test_game_module:event_cb(self, nEventID, arg)
	local nValue = arg:int(0);
	local fValue = arg:float(1);
	local strValue = arg:string(2);

	local ident = arg:object(3);
	local head = ident.head;
	local data = ident.data;

	io.write("Hello Lua EventCallBack nEventID:".. nEventID .. "\n");
	io.write("\r\targ:nValue:".. tostring(nValue) .. " fValue:"..tostring(fValue).. " strValue:"..tostring(strValue).." ident:".. ident:tostring() .. "\n");
end

function test_game_module:schedule(self, strHeartBeat, fTime, nCount)
	local obj = NFDataList();
	--local s = os.clock()
	local s = script_module:time();
	if oldTime == nil then
		oldTime = s
	end
	io.write("Hello Lua HeartCallBack5555555:".. strHeartBeat .. " Time:" .. (s-oldTime) .. "\n");
	oldTime = s;
end

function test_game_module:module_schedule(strHeartBeat, fTime, nCount)
	io.write("Hello Lua Module HeartCallBack55555:".. strHeartBeat .. " Time:" .. fTime .. "\n");
	
end

function test_game_module:class_common_event(self, strClassName, eventID, varData)
	io.write("class_common_event, ClassName: " .. tostring(strClassName) .. " EventID: " .. tostring(eventID) .. "\n");
end
