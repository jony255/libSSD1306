#ifndef LIBSSD1306_SSD1306_SSD1306_H
#define LIBSSD1306_SSD1306_SSD1306_H

#include "ssd1306/err.h"
#include "ssd1306/platform.h"

/**
 * @defgroup commands Commands
 *
 * All possible command groups are placed here.
 */

/**
 * @defgroup fundamental_commands Fundamental Commands
 *
 * @ingroup commands
 */

/** @{ */

/**
 * The enumerated commands from `Table 9-1`
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

/**
 * @implements SSD1306_DISPLAY_ON
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_turn_display_on(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_DISPLAY_OFF
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_turn_display_off(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_DISPLAY_IGNORE_RAM
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_ignore_ram_contents(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_DISPLAY_FOLLOW_RAM
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_follow_ram_contents(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_NORMAL_DISPLAY
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_normalize_display(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_INVERT_DISPLAY
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_invert_display(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_SET_CONTRAST_CTRL
 *
 * @param ctx struct that contains the platform dependent I/O
 * @param contrast_value set the contrast to any value between 0 and 255
 */
enum ssd1306_err ssd1306_set_contrast(struct ssd1306_ctx *ctx,
                                      uint8_t contrast_value);

/** @} */ /* fundamental_commands */

/**
 * @defgroup scrolling_commands Scrolling Commands
 *
 * @ingroup commands
 *
 * A quick note regarding the @c upper_bound and @c lower_bound arguments to
 * the horizontal scrolling commands:
 *
 * - @ref SSD1306_RIGHT_SCROLL
 * - @ref SSD1306_LEFT_SCROLL
 * - @ref SSD1306_RIGHT_VERT_SCROLL
 * - @ref SSD1306_LEFT_VERT_SCROLL
 *
 * You configure the bounds of the horizontal scrolling area manually. You do so
 * by passing in pages as the bounds of said scrolling area. There are two
 * boundaries you need to pass in, the @c upper_bound and @c lower_bound.
 *
 * - @c upper_bound, which refers to the starting address of the scrolling area.
 * - @c lower_bound, which refers to the end address of the scrolling area.
 *
 * However, as mentioned before, you don't pass in an individual row as the
 * bounds, you pass in a page. The @c upper_bound will refer to that page's
 * starting row and the @c lower_bound will refer to that page's ending row.
 *
 * Hopefully this illustration clears things up.
 *
 * Assume
 * `upper_bound = PAGE_0` and `lower_bound = PAGE_1`
 *
 * PAGE_0
 * ------
 * - ROW_0 (@c upper_bound)
 * - ROW_1
 * - ROW_2
 * - ROW_3
 * - ROW_4
 * - ROW_5
 * - ROW_6
 * - ROW_7
 *
 * PAGE_1
 * ------
 * - ROW_8
 * - ROW_9
 * - ROW_10
 * - ROW_11
 * - ROW_12
 * - ROW_13
 * - ROW_14
 * - ROW_15 (@c lower_bound)
 *
 * In this case, ONLY rows 0 through 15 will scroll horizontally.
 *
 * If you only want one page to scroll horizontally, then pass in the same page
 * to both @c upper_bound and @c lower_bound.
 *
 * To summarize,
 *   - Since the bounds are only addressable as pages, the amount of rows you
 *     can scroll by are only multiples of 8.
 *   - Starting addresses are also multiples of 8
 *     (i.e. you can't scroll in between pages).
 *   - @c lower_bound must be greater than or equal to @c upper_bound.
 */

/** @{ */

/**
 * The enumerated commands from `Table 9-2`
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

    /**
     * Sets the vertical scroll area for the following vertical scrolling
     * commands:
     *
     * - @ref SSD1306_RIGHT_VERT_SCROLL
     * - @ref SSD1306_LEFT_VERT_SCROLL
     */
    SSD1306_SET_VERT_SCROLL_AREA = 0xA3,
};

/**
 * These dummy bytes are used to space out arguments to the following commands:
 *
 * - @ref SSD1306_RIGHT_SCROLL
 * - @ref SSD1306_LEFT_SCROLL
 * - @ref SSD1306_RIGHT_VERT_SCROLL
 * - @ref SSD1306_LEFT_VERT_SCROLL
 */
enum ssd1306_dummy_byte {
    SSD1306_DUMMY_BYTE_0S = 0x00,
    SSD1306_DUMMY_BYTE_1S = 0xFF,
};

