#ifndef LIBSSD1306_SSD1306_PLATFORM_H
#define LIBSSD1306_SSD1306_PLATFORM_H

#include "ssd1306/err.h"

#include <stdint.h>

/**
 * @defgroup platform_dependent_operations Platform Dependent Operations
 *
 * This module houses all the types that need to be implemented per platform.
 *
 * @{
 */

/**
 * An instance of this struct must be populated and passed in to all functions
 * as it contains all the platform dependent operations.
 */
struct ssd1306_ctx;

/**
 * A callback typedef that is in charge of sending a command to the SSD1306.
 * The user can safely assume that @c ctx will be not be @c NULL.
 *
 * @param ctx Pointer to the @ref ssd1306_ctx.
 * @param cmd Command to send.
 *
 * @return an appropriate error code of type enum @ref ssd1306_err
 */
typedef enum ssd1306_err (*ssd1306_send_cmd_cb)(struct ssd1306_ctx *ctx,
                                                uint8_t cmd);
/**
 * A callback typedef that is in charge of writing data to the SSD1306's memory.
 * The user can safely assume that @c ctx will be not be @c NULL.
 *
 * @param ctx  Pointer to the @ref ssd1306_ctx.
 * @param data Data to write to RAM.
 *
 * @return an appropriate error code of type enum @ref ssd1306_err
 */
typedef enum ssd1306_err (*ssd1306_write_data_cb)(struct ssd1306_ctx *ctx,
                                                  uint8_t data);

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
     * User supplied callback that sends a command to the SSD1306.
     */
    const ssd1306_send_cmd_cb send_cmd;
    /**
     * User supplied callback that writes data to the SSD1306's memory.
     */
    const ssd1306_write_data_cb write_data;

    /**
     * Custom data that a user might want available in their supplied callbacks.
     */
    void *user_ctx;
};

/** @} */ /* platform_dependent_operations */

#endif /* LIBSSD1306_SSD1306_PLATFORM_H */
