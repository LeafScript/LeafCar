#include "debug_cli.h"
#include "base_type.h"
#include "error_code.h"
#include "log.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "led.h"

#define DEBUG_CLI_FIFO_SIZE   256

static uint8_t g_cli_fifo[DEBUG_CLI_FIFO_SIZE] = { 0 };
static leafcli_buffer_s g_cli_buff = {
    .group_id = LEAFCLI_GROUP_DEBUG_CLI,
    .fifo_size = DEBUG_CLI_FIFO_SIZE,
    .fifo = g_cli_fifo
};
static leafcli_cmd_s g_cli_cmd_list[] = {
    { "led_toggle", LED_Toggle, "void LED_Toggle(uint8_t num)" }
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
    if (ret != EC_OK) {
        printf("leafcli_register_buff failed\r\n");
        return EC_ERROR;
    }
    ret = leafcli_register_builtin_ctx(LEAFCLI_GROUP_DEBUG_CLI);
    if (ret != EC_OK) {
        printf("leafcli_register_builtin_ctx failed\r\n");
        return EC_ERROR;
    }
	ret = leafcli_register_ctx(&g_cli_ctx);
    if (ret != EC_OK) {
        printf("leafcli_register_ctx failed\r\n");
        return EC_ERROR;
    }
    return EC_OK;
}

void debug_cli_recv_data(uint8_t ch)
{
    leafcli_recv_data(ch, LEAFCLI_GROUP_DEBUG_CLI);
}