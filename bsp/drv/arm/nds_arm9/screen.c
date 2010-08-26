/*
 * DS display support.
 * Based off Dovoto's tutorials.
 * 2010 (C) The Lemon Man
 */

#include <types.h>
#include "screen.h"

static uint16_t *video_main 	= (uint16_t *)BG_BMP_RAM(0);
static uint16_t *video_sub 		= (uint16_t *)BG_BMP_RAM_SUB(0);

void screen_init(void)
{
	/* Initialize the screens */
	POWCNT1 = 0x820E;
	/* Setup the screens */
	DISPLAY_CR = 0x10005 | (1 << 11);
	SUB_DISPLAY_CR = 0x10005 | (1 << 11);
	/* Setup the vram banks */
	VRAM_A_CR = 0x80 | 1;
	VRAM_C_CR = 0x80 | 4;
	/* Setup the backgrounds */
	background_main.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0) | BG_WRAP_ON;
	background_sub.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0) | BG_WRAP_ON;
	/* Setup the matrices for both screens */
	background_main.bg3_rotation.xdy = 0;
	background_main.bg3_rotation.xdx = 1 << 8;
	background_main.bg3_rotation.ydx = 0;
	background_main.bg3_rotation.ydy = 1 << 8;
	background_main.bg3_rotation.cx = 0;
	background_main.bg3_rotation.cy = 0;		
 
	background_sub.bg3_rotation.xdy = 0;
	background_sub.bg3_rotation.xdx = 1 << 8;
	background_sub.bg3_rotation.ydx = 0;
	background_sub.bg3_rotation.ydy = 1 << 8;	
	background_sub.bg3_rotation.cx = 0;
	background_sub.bg3_rotation.cy = 0;
}

void screen_fill (uint32_t color)
{
	int pos;
	
	for (pos = 0; pos < SCREEN_HEIGHT*SCREEN_WIDTH*2; pos ++)
	{
		video_main[pos] = video_sub[pos] = (color & 0xFFFF) | (1 << 15);
	}
}

void screen_plot(int screen, uint8_t x, uint8_t y, uint32_t color)
{
	switch (screen)
	{
		case 0:
			video_main[x+y*SCREEN_WIDTH] = (color & 0xFFFF) | (1 << 15); break;
		case 1:
			video_sub[x+y*SCREEN_WIDTH] = (color & 0xFFFF) | (1 << 15); break;
	}
}