/**
 * These values are used to configure the start/end page addresses of the
 * following scrolling commands:
 *
 * - @ref SSD1306_RIGHT_SCROLL
 * - @ref SSD1306_LEFT_SCROLL
 * - @ref SSD1306_RIGHT_VERT_SCROLL
 * - @ref SSD1306_LEFT_VERT_SCROLL
 */
enum ssd1306_page {
    SSD1306_PAGE_0,
    SSD1306_PAGE_1,
    SSD1306_PAGE_2,
    SSD1306_PAGE_3,
    SSD1306_PAGE_4,
    SSD1306_PAGE_5,
    SSD1306_PAGE_6,
    SSD1306_PAGE_7,
};

/**
 * Useful macro for determining offests into pages.
 */
#define SSD1306_ROWS_PER_PAGE 8

/**
 * These values are used to set the time interval between each scroll step in
 * terms of frame frequency. They are used to configure the following horizontal
 * scrolling commands:
 *
 * - @ref SSD1306_RIGHT_SCROLL
 * - @ref SSD1306_LEFT_SCROLL
 * - @ref SSD1306_RIGHT_VERT_SCROLL
 * - @ref SSD1306_LEFT_VERT_SCROLL
 */
enum ssd1306_scroll_step {
    SSD1306_2_FRAMES = 0x07,
    SSD1306_3_FRAMES = 0x04,
    SSD1306_4_FRAMES = 0x05,
    SSD1306_5_FRAMES = 0x00,
    SSD1306_25_FRAMES = 0x06,
    SSD1306_64_FRAMES = 0x01,
    SSD1306_128_FRAMES = 0x02,
    SSD1306_256_FRAMES = 0x03,
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

/**
 * @warning writing/reading data and changing the horizontal scroll parameters
 *          are prohibited after calling this function
 *
 * @implements SSD1306_START_SCROLLING
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_start_scrolling(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_STOP_SCROLLING
 *
 * @param ctx struct that contains the platform dependent I/O
 */
enum ssd1306_err ssd1306_stop_scrolling(struct ssd1306_ctx *ctx);

/**
 * @implements SSD1306_RIGHT_SCROLL
 *
 * @param ctx         struct that contains the platform dependent I/O
 * @param upper_bound use this page's first row as the upper boundary of the
 *                    scrolling area
 * @param interval    time interval between each scroll step
 * @param lower_bound use this page's last row as the lower boundary of the
 *                    scrolling area
 *                    (must be greater than or equal to @c upper_bound)
 */
enum ssd1306_err ssd1306_scroll_right(struct ssd1306_ctx *ctx,
                                      enum ssd1306_page upper_bound,
                                      enum ssd1306_scroll_step interval,
                                      enum ssd1306_page lower_bound);

/**
 * @implements SSD1306_LEFT_SCROLL
 *
 * @param ctx         struct that contains the platform dependent I/O
 * @param upper_bound use this page's first row as the upper boundary of the
 *                    scrolling area
 * @param interval    time interval between each scroll step
 * @param lower_bound use this page's last row as the lower boundary of the
 *                    scrolling area
 *                    (must be greater than or equal to @c upper_bound)
 */
enum ssd1306_err ssd1306_scroll_left(struct ssd1306_ctx *ctx,
                                     enum ssd1306_page upper_bound,
                                     enum ssd1306_scroll_step interval,
                                     enum ssd1306_page lower_bound);

/**
 * @implements SSD1306_RIGHT_VERT_SCROLL
 *
 * @param ctx             struct that contains the platform dependent I/O
 * @param upper_bound     use this page's first row as the upper boundary of the
 *                        scrolling area
 * @param interval        time interval between each scroll step
 * @param lower_bound     use this page's last row as the lower boundary of the
 *                        scrolling area
 *                        (must be greater than or equal to @c upper_bound)
 * @param vertical_offset number of rows to scroll vertically each @c interval
 */
enum ssd1306_err ssd1306_scroll_vert_right(struct ssd1306_ctx *ctx,
                                           enum ssd1306_page upper_bound,
                                           enum ssd1306_scroll_step interval,
                                           enum ssd1306_page lower_bound,
                                           enum ssd1306_row vertical_offset);

/**
 * @implements SSD1306_LEFT_VERT_SCROLL
 *
 * @param ctx             struct that contains the platform dependent I/O
 * @param upper_bound     use this page's first row as the upper boundary of the
 *                        scrolling area
 * @param interval        time interval between each scroll step
 * @param lower_bound     use this page's last row as the lower boundary of the
 *                        scrolling area
 *                        (must be greater than or equal to @c upper_bound)
 * @param vertical_offset number of rows to scroll vertically each @c interval
 */
enum ssd1306_err ssd1306_scroll_vert_left(struct ssd1306_ctx *ctx,
                                          enum ssd1306_page upper_bound,
                                          enum ssd1306_scroll_step interval,
                                          enum ssd1306_page lower_bound,
                                          enum ssd1306_row vertical_offset);
/**
 * @implements SSD1306_SET_VERT_SCROLL_AREA
 *
 * @param ctx          struct that contains the platform dependent I/O
 * @param static_rows  number of rows in the top, fixed area
 * @param dynamic_rows number of rows in the bottom, scrolling area
 */
enum ssd1306_err ssd1306_set_vert_scroll_area(struct ssd1306_ctx *ctx,
                                              enum ssd1306_row static_rows,
                                              enum ssd1306_row dynamic_rows);

/** @} */ /* scrolling_commands */

/**
 * @defgroup address_setting_commands Address Settings Commands
 *
 * @ingroup commands
 */

/** @{ */

/**
 * The enumerated commands from `Table 9-3`
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
     * @note This command applies to the following addressing modes(s):
     * - @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or @ref ssd1306_page start_page[2:0]
     */
    SSD1306_SET_SINGLE_PAGE_ADDR = 0xB0,
    /**
     * Set the upper nybble of the column.
     *
     * @note This command applies to the following addressing modes(s):
     * - @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or upper_nybble[3:0]
     */
    SSD1306_SET_UPPER_NYBBLE_COL_ADDR = 0x10,
    /**
     * Set the lower nybble of the column.
     *
     * @note This command applies to the following addressing modes(s):
     * - @ref SSD1306_PAGE_ADDR_MODE
     *
     * @cmdarg_or lower_nybble[3:0]
     */
    SSD1306_SET_LOWER_NYBBLE_COL_ADDR = 0x00,

