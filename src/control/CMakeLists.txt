project(control_lib)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(INC_DIR ${SRC_DIR}/include)
set(COMMON_DIR ${SRC_DIR}/common)
set(BSP_DIR ${SRC_DIR}/bsp)
set(CONTROL_DIR ${SRC_DIR}/control)

set(LED_DIR ${BSP_DIR}/led)
set(REDWIRE_DIR ${BSP_DIR}/redwire)
set(TB6612_DIR ${BSP_DIR}/tb6612)
set(TIMER_DIR ${BSP_DIR}/timer)
set(USART_DIR ${BSP_DIR}/usart)

set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(MOTOR_DIR ${CONTROL_DIR}/motor)
set(K210_DIR ${CONTROL_DIR}/k210)
set(OPENMV_DIR ${CONTROL_DIR}/openmv)
set(SERVICE_TIMER_DIR ${CONTROL_DIR}/service_timer)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

include_directories(${COMMON_DIR}
    ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${USART_DIR}
    ${CAR_DIR} ${PID_DIR} ${MOTOR_DIR} ${K210_DIR} ${OPENMV_DIR} ${SERVICE_TIMER_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USMART_DIR})

file(GLOB_RECURSE CONTROL_SRCS "${CONTROL_DIR}/*.c")
add_library(control_lib STATIC ${CONTROL_SRCS})
target_link_libraries(control_lib bsp_lib)