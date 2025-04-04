#include "test_car_task.h"
#include "car_task_schedule.h"
#include "car_task_op.h"
#include "log.h"
#include "redwire.h"
#include "car_ctrl.h"
#include "car_op.h"
#include "carled.h"
#include "led.h"

static void test_led1_toggle(void *priv, bool is_finish)
{
    LED_Toggle(1);
    LEAF_LOG(LOG_INFO, "test_led1_toggle is_finish[%u]", is_finish);
    if (is_finish) {
        car_task_async_step_finish();
    }
}

static void test_led2_toggle(void *priv, bool is_finish)
{
    LED_Toggle(2);
    LEAF_LOG(LOG_INFO, "test_led2_toggle is_finish[%u]", is_finish);
    if (is_finish) {
        car_task_async_step_finish();
    }
}

static uint16_t test_get_step_4(void)
{
    return 4;
}

static uint16_t test_get_step_3(void)
{
    return 3;
}

static uint16_t test_get_step_15(void)
{
    return 15;
}

static car_op_s g_car_task_test_op_list[] = {
    // timer test
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_MUTI, .timer_param = { .trigger_ms = 500, .cb = test_led2_toggle, .trigger_times = 6 } },
    // op test
	{ CAR_OP_OBJ_OP, OP_OP_JUMP_NEXT, .op_param = { .step = 4 } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led2_toggle } },
	{ CAR_OP_OBJ_OP, OP_OP_JUMP_TO, .op_param = { .step = 8 } },
	{ CAR_OP_OBJ_OP, OP_OP_JUMP_PREV, .op_param = { .step = 3 } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },  // step 8
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led2_toggle } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = test_get_step_4 } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led2_toggle } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_TO, .op_param = { .get_step = test_get_step_15 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_PREV, .op_param = { .get_step = test_get_step_3 } },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led1_toggle } },  // step 15
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = test_led2_toggle } },
	{ CAR_OP_OBJ_OP, OP_OP_END },
};

int test_car_task_info_init(void)
{
	return car_task_info_init(g_car_task_test_op_list, ARRAY_SIZE(g_car_task_test_op_list));
}