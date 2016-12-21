TestModule2 = {}

register_module(TestModule2,"TestModule2");

function TestModule2.Init()
	io.write("TestModule2 Init!\n");
end

function TestModule2.AfterInit()
	io.write("TestModule2 AfterInit!" .. tostring(pLuaScriptModule) .. "\n");
end

function TestModule2.Execute()
	io.write("TestModule2 Execute!\n");
end

function TestModule2.BeforeShut()
	io.write("TestModule2 BeforeShut!\n");
end

function TestModule2.Shut()
	io.write("TestModule2 Shut!\n");
end