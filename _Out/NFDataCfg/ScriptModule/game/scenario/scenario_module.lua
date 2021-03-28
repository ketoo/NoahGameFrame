scenario_module = {}

scenario_module.scenario100 = nil
scenario_module.scenario200 = nil
scenario_module.scenario300 = nil
scenario_module.scenario400 = nil

testTable = {
    ["abc"] = "nil",
}

function scenario_module:reload()
    scenario_module.scenario100 = find_module("scenario100")
    scenario_module.scenario200 = find_module("scenario200")
    scenario_module.scenario300 = find_module("scenario300")
    scenario_module.scenario400 = find_module("scenario400")
end

function scenario_module:awake()
    scenario_module:reload()
end

function scenario_module:init()
    script_module:add_class_cb("Player", scenario_module, scenario_module.class_common_event);
end



function scenario_module:after_init()
end

function scenario_module:ready_execute()
end

function scenario_module:before_shut()
end

function scenario_module:shut()
end

function scenario_module:test(object_id)

end

function scenario_module:class_common_event(object_id, className, eventID, varData)

    if eventID == ObjectEvent.COE_CREATE_FINISH then

        local scenario_id = script_module:get_prop_int(object_id,"ScenarioId");
        local scenario_status_id = script_module:get_prop_int(object_id,"ScenarioStatusId");
        local scenario_step_id = script_module:get_prop_int(object_id,"ScenarioStepId");
        local scenario_equip_id = script_module:get_prop_int(object_id,"ScenarioEquipId");

        print(object_id);
        if scenario_id == 0 or (scenario_id == 100 and (scenario_status_id == 1 or scenario_status_id == 0)) then
            scenario_module.scenario100:do_logic(object_id)
        elseif scenario_id == 200 then
            scenario_module.scenario200:do_logic(object_id)
        elseif scenario_id == 300 then
            scenario_module.scenario300:do_logic(object_id)
        elseif scenario_id == 400 then
            scenario_module.scenario400:do_logic(object_id)
        end
    end
end
