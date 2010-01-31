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

#include "screen.h"
#include "font.h"

#define DIAG_CONSOLE_WIDTH  (256/FONT_WIDTH)
#define DIAG_CONSOLE_HEIGHT (192/FONT_HEIGHT)

static char console[DIAG_CONSOLE_WIDTH][DIAG_CONSOLE_HEIGHT];
static int  top;
static int  col;

/** display the console content */
void diag_update(void)
{
    int x,y;
    screen_init();
    for(x = 0; x < DIAG_CONSOLE_WIDTH;  x++ )
    for(y = 0; y < DIAG_CONSOLE_HEIGHT; y++ )
    {
        font_print_char(console[x][(y+top)%DIAG_CONSOLE_HEIGHT],3+x*FONT_WIDTH,2+y*FONT_HEIGHT);
    }
}

void diag_new_line(void)
{
    int x;
    for(x = 0; x < DIAG_CONSOLE_WIDTH;  x++ ) {
        console[x][top] = 0;
    }
    top = (top + 1) % DIAG_CONSOLE_HEIGHT;
    col = 0;
    diag_update();
}

void diag_init(void)
{
    #if defined(CONFIG_DIAG_SCREEN)
    int x,y;
    screen_init();
    top = 0;
    col = 0;
    for(x = 0; x < DIAG_CONSOLE_WIDTH;  x++ )
    for(y = 0; y < DIAG_CONSOLE_HEIGHT; y++ )
        console[x][y] = 0;
    #endif
}

static void diag_putc(char c)
{

    if(col >= DIAG_CONSOLE_WIDTH)
    {
        diag_new_line();
    }

    switch(c)
    {
        case 10:
        case 11:
        case 12:
        case 13:
            diag_new_line();
            break;
        default:
            console[col][(top+DIAG_CONSOLE_HEIGHT-1)%DIAG_CONSOLE_HEIGHT] = c;
            col++;
            break;
    }

    return;
}

void
diag_puts(char *buf)
{
    #if defined(CONFIG_DIAG_DESMUME)
    desmume_puts(buf);
    #endif

    #if defined(CONFIG_DIAG_SCREEN)
    while (*buf) diag_putc(*buf++);
    #endif
}

/*void
diag_init(void)
{
    screen_init();
    font_print_char('A',10,10);
    font_print_char('B',10,20);
    font_print_char('C',10,30);
    font_print_char('D',10,40);
    #if defined(CONFIG_DIAG_SCREEN)
    screen_init();
    #endif
}*/
