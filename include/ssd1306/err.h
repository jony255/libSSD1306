#ifndef LIBSSD1306_SSD1306_ERR_H
#define LIBSSD1306_SSD1306_ERR_H

/**
 * @defgroup ssd1306_return_codes Return Codes
 *
 * All library functions share a common set of possible return codes.
 * For the sake of not duplicating documentation, all of the library functions
 * will omit the following return codes from their @c \@return:
 *
 * - @ref SSD1306_OK
 *
 * - @ref SSD1306_CTX_NULL
 * - @ref SSD1306_SEND_CMD_NULL (all functions that implement @ref commands)
 *
 * - @ref SSD1306_READING_UNSUPPORTED
 *
 * The only time a function should explicitly state what the return codes are
 * is when they differ from the set of common return codes.
 */

/** @{ */

/**
 * An enum that contains of all possible return codes.
 */
enum ssd1306_err {
    SSD1306_OK, /**< The function did not encounter an error. */

    SSD1306_CTX_NULL,        /**< The @ref ssd1306_ctx passed in is @c NULL. */
    SSD1306_SEND_CMD_NULL,   /**< @ref ssd1306_ctx::send_cmd is @c NULL. */
    SSD1306_WRITE_DATA_NULL, /**< @ref ssd1306_ctx::write_data is @c NULL. */

    /**
     * The current wiring setup doesn't support reading from the @c SSD1306.
     * This is the case for serial wiring setups.
     *
     * @todo This return code only applies to @c ssd1306_ctx::read_data or
     *       @c ssd1306_ctx::read_status . These still need to be spec'd out.
     */
    SSD1306_READING_UNSUPPORTED,

    /**
     * A calculation involving the width of the OLED was performed and noticed
     * that the width was set to 0.
     */
    SSD1306_WIDTH_ZERO,
    /**
     * A calculation involving the height of the OLED was performed and noticed
     * that the height was set to 0.
     */
    SSD1306_HEIGHT_ZERO,

    /**
     * The @c upper_bound argument for setting up horizontal scrolling was not
     * within the dimensions of the OLED.
     *
     * @see ssd1306_scroll_right
     * @see ssd1306_scroll_left
     * @see ssd1306_scroll_vert_right
     * @see ssd1306_scroll_vert_left
     */
    SSD1306_UPPER_BOUND_OUT_OF_DIMENSION,
    /**
     * The @c lower_bound argument for setting up horizontal scrolling was not
     * within the dimensions of the OLED.
     *
     * @see ssd1306_scroll_right
     * @see ssd1306_scroll_left
     * @see ssd1306_scroll_vert_right
     * @see ssd1306_scroll_vert_left
     */
    SSD1306_LOWER_BOUND_OUT_OF_DIMENSION,
    /**
     * The @c upper_bound argument for setting up horizontal scrolling was
     * greater than @c lower_bound.
     *
     * @c upper_bound must be less than or equal to @c lower_bound.
     * Remember, @c upper_bound refers to the first row of the page and
     * @c lower_bound refers to the last row of the page.
     *
     * If the arguments are the same page, then the only thing scrolling
     * horizontally is that page.
     *
     * @see ssd1306_scroll_right
     * @see ssd1306_scroll_left
     * @see ssd1306_scroll_vert_right
     * @see ssd1306_scroll_vert_left
     */
    SSD1306_UPPER_BOUND_GT_LOWER_BOUND,
};

/**
 * Simple macro that propagates the return code of @c fn_call iff the return
 * code isn't @c SSD1306_OK.
 *
 * @param fn_call call expression to check
 */
#define SSD1306_RETURN_ON_ERR(fn_call)                                         \
    do {                                                                       \
        enum ssd1306_err ret = (fn_call);                                      \
                                                                               \
        if (ret != SSD1306_OK) {                                               \
            return ret;                                                        \
        }                                                                      \
    } while (0)

/** @} */ /* ssd1306_return_codes */

#endif /* LIBSSD1306_SSD1306_ERR_H */
