#include "car_task_schedule.h"
#include "car_task_op.h"
#include "task_gen.h"
#include "log.h"
#include "error_code.h"
#include "test_car_task.h"
#include "car_task_f.h"

enum {
    CAR_STEP_HANDLE,
    CAR_STEP_DEBUG
};

static int car_task_handle(task_context_s *task_ctx);
static int car_task_debug_step(task_context_s *task_ctx);

static task_exec_func g_car_task_exec_list[] = {
    car_task_handle,
    car_task_debug_step,
};
static task_context_s g_car_task_ctx;
static bool g_async_step_finished = false;
static struct {
    car_op_s *op_list;
    uint16_t op_list_size;
    uint16_t op_cur;
    uint16_t op_next;
} g_car_op_info;

int car_task_info_init(car_op_s *op_list, uint16_t op_list_size)
{
    if (op_list == NULL || op_list_size == 0) {
        return EC_ERROR;
    }
    g_car_op_info.op_list = op_list;
    g_car_op_info.op_list_size = op_list_size;
    g_car_op_info.op_cur = 0;
    g_car_op_info.op_next = 0;
    return EC_OK;
}

int car_task_set_op_next(uint16_t op_index)
{
    if (op_index >= g_car_op_info.op_list_size) {
        return EC_ERROR;
    }
    g_car_op_info.op_next = op_index;
    return EC_OK;
}

uint16_t car_task_get_op_cur(void)
{
    return g_car_op_info.op_cur;
}

void car_task_async_step_finish(void)
{
    g_async_step_finished = true;
}

static int car_task_handle(task_context_s *task_ctx)
{
    uint16_t op_cur = g_car_op_info.op_next;
    car_op_s *op = NULL;
    int ret;

    if (op_cur >= g_car_op_info.op_list_size) {
        return TASK_RES_FINISH;
    }
    g_car_op_info.op_cur = op_cur;
    g_car_op_info.op_next = op_cur + 1;
    g_async_step_finished = false;
    ret = car_op_handle(&g_car_op_info.op_list[op_cur]);
    if (ret != EC_OK) {
        return TASK_RES_FINISH;
    }
    task_next_step(task_ctx, CAR_STEP_HANDLE);
    return TASK_RES_ASYNC_WAIT;
}

static int car_task_debug_step(task_context_s *task_ctx)
{
    return TASK_RES_FINISH;
}

static void car_task_stop(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "car_task_stop");
}

static void car_task_async_back(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "car_task_async_back op_cur[%u] op_next[%u]", g_car_op_info.op_cur, g_car_op_info.op_next);
}

static void car_task_timeout(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "car_task_timeout");
}

static void car_task_finish(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "car_task_finish");
}

static void car_task_restart(task_context_s *task_ctx)
{
    LEAF_LOG(LOG_DEBUG, "car_task_restart");
}

int car_task_init(uint8_t task_id)
{
    task_context_s *ctx = &g_car_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(g_car_task_exec_list);
    ctx->task_name = "car_task";
    // 0: car_task_handle  1: car_task_debug_step
    ctx->exec_list = g_car_task_exec_list;
    ctx->stop = car_task_stop;
    ctx->async_back = car_task_async_back;
    ctx->timeout = car_task_timeout;
    ctx->finish = car_task_finish;
    ctx->restart = car_task_restart;
    return task_init(ctx);
}

int car_task_register(uint8_t car_task_id)
{
    switch (car_task_id) {
        case CAR_TASK_TEST:
            return test_car_task_info_init();
        case CAR_TASK_F:
            return car_task_f_info_init();
        default:
            LEAF_LOG(LOG_ERROR, "car_task_id[%u] invalid", car_task_id);
            return EC_ERROR;
    }
}

void car_task_start(void)
{
    if (g_car_op_info.op_list_size == 0) {
        return;
    }
    task_start(&g_car_task_ctx);
}

void car_task_scan(void)
{
    if (g_async_step_finished) {
	    task_async_cb(&g_car_task_ctx);
    }
    task_scan(&g_car_task_ctx);
}