#include "motor.h"
#include "board_config.h"
#include "log.h"
#include "tb6612.h"
#include "rcc_mng.h"

////电机1mm路程编码值（经典轮胎）
//#define FL_DISRANCE		31.1
//#define FR_DISRANCE		30.9
//#define BL_DISRANCE		30.75
//#define BR_DISRANCE		30.55

//电机1mm路程编码值（小蓝电机轮胎）
#define FL_DISRANCE		11.387
#define FR_DISRANCE		11.434
#define BL_DISRANCE		11.465
#define BR_DISRANCE		11.336

static tb6612_context_s g_tb6612_ctx[TB6612_ID_MAX] = {
    // TB6612_ID_0
    {
        .dir = {
            .port = TB6612_ID_0_DIR_PORT,
            .in1_pin = TB6612_ID_0_DIR_IN1_PIN, .in2_pin = TB6612_ID_0_DIR_IN2_PIN
        },
        .pwm = {
            .tim = TB6612_PWM_TIMER, .tim_comp = TB6612_PWM_COMP_1,
            .tim_remap = GPIO_PartialRemap1_TIM2 | GPIO_Remap_SWJ_JTAGDisable,
            .prescaler = 3599, .period = 1,
            .port = TB6612_ID_0_PWM_PORT, .pin = TB6612_ID_0_PWM_PIN,
        },
        .enc = {
            .tim = TB6612_ID_0_ENC_TIMER, .tim_remap = 0,
            .tim_period = ENCODER_TIM_PERIOD, .port = TB6612_ID_0_ENC_PORT,
            .a_pin = TB6612_ID_0_ENC_A_PIN, .b_pin = TB6612_ID_0_ENC_B_PIN
        }
    },
    // TB6612_ID_1
    {
        .dir = {
            .port = TB6612_ID_1_DIR_PORT,
            .in1_pin = TB6612_ID_1_DIR_IN1_PIN, .in2_pin = TB6612_ID_1_DIR_IN2_PIN
        },
        .pwm = {
            .tim = TB6612_PWM_TIMER, .tim_comp = TB6612_PWM_COMP_2,
            .tim_remap = GPIO_PartialRemap1_TIM2 | GPIO_Remap_SWJ_JTAGDisable,
            .prescaler = 3599, .period = 1,
            .port = TB6612_ID_1_PWM_PORT, .pin = TB6612_ID_1_PWM_PIN,
        },
        .enc = {
            .tim = TB6612_ID_1_ENC_TIMER, .tim_remap = 0,
            .tim_period = ENCODER_TIM_PERIOD, .port = TB6612_ID_1_ENC_PORT,
            .a_pin = TB6612_ID_1_ENC_A_PIN, .b_pin = TB6612_ID_1_ENC_B_PIN
        }
    },
    // TB6612_ID_2
    {
        .dir = {
            .port = TB6612_ID_2_DIR_PORT,
            .in1_pin = TB6612_ID_2_DIR_IN1_PIN, .in2_pin = TB6612_ID_2_DIR_IN2_PIN
        },
        .pwm = {
            .tim = TB6612_PWM_TIMER, .tim_comp = TB6612_PWM_COMP_3,
            .tim_remap = GPIO_PartialRemap1_TIM2 | GPIO_Remap_SWJ_JTAGDisable,
            .prescaler = 3599, .period = 1,
            .port = TB6612_ID_2_PWM_PORT, .pin = TB6612_ID_2_PWM_PIN,
        },
        .enc = {
            .tim = TB6612_ID_2_ENC_TIMER, .tim_remap = 0,
            .tim_period = ENCODER_TIM_PERIOD, .port = TB6612_ID_2_ENC_PORT,
            .a_pin = TB6612_ID_2_ENC_A_PIN, .b_pin = TB6612_ID_2_ENC_B_PIN
        }
    },
    // TB6612_ID_3
    {
        .dir = {
            .port = TB6612_ID_3_DIR_PORT,
            .in1_pin = TB6612_ID_3_DIR_IN1_PIN, .in2_pin = TB6612_ID_3_DIR_IN2_PIN
        },
        .pwm = {
            .tim = TB6612_PWM_TIMER, .tim_comp = TB6612_PWM_COMP_4,
            .tim_remap = GPIO_PartialRemap1_TIM2 | GPIO_Remap_SWJ_JTAGDisable,
            .prescaler = 3599, .period = 1,
            .port = TB6612_ID_3_PWM_PORT, .pin = TB6612_ID_3_PWM_PIN,
        },
        .enc = {
            .tim = TB6612_ID_3_ENC_TIMER, .tim_remap = 0,
            .tim_period = ENCODER_TIM_PERIOD, .port = TB6612_ID_3_ENC_PORT,
            .a_pin = TB6612_ID_3_ENC_A_PIN, .b_pin = TB6612_ID_3_ENC_B_PIN
        }
    },
};
static sMotor g_motor_ctrl[MOTOR_NUM];
static float g_motor_dist_1mm[MOTOR_NUM] = {
    FL_DISRANCE, FR_DISRANCE, BL_DISRANCE, BR_DISRANCE
};
static uint8_t g_motor_tb6612_map[] = {
    [FL_MOTOR] = TB6612_ID_3,
    [FR_MOTOR] = TB6612_ID_2,
    [BL_MOTOR] = TB6612_ID_1,
    [BR_MOTOR] = TB6612_ID_0
};

