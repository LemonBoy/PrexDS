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
 * diag.c - diagnostic message support
 */

#include <sys/bootinfo.h>
#include <kernel.h>
#include <cpufunc.h>

#if defined(CONFIG_DIAG_DESMUME)
#include "desmume.h"
#endif


#if defined(CONFIG_DIAG_SCREEN)

#include "font.h"
#include "video.h"

#define CONSOLE_WIDTH 32
#define CONSOLE_HEIGHT 24
#define TAB_SIZE 3

static int row = 0;
static int col = 0;

static void screen_new_row(void)
{
    volatile unsigned short *map = (unsigned short *)SCREEN_BASE_BLOCK_SUB(31);
    int i;
    row++;
    if(row >= CONSOLE_HEIGHT)
    {
        row--;

        for(i = CONSOLE_WIDTH; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++)
            map[i - CONSOLE_WIDTH] = map[i];

        for(i = 0; i < CONSOLE_WIDTH; i++)
            map[i + (CONSOLE_HEIGHT-1)*CONSOLE_WIDTH] = (' ' - 32);

    }
}

static void screen_putc(char c)
{
    volatile unsigned short *map = (unsigned short *)SCREEN_BASE_BLOCK_SUB(31);

    if(col >= CONSOLE_WIDTH)
    {
        col = 0;
        screen_new_row();
    }

    switch(c)
    {
        case 10:
        case 11:
        case 12:
        case 13:
            screen_new_row();
            col = 0;
            break;
        case 9:
            col += TAB_SIZE;
            break;
        default:
            map[col + row * CONSOLE_WIDTH] = (c - 32);
            col++;
            break;
    }

    return;
}

void screen_init(void)
{
    unsigned short *tile;
    int i;

    POWER_CR |= POWER_ALL_2D;
    VRAM_C_CR = VRAM_ENABLE | 4;
    SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_BG0_ACTIVE;

    SUB_BG0_CR = 128 | BG_TILE_BASE(0) | BG_MAP_BASE(31);
    SUB_BG_PALETTE[0] = RGB15(0, 0, 0);
    SUB_BG_PALETTE[1] = RGB15(0, 31, 0);

    tile = (unsigned short *)CHAR_BASE_BLOCK_SUB(0);

    for (i = 0; i < 3040; i++)
    {
        tile[i] = font_tile[i];
    }

    return;
}

#endif

void
diag_puts(char *buf)
{
    #if defined(CONFIG_DIAG_DESMUME)
    desmume_puts(buf);
    #endif

    #if defined(CONFIG_DIAG_SCREEN)
    while (*buf) screen_putc(*buf++);
    #endif
}

void
diag_init(void)
{
    #if defined(CONFIG_DIAG_SCREEN)
    screen_init();
    #endif
}
