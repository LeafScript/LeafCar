cmake_minimum_required(VERSION 3.10)
enable_language(ASM)
project(stm32f103c8t6_test)
set(TARGET ${PROJECT_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O1 -mcpu=cortex-m3 -mthumb -g -ffreestanding \
    --specs=nosys.specs -u _printf_float")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -mcpu=cortex-m3 -mthumb")

add_definitions(-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/../../../../../..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_EXE_LINKER_FLAGS "-T ${SRC_DIR}/STM32F103C8TX_FLASH.ld -g")

set(DOYOUNG_DIR ${3RD_DIR}/doyoung)
set(DY_USART_DIR ${DOYOUNG_DIR}/usart)

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

include_directories(${DY_USART_DIR} ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USMART_DIR}
    ${CM3_DIR} ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR})

file(GLOB_RECURSE ARM_SRCS "${ARM_DIR}/*.c")
add_library(arm_lib STATIC ${ARM_SRCS})
file(GLOB_RECURSE ST_SRCS "${ST_DIR}/*.c")
add_library(st_lib STATIC ${ST_SRCS})
file(GLOB_RECURSE DOYOUNG_SRCS "${DOYOUNG_DIR}/*.c")
add_library(doyoung_lib STATIC ${DOYOUNG_SRCS})
file(GLOB_RECURSE ALIENTEK_SRCS "${ALIENTEK_DIR}/*.c")
add_library(alientek_lib STATIC ${ALIENTEK_SRCS})

add_executable(${TARGET} ${SRC_DIR}/main.c ${ST_STARTUP_DIR}/startup_stm32f103c8tx.s)
target_link_libraries(${TARGET} alientek_lib doyoung_lib st_lib arm_lib)

# 将ELF文件转换为HEX/BIN文件
set(ELF_FILE ${TARGET})
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