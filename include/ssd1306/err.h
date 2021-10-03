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

/** @} */ /* ssd1306_return_codes */

#endif /* LIBSSD1306_SSD1306_ERR_H */
