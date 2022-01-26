/**
 * @file
 */

#ifndef LIBSSD1306_SSD1306_PLATFORM_H
#define LIBSSD1306_SSD1306_PLATFORM_H

#include "ssd1306/err.h"

#include <stddef.h> /* size_t */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup platform_dependent_operations Platform Dependent Operations
 *
 * This module houses all the types that need to be implemented per platform.
 */

/** @{ */

/**
 * An instance of this struct must be populated and passed in to all functions
 * as it contains all the platform dependent operations.
 */
struct ssd1306_ctx;

/**
 * A callback typedef that is in charge of sending a command and that command's
 * argument(s), if it has any, to the SSD1306. The user can safely assume that
 * @c ctx will be not be @c NULL. Before actually transferring the byte, you
 * must signal to the SSD1306 that the byte you are about to transfer is to be
 * interpreted as a command. In other words, before the transfer, set the @c D/C
 * line low.
 *
 * @param ctx struct that contains the platform dependent I/O
 * @param cmd command to send
 *
 * @return an appropriate error code of type enum @ref ssd1306_err
 */
typedef enum ssd1306_err (*ssd1306_send_cmd_cb)(struct ssd1306_ctx *ctx,
                                                uint8_t cmd);

/**
 * A callback typedef that is in charge of writing data to the SSD1306's memory.
 * The user can safely assume that @c ctx will be not be @c NULL. Before
 * actually transferring the byte, you must signal to the SSD1306 that the byte
 * you are about to transfer is to be interpreted as data. In other words,
 * before the transfer, set the @c D/C line high.
 *
 * @param ctx  struct that contains the platform dependent I/O
 * @param data data to write to RAM
 *
 * @return an appropriate error code of type enum @ref ssd1306_err
 */
typedef enum ssd1306_err (*ssd1306_write_data_cb)(struct ssd1306_ctx *ctx,
                                                  uint8_t data);

/**
 * A callback typedef that is in charge of writing data stored in some array to
 * the SSD1306's memory.
 *
 * @ref ssd1306_write_data_list defaults to calling @ref ssd1306_write_data on
 * each element of @c data_list. Since @ref ssd1306_write_data must perform the
 * necessary setup/teardown to accomplish the transaction, this can be very
 * expensive when sending a list of data. If one writes @c n bytes, then the
 * setup/teardown happens @c n times. This includes marking the byte about to be
 * sent as data and is done by setting the @c D/C line high.
 *
 * This optional callback lets one perform the necessary setup once, before
 * looping over the elements of @c data_list, and once, after the loop.
 *
 * The user can safely assume that @c ctx will be not be @c NULL.
 *
 * @param ctx           struct that contains the platform dependent I/O
 * @param data_list     list of data
 * @param data_list_len length of @c data_list
 *
 * @return an appropriate error code of type enum @ref ssd1306_err
 */
typedef enum ssd1306_err (*ssd1306_write_data_list_cb)(struct ssd1306_ctx *ctx,
                                                       const uint8_t *data_list,
                                                       size_t data_list_len);

/**
 * An instance of this struct must be populated and passed in to all functions
 * as it contains all the platform dependent operations.
 *
 * @note The struct was forward declared so that its present before defining the
 *       typedef. I also had to move the struct's docs to the forward
 *       declaration because, otherwise, the hover information displayed by the
 *       lsp server would show the doxygen grouping commands. For that reason, I
 *       also decided to leave this note here and not on the forward declaration
 *       as the hover would also pick that up. The information shown by the
 *       hover doesn't need to show this note.
 *
 * @note I also had to copy-paste the description here so doxygen picks it up.
 */
struct ssd1306_ctx {
    /**
     * User supplied callback that sends a command and that command's
     * argument(s), if it has any, to the SSD1306.
     *
     * Instead of calling this field directly, use @ref ssd1306_send_cmd.
     */
    const ssd1306_send_cmd_cb send_cmd;
    /**
     * User supplied callback that writes data to the SSD1306's memory.
     *
     * Instead of calling this field directly, use @ref ssd1306_write_data.
     */
    const ssd1306_write_data_cb write_data;
    /**
     * **Optional**, user supplied callback that writes a list of data to the
     * SSD1306's memory.
     *
     * Instead of calling this field directly, use @ref ssd1306_write_data_list.
     */
    const ssd1306_write_data_list_cb write_data_list;

    /**
     * Custom data that a user might want available in their supplied callbacks.
     */
    void *user_ctx;

    /**
     * Number of maximum columns the OLED supports.
     */
    const uint16_t width;
    /**
     * Number of maximum rows the OLED supports.
     */
    const uint16_t height;
};

/**
 * Standard macro used to calculate the length of an array.
 *
 * Use this macro to calculate the length of @c cmd_list or @c data_list when
 * calling @ref ssd1306_send_cmd_list or @ref ssd1306_write_data_list,
 * respectively.
 *
 * @param arr array to calculate the length of
 *
 * @return length of @c arr
 */
#define SSD1306_ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

/**
 * Small wrapper that calls `ssd1306_ctx::send_cmd`.
 *
 * It's to avoid seeing `ctx->send_cmd(ctx, cmd)` all over the place.
 *
 * @param ctx struct that contains the platform dependent I/O
 * @param cmd command to send
 *
 * @return the result of `ctx->send_cmd(ctx, cmd)`
 */
enum ssd1306_err ssd1306_send_cmd(struct ssd1306_ctx *ctx, uint8_t cmd);

/**
 * Sends all of the commands present in @c cmd_list.
 *
 * This is useful when sending commands that require multiple arguments.
 * The commands are sent by calling @ref ssd1306_ctx::send_cmd on each command
 * in the list. @ref ssd1306_send_cmd_list returns immediately if
 * @ref ssd1306_ctx::send_cmd returns anything other than @ref SSD1306_OK.
 *
 * @param ctx          struct that contains all of the platform dependent I/O
 * @param cmd_list     list of the commands/arguments
 * @param cmd_list_len length of @c cmd_list
 */
enum ssd1306_err ssd1306_send_cmd_list(struct ssd1306_ctx *ctx,
                                       const uint8_t *cmd_list,
                                       size_t cmd_list_len);

/**
 * Small wrapper that calls `ssd1306_ctx::write_data`.
 *
 * It's to avoid seeing `ctx->write_data(ctx, data)` all over the place.
 *
 * @param ctx  struct that contains the platform dependent I/O
 * @param data data to write to RAM
 *
 * @return the result of `ctx->write_data(ctx, data)`
 */
enum ssd1306_err ssd1306_write_data(struct ssd1306_ctx *ctx, uint8_t data);

/**
 * Writes all of the data present in @c data_list.
 *
 * Calls @ref ssd1306_ctx::write_data_list if it is not @c NULL
 *
 * Otherwise, @ref ssd1306_ctx::write_data is called on each element of
 * @c data_list. @ref ssd1306_write_data_list returns immediately if
 * @ref ssd1306_ctx::write_data returns anything other than @ref SSD1306_OK.
 *
 * @param ctx           struct that contains all of the platform dependent I/O
 * @param data_list     list of data
 * @param data_list_len length of @c data_list
 */
enum ssd1306_err ssd1306_write_data_list(struct ssd1306_ctx *ctx,
                                         const uint8_t *data_list,
                                         size_t data_list_len);

/** @} */ /* platform_dependent_operations */

#ifdef __cplusplus
}
#endif

#endif /* LIBSSD1306_SSD1306_PLATFORM_H */
