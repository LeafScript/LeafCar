#include "cartask.h"
#include "taskconf.h"
#include "task1.h"
#include "work1.h"
#include "test_task.h"
#include "base_type.h"

static struct {
	uint8_t task_id;
	void (*task_init)(void);
	void (*task_start)(void);
	void (*task_scan)(void);
} car_task_table[] = {
	{ CAR_TASK_0, NULL, NULL, car_task1_scan },
	{ CAR_TASK_1, NULL, NULL, Work1_Scan },
	{ CAR_TASK_2, NULL, NULL, TestTask_Scan },
};

static uint8_t g_cur_task_id = CAR_TASK_2;

void car_task_init(uint8_t task_id)
{
	if (task_id >= CAR_TASK_MAX || car_task_table[task_id].task_init == NULL) {
		return;
	}
	car_task_table[task_id].task_start();
}

void car_task_start(uint8_t task_id)
{
	if (task_id >= CAR_TASK_MAX || car_task_table[task_id].task_start == NULL) {
		return;
	}
	car_task_table[task_id].task_start();
}

void car_task_scan(void)
{
	uint8_t i;
	for (i = 0; i < ARRAY_SIZE(car_task_table); i++) {
		if (g_cur_task_id == car_task_table[i].task_id) {
			car_task_table[i].task_scan();
			return;
		}
	}
}