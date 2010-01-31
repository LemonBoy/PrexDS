/*-
 * Copyright (c) 2008, Kohsuke Ohtani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * head.S - generic header code for ARM platforms.
 */

#include <conf/config.h>
#include <machine/memory.h>
#include <machine/syspage.h>

@---------------------------------------------------------------------------------
@ DS processor selection
@---------------------------------------------------------------------------------
    .arch   armv5te
    .cpu    arm946e-s
@---------------------------------------------------------------------------------

#define CP15_DTCM_ENABLE    (1 << 16)
#define CP15_ITCM_ENABLE    (1 << 18)

#define ENTRY(x) .global x; .align; x##:

    .section ".text","ax"
    .code 32
/*
 * Loader entry point
 */
ENTRY(boot_entry)

    /* Disable interrupts (IME = 0) */
    mov r0, #0x04000000
    str r0, [r0, #0x208]

    bl cp15_setup

    mov r0, #0xd3       /* Enter SVC mode, Disable IRQ,FIQ */
    msr cpsr_c, r0
    ldr sp, =BOOTSTKTOP

    b   main

/* to make the bootloader size 0x1FFF (see boot/arm/arch/boot.ld) */
    .section .tail,"ax"
dummy:
    .byte   0xff

.end
