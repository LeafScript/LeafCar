#include "car_task_op.h"
#include "car_op.h"
#include "arm.h"
#include "usart.h"
#include "error_code.h"

static void car_op_car_handle(uint8_t op, car_op_param_s *param)
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
    }
}

static void car_op_arm_handle(uint8_t op)
{
    if (op == ARM_OP_FOLD) {
        Arm_SetStatus(1);
    } else if (op == ARM_OP_MIDDLE) {
        Arm_SetStatus(2);
    } else if (op == ARM_OP_LEFT) {
        Arm_SetStatus(3);
    } else {
        printf("invalid arm op[%u]\r\n", op);
    }
}

static void car_op_redwire_handle(uint8_t op, redwire_op_param_s *param)
{
    if (op == REDWIRE_OP_IS_FULL) {
        if (Redwire_isFull()) {
            param->cb();
        }
    } else if (op == REDWIRE_OP_IS_NOT_FULL) {
        if (!Redwire_isFull()) {
            param->cb();
        }
    } else {
        printf("invalid redwire op[%u]\r\n", op);
    }
}

int car_op_handle(car_op_s *op)
{
    switch (op->op_obj) {
        case CAR_OP_OBJ_CAR:
            car_op_car_handle(op->op, &op->car_param);
            break;
        case CAR_OP_OBJ_ARM:
            car_op_arm_handle(op->op);
            break;
        case CAR_OP_OBJ_REDWIRE:
            car_op_redwire_handle(op->op, &op->redwire_param);
            break;
        default:
            printf("invalid car op obj[%u]\r\n", op->op_obj);
            return EC_ERROR;
    }
    return EC_OK;
}