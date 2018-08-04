--package.path = '../../NFDataCfg/Script/?.lua;'

pLuaScriptModule = nil;
pPluginManager = nil;
function init_script_system(xPluginManager,xLuaScriptModule)
	pPluginManager = xPluginManager;
	pLuaScriptModule = xLuaScriptModule;
	
	io.write("\nHello Lua pPluginManager:" .. tostring(pPluginManager) .. " pLuaScriptModule:" .. tostring(pLuaScriptModule) .."\n\n");
	io.write();
	
end

function load_script_file(name)
	for i=1, #(name) do
		if package.loaded[name[i].tblName] then

		else
			local object = require(name[i].tblName);
			if nil == object then
				io.write("load_script_file " .. name[i].tblName .. " failed\n");
			else
				io.write("load_script_file " .. name[i].tblName .. " successed\n");
			end
		end
	end
end

function reload_script_file( name )
	if package.loaded[name] then
		package.loaded[name] = nil
	end
  
	local object = require(name);
	if nil == object then
		io.write("  reload_script_file " .. name .. " failed\n");
	else
		io.write("  reload_script_file " .. name .. " successed\n");
	end
end

function reload_script_table( name )
	io.write("----Begin reload lua list----\n");

	for i=1, #(name) do
		io.write("reload script : " .. tostring(name[i].tblName) .. "\n");
		reload_script_file(name[i].tblName)
	end
	
	io.write("----End reload lua list----\n");
end

function register_module(tbl, name)
	if ScriptList then
		for i=1, #(ScriptList) do
			if ScriptList[i].tblName == name then
				ScriptList[i].tbl = tbl;
			end
		end
	end

	if ScriptReloadList then
		for i=1, #(ScriptReloadList) do
			if ScriptReloadList[i].tblName == name then
				ScriptReloadList[i].tbl = tbl;
			end
		end
	end
end