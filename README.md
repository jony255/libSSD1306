# libSSD1306

## Table of Contents

- [Description](#description)
- [Commands](#commands)
    - [Command Definitions](#command_definitions)
    - [Classes of Commands](#classes_of_commands)
        - [Documenting Commands OR'd with Arguments](#documenting_commands_ord_with_arguments)
- [Communicating with the SSD1306](#communicating_with_the_ssd1306)
    - [Option #1 - Configuration done at link-time](#option_1_link_time)
        - [Pros](#option_1_link_time_pros)
        - [Cons](#option_1_link_time_cons)
    - [Option #2 - Configuration done at run-time](#option_2_run_time)
        - [Pros](#option_2_run_time_pros)
        - [Cons](#option_2_run_time_cons)
    - [And the Winner is...](#and_the_winner_is)

<a id="description"></a>
## Description

`libSSD1306` is a library meant for programming OLEDs driven by an
`SSD1306` controller. There are other IC's that mimic the instruction set of
the `SSD1306`. The most prominent clone out there is the `SSH1106`. Getting
this library to work with the `SSH1106` shouldn't require too much work.

<a id="commands"></a>
## Commands

<a id="command_definitions"></a>
### Command Definitions

All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they are in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding type declaration
`enum ssd1306_fundamental_command`.

<a id="classes_of_commands"></a>
### Classes of Commands

There are 3 "classes" of commands.

* Commands that do not accept arguments
* Commands that accept arguments
* Commands that accept arguments but the arguments are OR'd with the command

As far as the controller is concerned, there are no optional arguments, all
commands that accept arguments require them.

<a id="documenting_commands_ord_with_arguments"></a>
#### Documenting Commands OR'd with Arguments

The last class of commands will be annotated in the generated documentation as
`OR'd arguments`. The annotations will describe what the arguments are and
their position in the command. The format of the doxygen alias to generate said
documentation is as follows:

```
@cmdarg_or [@ref lib_defined_type] argument_name[x:y]
```

The `@ref lib_defined_type` is a reference link to a datatype defined by
`libSSD1306`, if one exists. Otherwise, leave it blank. Here is an example from
`include/ssd1306.h`.

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
### Option #1 - Configuration done at link-time

I will declare functions in a header file that I expect the user of the library
to implement. The build system will take care of linking that header file's
implementation (.c file) to the library.

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
### Option #2 - Configuration done at run-time

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

1. Some of the esoteric compilers haven't implemented function pointers

   todo(need source, I don't remember where I heard this...)

2. The compiler may not inline calls to the functions as a result of using
   function pointers. However, these functions aren't meant to be doing much
   anyway so it may not matter too much. `const`'ing the struct or some of the
   fields of the struct may fix the inlining issue.

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
