#include "ssd1306/ssd1306.h"

#include "ssd1306/err.h"
#include "ssd1306/platform.h"

/**
 * @ingroup fundamental_commands
 *
 * @{
 */

enum ssd1306_err
ssd1306_turn_display_on(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_DISPLAY_ON);
}

enum ssd1306_err
ssd1306_turn_display_off(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_DISPLAY_OFF);
}

enum ssd1306_err
ssd1306_ignore_ram_contents(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_DISPLAY_IGNORE_RAM);
}

enum ssd1306_err
ssd1306_follow_ram_contents(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_DISPLAY_FOLLOW_RAM);
}

enum ssd1306_err
ssd1306_normalize_display(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_NORMAL_DISPLAY);
}

enum ssd1306_err
ssd1306_invert_display(struct ssd1306_ctx *ctx)
{
    return ctx->send_cmd(ctx, SSD1306_INVERT_DISPLAY);
}

enum ssd1306_err
ssd1306_set_contrast(struct ssd1306_ctx *ctx, uint8_t contrast_value)
{
    enum ssd1306_err ret = ctx->send_cmd(ctx, SSD1306_DISPLAY_FOLLOW_RAM);

    if (ret != SSD1306_OK) {
        return ret;
    }

    return ctx->send_cmd(ctx, contrast_value);
}

/** @} */ /* fundamental_commands */
