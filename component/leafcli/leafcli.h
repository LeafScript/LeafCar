#ifndef _LEAFCLI_H_
#define _LEAFCLI_H_

#include "base_type.h"

/**
 *   platform       compiler        flash        flash (no builtin cmd)    ram      ram (no builtin cmd)
 * stm32f103zet6     gcc O1         +6184        +4688                     +208     +108
 */
// 1: enable leafcli  0: disable leafcli
#define LEAFCLI_ENABLE_SW               1
// 1: enable builtin command  0: disable builtin command
#define LEAFCLI_ENABLE_BUILTIN_CMD      0

#define LEAFCLI_USE_CRLF        1
#define LEAFCLI_MAX_BUFF_NUM    10
#define LEAFCLI_MAX_CMD_LEN     32
#define LEAFCLI_MAX_CTX_NUM     10
#define LEAFCLI_MAX_PARAM_NUM   6
#define LEAFCLI_MAX_PARAM_LEN   10

enum leafcli_error_code_e {
    LEAFCLI_EC_OK,
    LEAFCLI_EC_ERROR
};

typedef struct {
    uint8_t *cmd_name;
    void *cmd_func;     // only support base type argument
    uint8_t *cmd_desc;  // OPTIONAL
} leafcli_cmd_s;

// fifo - ring buffer
typedef struct {
    uint8_t group_id;   // bind with leafcli buffer (one to one)
    uint32_t fifo_size; // suggest >= 64 bytes
    void (*fifo_full_cb)(uint8_t group_id);     // OPTIONAL
    void (*fifo_full_solved)(uint8_t group_id); // OPTIONAL
    uint8_t *fifo;
    // private
    uint32_t rd_index;
    uint32_t wr_index;
    uint32_t recv_cmd_num;
    uint32_t proc_cmd_num;
    uint32_t parse_ch_num;
    bool full_flag;     // is fifo full
} leafcli_buffer_s;

typedef struct {
    uint8_t group_id;
    uint8_t *cmd_list_name;
    uint32_t cmd_num;
    leafcli_cmd_s *cmd_list;
} leafcli_context_s;

#if (LEAFCLI_ENABLE_SW == 1)

int leafcli_register_buff(leafcli_buffer_s *buff);
int leafcli_register_ctx(leafcli_context_s *ctx);
void leafcli_recv_data(uint8_t ch, uint8_t group_id);
/**
 * suggest NOT call this function in interrupt, unless you have ensured your command
 * will handle correctly in it.
 */
void leafcli_scan(void);

#if (LEAFCLI_ENABLE_BUILTIN_CMD == 1)
int leafcli_register_builtin_ctx(uint8_t group_id);
#else
static inline int leafcli_register_builtin_ctx(uint8_t group_id) { return LEAFCLI_EC_OK; }
#endif

#else
// mock all interface
static inline int leafcli_register_buff(leafcli_buffer_s *buff) { return LEAFCLI_EC_OK; }
static inline int leafcli_register_ctx(leafcli_context_s *ctx) { return LEAFCLI_EC_OK; }
static inline void leafcli_recv_data(uint8_t ch, uint8_t group_id) { }
static inline void leafcli_scan(void) { }
static inline int leafcli_register_builtin_ctx(uint8_t group_id) { return LEAFCLI_EC_OK; }

#endif

#endif