#ifndef LIBSSD1306_SSD1306_FONT_H
#define LIBSSD1306_SSD1306_FONT_H

#include "ssd1306/err.h"
#include "ssd1306/platform.h"

#include <stdint.h>

/**
 * Writes a string to the display.
 *
 * @param ctx struct that contains the platform dependent I/O
 * @param str string to write to the display
 */
enum ssd1306_err ssd1306_write_str(struct ssd1306_ctx *ctx, const uint8_t *str);

/**
 * Writes a character to the display.
 *
 * @param ctx struct that contains the platform dependent I/O
 * @param c   character to write to the display
 */
enum ssd1306_err ssd1306_write_char(struct ssd1306_ctx *ctx, uint8_t c);

#endif /* LIBSSD1306_SSD1306_FONT_H */
