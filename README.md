# libSSD1306

## Table of contents

- [Description](#description)
- [Prerequisites](#prerequisites)
- [Getting started](#getting_started)
    - [Download necessary files](#download_necessary_files)
    - [Configure and build the library](#configure_and_build_the_library)
    - [Build the documentation](#build_the_documentation)
- [Commands](#commands)
    - [Command definitions](#command_definitions)
    - [Classes of commands](#classes_of_commands)
        - [Documenting commands or'd with arguments](#documenting_commands_ord_with_arguments)
- [Communicating with the SSD1306](#communicating_with_the_ssd1306)
    - [Option #1 - Configuration done at link-time](#option_1_link_time)
        - [Pros](#option_1_link_time_pros)
        - [Cons](#option_1_link_time_cons)
    - [Option #2 - Configuration done at run-time](#option_2_run_time)
        - [Pros](#option_2_run_time_pros)
        - [Cons](#option_2_run_time_cons)
    - [And the winner is...](#and_the_winner_is)
    - [Implementing option 2](#implementing_option_2_four)
        - [Explaining platform dependent operations](#explaining_platform_dependent_operations)
        - [Code snippet using libopencm3s HAL](#code_snippet_using_libopencm3s_hal)

<a id="description"></a>
## Description

`libSSD1306` is a library meant for programming OLEDs driven by an
`SSD1306` controller. There are other IC's that mimic the instruction set of
the `SSD1306`. The most prominent clone out there is the `SSH1106`. Getting
this library to work with the `SSH1106` shouldn't require too much work.

<a id="prerequisites"></a>
## Prerequisites

- `meson`, `ninja`, and a `c99` compliant compiler for building the library
- `curl` or `wget` (optional, for downloading the `SSD1306`'s datasheet)
- `git` (optional, `libSSD1306` can be downloaded as a zip archive)
- `doxygen` (optional, to build the documentation)

<a id="getting_started"></a>
## Getting started

<a id="download_necessary_files"></a>
### Download necessary files

To begin using this library, clone the repo.

    $ git clone https://github.com/maybe-one-day-ubermensch/libSSD1306

Once you have cloned the repo, download the SSD1306's datasheet like so:

    $ ./scripts/dl_ssd1306_datasheet.sh

The script assumes you have either `curl` or `wget` in your `$PATH`. If you
don't have any of the two, install one of them. They are both pretty useful!
Alternatively, you can copy the datasheet's url from the script, paste it into
your browser, and download it from there.

<a id="configure_and_build_the_library"></a>
### Configure and build the library

Next, configure the build by calling `meson`:

    $ BUILD_DIR='build'
    $
    $ # The build-system currently defaults to building a static library.
    $ meson "$BUILD_DIR"

or

    $ # Tell meson to build a shared libray instead
    $ meson -Ddefault_libray=shared "$BUILD_DIR"

or

    $ # Tell meson to build both types of libraries
    $ meson -Ddefault_libray=both "$BUILD_DIR"

Finally, build the library:

    $ ninja -C "$BUILD_DIR"

todo(add examples on how to build with cross-compilers like clang or arm-none-eabi-gcc)
todo(those examples should include example [machine files](https://mesonbuild.com/Machine-files.html))

<a id="build_the_documentation"></a>
### Build the documentation

In order to build the documentation run the following command:

    $ doxygen ./docs/Doxyfile

Open the generated documentation by pointing your preferred browser to `docs/html/index.html`.
For example, if you use firefox, run the following command:

    $ firefox ./docs/html/index.html

<a id="commands"></a>
## Commands

<a id="command_definitions"></a>
### Command definitions

All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they are in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding type declaration
`enum ssd1306_fundamental_command`. All of the `enum` definitions can be found
in `./include/ssd1306/ssd1306.h`

<a id="classes_of_commands"></a>
### Classes of commands

There are 3 "classes" of commands.

* Commands that do not accept arguments
* Commands that accept arguments
* Commands that accept arguments but the arguments are OR'd with the command

As far as the controller is concerned, there are no optional arguments, all
commands that accept arguments require them.

<a id="documenting_commands_ord_with_arguments"></a>
#### Documenting commands or'd with arguments

The last class of commands are annotated in the generated documentation as
`OR'd arguments`. The annotations describe what the arguments are and their
position in the command. The format of the doxygen alias to generate said
documentation is as follows:

```
@cmdarg_or [@ref lib_defined_type] argument_name[x:y]
```

The `@ref lib_defined_type` is a reference link to a datatype defined by
`libSSD1306`, if one exists. Otherwise, leave it blank. Here is an example from
`include/ssd1306/ssd1306.h`.

```
# 2:0 = a 3 bit value going from bit 2 to bit 0
# That 3 bit value is of type ssd1306_page
@cmdarg_or @ref ssd1306_page start_page[2:0]
```

Here is an another example but without the type.

```
# 3:0 = a 4 bit value going from bit 3 to bit 0
@cmdarg_or upper_nybble[3:0]
```

<a id="communicating_with_the_ssd1306"></a>
## Communicating with the SSD1306

There is no universal way to configure all `I2C`, `SPI`, etc.. peripherals in
the world. There are some HAL's provided by manufacturers or RTOSes that enable
cross-platform development, but maintaining all of these configurations by
myself is error-prone. Instead, I will have the user define functions which I
have declared. There are 2 ways of implementing what I have in mind.

<a id="option_1_link_time"></a>
### Option #1 - configuration done at link-time

I will declare functions in a header file that I expect the user of the library
to implement. The user will have to write their glue code within `libSSD1306`'s
codebase.

<a id="option_1_link_time_pros"></a>
#### Pros

1. Function calls to the glue code can be inlined by the compiler.

<a id="option_1_link_time_cons"></a>
#### Cons

1. Stuck with only one way of communicating with the `SSD1306`.

2. Can complicate the build-system setup if more than one microcontroller is
   used within the same codebase.

3. The glue code must be compiled with the library and violates the
   [open-closed principle](https://en.wikipedia.org/wiki/Open-closed_principle).
   I could [weakly](https://en.wikipedia.org/wiki/Weak_symbol) define the
   functions to get around this, but that requires a compiler attribute which
   is not supported by standard C.

<a id="option_2_run_time"></a>
### Option #2 - configuration done at run-time

I will declare a struct whose fields consist of pointers to functions that are
exactly the same as the stubs I would have declared in the header file from
Option 1. The user must then pass a pointer to this struct to all of the
functions in the library.

<a id="option_2_run_time_pros"></a>
#### Pros

1. The user can change how the mcu can communicate with the `SSD1306` at
   run-time.

2. Can be compiled into a static library, separate from the glue code, then
   linked with the user's main application. As a result, this follows the
   [open-closed principle](https://en.wikipedia.org/wiki/Open-closed_principle).

<a id="option_2_run_time_cons"></a>
#### Cons

1. Some of the esoteric compilers haven't implemented function pointers.
   However, as mentioned in the prerequisites, a `c99` compliant compiler
   is required.

2. The compiler may not inline calls to the functions as a result of using
   function pointers. However, these functions aren't meant to be doing much
   anyway so it may not matter too much. `const`'ing the struct or some of the
   fields of the struct may fix the inlining issue.
   todo(a simple example on godbolt.com didn't show a difference)
   todo(between calling a function from a const/non-const pointer.)
   todo(I need to figure out what's going on...)

3. Extra `NULL` checking must be done by the functions to ensure the struct is
   valid. I can setup a compile-time switch to disable the checks with
   something like `-DNDEBUG` to denote a release build.

4. Might require a bit more memory as a result of storing `x` amount of
   function pointers, where `x` is the number of stubs declared. It's honestly
   not much but it might be on a *very* memory constrained platform.

<a id="and_the_winner_is"></a>
### And the winner is...

Option 2!!

Option 2 is more portable across c compilers and, I feel, is the cleanest. This
option doesn't require altering the `libSSD1306` codebase. The code the user
writes is, by design, independent of the library's.

<a id="implementing_option_2_four"></a>
### Implementing option 2

<a id="explaining_platform_dependent_operations"></a>
#### Explaining platform dependent operations

All of the code that pertains to platform dependent operations can be found in
`./include/ssd1306/platform.h`. In it, there are two `typedef`s and a `struct`
that are responsible for configuring the I/O of the library:

```c
typedef enum ssd1306_err (*ssd1306_send_cmd_cb)(struct ssd1306_ctx *ctx,
                                                uint8_t cmd);
typedef enum ssd1306_err (*ssd1306_write_data_cb)(struct ssd1306_ctx *ctx,
                                                  uint8_t data);

struct ssd1306_ctx {
    /**
     * User supplied callback that sends a command to the SSD1306.
     */
    ssd1306_send_cmd_cb send_cmd;
    /**
     * User supplied callback that writes data to the SSD1306's memory.
     */
    ssd1306_write_data_cb write_data;

    /**
     * Custom data that a user might want available in their supplied callbacks.
     */
    void *user_ctx;
};
```

As mentioned in
[Option #2 - Configuration done at run-time](#option_2_run_time), you will pass
in an instance of `struct ssd1306_ctx` to all functions. The struct's
`send_cmd` and `write_data` fields must store the pointer to the functions that
implement the I/O.

<a id="code_snippet_using_libopencm3s_hal"></a>
#### Code snippet using libopencm3s HAL

The following is a small example on how to configure/use `libSSD1306` with
[libopencm3's](https://libopencm3.org/) HAL.

The snippet assumes you have already configured the SPI peripheral and will
focus only on how to interface `libopencm3` with `libSSD1306`. The wiring used
in this example is explained in detail in the `SSD1306`'s datasheet (8.1.3) and
will be referred to from here on out as 4-wire SPI. (SDIN, SCLK, CS, D/C)

```c
/* chip select is active LOW */

#define CS_GPIO_BANK     GPIOC
#define CS_GPIO_PORT_NUM GPIO9

#define DC_GPIO_BANK     GPIOC
#define DC_GPIO_PORT_NUM GPIO2

static void set_chip_select_high(void) {
    gpio_set(CS_GPIO_BANK, CS_GPIO_PORT_NUM);
}
static void set_chip_select_low(void) {
    gpio_clear(CS_GPIO_BANK, CS_GPIO_PORT_NUM);
}

static void mark_as_data(void) {
    gpio_set(DC_GPIO_BANK, DC_GPIO_PORT_NUM);
}
static void mark_as_command(void) {
    gpio_clear(DC_GPIO_BANK, DC_GPIO_PORT_NUM);
}

static enum ssd1306_err send_cmd(struct ssd1306_ctx *ctx, uint8_t cmd) {
    set_chip_select_low();

    mark_as_command();

    spi_send(SPI2, (uint8_t)cmd);

    set_chip_select_high();

    return SSD1306_OK;
}

static enum ssd1306_err write_data(struct ssd1306_ctx *ctx, uint8_t data) {
    set_chip_select_low();

    mark_as_data();

    spi_send(SPI2, (uint8_t)data);

    set_chip_select_high();

    return SSD1306_OK;
}

int main(int argc, char *argv[]) {
    /*
     * libopencm3 setup done ...
     *
     * Assume GPIO ports C9 = chip select and C2 = data/command.
     * Assume SPI2 peripheral is being used.
     */

    struct ssd1306_ctx ctx = {
        .send_cmd = &send_cmd,
        .write_data = &write_data,
    };

    ssd1306_setup(&ctx);

    ssd1306_set_contrast(&ctx, 4);

    return 0;
}
```
