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
#define PAGE_4K     (0b01011 << 1)
#define PAGE_8K     (0b01100 << 1)
#define PAGE_16K    (0b01101 << 1)
#define PAGE_32K    (0b01110 << 1)
#define PAGE_64K    (0b00111 << 1)
#define PAGE_128K   (0b10000 << 1)
#define PAGE_256K   (0b10001 << 1)
#define PAGE_512K   (0b10010 << 1)
#define PAGE_1M     (0b10011 << 1)
#define PAGE_2M     (0b10100 << 1)
#define PAGE_4M     (0b10101 << 1)
#define PAGE_8M     (0b10110 << 1)
#define PAGE_16M    (0b10111 << 1)
#define PAGE_32M    (0b11000 << 1)
#define PAGE_64M    (0b11001 << 1)
#define PAGE_128M   (0b11010 << 1)
#define PAGE_256M   (0b11011 << 1)
#define PAGE_512M   (0b11100 << 1)
#define PAGE_1G     (0b11101 << 1)
#define PAGE_2G     (0b11110 << 1)
#define PAGE_4G     (0b11111 << 1)

#define ITCM_LOAD   (1<<19)
#define ITCM_ENABLE (1<<18)
#define DTCM_LOAD   (1<<17)
#define DTCM_ENABLE (1<<16)
#define DISABLE_TBIT    (1<<15)
#define ROUND_ROBIN (1<<14)
#define ALT_VECTORS (1<<13)
#define ICACHE_ENABLE   (1<<12)
#define BIG_ENDIAN  (1<<7)
#define DCACHE_ENABLE   (1<<2)
#define PROTECT_ENABLE  (1<<0)

#define ENTRY(x) .global x; .align; x##:

    .section ".text","ax"
    .code 32
/*
 * Loader entry point
 */
ENTRY(boot_entry)

@---------------------------------------------------------------------------------
    mov r0, #0x04000000         @ IME = 0;
    str r0, [r0, #0x208]

@---------------------------------------------------------------------------------
@ turn the power on for M3
@---------------------------------------------------------------------------------
    ldr     r1, =0x8203
    add r0,r0,#0x304
    strh    r1, [r0]

    ldr r1, =0x00002078         @ disable TCM and protection unit
    mcr p15, 0, r1, c1, c0

@---------------------------------------------------------------------------------
@ Protection Unit Setup added by Sasq
@---------------------------------------------------------------------------------
    @ Disable cache
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0       @ Instruction cache
    mcr p15, 0, r0, c7, c6, 0       @ Data cache

    @ Wait for write buffer to empty
    mcr p15, 0, r0, c7, c10, 4

    ldr r0, =0x0b000000
    orr r0,r0,#0x0a
    mcr p15, 0, r0, c9, c1,0        @ DTCM base = __dtcm_start, size = 16 KB

    mov r0,#0x20
    mcr p15, 0, r0, c9, c1,1        @ ITCM base = 0 , size = 32 MB

@---------------------------------------------------------------------------------
@ Setup memory regions similar to Release Version
@---------------------------------------------------------------------------------

    @-------------------------------------------------------------------------
    @ Region 0 - IO registers
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_64M | 0x04000000 | 1)
    mcr p15, 0, r0, c6, c0, 0

    @-------------------------------------------------------------------------
    @ Region 1 - Main Memory
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_4M | 0x02000000 | 1)
    mcr p15, 0, r0, c6, c1, 0

    @-------------------------------------------------------------------------
    @ Region 2 - alternate vector base
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_4K | 0x00000000 | 1)
    mcr p15, 0, r0, c6, c2, 0

    @-------------------------------------------------------------------------
    @ Region 3 - DS Accessory (GBA Cart)
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_128M | 0x08000000 | 1)
    mcr p15, 0, r0, c6, c3, 0

    @-------------------------------------------------------------------------
    @ Region 4 - DTCM
    @-------------------------------------------------------------------------
    ldr r0,=0x0b000000
    orr r0,r0,#(PAGE_16K | 1)
    mcr p15, 0, r0, c6, c4, 0

    @-------------------------------------------------------------------------
    @ Region 5 - ITCM
    @-------------------------------------------------------------------------
    ldr r0,=0x01000100

    @ align to 32k boundary
    mov r0,r0,lsr #15
    mov r0,r0,lsl #15

    orr r0,r0,#(PAGE_32K | 1)
    mcr p15, 0, r0, c6, c5, 0

    @-------------------------------------------------------------------------
    @ Region 6 - System ROM
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_32K | 0xFFFF0000 | 1)
    mcr p15, 0, r0, c6, c6, 0

    @-------------------------------------------------------------------------
    @ Region 7 - non cacheable main ram
    @-------------------------------------------------------------------------
    ldr r0,=( PAGE_4M  | 0x02400000 | 1)
    mcr p15, 0, r0, c6, c7, 0

    @-------------------------------------------------------------------------
    @ Write buffer enable
    @-------------------------------------------------------------------------
    ldr r0,=0b00000010
    mcr p15, 0, r0, c3, c0, 0

    @-------------------------------------------------------------------------
    @ DCache & ICache enable
    @-------------------------------------------------------------------------
    ldr r0,=0b01000010
    mcr p15, 0, r0, c2, c0, 0
    mcr p15, 0, r0, c2, c0, 1

    @-------------------------------------------------------------------------
    @ IAccess
    @-------------------------------------------------------------------------
    ldr r0,=0x36636633
    mcr p15, 0, r0, c5, c0, 3

    @-------------------------------------------------------------------------
    @ DAccess
    @-------------------------------------------------------------------------
    ldr r0,=0x36333633
    mcr     p15, 0, r0, c5, c0, 2

    @-------------------------------------------------------------------------
    @ Enable ICache, DCache, ITCM & DTCM
    @-------------------------------------------------------------------------
    mrc p15, 0, r0, c1, c0, 0
    ldr r1,= ITCM_ENABLE | DTCM_ENABLE | ICACHE_ENABLE | DCACHE_ENABLE | PROTECT_ENABLE
    orr r0,r0,r1
    mcr p15, 0, r0, c1, c0, 0

    @mov r0, #0x13       @ Switch to SVC Mode
    @msr cpsr, r0
    @ldr sp, =__sp_svc       @ Set SVC stack

    mov r0, #0xd3       /* Enter SVC mode, Disable IRQ,FIQ */
    msr cpsr_c, r0
    @ldr sp, stack_end
    ldr sp, =0x0b003900

    mov r0,#0x04000000
    mov r1,#0x00010000
    str r1,[r0]

    b   main

/* to make the bootloader size 0x1FFF (see boot/arm/arch/boot.ld) */
    .section .tail,"ax"
dummy:
    .byte   0xff

.end
