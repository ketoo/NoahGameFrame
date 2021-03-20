scenario_cfg={
}

function scenario_cfg:reload()
end

function scenario_cfg:awake()
    scenario_cfg:reload()
end

function scenario_cfg:init()
    --print(scenario_cfg[100][1][1].next_step);
    --print_table(scenario_cfg)
end



function scenario_cfg:after_init()
end

function scenario_cfg:ready_execute()
end

function scenario_cfg:before_shut()
end

function scenario_cfg:shut()
end

scenario_cfg[100] = {
    [1] = {
        [1] = {
            next_scenario=100, next_status=1, next_step=2
        },
        [2] = {
            next_scenario=100, next_status=1, next_step=3
        },
        [3] = {
            next_scenario=100, next_status=1, next_step=4
        },
        [4] = {
            next_scenario=100, next_status=1, next_step=5
        },
        [5] = {
            next_scenario=100, next_status=1, next_step=6
        },
        [6] = {
            next_scenario=100, next_status=2, next_step=1
        },
    },
    [2] = {
        [1] = {
            next_scenario=100, next_status=2, next_step=2
        },
        [2] = {
            next_scenario=100, next_status=2, next_step=3
        },
        [3] = {
            next_scenario=100, next_status=2, next_step=4
        },
        [4] = {
            next_scenario=100, next_status=2, next_step=5
        },
        [5] = {
            next_scenario=100, next_status=2, next_step=6
        },
        [6] = {
            next_scenario=100, next_status=2, next_step=7
        },
        [7] = {
            next_scenario=100, next_status=2, next_step=8
        },
        [8] = {
            next_scenario=100, next_status=2, next_step=9
        },
        [9] = {
            next_scenario=100, next_status=2, next_step=10
        },
        [10] = {
            next_scenario=100, next_status=2, next_step=11
        },
        [11] = {
            next_scenario=100, next_status=2, next_step=12
        },
        [12] = {
            next_scenario=100, next_status=2, next_step=13
        },
        [13] = {
            next_scenario=100, next_status=2, next_step=14
        },
        [14] = {
            next_scenario=100, next_status=2, next_step=15
        },
        [15] = {
            next_scenario=200, next_status=1, next_step=1
        },

    },
}