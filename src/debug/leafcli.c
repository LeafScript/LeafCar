#include "leafcli.h"
#include <string.h>
#include <stdlib.h>
#include "base_type.h"
#include "error_code.h"
#include "usart.h"

#if (LEAFCLI_USE_CRLF == 1)
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif
#define leafcli_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define leafcli_printf_newline(fmt, ...) printf(fmt NEWLINE, ##__VA_ARGS__)

static void leafcli_show_help(void);
static void leafcli_show_registerd_cmd(void);

static leafcli_cmd_s g_builtin_cmd_list[] = {
    { "help", leafcli_show_help, "show help" },
    { "reg_cmd", leafcli_show_registerd_cmd, "show registered command list" },
};
static leafcli_context_s g_builtin_reg_ctx = {
    .group_id = LEAFCLI_GROUP_ID_ALL,
    .cmd_list_name = "builtin cmd list",
    .cmd_num = ARRAY_SIZE(g_builtin_cmd_list),
    .cmd_list = g_builtin_cmd_list,
};
static leafcli_buffer_s *g_reg_buff_list[LEAFCLI_MAX_BUFF_NUM] = { 0 };
static uint8_t g_reg_buff_num = 0;
static leafcli_context_s *g_reg_ctx_list[LEAFCLI_MAX_CTX_NUM] = { 0 };
static uint8_t g_reg_ctx_num = 0;

static void leafcli_show_help(void)
{
    leafcli_context_s *ctx = &g_builtin_reg_ctx;
    uint32_t i;

    leafcli_printf_newline(">>>>> LEAFCLI BULITIN COMMAND LIST <<<<<");
    for (i = 0; i < ctx->cmd_num; i++) {
        leafcli_printf_newline("%s: %s", ctx->cmd_list[i].cmd_name, ctx->cmd_list[i].cmd_desc);
    }
}

static void leafcli_show_registerd_cmd(void)
{
    leafcli_context_s *ctx = NULL;
    uint32_t i, j;

    leafcli_printf_newline(">>>>> LEAFCLI REGISTERED COMMAND LIST <<<<<");
    for (i = 1; i < g_reg_ctx_num; i++) {
        leafcli_printf_newline("> %s:", ctx->cmd_list_name);
        ctx = g_reg_ctx_list[i];
        for (j = 0; j < ctx->cmd_num; j++) {
            if (ctx->cmd_list[j].cmd_desc == NULL) {
                leafcli_printf_newline("%s", ctx->cmd_list[j].cmd_name);
            } else {
                leafcli_printf_newline("%s: %s", ctx->cmd_list[j].cmd_name, ctx->cmd_list[j].cmd_desc);
            }
        }
    }
}

int leafcli_register_buff(leafcli_buffer_s *buff)
{
    uint32_t i;

    if (g_reg_buff_num >= LEAFCLI_MAX_BUFF_NUM || buff == NULL) {
        leafcli_printf_newline("leafcli: buff_num[%u] buff[%u]", g_reg_buff_num, buff == NULL);
        return EC_ERROR;
    }
    if (buff->fifo == NULL || buff->fifo_size == 0) {
        leafcli_printf_newline("leafcli: fifo[%u] fifo_size[%u]", buff->fifo == NULL, buff->fifo_size);
        return EC_ERROR;
    }
    for (i = 0; i < g_reg_buff_num; i++) {
        if (g_reg_buff_list[i]->group_id == buff->group_id || g_reg_buff_list[i]->fifo == buff->fifo) {
            leafcli_printf_newline("leafcli: group_id[%u,%u] fifo[%u]",
                g_reg_buff_list[i]->group_id, buff->group_id, g_reg_buff_list[i]->fifo == buff->fifo);
            return EC_ERROR;
        }
    }
    buff->rd_index = 0;
    buff->wr_index = 0;
    buff->recv_cmd_num = 0;
    buff->proc_cmd_num = 0;
    buff->full_flag = false;
    g_reg_buff_list[g_reg_buff_num] = buff;
    g_reg_buff_num++;
    return EC_OK;
}

