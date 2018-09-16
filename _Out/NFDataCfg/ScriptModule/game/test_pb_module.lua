test_pb_module = {}
register_module(test_pb_module,"test_pb_module");

local other_module = nil;
function test_pb_module.reload()
end

function test_pb_module.awake()
end

function test_pb_module.init()

	
end

function test_pb_module.after_init()
	io.write("test_pb_module after_init!----\n");
	io.write("-------------------------------------------------\n");
	script_module:import_proto_file("NFMsgShare.proto");
	
	local tbl = {
	property_name = "test_pb_property";
	data = 1;
	};
	
	local data = script_module:encode("NFMsg.PropertyInt", tbl);
	io.write(data .. "\n\n");
	
	local test_tbl =  script_module:decode("NFMsg.PropertyInt", data);
	PrintTable(test_tbl);
	
	io.write("-------------------------------------------------\n");
end

function test_pb_module.ready_execute()
end

function test_pb_module.before_shut()
end

function test_pb_module.shut()
end
