#include "ssd1306/ssd1306.h"

#include "ssd1306/err.h"
#include "ssd1306/platform.h"

#include <stddef.h> /* NULL */

/**
 * @todo Correctly check the validity of the parameters passed in to all
 *       functions. Also make sure to add those potential errors to 'err.h'.
 */

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

/**
 * @addtogroup fundamental_commands
 */

/** @{ */

enum ssd1306_err
ssd1306_turn_display_on(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DISPLAY_ON));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_turn_display_off(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DISPLAY_OFF));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_ignore_ram_contents(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DISPLAY_IGNORE_RAM));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_follow_ram_contents(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DISPLAY_FOLLOW_RAM));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_normalize_display(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_NORMAL_DISPLAY));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_invert_display(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_INVERT_DISPLAY));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_contrast(struct ssd1306_ctx *ctx, uint8_t contrast_value)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_SET_CONTRAST_CTRL));
    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, contrast_value));

    return SSD1306_OK;
}

/** @} */ /* fundamental_commands */

/**
 * @addtogroup scrolling_commands
 */

/** @{ */

enum ssd1306_err
ssd1306_start_scrolling(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_START_SCROLLING));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_stop_scrolling(struct ssd1306_ctx *ctx)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD | CHECK_WRITE_DATA));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_STOP_SCROLLING));

    /*
     * TODO(rewrite data to RAM to not risk data corruption)
     * Ideally, the data will be buffered by the library so that
     * rewriting the data to RAM can be as simple as just writing:
     *
     *       ssd1306_send_buffer(ctx);
     *
     * or something similar.
     */

    return SSD1306_OK;
}

/**
 * Of all the scrolling commands, there are 4 that configure horizontal
 * scrolling. The functions that implement those commands have to call this
 * function to configure the horizontal scrolling.
 */
static enum ssd1306_err
setup_horiz_scroll_params(struct ssd1306_ctx *ctx,
                          enum ssd1306_scrolling_command scroll_dir,
                          enum ssd1306_page upper_bound,
                          enum ssd1306_scroll_step interval,
                          enum ssd1306_page lower_bound)
{
    uint8_t cmd_list[] = {
        scroll_dir, SSD1306_DUMMY_BYTE_0S, upper_bound, interval, lower_bound,
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

/**
 * The two commands that only configure horizontal scrolling call this function.
 * The pattern they share is sending the same, two dummy bytes at the end of the
 * command sequence.
 */
static enum ssd1306_err
setup_horiz_nonvert_scroll_params(struct ssd1306_ctx *ctx,
                                  enum ssd1306_scrolling_command scroll_dir,
                                  enum ssd1306_page upper_bound,
                                  enum ssd1306_scroll_step interval,
                                  enum ssd1306_page lower_bound)
{
    SSD1306_RETURN_ON_ERR(setup_horiz_scroll_params(
        ctx, scroll_dir, upper_bound, interval, lower_bound));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DUMMY_BYTE_0S));
    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, SSD1306_DUMMY_BYTE_1S));

    return SSD1306_OK;
}

/**
 * Flags to determine which of the dimensions to check.
 */
enum dimension_check {
    CHECK_WIDTH = BIT(0),
    CHECK_HEIGHT = BIT(1),
};

/**
 * Check that the dimensions of the OLED are non-zero.
 *
 * @param ctx   container of the dimensions
 * @param flags bitwise or'd set of flags to determine which dimensions to check
 *
 * @return @ref SSD1306_WIDTH_ZERO  if the width is set to 0
 * @return @ref SSD1306_HEIGHT_ZERO if the height is set to 0
 * @return otherwise, @ref SSD1306_OK
 */
static enum ssd1306_err
check_dimensions(const struct ssd1306_ctx *ctx, enum dimension_check flags)
{
    if ((flags & CHECK_WIDTH) && ctx->width == 0) {
        return SSD1306_WIDTH_ZERO;
    }
    else if ((flags & CHECK_HEIGHT) && ctx->height == 0) {
        return SSD1306_HEIGHT_ZERO;
    }

    /*
     * Don't add an else clause returning SSD1306_OK. It will be easier to
     * conditionally compile the if checks out of the function should such a
     * need arise.
     */
    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_scroll_right(struct ssd1306_ctx *ctx, enum ssd1306_page upper_bound,
                     enum ssd1306_scroll_step interval,
                     enum ssd1306_page lower_bound)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));
    SSD1306_RETURN_ON_ERR(check_dimensions(ctx, CHECK_HEIGHT));

    SSD1306_RETURN_ON_ERR(setup_horiz_nonvert_scroll_params(
        ctx, SSD1306_RIGHT_SCROLL, upper_bound, interval, lower_bound));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_scroll_left(struct ssd1306_ctx *ctx, enum ssd1306_page upper_bound,
                    enum ssd1306_scroll_step interval,
                    enum ssd1306_page lower_bound)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));
    SSD1306_RETURN_ON_ERR(check_dimensions(ctx, CHECK_HEIGHT));

    SSD1306_RETURN_ON_ERR(setup_horiz_nonvert_scroll_params(
        ctx, SSD1306_LEFT_SCROLL, upper_bound, interval, lower_bound));

    return SSD1306_OK;
}

