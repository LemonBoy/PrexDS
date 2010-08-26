/*
 * DS display support.
 * Based off Dovoto's tutorials.
 * 2010 (C) The Lemon Man
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <types.h>

typedef struct {
	uint16_t x;
	uint16_t y;
} bg_scroll;
 
typedef struct {
    uint16_t xdx;
    uint16_t xdy;
    uint16_t ydx;
    uint16_t ydy;
    uint32_t cx;
    uint32_t cy;    
} bg_rotation;
 
typedef struct {
    uint16_t control[4];
    bg_scroll scroll[4];
    bg_rotation bg2_rotation;
    bg_rotation bg3_rotation;
} bg_attribute;

#define RGB15(r,g,b)  ((r)|((g)<<5)|((b)<<10))

#define SCREEN_HEIGHT 			192
#define SCREEN_WIDTH 			256

#define BG_WRAP_ON 				(1 << 13)
 
#define background_main			(*((bg_attribute *)0x04000008))
#define background_sub      	(*((bg_attribute *)0x04001008))

#define BG_BMP_RAM(base)  		(((base)*0x4000) + 0x06000000)
#define BG_BMP_RAM_SUB(base) 	(((base)*0x4000) + 0x06200000)

#define POWCNT1 				(*(volatile uint16_t*)0x04000304)
#define DISPLAY_CR 				(*(volatile uint32_t*)0x04000000)
#define SUB_DISPLAY_CR 			(*(volatile uint32_t*)0x04001000)
#define VRAM_A_CR 				(*(volatile uint8_t *)0x04000240)
#define VRAM_C_CR 				(*(volatile uint8_t *)0x04000242)
#define BG_BMP16_256x256 		((1 << 14) | (1 << 7) | (1 << 2))
#define BG_BMP_BASE(base)  		((base) << 8)	

void screen_init(void);
void screen_fill (uint32_t color);
void screen_plot(int screen, uint8_t x, uint8_t y, uint32_t color);

#endif /* SCREEN_H_ */
