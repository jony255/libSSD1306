#!/bin/sh

DATASHEET_URL='https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf'
PDF_FILE='SSD1306.pdf'

does_cmd_exist() {
    CMD="$1"

    command -v "$CMD" > /dev/null 2>&1
}

if does_cmd_exist curl; then
    DL_CMD='curl'
    DL_CMD_OPTS="--output $PDF_FILE"
elif does_cmd_exist wget; then
    DL_CMD='wget'
    DL_CMD_OPTS="--no-verbose --show-progress --output-document $PDF_FILE"
else
    printf 'could not find a suitable downloader (curl/wget)\n' 1>&2
    exit 1
fi

# Don't double quote OPTS if you need the OPTS variable to word split.
# You want the variable to "split" so the command's options are passed
# in correctly
#
# OPTS='--first-opt --second-opt'
#
# If opts IS double-quoted, the command would see this
# '--first-opt --second-opt' as its first and only argument.
#
# If opts ISN'T double-quoted, the command would see this
# --first-opt --second-opt
#
# --first-opt being its first argument and
# --second-opt being its second argument.
#
# One thing to keep in mind though, is that if OPTS can be empty, an
# empty pair of quotes would be passed into the command. The quoting
# makes no difference if there is only one option to pass in.
"$DL_CMD" $DL_CMD_OPTS "$DATASHEET_URL"