    /**
     * Set the start and end page addresses.
     *
     * @note This command applies to the following addressing modes(s):
     * - @ref SSD1306_HORIZ_ADDR_MODE
     * - @ref SSD1306_VERT_ADDR_MODE
     */
    SSD1306_SET_MULTIPLE_PAGE_ADDRS = 0x22,
    /**
     * Set the start and end column addresses.
     *
     * @note This command applies to the following addressing modes(s):
     * - @ref SSD1306_HORIZ_ADDR_MODE
     * - @ref SSD1306_VERT_ADDR_MODE
     */
    SSD1306_SET_MULTIPLE_COL_ADDRS = 0x21,
};

/**
 * The available modes used as arguments to the command
 * @ref SSD1306_SET_MEM_ADDR_MODE. For the description of the modes, assume you
 * start at `(PAGE_0, COL_0)`.
 */
enum ssd1306_addr_mode {
    /**
     * Walk the entire page until you reach the end (i.e. the last column).
     * Afterwards, go to the beginning of the next page and repeat. Once you
     * reach the last column of the last page, go to the very beginning
     * `(PAGE_0, COL_0)`.
     */
    SSD1306_HORIZ_ADDR_MODE,
    /**
     * Walk the entire column until you reach the end (i.e. the last page).
     * Afterwards, go to the beginning of the next column and repeat. Once you
     * reach the last page of the last column, go to the very beginning
     * `(PAGE_0, COL_0)`.
     */
    SSD1306_VERT_ADDR_MODE,
    /**
     * Walk the entire page until you reach the end (i.e. the last column).
     * Afterwards, loop back to where you started (i.e. `(PAGE_0, COL_0)`).
     *
     * In order to go to a different page, you must call the following command:
     *
     * - @ref SSD1306_SET_SINGLE_PAGE_ADDR
     *
     * If you want to go to a different column, refer to the following commands:
     *
     * - @ref SSD1306_SET_UPPER_NYBBLE_COL_ADDR
     * - @ref SSD1306_SET_LOWER_NYBBLE_COL_ADDR
     */
    SSD1306_PAGE_ADDR_MODE,

    SSD1306_NUM_ADDR_MODE,
};

