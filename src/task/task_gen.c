#include "task_gen.h"
#include "base_type.h"
#include "error_code.h"

enum task_status_e {
    TASK_STOP,
    TASK_RUNNING,
    TASK_ASYNC_BACK,
    TASK_TIMEOUT,
    TASK_FINISHED
};

typedef struct {
    uint8_t step;
    uint8_t next_step;
    uint8_t status;
} task_priv_info;

int task_init(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    int i;

    BUILD_BUG_ON(TASK_PRIV_INFO_SIZE != sizeof(task_priv_info));
    if (task_ctx->max_step == 0 || task_ctx->exec_list == NULL) {
        return EC_ERROR;
    }
    for (i = 0; i < task_ctx->max_step; i++) {
        if (task_ctx->exec_list[i] == NULL) {
            return EC_ERROR;
        }
    }
    info->step = 0;
    info->status = TASK_STOP;
    return EC_OK;
}

void task_start(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->step = 0;
    info->status = TASK_RUNNING;
}

static void task_current(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->status = TASK_RUNNING;
}

static void task_next(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->step = info->next_step;
    info->next_step++;
    info->status = TASK_RUNNING;
}

static void task_stop(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->status = TASK_STOP;
    if (task_ctx->stop != NULL) {
        task_ctx->stop(task_ctx);
    }
}

static void task_finish(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->status = TASK_FINISHED;
    if (task_ctx->finish != NULL) {
        task_ctx->finish(task_ctx);
    }
}

static void task_restart(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->step = 0;
    info->status = TASK_RUNNING;
    if (task_ctx->restart != NULL) {
        task_ctx->restart(task_ctx);
    }
}

static void task_async_wait(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    if (info->status == TASK_ASYNC_BACK) {
        task_next(task_ctx);
    } else {
        info->status = TASK_STOP;
    }
}

void task_async_cb(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    if (info->status == TASK_STOP) {
        task_next(task_ctx);
    } else {
        info->status = TASK_ASYNC_BACK;
    }
    if (task_ctx->async_back != NULL) {
        task_ctx->async_back(task_ctx);
    }
}

static bool is_task_timeout(task_context_s *task_ctx)
{
    return false;
}

static void task_timeout(task_context_s *task_ctx)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->status = TASK_FINISHED;
    if (task_ctx->timeout != NULL) {
        task_ctx->timeout(task_ctx);
    }
}

void task_scan(task_context_s *task_ctx)
{
    static void (*status_ops[])(task_context_s *) = {
        [TASK_RES_CURRENT] = task_current,
        [TASK_RES_NEXT] = task_next,
        [TASK_RES_STOP] = task_stop,
        [TASK_RES_ASYNC_WAIT] = task_async_wait,
        [TASK_RES_FINISH] = task_finish,
        [TASK_RES_RESTART] = task_restart
    };
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    int res;

    if (info->status == TASK_FINISHED) {
        return;
    }
    if (is_task_timeout(task_ctx)) {
        task_timeout(task_ctx);
        return;
    }
    if (info->status == TASK_STOP) {
        return;
    }
    if (info->step >= task_ctx->max_step) {
        task_finish(task_ctx);
        return;
    }
    res = task_ctx->exec_list[info->step](task_ctx);
    if (res >= TASK_RES_MAX) {
        task_finish(task_ctx);
        return;
    }
    status_ops[res](task_ctx);
}

void task_next_step(task_context_s *task_ctx, uint8_t next_step)
{
    task_priv_info *info = (task_priv_info *)task_ctx->priv;
    info->next_step = next_step;
}