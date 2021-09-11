libSSD1306
=======

Description
-----------
`libSSD1306` is a library meant for programming OLEDs driven by an
`SSD1306` controller. There are other IC's that mimic the instruction set of
the `SSD1306`. The most prominent one out there is the `SSH1106`. Getting this
library to work with the `SSD1306` clones shouldn't require too much work.

Commands
--------
There are 3 "classes" of commands.

* Commands that do not accept arguments
* Commands that accept arguments
* Commands that accept arguments but the arguments are OR'd with the command

The last 2 classes of commands will be annotated in the generated documentation
as `arguments` and `OR'd arguments`, respectively. The annotations will describe
what the arguments are and in what order the arguments should be sent. As
far as the controller is concerned, there are no optional arguments, all
arguments are required!

TODO: add example of command that must be OR'd with arguments

All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they are in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding `enum` identifier is `enum
ssd1306_fundamental_command`.
