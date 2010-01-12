/*
 * Put one character to Desmume emulator console.
 *
 * Important:
 * This BIOS call is not supported by actual NDS BIOS. So, you must
 * disable this function when you run it on actual NDS H/W.
 *
 * TODO file structure copied from libnds biosCalls.S, check what's really
 *      necessary.
 *
 */

    .thumb
    .align 4

@---------------------------------------------------------------------------------
    .global desmume_puts
    .thumb_func

@---------------------------------------------------------------------------------
desmume_puts:
@---------------------------------------------------------------------------------
    swi 0xfc
    bx  lr
