/*
 * Put one character to Desmume emulator console.
 *
 * Important:
 * This BIOS call is not supported by actual NDS BIOS. So, you must
 * disable this function when you run it on actual NDS H/W.
 *
 */

    .text
    .align 2

    .global desmume_puts

desmume_puts:

    swi 0xfcfcfc
    mov pc, lr
