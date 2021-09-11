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
All of the commands present in the datasheet are defined as `enum`s and grouped
according to the table they were in. For example, `Table 9-1`'s title is
`Fundamental Command`. Its corresponding `enum` identifier is
`enum ssd1306_fundamental_command`.

There are some commands that don't accept arguments. However, commands that do
accept arguments, require them. For example, `SSD1306_SET_CONTRAST_CTRL` accepts
one argument, the contrast level. This contrast level isn't optional and must be
supplied.
