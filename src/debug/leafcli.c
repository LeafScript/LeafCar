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

static uint32_t leafcli_show_help(void);
static uint32_t leafcli_show_registerd_cmd(void);
static uint32_t leafcli_show_registerd_buff(void);
static uint32_t leafcli_show_registerd_fifo(void);

static leafcli_cmd_s g_builtin_cmd_list[] = {
    { "help", leafcli_show_help, "show help" },
    { "reg_cmd", leafcli_show_registerd_cmd, "show registered command list" },
    { "buff", leafcli_show_registerd_buff, "show registered buffer info" },
    { "fifo", leafcli_show_registerd_fifo, "show registered fifo content" },
};
static leafcli_context_s g_builtin_reg_ctx = {
    .group_id = 0,
    .cmd_list_name = "builtin cmd list",
    .cmd_num = ARRAY_SIZE(g_builtin_cmd_list),
    .cmd_list = g_builtin_cmd_list,
};
static leafcli_buffer_s *g_reg_buff_list[LEAFCLI_MAX_BUFF_NUM] = { 0 };
static uint8_t g_reg_buff_num = 0;
static leafcli_context_s *g_reg_ctx_list[LEAFCLI_MAX_CTX_NUM] = { 0 };
static uint8_t g_reg_ctx_num = 0;

static uint32_t leafcli_show_help(void)
{
    leafcli_context_s *ctx = &g_builtin_reg_ctx;
    uint32_t i;

    leafcli_printf_newline(">>>>> LEAFCLI BULITIN COMMAND LIST <<<<<");
    for (i = 0; i < ctx->cmd_num; i++) {
        leafcli_printf_newline("%s: %s", ctx->cmd_list[i].cmd_name, ctx->cmd_list[i].cmd_desc);
    }
    return LEAFCLI_EC_OK;
}

static uint32_t leafcli_show_registerd_cmd(void)
{
    leafcli_context_s *ctx = NULL;
    uint32_t i, j;

    leafcli_printf_newline(">>>>> LEAFCLI REGISTERED COMMAND LIST <<<<<");
    for (i = 0; i < g_reg_ctx_num; i++) {
        ctx = g_reg_ctx_list[i];
        leafcli_printf_newline("> %s:", ctx->cmd_list_name);
        for (j = 0; j < ctx->cmd_num; j++) {
            if (ctx->cmd_list[j].cmd_desc == NULL) {
                leafcli_printf_newline("%s", ctx->cmd_list[j].cmd_name);
            } else {
                leafcli_printf_newline("%s: %s", ctx->cmd_list[j].cmd_name, ctx->cmd_list[j].cmd_desc);
            }
        }
    }
    return LEAFCLI_EC_OK;
}

static uint32_t leafcli_show_registerd_buff(void)
{
    leafcli_buffer_s *buff = NULL;
    uint32_t i;

    leafcli_printf_newline(">>>>> LEAFCLI BUFF LIST <<<<<");
    leafcli_printf_newline("group_id | size | rd_index | wr_index | recv_cmd | proc_cmd | parse_ch | full_flag");
    for (i = 0; i < g_reg_buff_num; i++) {
        buff = g_reg_buff_list[i];
        leafcli_printf_newline("%8u | %4u | %8u | %8u | %8u | %8u | %8u | %9s",
            buff->group_id, buff->fifo_size, buff->rd_index, buff->wr_index,
            buff->recv_cmd_num, buff->proc_cmd_num, buff->parse_ch_num, buff->full_flag ? "true" : "false");
    }
    return LEAFCLI_EC_OK;
}

#define FIFO_PRINT_LINE_BYTES 16

