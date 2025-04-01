#include "task_cfg.h"
#include "error_code.h"
#include "log.h"
#include "car_task_schedule.h"
#include "car_task_f.h"
#include "test_car_task.h"
#include "debug_task.h"

int task_schedule_init(void)
{
    int ret;
    // car_task_f_info_init();
	// ret = test_car_task_info_init();
    // if (ret != EC_OK) {
    //     printf("test_car_task_info_init failed\n");
    //     return ret;
    // }
	// ret = car_task_init(TASK_CAR);
    // if (ret != EC_OK) {
    //     printf("car_task_init failed\n");
    //     return ret;
    // }
    ret = debug_task_init(TASK_DEBUG);
    if (ret != EC_OK) {
        printf("debug_task_init failed\n");
        return ret;
    }
    return EC_OK;
}

int task_schedule_start(void)
{
    // car_task_start();
    debug_task_start();
}

void task_schedule_scan(void)
{
    // car_task_scan();
    debug_task_scan();
}