# NoahGameFrame.NFCLuaScriptMudule
##### Copyright © http://www.noahframe.com

[![Join the chat at https://gitter.im/ketoo/NoahGameFrame](https://badges.gitter.im/ketoo/NoahGameFrame.svg)](https://gitter.im/ketoo/NoahGameFrame?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

##### 交流QQ群：341159815

### How to Create a New LuaScriptModule

## Step 1
Create a Lua Script File, and Must Contain following functions
- reload()
- awake()
- init()
- ready_execute()
- after_init()
- before_shut()
- shut()

Mostly like this
# [test_module.lua](https://github.com/ketoo/NoahGameFrame/blob/master/_Out/NFDataCfg/ScriptModule/game/test_game_module.lua)

```lua
test_module = {}
register_module(test_module,"test_module");

function test_module.reload()
end

function test_module.awake()
	reload();
end

function test_module.init()
end

function test_module.after_init()
end

function test_module.ready_execute()
end

function test_module.before_shut()
end

function test_module.shut()
end

return TestModule2 --Do not forget return this
```

##Step 2
Add your LuaScriptModule Infomation into [script_list.lua](https://github.com/ketoo/NoahGameFrame/blob/master/_Out/NFDataCfg/ScriptModule/game/script_list.lua)


```lua
ScriptList={
    {tbl=nil, tblName="TestModule"},
    {tbl=nil, tblName="TestModule2"},
}

load_script_file(ScriptList)
```



##How to do hot fix?
Add your lua script file name on here [script_reload.lua](https://github.com/ketoo/NoahGameFrame/blob/master/_Out/NFDataCfg/ScriptModule/game/script_reload.lua)

```lua
local version_code = "1.0.1"
ScriptReloadList={
	{tbl=nil, tblName="test_module"},
}
```

Once you changed the version_code then the engine will reload the files that in ScriptReloadList.

## Enjoy it!