void motor_set_dir(uint8_t id, uint8_t dir)
{
    if (g_motor_ctrl[id].is_stop) {
        return;
    }
    
    tb6612_set_dir(&g_tb6612_ctx[g_motor_tb6612_map[id]], dir);
}

uint8_t motor_get_dir_val(uint8_t id)
{
    return g_motor_ctrl[id].dir;
}

void motor_update_dir_val(uint8_t id)
{
    int16_t pwm = g_motor_ctrl[id].pwm;
    if (pwm > 0) {
        g_motor_ctrl[id].dir = TB6612_FORWARD;
    } else if (pwm < 0) {
        g_motor_ctrl[id].dir = TB6612_BACK;
    } else {
        g_motor_ctrl[id].dir = TB6612_STOP;
    }
}

int16_t motor_get_pwm_val(uint8_t id)
{
    return g_motor_ctrl[id].pwm;
}

void motor_set_pwm(uint8_t id, int16_t pwm)
{
    uint8_t tb6612_id;
    uint16_t pwm_abs;
    if (g_motor_ctrl[id].is_stop) {
        return;
    }
    // 限幅
    if (pwm > MAX_PWM) {	
        pwm = MAX_PWM;
    } else if(pwm < -MAX_PWM) {
        pwm = -MAX_PWM;
    }
    tb6612_id = g_motor_tb6612_map[id];
    pwm_abs = (uint16_t)ABS(pwm);
    tb6612_set_pwm(&g_tb6612_ctx[tb6612_id], pwm_abs);
}

void motor_set_pwm_val(uint8_t id, int16_t pwm)
{
    if (g_motor_ctrl[id].is_stop) {
        return;
    }
    g_motor_ctrl[id].pwm = pwm;
}

void motor_inc_pwm_val(uint8_t id, int16_t pwm)
{
    if (g_motor_ctrl[id].is_stop) {
        return;
    }
    g_motor_ctrl[id].pwm += pwm;
}

void motor_update_encoder(void)
{
    uint8_t id, tb6612_id;
    for (id = 0; id < MOTOR_NUM; id++) {
        tb6612_id = g_motor_tb6612_map[id];
        g_motor_ctrl[id].encoder = 
            tb6612_encoder_read_and_reset(&g_tb6612_ctx[tb6612_id], ENCODER_TIM_INIT_VAL) -
            ENCODER_TIM_INIT_VAL;
        // 小车前进时编码器的正负与tb6612方向引脚接线相关，此处前进统一换算为正数
        if (id == FL_MOTOR || id == BL_MOTOR) {
            g_motor_ctrl[id].encCnter += g_motor_ctrl[id].encoder;
        } else {
            g_motor_ctrl[id].encCnter -= g_motor_ctrl[id].encoder;
        }
    }
}

int16_t motor_get_encoder_val(uint8_t id)
{
    return g_motor_ctrl[id].encoder;
}

