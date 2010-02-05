#include "cp15.h"

    .text
    .align 2

    .global cp15_setup

cp15_setup:

    /* Protection, DCache, ICache, TCM, Alternate vectors disabled */
    mov r0, #0
    mcr p15, 0, r0, c1, c0

    /* Drain write buffer : makes sure the prevous write has completed */
    mcr p15, 0, r0, c7, c10, 4

    /* DTCM base = 0x0b000000, size = 16 KB */
    mov r0, #0x0b000000
    orr r0, r0, # 0b00101 << 1
    mcr p15, 0, r0, c9, c1,0

    /* ITCM base = 0x01000000, size = 32 MB */
    mov r0, #0x01000000
    orr r0, r0, # 0b10000 << 1
    mcr p15, 0, r0, c9, c1,1

    /*--------------------------------------------------------------------------
     * Setup memory regions similar to Release Version (from libnds)
     *------------------------------------------------------------------------*/

    /* Region 0 - IO registers */
    ldr r0,=( CP15_REGION_64M | 0x04000000 | 1)
    mcr p15, 0, r0, c6, c0, 0

    /* Region 1 - Main Memory */
    ldr r0,=( CP15_REGION_4M | 0x02000000 | 1)
    mcr p15, 0, r0, c6, c1, 0

    /* Region 2 - alternate vector base */
    ldr r0,=( CP15_REGION_4K | 0x00000000 | 1)
    mcr p15, 0, r0, c6, c2, 0

    /* Region 3 - DS Accessory (GBA Cart) */
    ldr r0,=( CP15_REGION_128M | 0x08000000 | 1)
    mcr p15, 0, r0, c6, c3, 0

    /* Region 4 - DTCM */
    ldr r0,=0x0b000000
    orr r0,r0,#(CP15_REGION_16K | 1)
    mcr p15, 0, r0, c6, c4, 0

    /* Region 5 - ITCM */
    ldr r0,=0x01000000

    mov r0,r0,lsr #15 @--> align to 32k boundary
    mov r0,r0,lsl #15 @/

    orr r0,r0,#(CP15_REGION_32K | 1)
    mcr p15, 0, r0, c6, c5, 0

    /* Region 6 - System ROM */
    ldr r0,=( CP15_REGION_32K | 0xFFFF0000 | 1)
    mcr p15, 0, r0, c6, c6, 0

    /* Region 7 - non cacheable main ram */
    ldr r0,=( CP15_REGION_4M  | 0x02400000 | 1)
    mcr p15, 0, r0, c6, c7, 0

    /*--------------------------------------------------------------------------
     * Activating TCM and caches
     *------------------------------------------------------------------------*/

    /* Write buffer enable */
    ldr r0,=0b00000010
    mcr p15, 0, r0, c3, c0, 0

    /* DCache & ICache enable */
    ldr r0,=0b01000010
    mcr p15, 0, r0, c2, c0, 0
    mcr p15, 0, r0, c2, c0, 1

    /* IAccess */
    ldr r0,=0x36636633
    mcr p15, 0, r0, c5, c0, 3

    /* DAccess */
    ldr r0,=0x36333633
    mcr     p15, 0, r0, c5, c0, 2

    /* Enable ICache, DCache, ITCM & DTCM */
    mrc p15, 0, r0, c1, c0, 0
    ldr r1,= CP15_ITCM_ENABLE | CP15_DTCM_ENABLE | CP15_ICACHE_ENABLE | CP15_DCACHE_ENABLE | CP15_PROTECT_ENABLE
    orr r0,r0,r1
    mcr p15, 0, r0, c1, c0, 0

    mov pc, lr
