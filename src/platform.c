#include "ssd1306/platform.h"
#include "ssd1306/err.h"

#include <stddef.h> /* size_t */

enum ssd1306_err
ssd1306_send_cmd(struct ssd1306_ctx *ctx, uint8_t cmd)
{
    return ctx->send_cmd(ctx, cmd);
}

enum ssd1306_err
ssd1306_send_cmd_list(struct ssd1306_ctx *ctx, const uint8_t *cmd_list,
                      size_t cmd_list_len)
{
    for (size_t i = 0; i < cmd_list_len; i++) {
        uint8_t cmd = cmd_list[i];

        SSD1306_RETURN_ON_ERR(ssd1306_send_cmd(ctx, cmd));
    }

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_write_data(struct ssd1306_ctx *ctx, uint8_t data)
{

    return ctx->write_data(ctx, data);
}

enum ssd1306_err
ssd1306_write_data_list(struct ssd1306_ctx *ctx, const uint8_t *data_list,
                        size_t data_list_len)
{
    for (size_t i = 0; i < data_list_len; i++) {
        uint8_t data = data_list[i];

        SSD1306_RETURN_ON_ERR(ssd1306_write_data(ctx, data));
    }

    return SSD1306_OK;
}
