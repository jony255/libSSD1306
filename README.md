# libSSD1306

## Table of contents

- [Description](#description)
- [Prerequisites](#prerequisites)
- [Getting started](#getting_started)
    - [Download necessary files](#download_necessary_files)
    - [Compiling a native build of the library](#compiling_a_native_build_of_the_library)
    - [Compiling a cross build of the library](#compiling_a_cross_build_of_the_library)
    - [Build the documentation](#build_the_documentation)
- [SSD1306 commands](#ssd1306_commands)
    - [Command definitions](#command_definitions)
    - [Classes of commands](#classes_of_commands)
        - [Documenting commands or'd with arguments](#documenting_commands_ord_with_arguments)
- [Communicating with the SSD1306](#communicating_with_the_ssd1306)
    - [Option #1 - Configuration done at link-time of the static library](#option_1_link_time)
        - [Pros](#option_1_link_time_pros)
        - [Cons](#option_1_link_time_cons)
    - [Option #2 - Configuration done by the user's application](#option_2_users_application)
        - [Pros](#option_2_users_application_pros)
        - [Cons](#option_2_users_application_cons)
    - [And the winner is...](#and_the_winner_is)
    - [Implementing option 2](#implementing_option_2_four)
        - [Explaining platform dependent operations](#explaining_platform_dependent_operations)
        - [Example using libopencm3's HAL](#example_using_libopencm3s_hal)

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
- `make` (optional, provides aliases to quicken various aspects of developing/building the library)

<a id="getting_started"></a>
## Getting started

<a id="download_necessary_files"></a>
### Download necessary files

To begin using this library, clone the repo.

    $ git clone https://github.com/maybe-one-day-ubermensch/libSSD1306

Once you have cloned the repo, download the SSD1306's datasheet like so:

    $ ./extra/dl_ssd1306_datasheet.sh

The script assumes you have either `curl` or `wget` in your `$PATH`. If you
don't have any of the two, install one of them. They are both pretty useful!
Alternatively, you can copy the datasheet's url from the script, paste it into
your browser, and download it from there.

<a id="compiling_a_native_build_of_the_library"></a>
### Compiling a native build of the library

First, configure the build by calling `meson`:

    $ BUILD_DIR='build'
    $
    $ # meson defaults to building a shared library
    $ meson "$BUILD_DIR"

or

    $ # Tell meson to build a static library instead
    $ meson -Ddefault_library=static "$BUILD_DIR"

or

    $ # Tell meson to build both types of libraries
    $ meson -Ddefault_library=both "$BUILD_DIR"

Finally, build the library:

    $ ninja -C "$BUILD_DIR"

One thing to note is that `meson` defaults to building static libraries as
position independent. If you do not want to build static libraries as
position independent, then supply the `-Db_staticpic=false` argument when
intially configuring the build.

This method is quite simple for native builds, especially for single board
computers that run some linux distribution like the raspberry pi but what about
cross builds?

<a id="compiling_a_cross_build_of_the_library"></a>
### Compiling a cross build of the library

There is an
[example](https://github.com/maybe-one-day-ubermensch/libSSD1306-example.git)
repository that includes a complete and working example on how to cross compile
`libSSD1306`. Head over to the `machine-files` directory and read the
`README.md`. The `README.md` talks about how cross compiling `libSSD1306` with
`meson` looks like. In short, you independently describe each aspect of your
build in [machine files](https://mesonbuild.com/Machine-files.html) and, come
build time, layer the relavent machine files on top of one another.

<a id="build_the_documentation"></a>
### Build the documentation

In order to build and view the documentation, run the following command:

    $ make view-doxygen

This will open the generated `html` documentation defined in the `BROWSER`
environment variable. If `BROWSER` is empty, then it will default to `firefox`.
The `view-doxygen` targets always builds the documentation before opening it in
the browser.

However, if you only want to build the documentation, run the following command:

    $ make doxygen

<a id="ssd1306_commands"></a>
## SSD1306 commands

<a id="command_definitions"></a>
### Command definitions

All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they are in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding type declaration is
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
myself is error-prone. Instead, the user will have to implement an interface
that performs all the platform dependent I/O. As I see it, there are 2 ways of
implementing said interface.

<a id="option_1_link_time"></a>
### Option #1 - configuration done at link-time of the static library

The library's header files would contain the function prototypes of the interface
that needs to be implemented. The user would then have to write their glue code
within `libSSD1306`'s codebase.

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
   I could declare the functions as [weak symbols](https://en.wikipedia.org/wiki/Weak_symbol)
   to get around this, but that requires a compiler attribute which is not
   supported by standard C.

<a id="option_2_users_application"></a>
### Option #2 - configuration done by the user's application

The library's header file would declare a struct whose fields would be function
pointers to the platform dependent I/O. The user would then pass a pointer to
this struct to the library's functions.

<a id="option_2_users_application_pros"></a>
#### Pros

1. The user can change how the mcu can communicate with the `SSD1306` within
   their own code.

2. The user could choose to switch which functions are used to communicate
   with the `SSD1306`.

3. `libSSD1306` can be compiled into a static library, separate from the glue code,
   then linked with the user's main application. As a result, this follows the
   [open-closed principle](https://en.wikipedia.org/wiki/Open-closed_principle).

<a id="option_2_users_application_cons"></a>
#### Cons

1. The compiler may not inline calls to the functions as a result of using
   function pointers. However, these functions aren't meant to be doing much
   anyway so it may not matter too much. `const`'ing the struct or some of the
   fields of the struct may fix the inlining issue.
   todo(a simple example on godbolt.com didn't show a difference)
   todo(between calling a function from a const/non-const pointer.)
   todo(I need to figure out what's going on...)

2. Extra `NULL` checking must be done by the functions to ensure the struct is
   valid. I can setup a compile-time switch to disable the checks with
   something like `-DNDEBUG` to denote a release build.

3. Might require a bit more memory as a result of storing `x` amount of
   function pointers, where `x` is the number of platform dependent functions
   declared. It's honestly not much but it might be on a *very* memory
   constrained platform.

<a id="and_the_winner_is"></a>
### And the winner is...

Option 2!!

Option 2 is more portable across c compilers and, I feel, is the cleanest. This
option doesn't require altering the `libSSD1306` codebase. The code the user
writes is independent of the library's, as it should be.

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

    ...
};
```

As mentioned in [option #2](#option_2_users_application), you will pass
in an instance of `struct ssd1306_ctx *` to all library functions. The struct's
`ssd1306_ctx::send_cmd` and `ssd1306_ctx::write_data` fields are function pointers to the platform
dependent I/O.

<a id="example_using_libopencm3s_hal"></a>
#### Example using libopencm3's HAL

`libopencm3` provides a template repository for one to start their projects off
of. It is from that template that I built this
[example](https://github.com/maybe-one-day-ubermensch/libSSD1306-example.git)
repository. It provides a complete and working example to start you off.