static leafcli_buffer_s *leafcli_get_reg_buff(uint8_t group_id)
{
    uint32_t i;
    for (i = 0; i < g_reg_buff_num; i++) {
        if (g_reg_buff_list[i]->group_id == group_id) {
            return g_reg_buff_list[i];
        }
    }
    return NULL;    // IMPOSIBLE
}

int leafcli_register_ctx(leafcli_context_s *ctx)
{
    uint32_t i;

    if (g_reg_ctx_num >= LEAFCLI_MAX_CTX_NUM || ctx == NULL) {
        leafcli_printf_newline("leafcli: ctx_num[%u] ctx[%u]", g_reg_ctx_num, ctx == NULL);
        return EC_ERROR;
    }
    if (leafcli_get_reg_buff(ctx->group_id) == NULL) {
        leafcli_printf_newline("leafcli: group_id[%u] not registed", ctx->group_id);
        return EC_ERROR;
    }
    if (ctx->cmd_list_name == NULL) {
        leafcli_printf_newline("leafcli: list_name is NULL");
        return EC_ERROR;
    }
    if (ctx->cmd_num == 0 || ctx->cmd_list == NULL) {
        leafcli_printf_newline("leafcli: cmd_num[%u] cmd_list[%u]", ctx->cmd_num, ctx->cmd_list == NULL);
        return EC_ERROR;
    }
    for (i = 0; i < ctx->cmd_num; i++) {
        if (ctx->cmd_list[i].cmd_name == NULL) {
            leafcli_printf_newline("leafcli: cmd_name is NULL");
            return EC_ERROR;
        }
        if (strchr(ctx->cmd_list[i].cmd_name, ' ') != NULL) {
            leafcli_printf_newline("leafcli: cmd_name[%s] has space", ctx->cmd_list[i].cmd_name);
            return EC_ERROR;
        }
    }
    g_reg_ctx_list[g_reg_ctx_num] = ctx;
    g_reg_ctx_num++;
    return EC_OK;
}

int leafcli_register_builtin_ctx(void)
{
    return leafcli_register_ctx(&g_builtin_reg_ctx);
}

// '/r' or '/n' represent the end of a command
void leafcli_recv_data(uint8_t ch, uint8_t group_id)
{
    leafcli_buffer_s *buff = leafcli_get_reg_buff(group_id);
    if (buff == NULL) {
        return;
    }
    if (buff->wr_index - buff->rd_index >= buff->fifo_size) {
        buff->full_flag = true;
        if (buff->recv_cmd_num == buff->proc_cmd_num) {
            leafcli_printf_newline("leafcli: clean full fifo, group_id[%u]", group_id);
            buff->rd_index = buff->wr_index;
        }
        return;
    }
    if (ch == '\0') {
        return;
    } else if (ch == '\r' || ch == '\n') {
        ch = '\0';
    }
    if (ch == '\0') {
        buff->recv_cmd_num++;
    }
    buff->fifo[buff->wr_index] = ch;
    buff->wr_index++;
}

static bool leafcli_cmd_comp(leafcli_buffer_s *buff, uint8_t *cmd, uint32_t cmd_len)
{
    uint8_t last_ch;
    uint32_t i;
    for (i = 0; i < cmd_len; i++) {
        if (buff->fifo[(buff->rd_index + i) % buff->fifo_size] != cmd[i]) {
            return false;
        }
    }
    last_ch = buff->fifo[(buff->rd_index + cmd_len) % buff->fifo_size];
    if (last_ch != '\0' && last_ch != ' ') {
        return false;
    }
    return true;
}

static bool leafcli_parse_cmd(leafcli_buffer_s *buff, uint8_t *cmd_list_name, uint8_t *cmd_name)
{
    uint32_t cmd_len = strlen(cmd_name);

    if (buff->recv_cmd_num <= buff->proc_cmd_num) {
        return false;
    }
    if (leafcli_cmd_comp(buff, cmd_name, cmd_len)) {
        leafcli_printf_newline("> %s: %s", cmd_list_name, cmd_name);
        buff->rd_index += cmd_len + 1;
        return true;
    }
    return false;
}

