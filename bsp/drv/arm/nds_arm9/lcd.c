/*
 * Screen driver for the Nintendo DS
 * 2010 (C) The Lemon Man
 * 2010 (C) Cyril
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

#include "screen.h"
#include "font.h"

#define DIAG_CONSOLE_WIDTH  ((256/FONT_W)-1)
#define DIAG_CONSOLE_HEIGHT ((192/FONT_H)-1)

struct lcd_softc {
	device_t       	dev;
	device_t		ipcdev;
	device_t		inputdev;
	irq_t			irq;
	char          	console[DIAG_CONSOLE_WIDTH][DIAG_CONSOLE_HEIGHT];
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
	/* devctl */no_devctl,
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
lcd_putc(void *aux, int row, int col, int ch)
{
    struct lcd_softc *sc = aux;
    int line, bitPos;
    
    if (ch >= 0x1F && ch <= 0x7F) {
        ch -= 0x1F;
    } 
    else {
        ch = 0;
    }
    
    /*printf("%d %d\n", col, row);*/
    
    if (col > DIAG_CONSOLE_WIDTH)
		while (1);
    if (row > DIAG_CONSOLE_HEIGHT)
		while (1);
    
    sc->console[col][row] = (char)ch;
    
	for (line = 0; line < 8; line ++)
		for (bitPos = 0; bitPos < 8; bitPos ++)
			screen_plot(0, (col*FONT_W) + bitPos, (row*FONT_H) + line, ((__nokia_font[ch][line] >> (7 - bitPos)) & 1) ? 0xFFFFFFFF : 0x00000000);    

    return;
}

static void
lcd_copyrows(void *aux, int srcrow, int dstrow, int nrows)
{
    int x, i;
    struct lcd_softc *sc = aux;
    
    /*memcpy(&sc->console[0][dstrow], &sc->console[0][srcrow], DIAG_CONSOLE_WIDTH * nrows);*/

    for (x = 0; x < DIAG_CONSOLE_HEIGHT; x++)
    {
		for (i = 0; i < DIAG_CONSOLE_WIDTH; i++) {
			sc->console[i][x+dstrow] = sc->console[i][x+srcrow];
			lcd_putc(aux, x+dstrow, i, sc->console[i][x+dstrow]);
		}
	}
	
	while (1); /* Fixme, corrupts the screen */
}

static void
lcd_eraserows(void *aux, int row, int nrows)
{
    struct lcd_softc *sc = aux;
    
	memset(&sc->console[0][row], ' ', DIAG_CONSOLE_WIDTH * nrows);
}

static void
lcd_set_attr(void *aux, int attr)
{

}

static void
lcd_cursor(void *aux, int row, int col)
{
}

static void
lcd_get_cursor(void *aux, int *col, int *row)
{
    *col = 0;
    *row = 0;
}

static int
lcd_isr(void *arg)
{
	struct lcd_softc *sc = arg;
	uint16_t aux_keys;
	
	device_control(sc->ipcdev, 0, &aux_keys);
	device_control(sc->inputdev, 0, &aux_keys);
	
	return 0;
}

static int
lcd_init(struct driver *self)
{
    device_t dev, ipcdev, inputdev;
    struct lcd_softc *sc;

    dev = device_create(self, "lcd", D_CHR|D_TTY);
    sc = device_private(dev);
    
   	do {
		ipcdev = device_lookup("ipc");
		inputdev = device_lookup("kbd");
	} while(!ipcdev || !inputdev);
	
	sc->ipcdev = ipcdev;
	sc->inputdev = inputdev;
    sc->irq = irq_attach(0, IPL_DISPLAY, 0, lcd_isr, IST_NONE, sc);
    
    screen_init();
    screen_fill(0);
    
    memset(&sc->console[0][0], ' ', DIAG_CONSOLE_WIDTH * DIAG_CONSOLE_HEIGHT);
    
    wscons_attach_video(&wscons_lcd_ops, sc);
    
    return 0;
}
