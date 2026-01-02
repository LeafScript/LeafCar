cmake_minimum_required(VERSION 3.15)
enable_language(ASM)
project(leafcar C ASM)

if(NOT DEFINED PRODUCT)
    message(FATAL_ERROR "must define PRODUCT=[leafcar|standcar]")
endif()
message("PRODUCT=${PRODUCT}")

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -g -mcpu=cortex-m3 -mthumb -ffreestanding \
    --specs=nano.specs --specs=nosys.specs -u _printf_float -Wno-format-type-confusion")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -mcpu=cortex-m3 -mthumb")

add_compile_definitions(
    STM32F10X_HD
    USE_STDPERIPH_DRIVER
    # USE_FULL_ASSERT
)

# path definition
set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
# 转为绝对路径，消除../的相对路径问题
file(REAL_PATH ${PROJ_DIR} PROJ_DIR)
set(PROJ_DIRS
    3RD_DIR         ${PROJ_DIR}/3rd_party
    COMMON_DIR      ${PROJ_DIR}/common
    COMPONENT_DIR   ${PROJ_DIR}/component
    SRC_DIR         ${PROJ_DIR}/src
)
message(STATUS "ROOT_DIR: ${ROOT_DIR}")
message(STATUS "PROJ_DIR: ${PROJ_DIR}")
list(LENGTH PROJ_DIRS list_len)
math(EXPR list_len "${list_len} - 1")
foreach(idx RANGE 0 ${list_len} 2)
    list(GET PROJ_DIRS ${idx} dir_name)
    math(EXPR next_idx "${idx} + 1")
    list(GET PROJ_DIRS ${next_idx} dir_path)
    set(${dir_name} ${dir_path})
    message(STATUS "Set ${dir_name} = ${${dir_name}}")
endforeach()

# 3rd_party
set(ALIENTEK_DIR      ${3RD_DIR}/alientek)
set(AT_SYS_DIR        ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR      ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR     ${ALIENTEK_DIR}/usmart)
set(ARM_DIR           ${3RD_DIR}/arm)
set(CM3_DIR           ${ARM_DIR}/cmsis/cmsis_core/cm3)
set(ST_DIR            ${3RD_DIR}/st)
set(ST_STARTUP_DIR    ${ST_DIR}/startup/gcc)
set(ST_CMSIS_CORE_DIR ${ST_DIR}/cmsis/cmsis_core)
set(ST_SPL_INC_DIR    ${ST_DIR}/cmsis/spl/inc)
set(ST_USER_DIR       ${ST_DIR}/user_templete)
set(ST_USER_STM32F10X_DIR ${ST_USER_DIR}/stm32f10x)

# common
set(COMMON_SRC_DIR ${COMMON_DIR}/src)
set(COMMON_INC_DIR ${COMMON_DIR}/inc)

# component
set(LEAFCLI_DIR       ${COMPONENT_DIR}/leafcli)
set(TASK_GEN_DIR      ${COMPONENT_DIR}/task_gen)
set(SERVICE_TIMER_DIR ${COMPONENT_DIR}/service_timer)

# src
set(PLAT_DIR          ${SRC_DIR}/platform)
set(PLAT_LEAFCAR_DIR  ${PLAT_DIR}/leafcar)
set(SERVICE_DIR       ${SRC_DIR}/service)
set(CONTROL_DIR       ${SRC_DIR}/control)
set(BSP_DIR           ${SRC_DIR}/bsp)
set(DEBUG_DIR         ${SRC_DIR}/debug)

# service
set(FOUNDATION_DIR     ${SERVICE_DIR}/foundation)
set(TASK_DIR           ${SERVICE_DIR}/task)
set(CAR_TASK_DIR       ${TASK_DIR}/car_task)
set(CLI_TASK_DIR       ${TASK_DIR}/cli_task)
set(UPDATE_FW_TASK_DIR ${TASK_DIR}/update_fw_task)

# control
if(PRODUCT STREQUAL "leafcar")
    set(CONTROL_SUB_DIR ${CONTROL_DIR}/control_leafcar)
    set(CAR_DIR     ${CONTROL_SUB_DIR}/car)
    set(CARARM_DIR  ${CONTROL_SUB_DIR}/arm)
    set(MOTOR_DIR   ${CONTROL_SUB_DIR}/motor)
    set(K210_DIR    ${CONTROL_SUB_DIR}/k210)
    set(OPENMV_DIR  ${CONTROL_SUB_DIR}/openmv)
    set(PID_DIR     ${CONTROL_SUB_DIR}/pid)
    set(CONTROL_DIRS ${CAR_DIR} ${CARARM_DIR} ${MOTOR_DIR}
        ${K210_DIR} ${OPENMV_DIR} ${PID_DIR})
elseif(PRODUCT STREQUAL "standcar")
    set(CONTROL_SUB_DIR ${CONTROL_DIR}/control_standcar)
endif()

# bsp
set(BSP_SHARED_DIR  ${BSP_DIR}/bsp_shared)
if(PRODUCT STREQUAL "leafcar")
    set(BSP_LEAFCAR_DIR ${BSP_DIR}/bsp_leafcar)
    set(TB6612_DIR      ${BSP_SHARED_DIR}/tb6612)
    set(USART_DIR       ${BSP_SHARED_DIR}/usart)
    set(LED_DIR         ${BSP_LEAFCAR_DIR}/led)
    set(REDWIRE_DIR     ${BSP_LEAFCAR_DIR}/redwire)
    set(TIMER_DIR       ${BSP_LEAFCAR_DIR}/timer)
    set(BSP_DIRS ${TB6612_DIR} ${USART_DIR}
        ${LED_DIR} ${REDWIRE_DIR} ${TIMER_DIR})
