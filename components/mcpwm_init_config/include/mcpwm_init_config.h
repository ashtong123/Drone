#include "driver/mcpwm_timer.h"
#include "driver/mcpwm_oper.h"
#include "driver/mcpwm_cmpr.h"
#include "driver/mcpwm_gen.h"
#include "driver/mcpwm_types.h"

void mcpwm_timer_config(
        int group_id, 
        mcpwm_timer_clock_source_t clk_src,
        uint32_t resolution_hz,
        mcpwm_timer_count_mode_t count_mode,
        uint32_t period_ticks,
        int intr_priority,
        uint32_t update_period_on_empty,
        uint32_t update_period_on_sync,
        mcpwm_timer_handle_t *ret_timer);

void mcpwm_operator_config(
        int group_id,
        int intr_priority,
        uint32_t update_gen_action_on_tez,
        uint32_t update_gen_action_on_tep,
        uint32_t update_gen_action_on_sync,
        uint32_t update_dead_time_on_tez,
        uint32_t update_dead_time_on_tep,
        uint32_t update_dead_time_on_sync,
        mcpwm_oper_handle_t *ret_oper);

void mcpwm_comparator_config(
        int intr_priority,
        uint32_t update_cmp_on_tez,
        uint32_t update_cmp_on_tep,
        uint32_t update_cmp_on_sync,
        mcpwm_oper_handle_t oper,
        mcpwm_cmpr_handle_t *ret_cmpr);

void mcpwm_generator_config(
        int gen_gpio_num,
        uint32_t invert_pwm,
        uint32_t io_loop_back,
        uint32_t io_od_mode,
        uint32_t pull_up,
        uint32_t pull_down,
        mcpwm_oper_handle_t oper,
        mcpwm_gen_handle_t *ret_gen);

