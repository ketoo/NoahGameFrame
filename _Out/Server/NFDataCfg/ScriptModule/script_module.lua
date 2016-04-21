ScriptModule = {}
function ScriptModule.Init(...)
	for key, value in pairs(ScriptList) do
		if type(value) == "table" then
			ScriptList[key].tbl.Init(...);	
		end
	end
end

function ScriptModule.AfterInit(...)
	for key, value in pairs(ScriptList) do
		if type(value) == "table" then
			ScriptList[key].tbl.AfterInit(...);	
		end
	end
end

function ScriptModule.Execute(...)
	for key, value in pairs(ScriptList) do
		if type(value) == "table" then
			ScriptList[key].tbl.Execute(...);	
		end
	end
end

function ScriptModule.BeforeShut(...)
	for key, value in pairs(ScriptList) do
		if type(value) == "table" then
			ScriptList[key].tbl.BeforeShut(...);	
		end
	end
end

function ScriptModule.Shut(...)
	for key, value in pairs(ScriptList) do
		if type(value) == "table" then
			ScriptList[key].tbl.Shut(...);	
		end
	end
end