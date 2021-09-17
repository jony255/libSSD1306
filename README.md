# libSSD1306

## Description

`libSSD1306` is a library meant for programming OLEDs driven by an
`SSD1306` controller. There are other IC's that mimic the instruction set of
the `SSD1306`. The most prominent clone out there is the `SSH1106`. Getting this
library to work with the `SSH1106` shouldn't require too much work.

## Commands

### Command Definitions

All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they are in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding type declaration
`enum ssd1306_fundamental_command`.

### Classes of Commands

There are 3 "classes" of commands.

* Commands that do not accept arguments
* Commands that accept arguments
* Commands that accept arguments but the arguments are OR'd with the command

As far as the controller is concerned, there are no optional arguments, all
commands that accept arguments require them.

#### Documenting Commands OR'd with Arguments

The last class of commands will be annotated in the generated documentation as
`OR'd arguments`. The annotations will describe what the arguments are and their
position in the command. The format of the doxygen alias to generate said
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