/**
 * The two commands that configure horizontal and vertical scrolling call this
 * function. The pattern they share is sending the vertical offset at the end of
 * the command sequence.
 */
static enum ssd1306_err
setup_horiz_vert_scroll_params(struct ssd1306_ctx *ctx,
                               enum ssd1306_scrolling_command scroll_dir,
                               enum ssd1306_page upper_bound,
                               enum ssd1306_scroll_step interval,
                               enum ssd1306_page lower_bound,
                               enum ssd1306_row vertical_offset)
{
    SSD1306_RETURN_ON_ERR(setup_horiz_scroll_params(
        ctx, scroll_dir, upper_bound, interval, lower_bound));

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, vertical_offset));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_scroll_vert_right(struct ssd1306_ctx *ctx,
                          enum ssd1306_page upper_bound,
                          enum ssd1306_scroll_step interval,
                          enum ssd1306_page lower_bound,
                          enum ssd1306_row vertical_offset)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));
    SSD1306_RETURN_ON_ERR(check_dimensions(ctx, CHECK_HEIGHT));

    SSD1306_RETURN_ON_ERR(setup_horiz_vert_scroll_params(
        ctx, SSD1306_RIGHT_VERT_SCROLL, upper_bound, interval, lower_bound,
        vertical_offset));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_scroll_vert_left(struct ssd1306_ctx *ctx, enum ssd1306_page upper_bound,
                         enum ssd1306_scroll_step interval,
                         enum ssd1306_page lower_bound,
                         enum ssd1306_row vertical_offset)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));
    SSD1306_RETURN_ON_ERR(check_dimensions(ctx, CHECK_HEIGHT));

    SSD1306_RETURN_ON_ERR(setup_horiz_vert_scroll_params(
        ctx, SSD1306_LEFT_VERT_SCROLL, upper_bound, interval, lower_bound,
        vertical_offset));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_vert_scroll_area(struct ssd1306_ctx *ctx,
                             enum ssd1306_row static_rows,
                             enum ssd1306_row dynamic_rows)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    uint8_t cmd_list[] = {
        SSD1306_SET_VERT_SCROLL_AREA,
        static_rows,
        dynamic_rows,
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

/** @} */ /* scrolling_commands */

/**
 * @addtogroup address_setting_commands
 */

/** @{ */

enum ssd1306_err
ssd1306_set_addr_mode(struct ssd1306_ctx *ctx, enum ssd1306_addr_mode addr_mode)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    uint8_t cmd_list[] = {
        SSD1306_SET_MEM_ADDR_MODE,
        addr_mode,
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_page_addr(struct ssd1306_ctx *ctx, enum ssd1306_page page)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    uint8_t cmd = SSD1306_SET_SINGLE_PAGE_ADDR | (page & 0x07);

    SSD1306_RETURN_ON_ERR(ctx->send_cmd(ctx, cmd));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_col_addr(struct ssd1306_ctx *ctx, enum ssd1306_col col)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    /* I'm not sure if the upper or lower nybble need to be sent first. */
    uint8_t cmd_list[] = {
        SSD1306_SET_UPPER_NYBBLE_COL_ADDR | ((col & 0xF0) >> 4),
        SSD1306_SET_LOWER_NYBBLE_COL_ADDR | (col & 0x0F),
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_page_range(struct ssd1306_ctx *ctx, enum ssd1306_page start_page,
                       enum ssd1306_page end_page)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    uint8_t cmd_list[] = {
        SSD1306_SET_PAGE_ADDR_RANGE,
        start_page,
        end_page,
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

enum ssd1306_err
ssd1306_set_col_range(struct ssd1306_ctx *ctx, enum ssd1306_col start_col,
                      enum ssd1306_col end_col)
{
    SSD1306_RETURN_ON_ERR(check_ctx(ctx, CHECK_SEND_CMD));

    uint8_t cmd_list[] = {
        SSD1306_SET_COL_ADDR_RANGE,
        start_col,
        end_col,
    };

    SSD1306_RETURN_ON_ERR(
        ssd1306_send_cmd_list(ctx, cmd_list, SSD1306_ARRAY_LEN(cmd_list)));

    return SSD1306_OK;
}

/** @} */
