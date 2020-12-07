test_net_msg_module = {}

function test_net_msg_module:reload()
end

function test_net_msg_module:awake()
	test_net_msg_module:reload()
end

function test_net_msg_module:init()
end



function test_net_msg_module:after_init()
	script_module:log_info("test_net_msg_module after_init!----");
	--script_module:add_msg_cb(1212, test);
end

function test_net_msg_module:ready_execute()
end

function test_net_msg_module:before_shut()
end

function test_net_msg_module:shut()
end
--------------------------------------------------------------------------------



