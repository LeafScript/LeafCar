cmake_minimum_required(VERSION 3.10)
enable_language(ASM)
project(leafcar)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O1 -mcpu=cortex-m3 -mthumb -g -ffreestanding \
    --specs=nosys.specs -u _printf_float")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -mcpu=cortex-m3 -mthumb")

add_definitions(-DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(PLAT_DIR ${SRC_DIR}/platform)
set(CMAKE_EXE_LINKER_FLAGS "-T ${PLAT_DIR}/STM32F103ZETX_FLASH.ld -g")
set(INC_DIR ${SRC_DIR}/include)
set(BSP_DIR ${SRC_DIR}/bsp)
set(COMMON_DIR ${SRC_DIR}/common)
set(CONTROL_DIR ${SRC_DIR}/control)
set(TASK_DIR ${SRC_DIR}/task)
set(DEBUG_DIR ${SRC_DIR}/debug)

set(LED_DIR ${BSP_DIR}/led)
set(REDWIRE_DIR ${BSP_DIR}/redwire)
set(TB6612_DIR ${BSP_DIR}/tb6612)
set(TIMER_DIR ${BSP_DIR}/timer)
set(USART_DIR ${BSP_DIR}/usart)

set(CAR_DIR ${CONTROL_DIR}/car)
set(CARARM_DIR ${CONTROL_DIR}/arm)
set(K210_DIR ${CONTROL_DIR}/k210)
set(SERVICE_TIMER_DIR ${CONTROL_DIR}/service_timer)

set(CARTASK_DIR ${TASK_DIR}/cartask)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

set(ARM_DIR ${3RD_DIR}/arm)
set(CM3_DIR ${ARM_DIR}/cmsis/cmsis_core/cm3)

set(ST_DIR ${3RD_DIR}/st)
set(ST_STARTUP_DIR ${ST_DIR}/startup/gcc)
set(ST_CMSIS_CORE_DIR ${ST_DIR}/cmsis/cmsis_core)
set(ST_SPL_INC_DIR ${ST_DIR}/cmsis/spl/inc)
set(ST_USER_DIR ${ST_DIR}/user_templete)
set(ST_USER_STM32F10X_DIR ${ST_USER_DIR}/stm32f10x)

include_directories(${COMMON_DIR} ${DEBUG_DIR} ${INC_DIR} ${TASK_DIR}
    ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR}
    ${USART_DIR}
    ${CAR_DIR} ${CARARM_DIR} ${K210_DIR} ${SERVICE_TIMER_DIR} ${CARTASK_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USMART_DIR}
    ${CM3_DIR} ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR})

file(GLOB_RECURSE ARM_SRCS "${ARM_DIR}/*.c")
add_library(arm_lib STATIC ${ARM_SRCS})
file(GLOB_RECURSE ST_SRCS "${ST_DIR}/*.c")
add_library(st_lib STATIC ${ST_SRCS})
file(GLOB_RECURSE ALIENTEK_SRCS "${ALIENTEK_DIR}/*.c")
list(FILTER ALIENTEK_SRCS EXCLUDE REGEX "${AT_USMART_DIR}/usmart_config_templete.c")
add_library(alientek_lib STATIC ${ALIENTEK_SRCS})

add_subdirectory(${BSP_DIR} bsp_lib)
add_subdirectory(${CONTROL_DIR} control_lib)
add_subdirectory(${TASK_DIR} task_lib)
add_subdirectory(${DEBUG_DIR} debug_lib)

add_executable(leafcar ${SRC_DIR}/main.c ${ST_STARTUP_DIR}/startup_stm32f103zetx.s)
target_link_libraries(leafcar arm_lib st_lib alientek_lib bsp_lib control_lib task_lib debug_lib)

# 将ELF文件转换为HEX/BIN文件
set(ELF_FILE leafcar)
set(HEX_FILE ${ELF_FILE}.hex)
set(BIN_FILE ${ELF_FILE}.bin)
set(ARM_OBJCOPY arm-none-eabi-objcopy)
set(ARM_SIZE arm-none-eabi-size)
set(ARM_NM arm-none-eabi-nm)
set(ARM_OBJDUMP arm-none-eabi-objdump)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_SIZE} ${ELF_FILE}
)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_NM} ${ELF_FILE} > ${ELF_FILE}.map
    COMMENT "Generating ${ELF_FILE}.map"
)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_OBJDUMP} -d ${ELF_FILE} > ${ELF_FILE}.asm
    COMMENT "Generating ${ELF_FILE}.asm"
)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex ${ELF_FILE} ${HEX_FILE}
    COMMENT "Converting ${ELF_FILE} to ${HEX_FILE}"
)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O binary ${ELF_FILE} ${BIN_FILE}
    COMMENT "Converting ${ELF_FILE} to ${BIN_FILE}"
)

add_custom_target(convert_to_hex_bin ALL DEPENDS ${TARGET})