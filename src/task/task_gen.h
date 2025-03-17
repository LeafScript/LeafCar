#ifndef _TASK_GEN_H_
#define _TASK_GEN_H_

#include "base_type.h"

enum task_result_e {
    TASK_RES_CURRENT,
    TASK_RES_NEXT,
    TASK_RES_STOP,
    TASK_RES_ASYNC_WAIT,
    TASK_RES_FINISH,
    TASK_RES_RESTART,
    TASK_RES_MAX
};

#define TASK_PRIV_INFO_SIZE 2

typedef int (*task_exec_func)(void);     // return enum task_result_e

typedef struct {
    uint8_t task_id;
    uint8_t max_step;
    char *task_name;
    task_exec_func *exec_list;
    void (*stop)(void);
    void (*async_back)(void);
    void (*timeout)(void);
    void (*finish)(void);
    void (*restart)(void);
    uint8_t priv[TASK_PRIV_INFO_SIZE];
} task_context_t;

int task_init(task_context_t *task_ctx);
void task_start(task_context_t *task_ctx);
void task_scan(task_context_t *task_ctx);
void task_async_cb(task_context_t *task_ctx);

#endif