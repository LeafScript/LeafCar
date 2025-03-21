project(debug_lib)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(INC_DIR ${SRC_DIR}/include)
set(BSP_DIR ${SRC_DIR}/bsp)
set(CONTROL_DIR ${SRC_DIR}/control)
set(SERVICE_DIR ${SRC_DIR}/service)
set(FOUNDATION_DIR ${SERVICE_DIR}/foundation)
set(TASK_DIR ${SERVICE_DIR}/task)
set(DEBUG_DIR ${SRC_DIR}/debug)

set(LED_DIR ${BSP_DIR}/led)

set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(CARARM_DIR ${CONTROL_DIR}/arm)
set(K210_DIR ${CONTROL_DIR}/k210)

set(CAR_TASK_DIR ${TASK_DIR}/car_task)
set(UPDATE_TASK_DIR ${TASK_DIR}/update_task)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

set(ARM_DIR ${3RD_DIR}/arm)
set(CM3_DIR ${ARM_DIR}/cmsis/cmsis_core/cm3)

set(ST_DIR ${3RD_DIR}/st)
set(ST_CMSIS_CORE_DIR ${ST_DIR}/cmsis/cmsis_core)
set(ST_SPL_INC_DIR ${ST_DIR}/cmsis/spl/inc)
set(ST_USER_DIR ${ST_DIR}/user_templete)
set(ST_USER_STM32F10X_DIR ${ST_USER_DIR}/stm32f10x)

include_directories(${DEBUG_DIR} ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USMART_DIR}
    ${CM3_DIR} ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR} ${FOUNDATION_DIR}
    ${LED_DIR} ${K210_DIR} ${CAR_DIR} ${CAR_TASK_DIR} ${UPDATE_TASK_DIR} ${PID_DIR} ${CARARM_DIR})

file(GLOB_RECURSE DEBUG_SRCS "${DEBUG_DIR}/*.c")
add_library(debug_lib STATIC ${DEBUG_SRCS})