/**
 * These values are used as arguments to the following command:
 *
 * - @ref SSD1306_SET_MULTIPLE_COL_ADDRS
 *
 * These values can also be used as arguments to the following commands:
 *
 * - @ref SSD1306_SET_UPPER_NYBBLE_COL_ADDR
 * - @ref SSD1306_SET_LOWER_NYBBLE_COL_ADDR
 *
 * You have to make sure to grab the appropriate nybble of the value for both
 * commands.
 */
enum ssd1306_col {
    SSD1306_COL_0,
    SSD1306_COL_1,
    SSD1306_COL_2,
    SSD1306_COL_3,
    SSD1306_COL_4,
    SSD1306_COL_5,
    SSD1306_COL_6,
    SSD1306_COL_7,
    SSD1306_COL_8,
    SSD1306_COL_9,
    SSD1306_COL_10,
    SSD1306_COL_11,
    SSD1306_COL_12,
    SSD1306_COL_13,
    SSD1306_COL_14,
    SSD1306_COL_15,
    SSD1306_COL_16,
    SSD1306_COL_17,
    SSD1306_COL_18,
    SSD1306_COL_19,
    SSD1306_COL_20,
    SSD1306_COL_21,
    SSD1306_COL_22,
    SSD1306_COL_23,
    SSD1306_COL_24,
    SSD1306_COL_25,
    SSD1306_COL_26,
    SSD1306_COL_27,
    SSD1306_COL_28,
    SSD1306_COL_29,
    SSD1306_COL_30,
    SSD1306_COL_31,
    SSD1306_COL_32,
    SSD1306_COL_33,
    SSD1306_COL_34,
    SSD1306_COL_35,
    SSD1306_COL_36,
    SSD1306_COL_37,
    SSD1306_COL_38,
    SSD1306_COL_39,
    SSD1306_COL_40,
    SSD1306_COL_41,
    SSD1306_COL_42,
    SSD1306_COL_43,
    SSD1306_COL_44,
    SSD1306_COL_45,
    SSD1306_COL_46,
    SSD1306_COL_47,
    SSD1306_COL_48,
    SSD1306_COL_49,
    SSD1306_COL_50,
    SSD1306_COL_51,
    SSD1306_COL_52,
    SSD1306_COL_53,
    SSD1306_COL_54,
    SSD1306_COL_55,
    SSD1306_COL_56,
    SSD1306_COL_57,
    SSD1306_COL_58,
    SSD1306_COL_59,
    SSD1306_COL_60,
    SSD1306_COL_61,
    SSD1306_COL_62,
    SSD1306_COL_63,
    SSD1306_COL_64,
    SSD1306_COL_65,
    SSD1306_COL_66,
    SSD1306_COL_67,
    SSD1306_COL_68,
    SSD1306_COL_69,
    SSD1306_COL_70,
    SSD1306_COL_71,
    SSD1306_COL_72,
    SSD1306_COL_73,
    SSD1306_COL_74,
    SSD1306_COL_75,
    SSD1306_COL_76,
    SSD1306_COL_77,
    SSD1306_COL_78,
    SSD1306_COL_79,
    SSD1306_COL_80,
    SSD1306_COL_81,
    SSD1306_COL_82,
    SSD1306_COL_83,
    SSD1306_COL_84,
    SSD1306_COL_85,
    SSD1306_COL_86,
    SSD1306_COL_87,
    SSD1306_COL_88,
    SSD1306_COL_89,
    SSD1306_COL_90,
    SSD1306_COL_91,
    SSD1306_COL_92,
    SSD1306_COL_93,
    SSD1306_COL_94,
    SSD1306_COL_95,
    SSD1306_COL_96,
    SSD1306_COL_97,
    SSD1306_COL_98,
    SSD1306_COL_99,
    SSD1306_COL_100,
    SSD1306_COL_101,
    SSD1306_COL_102,
    SSD1306_COL_103,
    SSD1306_COL_104,
    SSD1306_COL_105,
    SSD1306_COL_106,
    SSD1306_COL_107,
    SSD1306_COL_108,
    SSD1306_COL_109,
    SSD1306_COL_110,
    SSD1306_COL_111,
    SSD1306_COL_112,
    SSD1306_COL_113,
    SSD1306_COL_114,
    SSD1306_COL_115,
    SSD1306_COL_116,
    SSD1306_COL_117,
    SSD1306_COL_118,
    SSD1306_COL_119,
    SSD1306_COL_120,
    SSD1306_COL_121,
    SSD1306_COL_122,
    SSD1306_COL_123,
    SSD1306_COL_124,
    SSD1306_COL_125,
    SSD1306_COL_126,
    SSD1306_COL_127,
};

