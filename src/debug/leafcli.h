#ifndef _LEAFCLI_H_
#define _LEAFCLI_H_

#include "base_type.h"

#define LEAFCLI_USE_CRLF        1
#define LEAFCLI_MAX_BUFF_NUM    10
#define LEAFCLI_MAX_CTX_NUM     10
#define LEAFCLI_MAX_PARAM_NUM   6
#define LEAFCLI_MAX_PARAM_LEN   10
#define LEAFCLI_GROUP_ID_ALL    0       // all groups contain the ctx

typedef struct {
    uint8_t *cmd_name;
    void *cmd_func;     // only support base type argument
    uint8_t *cmd_desc;  // OPTIONAL
} leafcli_cmd_s;

// fifo - ring buffer
typedef struct {
    uint8_t group_id;
    uint32_t fifo_size;
    uint8_t *fifo;
    // private
    uint32_t rd_index;
    uint32_t wr_index;
    uint32_t recv_cmd_num;
    uint32_t proc_cmd_num;
    bool full_flag;     // is full before
} leafcli_buffer_s;

typedef struct {
    uint8_t group_id;
    uint8_t *cmd_list_name;
    uint32_t cmd_num;
    leafcli_cmd_s *cmd_list;
} leafcli_context_s;

int leafcli_register_buff(leafcli_buffer_s *buff);
int leafcli_register_ctx(leafcli_context_s *ctx);
int leafcli_register_builtin_ctx(void);
void leafcli_recv_data(uint8_t ch, uint8_t group_id);
void leafcli_scan(void);

#endif