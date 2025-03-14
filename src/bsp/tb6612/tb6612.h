#ifndef __TB6612_H
#define __TB6612_H

#include "sys.h"

#define ENCODER_TIM_PERIOD 60000   // number of pulses per revolution
#define ENCODER_TIM_INIT_VAL 30000

enum encoder_id_e {
    TB6612_ID_0,
    TB6612_ID_1,
    TB6612_ID_2,
    TB6612_ID_3,
    TB6612_ID_MAX    
};

enum tb6612_dir_e {
	TB6612_FORWARD,
	TB6612_BACK,
	TB6612_STOP
};

void tb6612_init(void);
void tb6612_start(void);
void tb6612_set_dir(uint8_t id, uint8_t dir);
void tb6612_set_pwm(uint8_t id, int16_t pwm);
int tb6612_encoder_read_and_reset(uint8_t id);

#endif