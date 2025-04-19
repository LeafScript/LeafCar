#include "update_fw_task.h"
#include "task_gen.h"
#include "log.h"
#include "error_code.h"

static int download_fw_handle(task_context_s *task_ctx);
static int check_new_fw(task_context_s *task_ctx);

static task_exec_func g_update_fw_task_exec_list[] = {
    download_fw_handle,
    check_new_fw,
};
static task_context_s g_update_fw_task_ctx;
static bool g_update_fw_started = false;

static int download_fw_handle(task_context_s *task_ctx)
{
    return TASK_RES_ASYNC_WAIT;
}

static int check_new_fw(task_context_s *task_ctx)
{
    return TASK_RES_FINISH;
}

static void update_fw_task_stop(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "update_fw_task_stop");
}

static void update_fw_task_async_back(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "update_fw_task_async_back");
}

static void update_fw_task_finish(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "update_fw_task_finish");
    g_update_fw_started = false;
}

int update_fw_task_init(uint8_t task_id)
{
    task_context_s *ctx = &g_update_fw_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(g_update_fw_task_exec_list);
    ctx->task_name = "update_fw_task";
    ctx->exec_list = g_update_fw_task_exec_list;
    ctx->stop = update_fw_task_stop;
    ctx->async_back = update_fw_task_async_back;
    ctx->timeout = NULL;
    ctx->finish = update_fw_task_finish;
    ctx->restart = NULL;
    return task_init(ctx);
}

void update_fw_task_start(void)
{
    task_start(&g_update_fw_task_ctx);
    g_update_fw_started = true;
}

void update_fw_task_scan(void)
{
    if (!g_update_fw_started) {
        return;
    }
    task_scan(&g_update_fw_task_ctx);
}