#ifndef LIBSSD1306_SSD1306_H
#define LIBSSD1306_SSD1306_H

/**
 * @defgroup fundamental_commands Fundamental Commands
 * @{
 */

enum ssd1306_fundamental_command {
    SSD1306_DISPLAY_ON = 0xAF,  /**< Turn the display on. */
    SSD1306_DISPLAY_OFF = 0xAE, /**< Turn the display off. */

    /**
     * The display shows the RAM's content.
     */
    SSD1306_DISPLAY_FOLLOW_RAM = 0xA4,
    /**
     * The display ignores the RAM's content.
     */
    SSD1306_DISPLAY_IGNORE_RAM = 0xA5,

    /**
     * A value of '1' in RAM represents an on pixel.
     */
    SSD1306_NORMAL_DISPLAY = 0xA6,
    /**
     * A value of '0' in RAM represents an on pixel.
     */
    SSD1306_INVERT_DISPLAY = 0xA7,

    /**
     * Select 1 out of 256 contrast steps.
     */
    SSD1306_SET_CONTRAST_CTRL = 0x81,
};

/** @} */

#endif /* LIBSSD1306_SSD1306_H */
