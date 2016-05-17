ScriptModule = {}
function ScriptModule.Init(...)
	for i=1, #(ScriptList) do
		ScriptList[i].tbl.Init(...);
	end
end

function ScriptModule.AfterInit(...)
	for i=1, #(ScriptList) do
		ScriptList[i].tbl.AfterInit(...);
	end
end

function ScriptModule.Execute(...)
	for i=1, #(ScriptList) do
		ScriptList[i].tbl.Execute(...);
	end
end

function ScriptModule.BeforeShut(...)
	for i=1, #(ScriptList) do
		ScriptList[i].tbl.BeforeShut(...);
	end
end

function ScriptModule.Shut(...)
	for i=1, #(ScriptList) do
		ScriptList[i].tbl.Shut(...);
	end
end