static uint32_t leafcli_show_registerd_fifo(void)
{
    uint8_t *fifo = NULL;
    uint32_t i, j, fifo_size;

    leafcli_printf_newline(">>>>> LEAFCLI FIFO LIST <<<<<");
    for (i = 0; i < g_reg_buff_num; i++) {
        fifo = g_reg_buff_list[i]->fifo;
        fifo_size = g_reg_buff_list[i]->fifo_size;
        leafcli_printf_newline("group_id: %u", g_reg_buff_list[i]->group_id);
        for (j = 0; j < fifo_size; j++) {
            if (j % FIFO_PRINT_LINE_BYTES == 0) {
                leafcli_printf("[%03u]-[%03u]: ", j, j + FIFO_PRINT_LINE_BYTES);
            }
            leafcli_printf("%02x ", fifo[j]);
            if (j % FIFO_PRINT_LINE_BYTES == FIFO_PRINT_LINE_BYTES - 1) {
                leafcli_printf_newline("");
            }
        }
    }
    return LEAFCLI_EC_OK;
}

int leafcli_register_buff(leafcli_buffer_s *buff)
{
    uint32_t i;

    if (g_reg_buff_num >= LEAFCLI_MAX_BUFF_NUM || buff == NULL) {
        leafcli_printf_newline("leafcli: buff_num[%u] buff[%u]", g_reg_buff_num, buff == NULL);
        return LEAFCLI_EC_ERROR;
    }
    if (buff->fifo == NULL || buff->fifo_size == 0) {
        leafcli_printf_newline("leafcli: fifo[%u] fifo_size[%u]", buff->fifo == NULL, buff->fifo_size);
        return LEAFCLI_EC_ERROR;
    }
    for (i = 0; i < g_reg_buff_num; i++) {
        if (g_reg_buff_list[i]->group_id == buff->group_id || g_reg_buff_list[i]->fifo == buff->fifo) {
            leafcli_printf_newline("leafcli: group_id[%u,%u] fifo[%u]",
                g_reg_buff_list[i]->group_id, buff->group_id, g_reg_buff_list[i]->fifo == buff->fifo);
            return LEAFCLI_EC_ERROR;
        }
    }
    buff->rd_index = 0;
    buff->wr_index = 0;
    buff->recv_cmd_num = 0;
    buff->proc_cmd_num = 0;
    buff->parse_ch_num = 0;
    buff->full_flag = false;
    g_reg_buff_list[g_reg_buff_num] = buff;
    g_reg_buff_num++;
    return LEAFCLI_EC_OK;
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
        return LEAFCLI_EC_ERROR;
    }
    if (leafcli_get_reg_buff(ctx->group_id) == NULL) {
        leafcli_printf_newline("leafcli: group_id[%u] not registed", ctx->group_id);
        return LEAFCLI_EC_ERROR;
    }
    if (ctx->cmd_list_name == NULL) {
        leafcli_printf_newline("leafcli: list_name is NULL");
        return LEAFCLI_EC_ERROR;
    }
    if (ctx->cmd_num == 0 || ctx->cmd_list == NULL) {
        leafcli_printf_newline("leafcli: cmd_num[%u] cmd_list[%u]", ctx->cmd_num, ctx->cmd_list == NULL);
        return LEAFCLI_EC_ERROR;
    }
    for (i = 0; i < ctx->cmd_num; i++) {
        if (ctx->cmd_list[i].cmd_name == NULL) {
            leafcli_printf_newline("leafcli: cmd_name is NULL");
            return LEAFCLI_EC_ERROR;
        }
        if (strchr(ctx->cmd_list[i].cmd_name, ' ') != NULL) {
            leafcli_printf_newline("leafcli: cmd_name[%s] has space", ctx->cmd_list[i].cmd_name);
            return LEAFCLI_EC_ERROR;
        }
        if (strlen(ctx->cmd_list[i].cmd_name) > LEAFCLI_MAX_CMD_LEN) {
            leafcli_printf_newline("leafcli: cmd_name[%s] is too long", ctx->cmd_list[i].cmd_name);
            return LEAFCLI_EC_ERROR;
        }
    }
    g_reg_ctx_list[g_reg_ctx_num] = ctx;
    g_reg_ctx_num++;
    return LEAFCLI_EC_OK;
}

int leafcli_register_builtin_ctx(uint8_t group_id)
{
    g_builtin_reg_ctx.group_id = group_id;
    return leafcli_register_ctx(&g_builtin_reg_ctx);
}

#define IS_CMD_END(ch) ((ch) == '\r' || (ch) == '\n')

