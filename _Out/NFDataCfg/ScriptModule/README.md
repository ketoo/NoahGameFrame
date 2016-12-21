# NoahGameFrame.NFCLuaScriptMudule
##### Copyright © http://www.noahframe.com

[![Join the chat at https://gitter.im/ketoo/NoahGameFrame](https://badges.gitter.im/ketoo/NoahGameFrame.svg)](https://gitter.im/ketoo/NoahGameFrame?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

#####交流QQ群：341159815

### How to Create a New LuaScriptModule

##Step 1
Create a Lua Script File, and Must Contain following functions
- Init()
- AfterInit()
- Execute()
- BeforeShut()
- Shut()

Mostly like this
# [TestModule2.lua](https://github.com/ketoo/NoahGameFrame/tree/develop/_Out/Server/NFDataCfg/ScriptModule/TestModule2.lua)

```lua
TestModule2 = {}

function TestModule2.Init()
    io.write("TestModule2 Init!\n");
end

function TestModule2.AfterInit()
    io.write("TestModule2 AfterInit!\n");
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

return TestModule2 --Do not forget return this
```
##Step 2
Add your LuaScriptModule Infomation into [script_list.lua](https://github.com/ketoo/NoahGameFrame/tree/develop/_Out/Server/NFDataCfg/ScriptModule/script_list.lua)
```lua
ScriptList={
    {tbl=nil, tblName="TestModule"},
    {tbl=nil, tblName="TestModule2"},
}

load_script_file(ScriptList)
```
##Enjoy it!
