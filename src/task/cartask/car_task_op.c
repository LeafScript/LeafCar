#include "car_task_op.h"
#include "car_op.h"
#include "arm.h"
#include "redwire.h"
#include "carled.h"
#include "service_timer.h"
#include "usart.h"
#include "error_code.h"
#include "car_task_schedule.h"

static void car_op_async_cb(void)
{
    car_task_async_cb(NULL);
}

static int car_op_car_handle(uint8_t op, car_op_param_s *param)
{
    if (op == CAR_OP_FORWARD) {
        car_forward(param->dist, param->speed);
    } else if (op == CAR_OP_BACK) {
        car_back(param->dist, param->speed);
    } else if (op == CAR_OP_TRACK) {
        car_track(param->dist, param->speed, param->is_forward);
    } else if (op == CAR_OP_TURN) {
        car_turn(param->dist, param->speed, param->is_right);
    } else {
        printf("invalid car op[%u]\r\n", op);
        return EC_ERROR;
    }
    return EC_OK;
}

static int car_op_arm_handle(uint8_t op)
{
    if (op == ARM_OP_FOLD) {
        Arm_SetStatus(1);
    } else if (op == ARM_OP_MIDDLE) {
        Arm_SetStatus(2);
    } else if (op == ARM_OP_LEFT) {
        Arm_SetStatus(3);
    } else {
        printf("invalid arm op[%u]\r\n", op);
        return EC_ERROR;
    }
    car_op_async_cb();
    return EC_OK;
}

static int car_op_redwire_handle(uint8_t op, redwire_op_param_s *param)
{
    if (op == REDWIRE_OP_IS_FULL) {
        if (Redwire_isFull()) {
            param->cb(param->priv);
        }
    } else if (op == REDWIRE_OP_IS_NOT_FULL) {
        if (!Redwire_isFull()) {
            param->cb(param->priv);
        }
    } else {
        printf("invalid redwire op[%u]\r\n", op);
        return EC_ERROR;
    }
    return EC_OK;
}

static int car_op_led_handle(uint8_t op, led_op_param_s *param)
{
    uint8_t is_on = (op == LED_OP_ON) ? 1 : 0;
    if (param->color == LED_RED) {
        RedLed = is_on;
    } else if (param->color == LED_YELLOW) {
        YelLed = is_on;
    } else if (param->color == LED_GREEN) {
        GreLed = is_on;
    } else {
        printf("invalid led op[%u]\r\n", op);
        return EC_ERROR;
    }
    car_op_async_cb();
    return EC_OK;
}

static int car_op_timer_handle(uint8_t op, timer_op_param_s *param)
{
    if (op == TIMER_OP_TRIGGER_ONCE) {
        if (service_timer_trigger_once(param->trigger_ms, param->cb, param->priv) != EC_OK) {
            printf("service timer trigger once error\r\n");
            return EC_ERROR;
        }
    } else if (op == TIMER_OP_TRIGGER_MUTI) {
        if (service_timer_trigger_muti(param->trigger_ms, param->cb, param->priv, param->trigger_times) != EC_OK) {
            printf("service timer trigger muti error\r\n");
            return EC_ERROR;
        }
    } else {
        printf("invalid timer op[%u]\r\n", op);
        return EC_ERROR;
    }
    return EC_OK;
}

static int car_op_op_handle(uint8_t op, op_op_param_s *param)
{
    uint16_t to_step;
    int ret;

    if (op == OP_OP_JUMP_TO) {
        to_step = param->step;
    } else if (op == OP_OP_JUMP_PREV) {
        to_step = car_task_get_op_index() - param->step;
    } else if (op == OP_OP_JUMP_NEXT) {
        to_step = car_task_get_op_index() + param->step;
    } else if (op == OP_OP_FUNC_JUMP_TO) {
        if (param->get_step == NULL) {
            return EC_ERROR;
        }
        to_step = param->get_step();
    } else if (op == OP_OP_FUNC_JUMP_PREV) {
        if (param->get_step == NULL) {
            return EC_ERROR;
        }
        to_step = car_task_get_op_index() - param->get_step();
    } else if (op == OP_OP_FUNC_JUMP_NEXT) {
        if (param->get_step == NULL) {
            return EC_ERROR;
        }
        to_step = car_task_get_op_index() + param->get_step();
    } else if (op == OP_OP_END) {
        return EC_ERROR;
    } else {
        printf("invalid op op[%u]\r\n", op);
        return EC_ERROR;
    }
    ret = car_task_set_op_index(to_step);
    if (ret != EC_OK) {
        return EC_ERROR;
    }
    car_op_async_cb();
    return EC_OK;
}

int car_op_handle(car_op_s *op)
{
    switch (op->op_obj) {
        case CAR_OP_OBJ_CAR:
            return car_op_car_handle(op->op, &op->car_param);
        case CAR_OP_OBJ_ARM:
            return car_op_arm_handle(op->op);
        case CAR_OP_OBJ_REDWIRE:
            return car_op_redwire_handle(op->op, &op->redwire_param);
        case CAR_OP_OBJ_LED:
            return car_op_led_handle(op->op, &op->led_param);
        case CAR_OP_OBJ_TIMER:
            return car_op_timer_handle(op->op, &op->timer_param);
        case CAR_OP_OBJ_OP:
            return car_op_op_handle(op->op, &op->op_param);
        default:
            printf("invalid car op obj[%u]\r\n", op->op_obj);
            return EC_ERROR;
    }
}