#ifndef LIBOLED_LIBOLED_H
#define LIBOLED_LIBOLED_H

/**
 * @defgroup fundamental_commands Fundamental Commands
 * @{
 */

enum liboled_fundamental_commands {
    LIBOLED_DISPLAY_ON = 0xAF,  /**< Turn the display on. */
    LIBOLED_DISPLAY_OFF = 0xAE, /**< Turn the display off. */

    /**
     * The display shows the RAM's content.
     */
    LIBOLED_DISPLAY_FOLLOW_RAM = 0xA4,
    /**
     * The display ignores the RAM's content.
     */
    LIBOLED_DISPLAY_IGNORE_RAM = 0xA5,

    /**
     * A value of '1' in RAM represents an on pixel.
     */
    LIBOLED_NORMAL_DISPLAY = 0xA6,
    /**
     * A value of '0' in RAM represents an on pixel.
     */
    LIBOLED_INVERT_DISPLAY = 0xA7,

    /**
     * Select 1 out of 256 contrast steps.
     * @note You must send the contrast step right after (8 bits long)
     */
    LIBOLED_SET_CONTRAST_CTRL = 0x81,
};

/** @} */

#endif /* LIBOLED_LIBOLED_H */
