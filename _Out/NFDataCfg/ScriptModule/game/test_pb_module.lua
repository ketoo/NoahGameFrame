test_pb_module = {}

function test_pb_module:reload()
end

function test_pb_module:awake()
end

function test_pb_module:init()
	script_module:import_proto_file("NFMsgShare.proto");
	script_module:import_proto_file("NFMsgPreGame.proto");
	script_module:import_proto_file("NFMsgExtra.proto");
end



function test_pb_module:after_init()
	print("test_pb_module after_init!----");
	--test1();
	--test2();
	--test3();
	
	
end

function test_pb_module:ready_execute()
end

function test_pb_module:before_shut()
end

function test_pb_module:shut()
end

--------------------------------------------------------------------------------
function test1()
	local tbl = {
		property_name = "NFMsg.PropertyInt",
		data = 1,
	};
	
	local data = script_module:encode("NFMsg.PropertyInt", tbl);
	local test_tbl =  script_module:decode("NFMsg.PropertyInt", data);
	--print_table(test_tbl);
	
	assert(tbl.property_name == test_tbl.property_name, "property_name not equal "..tbl.property_name);
	assert(tbl.data == test_tbl.data, "property_data not equal "..tbl.data);
end

function test2()

	local tbl_data = {
		x = 1;
		y = 2;
		z = 3;
	};
	
	
	local tbl = {
		property_name = "NFMsg.PropertyVector3";
		data = tbl_data;
	};
	
	print_table(tbl);
	
	local data = script_module:encode("NFMsg.PropertyVector3", tbl);
	
	print(">>>>>>>>>");
	
	local test_tbl =  script_module:decode("NFMsg.PropertyVector3", data);
	
	print_table(test_tbl);
	
	assert(tbl.property_name == test_tbl.property_name, "property_name not equal "..tbl.property_name);
	--assert(tbl.data.x == test_tbl.data.x, "property_data not equal "..tbl.data);
end


function test3()

end



