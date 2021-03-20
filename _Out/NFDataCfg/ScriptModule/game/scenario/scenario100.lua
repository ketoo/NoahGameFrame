scenario100 = {}

function scenario100:reload()
end

function scenario100:awake()
    scenario100:reload()
end

function scenario100:init()
end

function scenario100:after_init()
end

function scenario100:ready_execute()
end

function scenario100:before_shut()
end

function scenario100:shut()
end

function scenario100:do_logic(object_id)

    script_module:set_prop_int(object_id, "ScenarioId", 100);
    script_module:set_prop_int(object_id, "ScenarioStatusId", 1);

    local scenario_id = script_module:get_prop_int(object_id,"ScenarioId");
    local scenario_status_id = script_module:get_prop_int(object_id,"ScenarioStatusId");
    local scenario_step_id = script_module:get_prop_int(object_id,"ScenarioStepId");
    local scenario_equip_id = script_module:get_prop_int(object_id,"ScenarioEquipId");

end