// '/r' or '/n' represent the end of a command
// NOTE: ONLY modify the charactor in fifo when parse it,
//       so this function simplely write charactors into fifo
void leafcli_recv_data(uint8_t ch, uint8_t group_id)
{
    leafcli_buffer_s *buff = leafcli_get_reg_buff(group_id);
    uint8_t last_wr_ch;

    if (buff->wr_index >= buff->rd_index + buff->fifo_size) {
        if (!buff->full_flag && buff->fifo_full_cb != NULL) {
            buff->fifo_full_cb(group_id);
        }
        buff->full_flag = true;
        return;
    }
    last_wr_ch = (buff->wr_index > 0) ? buff->fifo[(buff->wr_index - 1) % buff->fifo_size] : '\0';
    if (IS_CMD_END(ch) && !IS_CMD_END(last_wr_ch)) {
        buff->recv_cmd_num++;
    }
    buff->fifo[buff->wr_index % buff->fifo_size] = ch;
    buff->wr_index++;
}

// make sure (index + offset) >= 0
static inline uint8_t get_fifo_rd_ch(leafcli_buffer_s *buff, uint32_t offset)
{
    return buff->fifo[(buff->rd_index + offset) % buff->fifo_size];
}

static bool leafcli_cmd_comp(leafcli_buffer_s *buff, uint8_t *cmd, uint32_t cmd_len)
{
    uint32_t offset = buff->parse_ch_num;
    uint8_t ch;
    uint32_t i;
    for (i = 0; i < cmd_len; i++) {
        if (get_fifo_rd_ch(buff, offset + i) != cmd[i]) {
            return false;
        }
    }
    ch = get_fifo_rd_ch(buff, offset + cmd_len);
    if (!IS_CMD_END(ch) && ch != ' ') {
        return false;
    }
    return true;
}

static bool leafcli_parse_cmd(leafcli_buffer_s *buff, uint8_t *cmd_name)
{
    uint32_t cmd_len = strlen(cmd_name);
    if (!leafcli_cmd_comp(buff, cmd_name, cmd_len)) {
        return false;
    }
    buff->parse_ch_num += cmd_len;
    return true;
}

static bool leafcli_parse_cmd_param(leafcli_buffer_s *buff, uint32_t *param, uint8_t *param_num)
{
    // param_str[param_index][str_index]
    uint8_t param_str[LEAFCLI_MAX_PARAM_NUM][LEAFCLI_MAX_PARAM_LEN + 1] = { 0 };
    uint8_t param_index = 0;
    uint32_t str_index = 0;
    uint32_t offset = buff->parse_ch_num;
    uint8_t ch = get_fifo_rd_ch(buff, offset);

    // parse cmd parameters
    while (!IS_CMD_END(ch) && offset < buff->fifo_size) {
        if (param_index >= LEAFCLI_MAX_PARAM_NUM || str_index >= LEAFCLI_MAX_PARAM_LEN + 1) {
            leafcli_printf_newline("leafcli: param[%u] str_index[%u]", param_index, str_index);
            return false;
        }
        if ((ch < '0' || ch > '9') && ch != ' ') {
            leafcli_printf_newline("leafcli: param[%u] format invalid, offset[%u] ch[0x%x]",
                param_index, offset, ch);
            return false;
        }
        if (ch == ' ') {
            param[param_index] = atoi(param_str[param_index]);
            param_index++;
            str_index = 0;
        } else {
            param_str[param_index][str_index] = ch;
            str_index++;
        }
        offset++;
        ch = get_fifo_rd_ch(buff, offset);
        // last param
        if (IS_CMD_END(ch)) {
            param[param_index] = atoi(param_str[param_index]);
            param_index++;
        }
    }
    *param_num = param_index;
    buff->parse_ch_num = offset;
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
    leafcli_printf_newline("[ret] %u (0x%x)", cmd_ret, cmd_ret);
}

static void skip_ch(leafcli_buffer_s *buff, bool (*skip_judge)(uint8_t ch))
{
    uint8_t ch = get_fifo_rd_ch(buff, buff->parse_ch_num);
    while (skip_judge(ch) && buff->parse_ch_num < buff->fifo_size) {
        buff->parse_ch_num++;
        ch = get_fifo_rd_ch(buff, buff->parse_ch_num);
    }
}

