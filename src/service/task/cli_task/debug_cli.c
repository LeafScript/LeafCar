#include "debug_cli.h"
#include "base_type.h"
#include "error_code.h"
#include "log.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "led.h"
#include "car_task_schedule.h"
#include "delay.h"
#include "car_basic.h"

static uint32_t debug_cli_delay_ms(uint32_t ms);
static uint32_t debug_cli_delay_s(uint32_t s);

static leafcli_cmd_s g_debug_cli_cmd_list[] = {
    { "get_log_level", leaf_log_get_level, "uint8_t leaf_log_get_level(void)" },
    { "set_log_level", leaf_log_set_level, "void leaf_log_set_level(uint8_t level)" },
    { "delay_ms", debug_cli_delay_ms, "uint32_t debug_cli_delay_ms(uint32_t ms)" },
    { "delay_s", debug_cli_delay_s, "uint32_t debug_cli_delay_s(uint32_t s)" },
    { "led_toggle", LED_Toggle, "void LED_Toggle(uint8_t num)" },
    { "car_task_register", car_task_register, "int car_task_register(uint8_t car_task_id)" },
    { "car_task_start", car_task_start, "void car_task_start(void)" },
    { "car_status_print", car_print, "void car_print(void)" },
};
static leafcli_context_s g_debug_cli_ctx = {
    .group_id = LEAFCLI_GROUP_LEAFCAR,
    .cmd_list_name = "debug cli cmd list",
    .cmd_num = ARRAY_SIZE(g_debug_cli_cmd_list),
    .cmd_list = g_debug_cli_cmd_list
};

static uint32_t debug_cli_delay_ms(uint32_t ms)
{
    uint32_t i, s, remain_ms;
    s = ms / 1000;
    remain_ms = ms % 1000;
    for (i = 0; i < s; i++) {
        delay_ms(1000);
    }
    if (remain_ms > 0) {
        delay_ms(remain_ms);
    }
    return 0;
}

static uint32_t debug_cli_delay_s(uint32_t s)
{
    uint32_t i;
    for (i = 0; i < s; i++) {
        delay_ms(1000);
    }
    return 0;
}

int debug_cli_register(void)
{
    int ret;
	ret = leafcli_register_ctx(&g_debug_cli_ctx);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "debug_cli_register failed");
        return EC_ERROR;
    }
    return EC_OK;
}