local version_code = "1.0.9"
local force_load = true

GameScriptReloadList={
	--["test_object_module"] = true,
	{tbl=nil, tblName="test_object_module"},
	{tbl=nil, tblName="test_object_prop_module"},
	{tbl=nil, tblName="test_object_record_module"},
	--{tbl=nil, tblName="test_pb_module"},
	--{tbl=nil, tblName="test_pb_msg_module"},
	--{tbl=nil, tblName="test_net_module"},
	--{tbl=nil, tblName="test_net_msg_module"},
	{tbl=nil, tblName="test_http_module"},
	{tbl=nil, tblName="test_event_module"},

	{tbl=nil, tblName="scenario_cfg"},
	{tbl=nil, tblName="scenario_module"},
	{tbl=nil, tblName="scenario100"},
	{tbl=nil, tblName="scenario200"},
	{tbl=nil, tblName="scenario300"},
	{tbl=nil, tblName="scenario400"},

}


local old_version_code = script_module:get_version_code()
if (old_version_code == "") then
	script_module:set_version_code(version_code);
else
	local need_load = false
	if true == force_load then
		need_load = true
	else
		if (old_version_code ~= version_code) then
			need_load = true
		end
	end

	if true == need_load then
		script_module:log_info("start to reload game scripts as old_version_code == " .. old_version_code .. " and now version code = "..version_code);

		script_module:set_version_code(version_code);
		load_script_file(GameScriptReloadList, true);
	end
end

