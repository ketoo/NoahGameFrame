test_object_prop_module = {}

function test_object_prop_module:reload()
end

function test_object_prop_module:awake()
	script_module:log_info("test_object_prop_module awake!----");
	test_object_prop_module:reload()
end

function test_object_prop_module:init()
	script_module:log_info("test_object_prop_module init!----");

	script_module:add_class_cb("Player", test_object_prop_module, test_object_prop_module.class_common_event);
end

function test_object_prop_module:after_init()
	script_module:log_info("test_object_prop_module after_init!----");
end

function test_object_prop_module:ready_execute()
end

function test_object_prop_module:before_shut()
end

function test_object_prop_module:shut()
end

function test_object_prop_module:class_common_event(object_id, className, eventID, varData)
	print(object_id:tostring() .. ", classname:" .. className .. ", eventID:" .. eventID .. ", varData:" .. tostring(varData))

	if eventID == ObjectEvent.COE_CREATE_READY then
		script_module:add_prop_cb(object_id, "MAXHP", test_object_prop_module, test_object_prop_module.max_hp_prop_cb);
		script_module:add_schedule(object_id, "player_prop_schedule", test_object_prop_module, test_object_prop_module.schedule, 2, 55555)
	end
end


function test_object_prop_module:max_hp_prop_cb(id, propertyName, oldVar, newVar)
	script_module:log_info(id:tostring() .. ", propertyName:" .. propertyName);
	print(tostring(oldVar) .. "," .. tostring(newVar))
	local oldValue = oldVar:int();
	local newValue = newVar:int();

	script_module:log_info("Hello Lua max_prop_cb oldVar:" .. tostring(oldValue) .. " newVar:" .. tostring(newValue));
	
end


function test_object_prop_module:schedule(id, heartBeat, time, count)
	script_module:log_info(id:tostring() .. ", schedule:" .. heartBeat .. ", count:" .. count);

	script_module:set_prop_int(id,"MAXHP",count);
end