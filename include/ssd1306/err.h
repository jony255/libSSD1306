#ifndef LIBSSD1306_SSD1306_ERR_H
#define LIBSSD1306_SSD1306_ERR_H

/**
 * @defgroup ssd1306_return_codes Return Codes
 *
 * @{
 */

/**
 * An enum that contains of all possible return codes.
 */
enum ssd1306_err {
    SSD1306_OK, /**< The function did not encounter an error. */

    /**
     * The current wiring setup doesn't support reading from the @c SSD1306.
     * This is the case for serial wiring setups.
     */
    SSD1306_READING_UNSUPPORTED,
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
