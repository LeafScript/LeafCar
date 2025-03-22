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

#define TASK_PRIV_INFO_SIZE 3

typedef struct _task_context_s task_context_s;
typedef int (*task_exec_func)(task_context_s *task_ctx);     // return enum task_result_e
struct _task_context_s {
    uint8_t task_id;
    uint8_t max_step;
    char *task_name;
    task_exec_func *exec_list;
    void (*stop)(task_context_s *task_ctx);
    void (*async_back)(task_context_s *task_ctx);
    void (*timeout)(task_context_s *task_ctx);
    void (*finish)(task_context_s *task_ctx);
    void (*restart)(task_context_s *task_ctx);
    uint8_t priv[TASK_PRIV_INFO_SIZE];
};

// should NOT be used in interrupt or muti thread environment
int task_init(task_context_s *task_ctx);
void task_start(task_context_s *task_ctx);
void task_scan(task_context_s *task_ctx);
void task_async_cb(task_context_s *task_ctx);
void task_next_step(task_context_s *task_ctx, uint8_t step);

#endif