package.path = '../NFDataCfg/ScriptModule/?.lua;../NFDataCfg/ScriptModule/json/?.lua;../NFDataCfg/ScriptModule/cfg/?.lua;../NFDataCfg/ScriptModule/game/scenario/?.lua;../NFDataCfg/ScriptModule/game/?.lua;../NFDataCfg/ScriptModule/world/?.lua;../NFDataCfg/ScriptModule/proxy/?.lua;../NFDataCfg/ScriptModule/master/?.lua;../NFDataCfg/ScriptModule/login/?.lua;'

require("NFScriptEnum");

script_module = nil;
function init_script_system(xLuaScriptModule)
	script_module = xLuaScriptModule;

	script_module:log_info("Hello Lua, init script_module, " .. tostring(script_module));

	local app_id = script_module:app_id();
	local app_type = script_module:app_type();

	if NF_SERVER_TYPES.NF_ST_GAME == app_type then
		script_module:log_info("Hello Lua, NF_ST_GAME");
		require("./game/game_script_list");
	elseif NF_SERVER_TYPES.NF_ST_WORLD == app_type then
		script_module:log_info("Hello Lua, NF_ST_WORLD");
		require("./world/world_script_list");
	elseif NF_SERVER_TYPES.NF_ST_PROXY == app_type then
		script_module:log_info("Hello Lua, NF_ST_PROXY");
		require("./proxy/proxy_script_list");
	elseif NF_SERVER_TYPES.NF_ST_LOGIN == app_type then
		script_module:log_info("Hello Lua, NF_ST_LOGIN");
		require("./login/login_script_list");
	elseif NF_SERVER_TYPES.NF_ST_MASTER == app_type then
		script_module:log_info("Hello Lua, NF_ST_MASTER");
		require("./master/master_script_list");
	else
	end

end

function load_script_file(fileList, isReload)

	for i=1, #(fileList) do
		if package.loaded[fileList[i].tblName] then
			package.loaded[fileList[i].tblName] = nil
		end

		script_module:log_info("start to load " .. fileList[i].tblName);

		local oldTbl =_G[fileList[i].tblName];
		local object = require(fileList[i].tblName);
		if true == object then
			local newTbl =_G[fileList[i].tblName];
			register_module(newTbl, fileList[i].tblName, isReload);
			fileList[i].tbl = newTbl

			if oldTbl ~= nil then
				script_module:log_info("reload_script_file " .. fileList[i].tblName .. " succeed");
			end
		else
			script_module:log_info("load_script_file " .. fileList[i].tblName .. " failed");
		end
	end

	if true == isReload then
		for i=1, #(fileList) do
			fileList[i].tbl:awake()
		end
		for i=1, #(fileList) do
			fileList[i].tbl:init()
		end
		for i=1, #(fileList) do
			fileList[i].tbl:after_init()
		end

	end
end

function register_module(tbl, name, isReload)
	script_module:log_info("try to register module " .. name);
	script_module:register_module(name, tbl);
	if ScriptList then
		local isFind = false
		for i=1, #(ScriptList) do
			if ScriptList[i].tblName == name then
				ScriptList[i].tbl = tbl;
				isFind = true
				script_module:log_info("register module " .. name .. " succeed");
			end
		end
		if not isFind then
			table.insert(ScriptList,{tblName = name, tbl = tbl})
		end
		if true == isReload then
			for i=1, #(ScriptList) do
				if  ScriptList[i].tbl ~= nil then
					ScriptList[i].tbl:reload();
				end
			end
		end
	end
end

function find_module(name)
	if ScriptList then
		for i=1, #(ScriptList) do
			if ScriptList[i].tblName == name then
				return ScriptList[i].tbl;
			end
		end
	end
end
---------------------------------------------
function module_awake(...)
	script_module:log_info("lua module awake");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:awake(...);
		end
	end
end


function module_init(...)
	script_module:log_info("lua module init");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:init(...);
		end
	end
end

function module_after_init(...)
	script_module:log_info("lua module after init");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:after_init(...);
		end
	end
end

function module_ready_execute(...)
	script_module:log_info("lua module execute");
	if ScriptList then
		for i=1, #(ScriptList) do
		ScriptList[i].tbl:ready_execute(...);
		end
	end
end

function module_before_shut(...)
	script_module:log_info("lua module before shut");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:before_shut(...);
		end
	end
end

function module_shut(...)
	script_module:log_info("lua module shut");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:shut(...);
		end
	end
end

function print_table(table)
	if table == nil then
		print("the table is nil");
		print(debug.traceback())
		return;
	end
	
	local key = ""
	level =  1
	local indent = ""
	for i = 1, level do
	indent = indent.."  "
	end
	
	if key ~= "" then
	print(indent..key.." ".."=".." ".."{")
	else
	print(indent .. "{")
	end
	
	key = ""
	for k,v in pairs(table) do
	if type(v) == "table" then
		key = k
		print(indent .. key .. " =")
		print_table(v, level + 1)
	else
		local content = string.format("%s%s = %s", indent .. "  ",tostring(k), tostring(v))
		print(content..";")
		end
	end
	print(indent .. "}")

end
----------------------------------------------
