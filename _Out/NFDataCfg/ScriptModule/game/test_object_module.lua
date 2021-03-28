test_object_module = {}

test_object_module.playerObject = nil
test_object_module.test_http_module = nil

function test_object_module:reload()
    test_object_module.test_http_module = find_module("test_http_module")
end

function test_object_module:awake()
    script_module:log_info("test_object_module awake!----");
    test_object_module:reload()
end

function test_object_module:init()

    test_http_module.test()

    script_module:log_info("test_object_module init!----");

    script_module:add_module_schedule("add_game_module_schedule", self, self.module_schedule, 10, 55555);
end

function test_object_module:after_init()
    script_module:log_info("test_object_module after_init!----");
end

function test_object_module:ready_execute()
end

function test_object_module:before_shut()
end

function test_object_module:shut()
end


function test_object_module:module_schedule(heartBeat, time, count)

    if test_object_module.playerObject == nil then
        test_object_module.playerObject = {}
        for i = 1, 1 do
            --local ident = script_module:create_object(NFGUID(), 1, 0, "Player", "", NFDataList());
            --test_object_module.playerObject[i] = ident
        end
    else
        for i=1, #(test_object_module.playerObject) do
            local ident = test_object_module.playerObject[i]
            script_module:destroy_object(ident)
        end
        test_object_module.playerObject = nil
    end

end
