/*-
 * Copyright (c) 2005-2009, Kohsuke Ohtani
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
 * lcd.c - GBA LCD video driver
 */

#include <driver.h>
#include <wscons.h>
#include "lcd.h"

/* #define DEBUG_LCD 1 */

#ifdef DEBUG_LCD
#define DPRINTF(a) printf a
#else
#define DPRINTF(a)
#endif

struct lcd_softc {
	device_t	dev;
	uint16_t	*vram;
};

static int	lcd_init(struct driver *);
static void	lcd_cursor(void*, int, int);
static void	lcd_putc(void *, int, int, int);
static void	lcd_copyrows(void *,int, int, int);
static void	lcd_eraserows(void *,int, int);
static void	lcd_set_attr(void *, int);
static void	lcd_get_cursor(void *, int *, int *);

static struct devops lcd_devops = {
	/* open */	no_open,
	/* close */	no_close,
	/* read */	no_read,
	/* write */	no_write,
	/* ioctl */	no_ioctl,
	/* devctl */	no_devctl,
};

struct driver lcd_driver = {
	/* name */	"lcd",
	/* devops */	&lcd_devops,
	/* devsz */	sizeof(struct lcd_softc),
	/* flags */	0,
	/* probe */	NULL,
	/* init */	lcd_init,
	/* shutdown */	NULL,
};

static struct wscons_video_ops wscons_lcd_ops = {
	lcd_cursor,		/* cursor */
	lcd_putc,		/* putc */
	lcd_copyrows,		/* copyrows */
	lcd_eraserows,		/* eraserows */
	lcd_set_attr,		/* set_attr */
	lcd_get_cursor,		/* get_cursor */
};

static void
lcd_cursor(void *aux, int row, int col)
{

}

static void
lcd_putc(void *aux, int row, int col, int ch)
{
        printf("%c",ch);
}

static void
lcd_copyrows(void *aux, int srcrow, int dstrow, int nrows)
{

}

static void
lcd_eraserows(void *aux, int row, int nrows)
{

}

static void
lcd_set_attr(void *aux, int attr)
{

}

static void
lcd_get_cursor(void *aux, int *col, int *row)
{

}

static int
lcd_init(struct driver *self)
{
	device_t dev;
	struct lcd_softc *sc;
	dev = device_create(self, "lcd", D_CHR|D_TTY);
	sc = device_private(dev);
	wscons_attach_video(&wscons_lcd_ops, sc);
	return 0;
}
