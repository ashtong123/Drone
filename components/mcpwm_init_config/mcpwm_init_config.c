#include "mcpwm_init_config.h"

// ----- CONFIG MCPWM TIMER -----
void mcpwm_timer_config(
        int group_id, 
        mcpwm_timer_clock_source_t clk_src,
        uint32_t resolution_hz,
        mcpwm_timer_count_mode_t count_mode,
        uint32_t period_ticks,
        int intr_priority,
        uint32_t update_period_on_empty,
        uint32_t update_period_on_sync,
        mcpwm_timer_handle_t *ret_timer)
{

    const mcpwm_timer_config_t timer_config = {
        .group_id = group_id,
        .clk_src = clk_src,
        .resolution_hz = resolution_hz,
        .count_mode = count_mode,
        .period_ticks = period_ticks,
        .intr_priority = intr_priority,
        .flags.update_period_on_empty = update_period_on_empty,
        .flags.update_period_on_sync = update_period_on_sync,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, ret_timer));
    return;
}

// ----- CONFIG MCPWM OPERATOR -----
void mcpwm_operator_config(
        int group_id,
        int intr_priority,
        uint32_t update_gen_action_on_tez,
        uint32_t update_gen_action_on_tep,
        uint32_t update_gen_action_on_sync,
        uint32_t update_dead_time_on_tez,
        uint32_t update_dead_time_on_tep,
        uint32_t update_dead_time_on_sync,
        mcpwm_oper_handle_t *ret_oper)
{
    const mcpwm_operator_config_t operator_config = {
        .group_id = group_id,
        .intr_priority = intr_priority,
        .flags.update_gen_action_on_tez = update_gen_action_on_tez,
        .flags.update_gen_action_on_tep = update_gen_action_on_tep,
        .flags.update_gen_action_on_sync = update_gen_action_on_sync,
        .flags.update_dead_time_on_tez = update_dead_time_on_tez,
        .flags.update_dead_time_on_tep = update_dead_time_on_tep,
        .flags.update_dead_time_on_sync = update_dead_time_on_sync,
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, ret_oper));
    return;
}

// ----- CONFIG MCPWM COMPARATOR -----
void mcpwm_comparator_config(
        int intr_priority,
        uint32_t update_cmp_on_tez,
        uint32_t update_cmp_on_tep,
        uint32_t update_cmp_on_sync,
        mcpwm_oper_handle_t oper,
        mcpwm_cmpr_handle_t *ret_cmpr)
{
    const mcpwm_comparator_config_t comp_config = {
        .intr_priority = intr_priority,
        .flags.update_cmp_on_tez = update_cmp_on_tez,
        .flags.update_cmp_on_tep = update_cmp_on_tep,
        .flags.update_cmp_on_sync = update_cmp_on_sync,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comp_config, ret_cmpr)); 
    return;
}

// ----- CONFIG MCPWM GENERATOR -----
void mcpwm_generator_config(
        int gen_gpio_num,
        uint32_t invert_pwm,
        uint32_t io_loop_back,
        uint32_t io_od_mode,
        uint32_t pull_up,
        uint32_t pull_down,
        mcpwm_oper_handle_t oper,
        mcpwm_gen_handle_t *ret_gen)
{
    const mcpwm_generator_config_t gen_config = {
        .gen_gpio_num = gen_gpio_num,
        .flags.invert_pwm = invert_pwm,
        .flags.io_loop_back = io_loop_back,
        .flags.io_od_mode = io_od_mode,
        .flags.pull_up = pull_up,
        .flags.pull_down = pull_down,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &gen_config, ret_gen));
    return;
} 