elseif(PRODUCT STREQUAL "standcar")
    set(BSP_STANDCAR_DIR ${BSP_DIR}/bsp_standcar)
endif()

# 验证目录是否存在
macro(verify_dir dir_name)
    if(NOT EXISTS ${dir_name})
        message(FATAL_ERROR "Directory ${dir_name} does NOT exist! Check the path.")
    else()
        message(STATUS "Verified directory: ${dir_name}")
    endif()
endmacro()

verify_dir(${LEAFCLI_DIR})
verify_dir(${TASK_GEN_DIR})
verify_dir(${SERVICE_TIMER_DIR})
verify_dir(${SERVICE_DIR})
verify_dir(${CONTROL_DIR})
verify_dir(${BSP_DIR})
verify_dir(${DEBUG_DIR})

add_library(project_common INTERFACE)

# 公共头文件路径
target_include_directories(project_common INTERFACE
    # 3rd_party
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${CM3_DIR}
    ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR}
    # common
    ${COMMON_INC_DIR}
    # component
    ${LEAFCLI_DIR} ${TASK_GEN_DIR} ${SERVICE_TIMER_DIR}
    # platform
    ${PLAT_DIR} ${PLAT_LEAFCAR_DIR}
    # service
    ${FOUNDATION_DIR} ${TASK_DIR} ${CAR_TASK_DIR} ${CLI_TASK_DIR} ${UPDATE_FW_TASK_DIR}
    # control
    ${CONTROL_DIRS}
    # bsp
    ${BSP_DIRS}
    # debug
    ${DEBUG_DIR}
)

# 用列表传递，CMake会自动拆分为独立选项，GCC正确解析
target_compile_options(project_common INTERFACE
    $<$<COMPILE_LANGUAGE:C>:${C_CPU_FLAGS}>
    $<$<COMPILE_LANGUAGE:ASM>:${ASM_CPU_FLAGS}>
)

# 传递宏定义
target_compile_definitions(project_common INTERFACE
    STM32F10X_HD
    USE_STDPERIPH_DRIVER
)

# 3rd_party 静态库
file(GLOB_RECURSE ARM_SRCS "${ARM_DIR}/*.c")
add_library(arm_lib STATIC ${ARM_SRCS})
target_link_libraries(arm_lib PRIVATE project_common)

file(GLOB_RECURSE ST_SRCS "${ST_DIR}/*.c")
add_library(st_lib STATIC ${ST_SRCS})
target_link_libraries(st_lib PRIVATE project_common)

file(GLOB_RECURSE ALIENTEK_SRCS "${ALIENTEK_DIR}/*.c")
list(FILTER ALIENTEK_SRCS EXCLUDE REGEX "${AT_USMART_DIR}")
add_library(alientek_lib STATIC ${ALIENTEK_SRCS})
target_link_libraries(alientek_lib PRIVATE project_common st_lib)

# common 静态库
file(GLOB_RECURSE COMMON_SRCS "${COMMON_SRC_DIR}/*.c")
add_library(common_lib STATIC ${COMMON_SRCS})
target_link_libraries(common_lib PRIVATE project_common)

# 子目录编译
add_subdirectory(${LEAFCLI_DIR} leafcli_lib)
add_subdirectory(${TASK_GEN_DIR} task_gen_lib)
add_subdirectory(${SERVICE_TIMER_DIR} service_timer_lib)
add_subdirectory(${SERVICE_DIR} service_lib)
add_subdirectory(${CONTROL_DIR} control_lib)
add_subdirectory(${BSP_DIR} bsp_lib)
add_subdirectory(${DEBUG_DIR} debug_lib)

set(CMAKE_EXE_LINKER_FLAGS "-T ${PLAT_DIR}/STM32F103ZETX_FLASH.ld -g")

add_executable(leafcar 
    ${SRC_DIR}/main.c 
    ${ST_STARTUP_DIR}/startup_stm32f103zetx.s
)

target_link_libraries(leafcar PRIVATE
    # 3rd_party
    arm_lib st_lib alientek_lib
    # common
    common_lib
    # component
    leafcli_lib task_gen_lib service_timer_lib
    # src
    bsp_lib control_lib service_lib debug_lib
    # 公共配置
    project_common
)

# 自定义命令
find_program(ARM_OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(ARM_SIZE    arm-none-eabi-size REQUIRED)
find_program(ARM_NM      arm-none-eabi-nm REQUIRED)
find_program(ARM_OBJDUMP arm-none-eabi-objdump REQUIRED)

set(ELF_FILE ${PROJECT_NAME})
set(HEX_FILE ${ELF_FILE}.hex)
set(BIN_FILE ${ELF_FILE}.bin)
set(MAP_FILE ${ELF_FILE}.map)
set(ASM_FILE ${ELF_FILE}.asm)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${ARM_SIZE} --format=berkeley ${ELF_FILE}
    COMMAND ${ARM_NM} -n ${ELF_FILE} > ${MAP_FILE}  # -n：按地址排序，更易读
    COMMAND ${ARM_OBJDUMP} -S -d -C ${ELF_FILE} > ${ASM_FILE}
    COMMAND ${ARM_OBJCOPY} -O ihex ${ELF_FILE} ${HEX_FILE}
    COMMAND ${ARM_OBJCOPY} -O binary ${ELF_FILE} ${BIN_FILE}
    COMMENT "Post-build steps: size -> map -> asm -> hex -> bin"
)

add_custom_target(convert_to_hex_bin ALL DEPENDS ${ELF_FILE})