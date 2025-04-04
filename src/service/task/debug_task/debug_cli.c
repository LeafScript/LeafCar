#include "debug_cli.h"
#include "base_type.h"
#include "error_code.h"
#include "log.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "led.h"
#include "car_task_schedule.h"

#define DEBUG_CLI_FIFO_SIZE   256

static uint8_t g_cli_fifo[DEBUG_CLI_FIFO_SIZE] = { 0 };
static leafcli_buffer_s g_cli_buff = {
    .group_id = LEAFCLI_GROUP_DEBUG_CLI,
    .fifo_size = DEBUG_CLI_FIFO_SIZE,
    .fifo = g_cli_fifo
};
static leafcli_cmd_s g_cli_cmd_list[] = {
    { "led_toggle", LED_Toggle, "void LED_Toggle(uint8_t num)" },
    { "car_task_register", car_task_register, "int car_task_register(uint8_t car_task_id)" }
};
static leafcli_context_s g_cli_ctx = {
    .group_id = LEAFCLI_GROUP_DEBUG_CLI,
    .cmd_list_name = "debug cli cmd list",
    .cmd_num = ARRAY_SIZE(g_cli_cmd_list),
    .cmd_list = g_cli_cmd_list
};

int debug_cli_init(void)
{
    int ret = leafcli_register_buff(&g_cli_buff);
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