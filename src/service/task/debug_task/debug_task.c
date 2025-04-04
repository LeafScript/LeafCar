#include "debug_task.h"
#include "base_type.h"
#include "task_gen.h"
#include "log.h"
#include "error_code.h"
#include "leafcli.h"
#include "debug_cli.h"

enum {
    DEBUG_STEP_HANDLE,
};

static int debug_task_handle(task_context_s *task_ctx);

static task_exec_func g_debug_task_exec_list[] = {
    debug_task_handle,
};
static task_context_s g_debug_task_ctx;

static int debug_task_handle(task_context_s *task_ctx)
{
    leafcli_scan();
    return TASK_RES_CURRENT;
}

static void debug_task_stop(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "debug_task_stop");
}

static void debug_task_async_back(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "debug_task_async_back");
}

static void debug_task_finish(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "debug_task_finish");
}

static int debug_task_ctx_init(uint8_t task_id)
{
    task_context_s *ctx = &g_debug_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(g_debug_task_exec_list);
    ctx->task_name = "debug_task";
    ctx->exec_list = g_debug_task_exec_list;
    ctx->stop = debug_task_stop;
    ctx->async_back = debug_task_async_back;
    ctx->timeout = NULL;
    ctx->finish = debug_task_finish;
    ctx->restart = NULL;
    return task_init(ctx);
}

int debug_task_init(uint8_t task_id)
{
    int ret = debug_cli_init();
    if (ret != EC_OK) {
        return ret;
    }
    ret = debug_task_ctx_init(task_id);
    if (ret != EC_OK) {
        return ret;
    }
    return EC_OK;
}

void debug_task_start(void)
{
    task_start(&g_debug_task_ctx);
}

void debug_task_scan(void)
{
    task_scan(&g_debug_task_ctx);
}