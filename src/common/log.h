#ifndef _LOG_H_
#define _LOG_H_

#include "base_type.h"
#include "usart.h"

#define LEAF_LOG_NEW_LINE "\r\n"

enum leaf_log_level_e {
    LOG_OFF = 0,
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE,
    LOG_ALL,
    LOG_LEVEL_NUM
};

#define LEAF_LOG(level, fmt, ...) \
do { \
    BUILD_BUG_ON(level < 0 || level > LOG_ALL); \
    if (level <= leaf_log_get_level()) { \
        printf("[%s:%d] " fmt LEAF_LOG_NEW_LINE, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
} while (0)

uint8_t leaf_log_get_level(void);
void leaf_log_set_level(uint8_t level);

#endif