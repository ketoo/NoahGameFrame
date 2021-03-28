test_net_module = {}

function test_net_module:reload()
end

function test_net_module:awake()
    test_net_module:reload()
end

function test_net_module:init()
end



function test_net_module:after_init()
    script_module:log_info("test_pb_msg_module after_init!----");
    --script_module:add_msg_cb(1212, test);
end

function test_net_module:ready_execute()
end

function test_net_module:before_shut()
end

function test_net_module:shut()
end

function test()
end

--------------------------------------------------------------------------------



