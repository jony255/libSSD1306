#include "ssd1306/platform.h"
#include "ssd1306/err.h"

#include <stddef.h> /* size_t */

#define BITU(n) (1U << (n))
#define BIT(n)  BITU(n)

/**
 * Flags used to determine which of the callbacks to check.
 */
enum cb_check {
    CHECK_SEND_CMD = BIT(0),
    CHECK_WRITE_DATA = BIT(1),
};

/**
 * Check that ctx and its callbacks aren't NULL.
 *
 * @param ctx   ctx to NULL-check
 * @param flags bitwise or'd set of flags to determine which callbacks to check
 *
 * @return
 *       - the appropriate NULL return code for the NULL argument
 *       - SSD1306_OK otherwise
 */
static enum ssd1306_err
check_ctx(const struct ssd1306_ctx *ctx, enum cb_check flags)
{
    if (ctx == NULL) {
        return SSD1306_CTX_NULL;
    }
    else if ((flags & CHECK_SEND_CMD) && ctx->send_cmd == NULL) {
        return SSD1306_SEND_CMD_NULL;
    }
    else if ((flags & CHECK_WRITE_DATA) && ctx->write_data == NULL) {
        return SSD1306_WRITE_DATA_NULL;
    }

    /*
     * Don't add an else clause returning SSD1306_OK. It will be easier to
     * conditionally compile the if checks out of the function should such a
     * need arise.
     */
    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_send_cmd(struct ssd1306_ctx *ctx, uint8_t cmd)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    return ctx->send_cmd(ctx, cmd);
}

enum ssd1306_err
ssd1306_send_cmd_list(struct ssd1306_ctx *ctx, const uint8_t *cmd_list,
                      size_t cmd_list_len)
{
    if (cmd_list == NULL) {
        return SSD1306_CMD_LIST_NULL;
    }

    for (size_t i = 0; i < cmd_list_len; i++) {
        uint8_t cmd = cmd_list[i];

        SSD1306_RETURN_ON_ERR(ssd1306_send_cmd(ctx, cmd));
    }

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_write_data(struct ssd1306_ctx *ctx, uint8_t data)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_WRITE_DATA));

    return ctx->write_data(ctx, data);
}

enum ssd1306_err
ssd1306_write_data_list(struct ssd1306_ctx *ctx, const uint8_t *data_list,
                        size_t data_list_len)
{
    if (data_list == NULL) {
        return SSD1306_DATA_LIST_NULL;
    }

    for (size_t i = 0; i < data_list_len; i++) {
        uint8_t data = data_list[i];

        SSD1306_RETURN_ON_ERR(ssd1306_write_data(ctx, data));
    }

    return SSD1306_OK;
}
