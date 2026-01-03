#include "active_fw_task.h"
#include "task_gen.h"
#include "log.h"
#include "error_code.h"

static int stop_service_handle(task_context_s *task_ctx);
static int wait_service_stop(task_context_s *task_ctx);
static int active_new_fw(task_context_s *task_ctx);

static task_exec_func g_active_fw_task_exec_list[] = {
    stop_service_handle,
    wait_service_stop,
    active_new_fw
};
static task_context_s g_active_fw_task_ctx;

static int stop_service_handle(task_context_s *task_ctx)
{
    return TASK_RES_ASYNC_WAIT;
}

static int wait_service_stop(task_context_s *task_ctx)
{
    return TASK_RES_ASYNC_WAIT;
}

static int active_new_fw(task_context_s *task_ctx)
{
    return TASK_RES_FINISH;
}

static void active_fw_task_stop(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "active_fw_task_stop");
}

static void active_fw_task_async_back(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "active_fw_task_async_back");
}

static void active_fw_task_finish(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "active_fw_task_finish");
}

int active_fw_task_init(uint8_t task_id)
{
    task_context_s *ctx = &g_active_fw_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(g_active_fw_task_exec_list);
    ctx->task_name = "active_fw_task";
    ctx->exec_list = g_active_fw_task_exec_list;
    ctx->stop = active_fw_task_stop;
    ctx->async_back = active_fw_task_async_back;
    ctx->timeout = NULL;
    ctx->finish = active_fw_task_finish;
    ctx->restart = NULL;
    return task_init(ctx);
}

void active_fw_task_start(void)
{
    task_start(&g_active_fw_task_ctx);
}

void active_fw_task_scan(void)
{
    task_scan(&g_active_fw_task_ctx);
}