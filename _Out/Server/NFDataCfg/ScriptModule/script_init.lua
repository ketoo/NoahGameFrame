package.path = '../../NFDataCfg/Script/?.lua;'


pPluginManager = nil;
function init_script_system(xPluginManager)
	pPluginManager = xPluginManager;
	
	io.write("\nHello Lua " .. tostring(pPluginManager) .. "\n\n");
	io.write();
	
end

function load_script_file(name)
	if package.loaded[name] then
		return 1;
	else
		local object = require(name);
		if nil == object then
			io.write("load_script_file " .. name .. " failed\n");		
			return 0;
		else
			io.write("load_script_file " .. name .. " successed\n");			
			return 1;
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
		reload_script_file(tostring(key))
	end
	
	io.write("----End reload lua list----\n");
	name = nil;
end


function Init(...)
	Test:Init(...)
end

function AfterInit(...)
	Test:AfterInit(...)
end

function Execute(...)
	Test:Execute(...)
end

function BeforeShut(...)
	Test:BeforeShut(...)

end

function Shut(...)
	Test:Shut(...)
end


function MaxPropertyCallBack(...)
	Test:MaxPropertyCallBack(...)
end

function TaskListCallBack(...)
	Test:TaskListCallBack(...)
end

function EventCallBack(...)
	Test:EventCallBack(...)

end

function HearCallBack(...)
	Test:HearCallBack(...)
end



