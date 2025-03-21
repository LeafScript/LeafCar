#include "service_timer.h"
#include "error_code.h"

#define SERVICE_TIMER_MAX_REGISTER_NUM 10

typedef struct {
    bool is_registered;
    uint32_t trigger_ms;
    uint32_t trigger_times;     // 0: loop forever
    uint32_t counter;
    uint32_t times;
    void (*cb)(void *priv);
    void *priv;
} register_counter_s;

static uint64_t g_global_counter;
static uint32_t g_period_ms;
static uint32_t g_register_num;
register_counter_s g_register_counter[SERVICE_TIMER_MAX_REGISTER_NUM];

void service_timer_init(uint32_t period_ms)
{
    int i;

    g_global_counter = 0;
    g_period_ms = period_ms;
    g_register_num = 0;
    for (i = 0; i < ARRAY_SIZE(g_register_counter); i++) {
        g_register_counter[i].is_registered = false;
    }
}

static void service_timer_scan_one(register_counter_s *reg_counter)
{
    reg_counter->counter++;
    if (reg_counter->counter * g_period_ms < reg_counter->trigger_ms) {
        return;
    }
    reg_counter->counter = 0;
    reg_counter->times++;
    reg_counter->cb(reg_counter->priv);
    if (reg_counter->trigger_times != 0 && reg_counter->times >= reg_counter->trigger_times) {
        reg_counter->is_registered = false;
        reg_counter->cb = NULL;
    }
}

void service_timer_scan(void)
{
    register_counter_s *reg_counter = NULL;
    int i;

    g_global_counter++;
    for (i = 0; i < ARRAY_SIZE(g_register_counter); i++) {
        reg_counter = &g_register_counter[i];
        if (reg_counter->is_registered) {
            service_timer_scan_one(reg_counter);
        }
    }
}

int service_timer_trigger_muti(uint32_t trigger_ms, void (*cb)(void *priv), void *priv, uint32_t trigger_times)
{
    register_counter_s *reg_counter = NULL;
    int i;

    if (g_register_num >= SERVICE_TIMER_MAX_REGISTER_NUM ||
        (trigger_ms % g_period_ms) != 0 || cb == NULL) {
        return EC_ERROR;
    }
    for (i = 0; i < ARRAY_SIZE(g_register_counter); i++) {
        if (g_register_counter[i].is_registered == false) {
            reg_counter = &g_register_counter[i];
            break;
        }
    }
    reg_counter->is_registered = true;
    reg_counter->trigger_ms = trigger_ms;
    reg_counter->trigger_times = trigger_times;
    reg_counter->counter = 0;
    reg_counter->times = 0;
    reg_counter->cb = cb;
    reg_counter->priv = priv;
    g_register_num++;
    return EC_OK;
}

int service_timer_trigger_once(uint32_t trigger_ms, void (*cb)(void *priv), void *priv)
{
    return service_timer_trigger_muti(trigger_ms, cb, priv, 1);
}

int service_timer_loop(uint32_t trigger_ms, void (*cb)(void *priv), void *priv)
{
    return service_timer_trigger_muti(trigger_ms, cb, priv, 0);
}