static bool cmd_end_and_space_judge(uint8_t ch) { return (IS_CMD_END(ch) || ch == ' '); }
static bool cmd_end_judge(uint8_t ch) { return IS_CMD_END(ch); }
static bool space_judge(uint8_t ch) { return (ch == ' '); }
static bool not_cmd_end_judge(uint8_t ch) { return !IS_CMD_END(ch); }
static void skip_cmd_end_and_space(leafcli_buffer_s *buff) { skip_ch(buff, cmd_end_and_space_judge); }
static void skip_cmd_end(leafcli_buffer_s *buff) { skip_ch(buff, cmd_end_judge); }
static void skip_space(leafcli_buffer_s *buff) { skip_ch(buff, space_judge); }

static void skip_to_cmd_end(leafcli_buffer_s *buff) { skip_ch(buff, not_cmd_end_judge); }

// clear command and it's parameters
static void fifo_rd_clear(leafcli_buffer_s *buff)
{
    uint32_t index = buff->rd_index;
    uint32_t len = buff->parse_ch_num;
    uint32_t i;

    if ((index + len) > buff->wr_index) {
        leafcli_printf_newline("leafcli: fifo_rd_clear abnormal, rd_index[%u] + parse_ch_num[%u] > wr_index[%u]",
            index, len, buff->wr_index);
        len = buff->fifo_size;
    }
    for (i = 0; i < len; i++) {
        buff->fifo[(index + i) % buff->fifo_size] = 0;
    }
    buff->rd_index += buff->parse_ch_num;
    buff->parse_ch_num = 0;
    if (buff->full_flag && buff->fifo_full_solved != NULL) {
        buff->fifo_full_solved(buff->group_id);
    }
    buff->full_flag = false;
}

static void print_parsed_param(uint32_t *param, uint8_t param_num)
{
    uint32_t i;

    if (param_num == 0) {
        return;
    }
    leafcli_printf("-> param: ");
    for (i = 0; i < param_num; i++) {
        leafcli_printf("%u ", param[i]);
    }
    leafcli_printf_newline("");
}

// process one command at a time
static void leafcli_parse_exec_cmd(leafcli_buffer_s *buff)
{
    uint32_t param[LEAFCLI_MAX_PARAM_NUM] = { 0 };
    uint8_t param_num;
    leafcli_context_s *ctx = NULL;
    uint32_t i, j;
    for (i = 0; i < g_reg_ctx_num; i++) {
        ctx = g_reg_ctx_list[i];
        if (ctx->group_id != buff->group_id) {
            continue;
        }
        for (j = 0; j < ctx->cmd_num; j++) {
            skip_cmd_end_and_space(buff);
            if (!leafcli_parse_cmd(buff, ctx->cmd_list[j].cmd_name)) {
                continue;
            }
            leafcli_printf_newline("-> %s: %s", ctx->cmd_list_name, ctx->cmd_list[j].cmd_name);
            skip_space(buff);
            if (!leafcli_parse_cmd_param(buff, param, &param_num)) {
                continue;
            }
            leafcli_printf_newline("DEBUG: param_num[%u]", param_num);
            print_parsed_param(param, param_num);
            skip_cmd_end(buff);
            leafcli_exec_cmd(ctx->cmd_list[j].cmd_func, param, param_num);
            fifo_rd_clear(buff);
            return;
        }
    }
    // command parse failed
    // 1. command not match
    // 2. command match but parameters format invalid
    leafcli_printf_newline("leafcli: cmd parse failed, group_id[%u]", buff->group_id);
    skip_to_cmd_end(buff);
    skip_cmd_end(buff);
    fifo_rd_clear(buff);
}

void leafcli_scan(void)
{
    leafcli_buffer_s *buff = NULL;
    uint32_t i;
    for (i = 0; i < g_reg_buff_num; i++) {
        buff = g_reg_buff_list[i];
        // parse and execute all commands in buff
        while (buff->proc_cmd_num < buff->recv_cmd_num) {
            leafcli_parse_exec_cmd(buff);
            buff->proc_cmd_num++;
        }
    }
}