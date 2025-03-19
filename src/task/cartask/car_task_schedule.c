#include "car_task_schedule.h"
#include "car_task_op.h"
#include "task_gen.h"
#include "usart.h"
#include "error_code.h"

enum {
    CAR_STEP_HANDLE,
    CAR_STEP_DEBUG
};

static int car_task_handle(task_context_t *task_ctx);
static int car_task_debug_step(task_context_t *task_ctx);

static task_exec_func car_task_exec_list[] = {
    car_task_handle,
    car_task_debug_step,
};
static task_context_t g_car_task_ctx;
static struct {
    car_op_s *op_list;
    uint16_t op_list_size;
    uint16_t op_index;
} g_car_op_info;

int car_task_info_init(car_op_s *op_list, uint16_t op_list_size)
{
    if (op_list == NULL || op_list_size == 0) {
        return EC_ERROR;
    }
    g_car_op_info.op_list = op_list;
    g_car_op_info.op_list_size = op_list_size;
    g_car_op_info.op_index = 0;
    return EC_OK;
}

static int car_task_handle(task_context_t *task_ctx)
{
    uint16_t op_index = g_car_op_info.op_index;
    car_op_s *op = NULL;
    int ret;

    if (op_index >= g_car_op_info.op_list_size) {
        return TASK_RES_FINISH;
    }
    ret = car_op_handle(&g_car_op_info.op_list[op_index]);
    if (ret != EC_OK) {
        return TASK_RES_FINISH;
    }
    task_next_step(task_ctx, CAR_STEP_HANDLE);
    return TASK_RES_NEXT;
}

static int car_task_debug_step(task_context_t *task_ctx)
{
    return TASK_RES_FINISH;
}

static void car_task_stop(void)
{
    printf("car_task_stop\r\n");
}

static void car_async_back(void)
{
    printf("car_async_back\r\n");
}

static void car_task_timeout(void)
{
    printf("car_task_timeout\r\n");
}

static void car_task_finish(void)
{
    printf("car_task_finish\r\n");
}

static void car_task_restart(void)
{
    printf("car_task_restart\r\n");
}

int car_task_init(uint8_t task_id)
{
    task_context_t *ctx = &g_car_task_ctx;
    ctx->task_id = task_id;
    ctx->max_step = ARRAY_SIZE(car_task_exec_list);
    ctx->task_name = "car_task";
    // 0: car_task_handle  1: car_task_debug_step
    ctx->exec_list = car_task_exec_list;
    ctx->stop = car_task_stop;
    ctx->async_back = car_async_back;
    ctx->timeout = car_task_timeout;
    ctx->finish = car_task_finish;
    ctx->restart = car_task_restart;
    return task_init(&g_car_task_ctx);
}

void car_task_start(void)
{
    task_start(&g_car_task_ctx);
}

void car_task_scan(void)
{
    task_scan(&g_car_task_ctx);
}