/** @} */ /* address_setting_commands */

/**
 * @defgroup hardware_configuration_commands Hardware Configuration Commands
 *
 * @ingroup commands
 */

/** @{ */

/**
 * The enumerated commands from `Table 9-4`
 */
enum ssd1306_hw_config_command {
    /**
     * Set the display's start line register.
     *
     * @cmdarg_or @ref ssd1306_row display_start_line_register[5:0]
     */
    SSD1306_SET_DISPLAY_START_LINE = 0x40,
    /**
     * Set a vertical offset.
     */
    SSD1306_SET_VERT_DISPLAY_OFFSET = 0xD3,

    /**
     * Vertically reflect the display.
     *
     * @note The effect takes place on subsequent data writes.
     */
    SSD1306_REFLECT_DISPLAY_VERT = 0xA0,
    /**
     * Don't vertically reflect the display.
     *
     * @note The effect takes place on subsequent data writes.
     */
    SSD1306_DONT_REFLECT_DISPLAY_VERT = 0xA1,

    /**
     * Horizontally reflect the display.
     *
     * @note The effect takes place on subsequent data writes.
     */
    SSD1306_REFLECT_DISPLAY_HORIZ = 0xC0,
    /**
     * Don't horizontally reflect the display.
     *
     * @note The effect takes place on subsequent data writes.
     */
    SSD1306_DONT_REFLECT_DISPLAY_HORIZ = 0xC8,
    /**
     * Set multiple, alternative common pin configurations.
     */
    SSD1306_CONFIG_COMMON_PINS = 0xDA,
    /**
     * Set the number of active rows. Choosing a value of @c SSD1306_ROW_15
     * means that rows 0 to 15 will be active, a total of 16 rows.
     *
     * @note The argument must be a value between @c SSD1306_ROW_15 and
     *       @c SSD1306_ROW_63.
     */
    SSD1306_SET_NUM_ACTIVE_ROWS = 0xA8,
};

/**
 * These are various configurations available for the common pins. They are
 * meant to be used as arguments to the following command:
 *
 * @ref SSD1306_CONFIG_COMMON_PINS
 *
 * @todo Find apt descriptions for the various configurations!!
 */
enum ssd1306_common_pin_config {
    SSD1306_SEQUENTIAL_COMMON_PINS = 0x02,
    SSD1306_ALT_COMMON_PINS = 0x12,

    SSD1306_ENABLE_LEFT_RIGHT_REMAP = 0x22,
    SSD1306_DISABLE_LEFT_RIGHT_REMAP = 0x02,
};

/** @} */ /* hardware_configuration_commands */

/**
 * @defgroup timing_and_driving_scheme_setting_commands \
 *      Timing/Driving Scheme Setting Commands
 *
 * @ingroup commands
 */

/** @{ */

/**
 * The enumerated commands from `Table 9-5`
 */
enum ssd1306_timing_and_driving_command {
    /**
     * Set the display clock divide ratio as well as the oscillator's
     * frequency.
     */
    SSD1306_SET_CLOCK_DIV_AND_OSC_FREQ = 0xD5,
    /**
     * Set Pre-charge period
     */
    SSD1306_SET_PRECHARGE_PERIOD = 0xD9,
    /**
     * Set Vcomh's deselect level.
     */
    SSD1306_SET_V_COMH_DESELECT_LEVEL = 0xDB,
    /**
     * No operation.
     */
    SSD1306_NOOP = 0xE3,
};

/**
 * These values are meant to be used as arguments to the following commnand:
 *
 * @ref SSD1306_SET_V_COMH_DESELECT_LEVEL
 */
enum ssd1306_vcomh_deselect_level {
    SSD1306_POINT_65_X_VCC = 0x00, /**< Multiply Vcc by ~0.65 */
    SSD1306_POINT_77_X_VCC = 0x20, /**< Multiply Vcc by ~0.77 */
    SSD1306_POINT_83_X_VCC = 0x30, /**< Multiply Vcc by ~0.83 */
};

/** @} */ /* timing_and_driving_scheme_setting_commands */

#endif /* LIBSSD1306_SSD1306_SSD1306_H */
