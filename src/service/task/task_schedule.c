#include "task_cfg.h"
#include "error_code.h"
#include "log.h"
#include "car_task_schedule.h"
#include "cli_task.h"
#include "update_fw_task.h"

int task_schedule_init(void)
{
    int ret;
	ret = car_task_init(TASK_CAR);
    if (ret != EC_OK) {
        LEAF_LOG(LOG_ERROR, "car_task_init failed\n");
        return ret;
    }
    ret = cli_task_init(TASK_CLI);
    if (ret != EC_OK) {
        LEAF_LOG(LOG_ERROR, "cli_task_init failed\n");
        return ret;
    }
    ret = update_fw_task_init(TASK_UPDATE_FW);
    if (ret != EC_OK) {
        LEAF_LOG(LOG_ERROR, "update_fw_task_init failed\n");
        return ret;
    }
    return EC_OK;
}

int task_schedule_start(void)
{
    car_task_start();
    cli_task_start();
}

void task_schedule_scan(void)
{
    car_task_scan();
    cli_task_scan();
    update_fw_task_scan();
}