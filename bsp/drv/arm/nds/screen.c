#include "screen.h"

#define SCREEN ((volatile unsigned short*)0x06800000)

#define SCREEN_WIDTH  (256)
#define SCREEN_HEIGHT (192)

/* Setup the screen in frame buffer mode */
void screen_init(void)
{
    /* power on LCD and 2D A */
    (*(volatile unsigned short *)0x04000304) = 0x3;
    /* VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD; */
    (*(volatile unsigned char  *)0x04000240) = 1<<7;
    /* Mode framebuffer 0 (VRAM A direct mapping) */
    (*(volatile unsigned int *)0x04000000) = 0x00020000;
}

void screen_plot(unsigned char x,unsigned char y, unsigned int hexColor)
{
    unsigned char red   = ((hexColor >> 16) & 0xFF) >> 3;
    unsigned char green = ((hexColor >>  8) & 0xFF) >> 3;
    unsigned char blue  = ((hexColor >>  0) & 0xFF) >> 3;
    SCREEN[x+y*SCREEN_WIDTH] = (1 << 15) | ((blue)<<10) | ((green)<<5) | (red);
}
