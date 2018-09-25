test_pb_msg_module = {}
register_module(test_pb_msg_module,"test_pb_msg_module");

function test_pb_msg_module:reload()
end

function test_pb_msg_module:awake()
end

function test_pb_msg_module:init()
end



function test_pb_msg_module:after_init()
	io.write("test_pb_msg_module after_init!----\n");
	print("-------------------------------------------------\n");
	--script_module:add_msg_cb(1212, test);
	print("-------------------------------------------------\n");
end

function test_pb_msg_module:ready_execute()
end

function test_pb_msg_module:before_shut()
end

function test_pb_msg_module:shut()
end

function test()
end

--------------------------------------------------------------------------------



