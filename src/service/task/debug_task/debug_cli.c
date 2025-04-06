#include "debug_cli.h"
#include "base_type.h"
#include "error_code.h"
#include "log.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "led.h"
#include "car_task_schedule.h"
#include "delay.h"

#define DEBUG_CLI_FIFO_SIZE   256

static void debug_cli_fifo_full_cb(uint8_t group_id);

static uint8_t g_cli_fifo[DEBUG_CLI_FIFO_SIZE] = { 0 };
static leafcli_buffer_s g_cli_buff = {
    .group_id = LEAFCLI_GROUP_DEBUG_CLI,
    .fifo_size = DEBUG_CLI_FIFO_SIZE,
    .fifo_full_cb = debug_cli_fifo_full_cb,
    .fifo_full_solved = debug_cli_fifo_full_cb,
    .fifo = g_cli_fifo
};

static uint32_t debug_cli_delay_ms(uint32_t ms);
static uint32_t debug_cli_delay_s(uint32_t s);

static leafcli_cmd_s g_cli_cmd_list[] = {
    { "led_toggle", LED_Toggle, "void LED_Toggle(uint8_t num)" },
    { "car_task_register", car_task_register, "int car_task_register(uint8_t car_task_id)" },
    { "delay_ms", debug_cli_delay_ms, "uint32_t debug_cli_delay_ms(uint32_t ms)" },
    { "delay_s", debug_cli_delay_s, "uint32_t debug_cli_delay_s(uint32_t s)" },
};
static leafcli_context_s g_cli_ctx = {
    .group_id = LEAFCLI_GROUP_DEBUG_CLI,
    .cmd_list_name = "debug cli cmd list",
    .cmd_num = ARRAY_SIZE(g_cli_cmd_list),
    .cmd_list = g_cli_cmd_list
};

static void debug_cli_fifo_full_cb(uint8_t group_id)
{
    LED_Toggle(1);
    LEAF_LOG(LOG_ERROR, "debug cli fifo full, group_id: %d", group_id);
}

static void debug_cli_fifo_full_solved(uint8_t group_id)
{
    LED_Toggle(1);
    LEAF_LOG(LOG_ERROR, "debug cli fifo full solved, group_id: %d", group_id);
}

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

int debug_cli_init(void)
{
    int ret;
    ret = leafcli_register_buff(&g_cli_buff);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcli_register_buff failed");
        return EC_ERROR;
    }
    ret = leafcli_register_builtin_ctx(LEAFCLI_GROUP_DEBUG_CLI);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcli_register_builtin_ctx failed");
        return EC_ERROR;
    }
	ret = leafcli_register_ctx(&g_cli_ctx);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcli_register_ctx failed");
        return EC_ERROR;
    }
    return EC_OK;
}

void debug_cli_recv_data(uint8_t ch)
{
    leafcli_recv_data(ch, LEAFCLI_GROUP_DEBUG_CLI);
}