void motor_update_distance(void)
{
    uint8_t id;
    for (id = 0; id < MOTOR_NUM; id++) {
        g_motor_ctrl[id].distance = g_motor_ctrl[id].encCnter / g_motor_ctrl[id].dist_1mm;
    }
}

// encCnter与distance为换算关系，一起清除
void motor_clear_distance(uint8_t id)
{
    g_motor_ctrl[id].encCnter = 0;
    g_motor_ctrl[id].distance = 0;
}

//计算电机带动轮子的路程
float motor_get_distance_val(uint8_t id)
{
    return g_motor_ctrl[id].distance;
}

void motor_set_stop(uint8_t id)
{
    motor_set_dir(id, TB6612_STOP);
    motor_set_pwm(id, 0);
    g_motor_ctrl[id].pwm = 0;
    g_motor_ctrl[id].is_stop = true;
}

void motor_cancel_stop(uint8_t id)
{
    g_motor_ctrl[id].is_stop = false;
}

static void motor_ctrl_init_one(uint8_t id)
{
    sMotor *motor = &g_motor_ctrl[id];
    motor->id = id;
    motor->dir = TB6612_STOP;
    motor->pwm = 0;
    motor->encoder = 0;
    motor->encCnter = 0;
    motor->is_stop = true;
    motor->distance = 0;
    motor->dist_1mm = g_motor_dist_1mm[id];
}

static void motor_ctrl_init(void)
{
    uint8_t id;
    for (id = 0; id < MOTOR_NUM; id++) {
        motor_ctrl_init_one(id);
    }
}

static void motor_encoder_init(void)
{
    uint8_t id, tb6612_id;
    for (id = 0; id < MOTOR_NUM; id++) {
        tb6612_id = g_motor_tb6612_map[id];
        tb6612_encoder_read_and_reset(&g_tb6612_ctx[tb6612_id], ENCODER_TIM_INIT_VAL);
    }
}

static void motor_start(void)
{
    uint8_t id;
    for (id = 0; id < TB6612_ID_MAX; id++) {
        tb6612_start(&g_tb6612_ctx[id]);
    }
    for (id = 0; id < MOTOR_NUM; id++) {
        motor_set_dir(id, TB6612_STOP);
        motor_set_pwm(id, 0);
    }
}

void motor_init(void)
{
    uint8_t id;

    rcc_enable(RCC_APB1, RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4);
    rcc_enable(RCC_APB2, RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
        RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_AFIO);

    for (id = 0; id < TB6612_ID_MAX; id++) {
        tb6612_init(&g_tb6612_ctx[id]);
    }
    motor_encoder_init();
    motor_ctrl_init();
    motor_start();
}

//打印电机参数
void motor_print(uint8_t id)
{
    switch(id){
        case FL_MOTOR: printf("FLMotor - "); break;
        case FR_MOTOR: printf("FRMotor - "); break;
        case BL_MOTOR: printf("BLMotor - "); break;
        case BR_MOTOR: printf("BRMotor - "); break;
    }
    printf("pwm:%d - ", g_motor_ctrl[id].pwm);
    if(id == FL_MOTOR || id == BL_MOTOR) {
        printf("encoder:%d - ", g_motor_ctrl[id].encoder);
    } else if(id == FR_MOTOR || id == BR_MOTOR) {
        printf("encoder:%d - ", -g_motor_ctrl[id].encoder);
    }
    printf("encCnter:%ld - ", g_motor_ctrl[id].encCnter);
    printf("distance:%.2f - ", motor_get_distance_val(id));
    switch(g_motor_ctrl[id].dir){
        case TB6612_FORWARD:	printf("Forward"); break;
        case TB6612_BACK:      printf("Back");    break;
        case TB6612_STOP:      printf("Stop");    break;
    }
    printf(LEAF_LOG_NEW_LINE);
}

TB6612_DEF_TIMX_ENC_IRQ_HANDLE(3)
TB6612_DEF_TIMX_ENC_IRQ_HANDLE(8)
TB6612_DEF_TIMX_ENC_IRQ_HANDLE(1)
TB6612_DEF_TIMX_ENC_IRQ_HANDLE(4)
