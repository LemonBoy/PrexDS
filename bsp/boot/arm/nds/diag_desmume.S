/*
 * Print a string to DeSmuME emulator console.
 *
 * Important:
 * This BIOS call is not supported by actual NDS BIOS. So, you must
 * disable this function when you run it on actual NDS H/W.
 * (by commenting DIAG_DESMUME in the config file)
 *
 */

    .text
    .align 2

    .global diag_desmume_puts

diag_desmume_puts:

    swi 0xfcfcfc
    mov pc, lr
