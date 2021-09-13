#ifndef LIBSSD1306_SSD1306_H
#define LIBSSD1306_SSD1306_H

/**
 * @defgroup fundamental_commands Fundamental Commands
 * @{
 */

/**
 * The enumerated commands from @c Table 9-1
 */
enum ssd1306_fundamental_command {
    SSD1306_DISPLAY_ON = 0xAF,  /**< Turn the display on. (i.e. Normal Mode) */
    SSD1306_DISPLAY_OFF = 0xAE, /**< Turn the display off. (i.e. Sleep Mode) */

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

/**
 * @defgroup scrolling_commands Scrolling Commands
 * @{
 */

/**
 * The enumerated commands from @c Table 9-2
 */
enum ssd1306_scrolling_command {
    /**
     * Start scrolling configured by other commands. This command must be sent
     * *after* configuring a scrolling command and only activates scrolling for
     * the last scrolling command issued.
     */
    SSD1306_START_SCROLLING = 0x2F,
    /**
     * Stop scrolling configured by other commands.
     *
     * @warning After sending this command, you must rewrite the data or you
     * risk data corruption.
     */
    SSD1306_STOP_SCROLLING = 0x2E,

    SSD1306_RIGHT_SCROLL = 0x26, /**< Scroll the display to the right. */
    SSD1306_LEFT_SCROLL = 0x27,  /**< Scroll the display to the left. */

    SSD1306_VERT_SCROLL = 0xA3, /**< Scroll the display vertically. */

    /**
     * Scroll the display to the right and vertically (optional).
     * This command can be configured to only scroll the right. However, you
     * might as well just use @ref SSD1306_RIGHT_SCROLL at that point.
     */
    SSD1306_RIGHT_VERT_SCROLL = 0x29,
    /**
     * Scroll the display to the left and vertically (optional).
     * This command can be configured to only scroll the left. However, you
     * might as well just use @ref SSD1306_LEFT_SCROLL at that point.
     */
    SSD1306_LEFT_VERT_SCROLL = 0x2A,
};

/**
 * These dummy bytes are used to space out arguments to the following commands:
 *
 * @ref SSD1306_RIGHT_SCROLL \n
 * @ref SSD1306_LEFT_SCROLL \n\n
 *
 * @ref SSD1306_RIGHT_VERT_SCROLL \n
 * @ref SSD1306_LEFT_VERT_SCROLL \n
 */
enum ssd1306_dummy_byte {
    SSD1306_DUMMY_BYTE_0S = 0x00, /**< Filler byte comprised of all 0's. */
    SSD1306_DUMMY_BYTE_1S = 0xFF, /**< Filler byte comprised of all 1's. */
};

/**
 * A page is a set of 8 rows. These values are used to select the start/end
 * page addresses of various commands.
 *
 * @ref SSD1306_RIGHT_SCROLL \n
 * @ref SSD1306_LEFT_SCROLL \n\n
 *
 * @ref SSD1306_RIGHT_VERT_SCROLL \n
 * @ref SSD1306_LEFT_VERT_SCROLL \n
 */
enum ssd1306_page {
    SSD1306_PAGE_0,    /**< Select page 0. */
    SSD1306_PAGE_1,    /**< Select page 1. */
    SSD1306_PAGE_2,    /**< Select page 2. */
    SSD1306_PAGE_3,    /**< Select page 3. */
    SSD1306_PAGE_4,    /**< Select page 4. */
    SSD1306_PAGE_5,    /**< Select page 5. */
    SSD1306_PAGE_6,    /**< Select page 6. */
    SSD1306_PAGE_7,    /**< Select page 7. */
    SSD1306_NUM_PAGES, /**< Total number of pages. */
};

/**
 * These values are used to set the time interval between each scroll step in
 * terms of frame frequency. They are used to configure the following horizontal
 * scrolling commands:
 *
 * @ref SSD1306_RIGHT_SCROLL \n
 * @ref SSD1306_LEFT_SCROLL \n\n
 *
 * @ref SSD1306_RIGHT_VERT_SCROLL \n
 * @ref SSD1306_LEFT_VERT_SCROLL \n
 */
enum ssd1306_scroll_step_frame_freq {
    SSD1306_2_FRAMES = 0x07,   /**< Scroll step of 2 frames. */
    SSD1306_3_FRAMES = 0x04,   /**< Scroll step of 3 frames.*/
    SSD1306_4_FRAMES = 0x05,   /**< Scroll step of 4 frames.*/
    SSD1306_5_FRAMES = 0x00,   /**< Scroll step of 5 frames.*/
    SSD1306_25_FRAMES = 0x06,  /**< Scroll step of 25 frames.*/
    SSD1306_64_FRAMES = 0x01,  /**< Scroll step of 64 frames.*/
    SSD1306_128_FRAMES = 0x02, /**< Scroll step of 128 frames.*/
    SSD1306_256_FRAMES = 0x03, /**< Scroll step of 256 frames.*/
};

/**
 * These values are used to select either an individual row in RAM or a
 * quantity of rows. For examples, depending on where you're using it,
 * @c SSD1306_ROW_5 could mean you are referring to the 6th row, or it could
 * mean a total of 5 rows.
 */
enum ssd1306_row {
    SSD1306_ROW_0,
    SSD1306_ROW_1,
    SSD1306_ROW_2,
    SSD1306_ROW_3,
    SSD1306_ROW_4,
    SSD1306_ROW_5,
    SSD1306_ROW_6,
    SSD1306_ROW_7,
    SSD1306_ROW_8,
    SSD1306_ROW_9,
    SSD1306_ROW_10,
    SSD1306_ROW_11,
    SSD1306_ROW_12,
    SSD1306_ROW_13,
    SSD1306_ROW_14,
    SSD1306_ROW_15,
    SSD1306_ROW_16,
    SSD1306_ROW_17,
    SSD1306_ROW_18,
    SSD1306_ROW_19,
    SSD1306_ROW_20,
    SSD1306_ROW_21,
    SSD1306_ROW_22,
    SSD1306_ROW_23,
    SSD1306_ROW_24,
    SSD1306_ROW_25,
    SSD1306_ROW_26,
    SSD1306_ROW_27,
    SSD1306_ROW_28,
    SSD1306_ROW_29,
    SSD1306_ROW_30,
    SSD1306_ROW_31,
    SSD1306_ROW_32,
    SSD1306_ROW_33,
    SSD1306_ROW_34,
    SSD1306_ROW_35,
    SSD1306_ROW_36,
    SSD1306_ROW_37,
    SSD1306_ROW_38,
    SSD1306_ROW_39,
    SSD1306_ROW_40,
    SSD1306_ROW_41,
    SSD1306_ROW_42,
    SSD1306_ROW_43,
    SSD1306_ROW_44,
    SSD1306_ROW_45,
    SSD1306_ROW_46,
    SSD1306_ROW_47,
    SSD1306_ROW_48,
    SSD1306_ROW_49,
    SSD1306_ROW_50,
    SSD1306_ROW_51,
    SSD1306_ROW_52,
    SSD1306_ROW_53,
    SSD1306_ROW_54,
    SSD1306_ROW_55,
    SSD1306_ROW_56,
    SSD1306_ROW_57,
    SSD1306_ROW_58,
    SSD1306_ROW_59,
    SSD1306_ROW_60,
    SSD1306_ROW_61,
    SSD1306_ROW_62,
    SSD1306_ROW_63,
};

/** @} */

/**
 * @defgroup address_setting_commands Address Settings Commands
 * @{
 */

/**
 * The enumerated commands from @c Table 9-3
 */
enum ssd1306_addr_setting_command {
    /**
     * Set the addressing mode to any of the modes defined in
     * @ref ssd1306_addr_mode.
     */
    SSD1306_SET_MEM_ADDR_MODE = 0x20,

