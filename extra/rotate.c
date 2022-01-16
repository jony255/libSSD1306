/**
 * Rotate the 8x8 array present in `font8x8_basic.h` by +90 degrees.
 *
 * The result of the conversion is printed to stdout.
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "font8x8_basic.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define FONT_WIDTH   ARR_LEN(font8x8_basic[0])

/**
 * Prints the contents of an array to `stdout` in a way similar to the one
 * defined in `font8x8_basic.h`.
 */
static void
print_arr(const uint8_t *arr, uint8_t glyph_index)
{
    printf("    {");

    for (size_t i = 0; i < FONT_WIDTH; i++) {
        uint8_t val = arr[i];

        printf("0x%02X", val);

        /*
         * Don't print the last comma so `clang-format` doesn't format all of
         * the elements vertically. I want them all horizontal.
         */
        if (i != FONT_WIDTH - 1) {
            printf(",");
        }
    }

    printf("}, /* U+00%02X (%c) */\n", glyph_index,
           isprint(glyph_index) ? glyph_index : ' ');
}

/**
 * Extracts a single bit from the byte and returns it.
 *
 * @param num number to check for a set bit
 * @param bit bit to check within `num`
 *
 * @return 1 if the bit `bit` was set within `num`
 * @return 0 otherwise
 */
static uint8_t
extract_bit(uint8_t num, uint8_t bit)
{
    return (num >> bit) & 0x01;
}

/**
 * Rotates an 8 bit x 8 bit matrix +90 degrees.
 *
 * @param original array to rotate
 * @param output   array to place result of rotation
 */
static void
rotate_8x8_bit_plus_90(const uint8_t *original, uint8_t *output)
{
    // for each element of output
    for (size_t i = 0; i < FONT_WIDTH; i++) {
        uint8_t tmp = 0x00;

        // for each element in original
        // reversed to omit 2 `FONT_WIDTH - j` calculations
        for (int j = FONT_WIDTH - 1; j >= 0; j--) {
            uint8_t current_val = original[j];

            uint8_t bit = extract_bit(current_val, i);

            tmp |= (bit << j);
        }

        output[i] = tmp;
    }
}

int
main(void)
{
    uint8_t output[FONT_WIDTH] = {0};

    printf(
        "#include <stdint.h>\n\n"
        "static const uint8_t font8x8_basic[][%lu] = {\n",
        FONT_WIDTH);

    for (size_t i = 0; i < ARR_LEN(font8x8_basic); i++) {
        const uint8_t *const original_glyph = font8x8_basic[i];

        rotate_8x8_bit_plus_90(original_glyph, output);

        print_arr(output, i);
    }

    printf("};\n");

    return EXIT_SUCCESS;
}
