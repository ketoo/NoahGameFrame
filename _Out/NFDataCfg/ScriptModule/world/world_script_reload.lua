local version_code = "1.0.0"
WorldScriptReloadList={
	--{tbl=nil, tblName="test_pb_module"},
}

io.write("start to reload, version: " ..version_code .. "\n");

local old_version_code = script_module:get_version_code()
if (old_version_code == "") then
	script_module:set_version_code(version_code);
else
	if (old_version_code ~= version_code) then
		script_module:log_info("old_version_code == " .. old_version_code .. " ++ now version code = "..version_code);
		
		script_module:set_version_code(version_code);
		load_script_file(WorldScriptReloadList);
	end
end

