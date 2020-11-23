test_event_module = {}

function test_event_module:reload()
end

function test_event_module:awake()
    test_event_module:reload()
end

function test_event_module:init()
    script_module:add_class_cb("Player", test_event_module, test_event_module.class_common_event);
end

function test_event_module:after_init()


end

function test_event_module:ready_execute()
end

function test_event_module:before_shut()
end

function test_event_module:shut()
end

function test_event_module:class_common_event(object_id, className, eventID, varData)
    print(object_id:tostring() .. ", classname:" .. className .. ", eventID:" .. eventID .. ", varData:" .. tostring(varData))

    if eventID == ObjectEvent.COE_CREATE_READY then
        script_module:add_event_cb(object_id, 1, test_event_module, test_event_module.event_cb);
    end
    if eventID == ObjectEvent.COE_CREATE_FINISH then
        local dataList = NFDataList();
        dataList:add_int(21);
        dataList:add_float(22.5);
        dataList:add_string("str23");

        local ident = NFGUID();
        ident.head = 241;
        ident.data = 242;
        dataList:add_object(ident);

        script_module:do_event(object_id, 1, dataList);
    end
end

function test_event_module:event_cb(object_id, eventID, arg)
	local nValue = arg:int(0);
	local fValue = arg:float(1);
	local strValue = arg:string(2);

	local ident = arg:object(3);
	local head = ident.head;
	local data = ident.data;

	script_module:log_info("Hello Lua EventCallBack, object_id:" .. object_id:tostring() .. "  eventID:".. eventID .. "");
	script_module:log_info("arg:nValue:".. tostring(nValue) .. " fValue:"..tostring(fValue).. " strValue:"..tostring(strValue).." ident:".. ident:tostring() .. "");
end