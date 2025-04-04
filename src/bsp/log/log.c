#include "log.h"

static uint8_t g_log_level = LOG_ALL;

void leaf_log_set_level(uint8_t level)
{
    g_log_level = (level > LOG_ALL) ? LOG_ALL : level;
}

uint8_t leaf_log_get_level(void)
{
    return g_log_level;
}