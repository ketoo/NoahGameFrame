test_object_record_module = {}

function test_object_record_module:reload()
end

function test_object_record_module:awake()
	script_module:log_info("test_object_record_module awake!----");
	test_object_record_module:reload()
end

function test_object_record_module:init()
	script_module:log_info("test_object_record_module init!----");

	script_module:add_class_cb("Player", test_object_record_module, test_object_record_module.class_common_event);
end

function test_object_record_module:after_init()
	script_module:log_info("test_object_record_module after_init!----");
end

function test_object_record_module:ready_execute()
end

function test_object_record_module:before_shut()
end

function test_object_record_module:shut()
end

function test_object_record_module:class_common_event(object_id, className, eventID, varData)
	print(object_id:tostring() .. ", classname:" .. className .. ", eventID:" .. eventID .. ", varData:" .. tostring(varData))

	if eventID == ObjectEvent.COE_CREATE_READY then
		script_module:add_schedule(object_id, "player_record_schedule", test_object_record_module, test_object_record_module.schedule, 2, 55555)

		script_module:add_record_cb(object_id, "Inventory", test_object_record_module, test_object_record_module.inventory_list_cb);

	end
end

function test_object_record_module:schedule(id, heartBeat, time, count)
end

function test_object_record_module:inventory_list_cb(id, recordName, nOpType, row, col, oldVar, newVar)

	script_module:log_info(recordName .. ", type:" .. nOpType .. ", row:" .. row .. ", col:" .. col)
	if nOpType == RecordOpType.Add then
	end

	if nOpType == RecordOpType.Update then
		if col == 0 then
			local nOldVar = oldVar:string();
			local nNewVar = newVar:string();

			script_module:log_info(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "");
		end
		if col == 1 then
			local nOldVar = oldVar:int();
			local nNewVar = newVar:int();

			script_module:log_info(" nOpType:".. tostring(nOpType).. " oldVar:".. tostring(nOldVar) .." newVar:" .. tostring(nNewVar) .. "");
		end
	end

end