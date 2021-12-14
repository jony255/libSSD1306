#include "ssd1306/platform.h"
#include "ssd1306/err.h"

#include <stddef.h> /* size_t */

enum ssd1306_err
ssd1306_send_cmd_list(struct ssd1306_ctx *ctx, const uint8_t *cmd_list,
                      size_t cmd_list_len)
{
    for (size_t i = 0; i < cmd_list_len; i++) {
        uint8_t cmd = cmd_list[i];

        SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, cmd));
    }

    return SSD1306_OK;
}
