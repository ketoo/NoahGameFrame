local version_code = "1.0.0"
MasterScriptReloadList={
	--{tbl=nil, tblName="test_pb_module"},
}

local old_version_code = script_module:get_version_code()
if (old_version_code == "") then
	script_module:set_version_code(version_code);
else
	if (old_version_code ~= version_code) then
		script_module:log_info("start to reload master scripts as old_version_code == " .. old_version_code .. " and now version code = "..version_code);
		
		script_module:set_version_code(version_code);
		load_script_file(MasterScriptReloadList, true);
	end
end

