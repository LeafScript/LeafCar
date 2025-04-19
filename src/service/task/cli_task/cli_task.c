#include "cli_task.h"
#include "base_type.h"
#include "task_gen.h"
#include "log.h"
#include "error_code.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "leafcar_cli.h"
#include "debug_cli.h"
#include "led.h"

enum {
    CLI_STEP_HANDLE,
};

static int cli_task_handle(task_context_s *task_ctx);

static task_exec_func g_cli_task_exec_list[] = {
    cli_task_handle,
};
static task_context_s g_cli_task_ctx;

static int cli_task_handle(task_context_s *task_ctx)
{
    leafcli_scan();
    return TASK_RES_CURRENT;
}

void cli_recv_data(uint8_t ch)
{
    leafcli_recv_data(ch, LEAFCLI_GROUP_LEAFCAR);
}

static void cli_task_stop(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "cli_task_stop");
}

static void cli_task_async_back(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "cli_task_async_back");
}

static void cli_task_finish(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "cli_task_finish");
}

static int cli_task_ctx_init(uint8_t task_id)
{
    task_context_s *ctx = &g_cli_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(g_cli_task_exec_list);
    ctx->task_name = "cli task";
    ctx->exec_list = g_cli_task_exec_list;
    ctx->stop = cli_task_stop;
    ctx->async_back = cli_task_async_back;
    ctx->timeout = NULL;
    ctx->finish = cli_task_finish;
    ctx->restart = NULL;
    return task_init(ctx);
}

void cli_task_start(void)
{
    task_start(&g_cli_task_ctx);
}

void cli_task_scan(void)
{
    task_scan(&g_cli_task_ctx);
}

#define CLI_FIFO_SIZE   256

static void cli_fifo_full_cb(uint8_t group_id);
static void cli_fifo_full_solved(uint8_t group_id);

static uint8_t g_cli_fifo[CLI_FIFO_SIZE] = { 0 };
static leafcli_buffer_s g_cli_buff = {
    .group_id = LEAFCLI_GROUP_LEAFCAR,
    .fifo_size = CLI_FIFO_SIZE,
    .fifo_full_cb = cli_fifo_full_cb,
    .fifo_full_solved = cli_fifo_full_solved,
    .fifo = g_cli_fifo
};

static void cli_fifo_full_cb(uint8_t group_id)
{
    LED_Toggle(1);
    LEAF_LOG(LOG_ERROR, "cli fifo full, group_id: %d", group_id);
}

static void cli_fifo_full_solved(uint8_t group_id)
{
    LED_Toggle(1);
    LEAF_LOG(LOG_ERROR, "cli fifo full solved, group_id: %d", group_id);
}

int cli_task_init(uint8_t task_id)
{
    int ret;
    ret = leafcli_register_buff(&g_cli_buff);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcli_register_buff failed");
        return EC_ERROR;
    }
    ret = leafcli_register_builtin_ctx(LEAFCLI_GROUP_LEAFCAR);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcli_register_builtin_ctx failed");
        return EC_ERROR;
    }
    ret = debug_cli_register();
    if (ret != EC_OK) {
        return ret;
    }
    ret = leafcar_cli_register();
    if (ret != EC_OK) {
        return ret;
    }
    ret = cli_task_ctx_init(task_id);
    if (ret != EC_OK) {
        return ret;
    }
    return EC_OK;
}