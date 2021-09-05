#ifndef LIBOLED_LIBOLED_H
#define LIBOLED_LIBOLED_H

/**
 * @defgroup fundamental_commands Fundamental Commands
 * @{
 */

/**
 * Select 1 out of 256 contrast steps
 * @note You must send the contrast step right after (8 bits long)
 */
#define LIBOLED_SET_CONTRAST_CTRL 0x81

#define LIBOLED_OUTPUT_FOLLOW_RAM 0xA4 /**< Output follows RAM content */
#define LIBOLED_OUTPUT_IGNORE_RAM 0xA5 /**< Output ignores RAM content */

#define LIBOLED_NORMAL_DISPLAY 0xA6 /**< Value of 1 represents an on pixel */
#define LIBOLED_INVERT_DISPLAY 0xA7 /**< Value of 0 represents an on pixel */

#define LIBOLED_DISPLAY_OFF 0xAE /**< Turn the display off */
#define LIBOLED_DISPLAY_ON  0xAF /**< Turn the display on */

/** @} */

#endif /* LIBOLED_LIBOLED_H */
