#ifndef LIBOLED_LIBOLED_H
#define LIBOLED_LIBOLED_H

/**
 * @defgroup fundamental_commands Fundamental Commands
 * @{
 */

enum liboled_fundamental_commands {
    LIBOLED_DISPLAY_ON = 0xAF,  /**< Turn the display on */
    LIBOLED_DISPLAY_OFF = 0xAE, /**< Turn the display off */

    LIBOLED_OUTPUT_FOLLOW_RAM = 0xA4, /**< Output follows RAM content */
    LIBOLED_OUTPUT_IGNORE_RAM = 0xA5, /**< Output ignores RAM content */

    LIBOLED_NORMAL_DISPLAY = 0xA6, /**< Value of 1 represents an on pixel */
    LIBOLED_INVERT_DISPLAY = 0xA7, /**< Value of 0 represents an on pixel */

    /**
     * Select 1 out of 256 contrast steps
     * @note You must send the contrast step right after (8 bits long)
     */
    LIBOLED_SET_CONTRAST_CTRL = 0x81,
};

/** @} */

#endif /* LIBOLED_LIBOLED_H */