static bool leafcli_parse_cmd_param(leafcli_buffer_s *buff, uint32_t *param, uint8_t *param_num)
{
    uint8_t param_str[LEAFCLI_MAX_PARAM_NUM][LEAFCLI_MAX_PARAM_LEN + 1] = { 0 };
    uint8_t param_index = 0;
    uint32_t str_index = 0;
    uint8_t ch = buff->fifo[buff->rd_index % buff->fifo_size];
    while (ch != '\0') {
        ch = buff->fifo[buff->rd_index % buff->fifo_size];
        if (param_index >= LEAFCLI_MAX_PARAM_NUM || str_index >= LEAFCLI_MAX_PARAM_LEN + 1) {
            leafcli_printf_newline("leafcli: param[%u] str_index[%u]", param_index, str_index);
            while (buff->fifo[buff->rd_index % buff->fifo_size] != '\0') {
                buff->rd_index++;
            }
            return false;
        }
        if ((ch < '0' || ch > '9') && ch != ' ') {
            leafcli_printf_newline("leafcli: param[%u] format invalid", param_index);
            while (buff->fifo[buff->rd_index % buff->fifo_size] != '\0') {
                buff->rd_index++;
            }
            return false;
        }
        if (ch != ' ' && ch != '\0') {
            param_str[param_index][str_index] = ch;
            str_index++;
            buff->rd_index++;
            continue;
        }
        param[param_index] = atoi(param_str[param_index]);
        str_index = 0;
        param_index++;
        buff->rd_index++;
    }
    *param_num = param_index;
    return true;
}

// return value parse as uint32_t
static void leafcli_exec_cmd(void *cmd_func, uint32_t *param, uint8_t param_num)
{
    uint32_t cmd_ret;
    switch (param_num) {
        case 0:
            cmd_ret = ((uint32_t (*)(void))cmd_func)();
            break;
        case 1:
            cmd_ret = ((uint32_t (*)(uint32_t))cmd_func)(param[0]);
            break;
        case 2:
            cmd_ret = ((uint32_t (*)(uint32_t, uint32_t))cmd_func)(param[0], param[1]);
            break;
        case 3:
            cmd_ret = ((uint32_t (*)(uint32_t, uint32_t, uint32_t))cmd_func)(param[0], param[1], param[2]);
            break;
        case 4:
            cmd_ret = ((uint32_t (*)(uint32_t, uint32_t, uint32_t, uint32_t))cmd_func)(param[0], param[1], param[2], param[3]);
            break;
        case 5:
            cmd_ret = ((uint32_t (*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))cmd_func)(param[0], param[1], param[2], param[3], param[4]);
            break;
        case 6:
            cmd_ret = ((uint32_t (*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))cmd_func)(param[0], param[1], param[2], param[3], param[4], param[5]);
            break;
        default:    // IMPOSIBLE
            leafcli_printf_newline("leafcli: param_num[%u] invalid", param_num);
            break;
    }
    leafcli_printf_newline("> %u (0x%x)", cmd_ret, cmd_ret);
}

void leafcli_scan(void)
{
    uint32_t param[LEAFCLI_MAX_PARAM_NUM] = { 0 };
    uint8_t param_num;
    leafcli_buffer_s *buff = NULL;
    leafcli_context_s *ctx = NULL;
    uint32_t i, j;
    for (i = 0; i < g_reg_ctx_num; i++) {
        ctx = g_reg_ctx_list[i];
        for (j = 0; j < ctx->cmd_num; j++) {
            buff = leafcli_get_reg_buff(ctx->group_id);
            if (!leafcli_parse_cmd(buff, ctx->cmd_list_name, ctx->cmd_list[j].cmd_name)) {
                continue;
            }
            if (leafcli_parse_cmd_param(buff, param, &param_num)) {
                leafcli_exec_cmd(ctx->cmd_list[j].cmd_func, param, param_num);
                buff->proc_cmd_num++;
            }
        }
    }
}