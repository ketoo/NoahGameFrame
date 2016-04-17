ScriptModule = {}
function ScriptModule.Init(...)
	TestModule:Init(...)
end

function ScriptModule.AfterInit(...)
	TestModule:AfterInit(...)
end

function ScriptModule.Execute(...)
	TestModule:Execute(...)
end

function ScriptModule.BeforeShut(...)
	TestModule:BeforeShut(...)
end

function ScriptModule.Shut(...)
	TestModule:Shut(...)
end

function ScriptModule.OnClassCommonEvent(...)
	TestModule.OnClassCommonEvent(...)
end

function ScriptModule.OnRecordCommonEvent(...)
	TestModule.OnRecordCommonEvent(...)
end

function ScriptModule.OnPropertyCommEvent(...)
	TestModule.OnPropertyCommEvent(...)
end


--ScriptModuleAPI

function ScriptModule.DoEvent(...)
	io.write(tostring("ScriptModule.DoEvent\n"))
end

function ScriptModule.ExistElement(...)
	io.write(tostring("ScriptModule.ExistElement\n"))
end

function ScriptModule.GetElementPropertyInt(...)
	io.write(tostring("ScriptModule.GetElementPropertyInt\n"))
end

function ScriptModule.GetElementPropertyFloat(...)
	io.write(tostring("ScriptModule.GetElementPropertyFloat\n"))
end

function ScriptModule.GetElementPropertyString(...)
	io.write(tostring("ScriptModule.GetElementPropertyString\n"))
end

function ScriptModule.AddPropertyCallBack(...)
	io.write(tostring("ScriptModule.AddPropertyCallBack\n"))
end

function ScriptModule.AddRecordCallBack(...)
	io.write(tostring("ScriptModule.AddRecordCallBack\n"))
end

function ScriptModule.AddEventCallBack(...)
	io.write(tostring("ScriptModule.AddEventCallBack\n"))
end

function ScriptModule.AddHeartBeat(...)
	io.write(tostring("ScriptModule.AddHeartBeat\n"))
end

function ScriptModule.FindHeartBeat(...)
	io.write(tostring("ScriptModule.FindHeartBeat\n"))
end

function ScriptModule.RemoveHeartBeat(...)
	io.write(tostring("ScriptModule.RemoveHeartBeat\n"))
end

function ScriptModule.ExistContainer(...)
	io.write(tostring("ScriptModule.ExistContainer\n"))
end

function ScriptModule.SetPropertyInt(...)
	io.write(tostring("ScriptModule.SetPropertyInt\n"))
end

function ScriptModule.SetPropertyFloat(...)
	io.write(tostring("ScriptModule.SetPropertyFloat\n"))
end

function ScriptModule.SetPropertyString(...)
	io.write(tostring("ScriptModule.SetPropertyString\n"))
end

function ScriptModule.SetPropertyObject(...)
	io.write(tostring("ScriptModule.SetPropertyObject\n"))
end

function ScriptModule.GetPropertyInt(...)
	io.write(tostring("ScriptModule.GetPropertyInt\n"))
end

function ScriptModule.GetPropertyFloat(...)
	io.write(tostring("ScriptModule.GetPropertyFloat\n"))
end

function ScriptModule.GetPropertyString(...)
	io.write(tostring("ScriptModule.GetPropertyString\n"))
end

function ScriptModule.GetPropertyObject(...)
	io.write(tostring("ScriptModule.GetPropertyObject\n"))
end

function ScriptModule.SetRecordInt(...)
	io.write(tostring("ScriptModule.SetRecordInt\n"))
end

function ScriptModule.SetRecordFloat(...)
	io.write(tostring("ScriptModule.SetRecordFloat\n"))
end

function ScriptModule.SetRecordString(...)
	io.write(tostring("ScriptModule.SetRecordString\n"))
end

function ScriptModule.SetRecordObject(...)
	io.write(tostring("ScriptModule.SetRecordObject\n"))
end

function ScriptModule.GetRecordInt(...)
	io.write(tostring("ScriptModule.GetRecordInt\n"))
end

function ScriptModule.GetRecordFloat(...)
	io.write(tostring("ScriptModule.GetRecordFloat\n"))
end

function ScriptModule.GetRecordString(...)
	io.write(tostring("ScriptModule.GetRecordString\n"))
end

function ScriptModule.GetRecordObject(...)
	io.write(tostring("ScriptModule.GetRecordObject\n"))
end

function ScriptModule.AddRow(...)
	io.write(tostring("ScriptModule.AddRow\n"))
end
