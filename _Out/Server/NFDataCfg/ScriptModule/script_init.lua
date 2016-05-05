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
	for key, value in pairs(name) do
		if type(value) == "table" then
			local tblObject = nil;
			for k, v in pairs(value) do
				if package.loaded[v] then

				else
					local object = require(v);
					if nil == object then
						io.write("load_script_file " .. v .. " failed\n");
					else
						io.write("load_script_file " .. v .. " successed\n");
					end
				end
			end
		end
	end
end

function reload_script_file( name )
  if package.loaded[name] then
	package.loaded[name] = nil
  end
  
  load_script_file( name )
end

function reload_script_table( name )
	io.write("----Begin reload lua list----\n");
	
	for key, value in pairs(name) do
		if type(value) == "table" then
			local tblObject = nil;
			for k, v in pairs(value) do
				reload_script_file(tostring(value))
			end
		end
	end
	
	io.write("----End reload lua list----\n");
end

function register_module(tbl, name)
	if ScriptList then
		for key, value in pairs(ScriptList) do
			local tblObject = nil;
			if type(value) == "table" then
				for k, v in pairs(value) do
					if v == name then
						tblObject = tbl;
					end
				end
			end

			if tblObject ~= nil then
				io.write("----register_module successed----\n");
				ScriptList[key].tbl = tblObject;
			end
		end
	end

	if ScriptReloadList then
		for key, value in pairs(ScriptReloadList) do
			local tblObject = nil;
			if type(value) == "table" then
				for k, v in pairs(value) do
					if v == name then
						tblObject = tbl;
					end
				end
			end

			if tblObject ~= nil then
				io.write("----register_module successed----\n");
				ScriptReloadList[key].tbl = tblObject;
			end
		end
	end
end