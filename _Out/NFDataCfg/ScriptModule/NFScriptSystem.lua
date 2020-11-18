package.path = '../NFDataCfg/ScriptModule/?.lua;../NFDataCfg/ScriptModule/game/?.lua;../NFDataCfg/ScriptModule/world/?.lua;../NFDataCfg/ScriptModule/proxy/?.lua;../NFDataCfg/ScriptModule/master/?.lua;../NFDataCfg/ScriptModule/login/?.lua;'

require("NFScriptEnum");

script_module = nil;
function init_script_system(xLuaScriptModule)
	script_module = xLuaScriptModule;

	script_module:log_info("Hello Lua, init script_module");

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

function load_script_file(fileList)
	for i=1, #(fileList) do
		if package.loaded[fileList[i].tblName] then
			package.loaded[fileList[i].tblName] = nil
		end

		script_module:log_info("start to load " .. fileList[i].tblName);

		local oldTbl =_G[fileList[i].tblName];
		local object = require(fileList[i].tblName);
		if true == object then
			local newTbl =_G[fileList[i].tblName];
			register_module(newTbl, fileList[i].tblName);
			if oldTbl ~= nil then
				script_module:log_info("reload_script_file " .. fileList[i].tblName .. " successed");
			end
		else
			script_module:log_info("load_script_file " .. fileList[i].tblName .. " failed");
		end
	end
end

--[[
if you write code under the rule of NF, then you don't need these functions listed below,
but if you write code with free style and still want hot fix feature then you need these functions listed below.

function reload_script_file( tblName )
	local old_module = _G[tblName]
    package.loaded[tblName] = nil
	local object = require(tblName);
	if nil == object then
		print("  reload_script_file " .. tblName .. " failed\n");
	else
		print("  reload_script_file " .. tblName .. " successed\n");
	end
	
    local new_module = _G[tblName]
    for k, v in pairs(new_module) do
        old_module[k] = v
    end
end

function reload_script_table( nameList )
	io.write("----Begin reload lua list----\n");

	local ret = 0;
	for i=1, #(nameList) do
		ret = 1;
		print("reload script : " .. tostring(nameList[i].tblName) .. "\n");
		reload_script_file(nameList[i].tblName)
	end

	io.write("----End reload lua list----\n");

	if ret == 1 then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl.reload();
		end
	end
end
--]]

function register_module(tbl, name)
	script_module:log_info("try to register module " .. name);
	script_module:register_module(name, tbl);
	if ScriptList then
		for i=1, #(ScriptList) do
			if ScriptList[i].tblName == name then
				ScriptList[i].tbl = tbl;
				script_module:log_info("register module " .. name .. " successed");
			end
		end
		for i=1, #(ScriptList) do
			if  ScriptList[i].tbl ~= nil then
				ScriptList[i].tbl.reload();
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
	print("lua module awake");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:awake(...);
		end
	end
end


function module_init(...)
	print("lua module init");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:init(...);
		end
	end
end

function module_after_init(...)
	print("lua module after init");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:after_init(...);
		end
	end
end

function module_ready_execute(...)
	print("lua module execute");
	if ScriptList then
		for i=1, #(ScriptList) do
		ScriptList[i].tbl:ready_execute(...);
		end
	end
end

function module_before_shut(...)
	print("lua module before shut");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:before_shut(...);
		end
	end
end

function module_shut(...)
	print("lua module shut");
	if ScriptList then
		for i=1, #(ScriptList) do
			ScriptList[i].tbl:shut(...);
		end
	end
end

function print_table(table)
	if table == nil then
		print("the table is nil");
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
