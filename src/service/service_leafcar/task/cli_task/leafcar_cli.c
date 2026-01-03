#include "leafcar_cli.h"
#include "base_type.h"
#include "error_code.h"
#include "log.h"
#include "mem_cfg.h"
#include "leafcli.h"
#include "update_fw_task.h"

static leafcli_cmd_s g_leafcar_cli_cmd_list[] = {
    { "updata_fw", update_fw_task_start, "void update_fw_task_start(void)" },
};
static leafcli_context_s g_leafcar_cli_ctx = {
    .group_id = LEAFCLI_GROUP_LEAFCAR,
    .cmd_list_name = "leafcar cli cmd list",
    .cmd_num = ARRAY_SIZE(g_leafcar_cli_cmd_list),
    .cmd_list = g_leafcar_cli_cmd_list
};

int leafcar_cli_register(void)
{
    int ret;
	ret = leafcli_register_ctx(&g_leafcar_cli_ctx);
    if (ret != LEAFCLI_EC_OK) {
        LEAF_LOG(LOG_ERROR, "leafcar_cli_register failed");
        return EC_ERROR;
    }
    return EC_OK;
}