    /**
     * Set the page address.
     *
     * @note This command applies to the following addressing modes:\n
     * @note @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or @ref ssd1306_page start_page[2:0]
     */
    SSD1306_SET_SINGLE_PAGE_ADDR = 0xB0,
    /**
     * Set the upper nybble of the column.
     *
     * @note This command applies to the following addressing modes:\n
     * @note @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or upper_nybble[3:0]
     */
    SSD1306_SET_UPPER_NYBBLE_COL_ADDR = 0x10,
    /**
     * Set the lower nybble of the column.
     *
     * @note This command applies to the following addressing modes:\n
     * @note @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or lower_nybble[3:0]
     */
    SSD1306_SET_LOWER_NYBBLE_COL_ADDR = 0x00,

    /**
     * Set the start and end page addresses.
     *
     * @note This command applies to the following addressing modes:\n
     * @note @ref SSD1306_HORIZ_ADDR_MODE
     * @note @ref SSD1306_VERT_ADDR_MODE
     */
    SSD1306_SET_MULTIPLE_PAGE_ADDRS = 0x22,
    /**
     * Set the start and end column addresses.
     *
     * @note This command applies to the following addressing modes:\n
     * @note @ref SSD1306_HORIZ_ADDR_MODE
     * @note @ref SSD1306_VERT_ADDR_MODE
     */
    SSD1306_SET_MULTIPLE_COL_ADDRS = 0x21,
};

/**
 * The available modes used as arguments to the command
 * @ref SSD1306_SET_MEM_ADDR_MODE. For the description of the modes, assume you
 * start at @c (PAGE_0, COL_0).
 */
enum ssd1306_addr_mode {
    /**
     * Walk the entire page until you reach the end (i.e. the last column).
     * Afterwards, go to the beginning of the next page and repeat. Once you
     * reach the last column of the last page, go to the very beginning
     * @c (PAGE_0, COL_0).
     */
    SSD1306_HORIZ_ADDR_MODE,
    /**
     * Walk the entire column until you reach the end (i.e. the last page).
     * Afterwards, go to the beginning of the next column and repeat. Once you
     * reach the last page of the last column, go to the very beginning
     * @c (PAGE_0, COL_0).
     */
    SSD1306_VERT_ADDR_MODE,
    /**
     * Walk the entire page until you reach the end (i.e. the last column).
     * Afterwards, loop back to where you started (i.e. @c (PAGE_0, COL_0)).
     *
     * In order to go to a different page, you must call the following command:
     *
     * @ref SSD1306_SET_SINGLE_PAGE_ADDR.
     *
     * If you want to go to a different column, refer to the following commands:
     *
     * @ref SSD1306_SET_UPPER_NYBBLE_COL_ADDR \n
     * @ref SSD1306_SET_LOWER_NYBBLE_COL_ADDR \n
     */
    SSD1306_PAGE_ADDR_MODE,

    SSD1306_NUM_ADDR_MODE,
};

/** @} */

#endif /* LIBSSD1306_SSD1306_H */
