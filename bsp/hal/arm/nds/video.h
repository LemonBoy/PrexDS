/*
 * video.h
 *
 *  Created on: 15 janv. 2010
 *      Author: cyril
 */

#ifndef VIDEO_H_
#define VIDEO_H_

#define BIT(n) (1 << (n))

#define POWER_LCD                       BIT(0)
#define POWER_2D_A                      BIT(1)
#define POWER_MATRIX            BIT(2)
#define POWER_3D_CORE           BIT(3)
#define POWER_2D_B                      BIT(9)
#define POWER_SWAP_LCDS         BIT(15)

#define MODE_0_2D      0x10000
#define DISPLAY_BG0_ACTIVE    (1 << 8)

#define POWER_ALL_2D     (POWER_LCD |POWER_2D_A |POWER_2D_B)

#define POWER_CR        (*(volatile unsigned short *)0x04000304)

#define SUB_DISPLAY_CR  (*(volatile unsigned long *)0x04001000)

#define VRAM_C_CR       (*(volatile unsigned char *)0x04000242)
#define VRAM_ENABLE     (1<<7)

#define BG0_CR          (*(volatile unsigned short *)0x04000008)
#define SUB_BG0_CR      (*(volatile unsigned short *)0x04001008)
#define BG_TILE_BASE(base) ((base) << 2)
#define BG_MAP_BASE(base)  ((base) << 8)
#define BG_BMP_BASE(base)  ((base) << 8)

#define BG_PALETTE      ((volatile unsigned short *)0x05000000)
#define SUB_BG_PALETTE  ((volatile unsigned short *)0x05000400)

#define RGB15(r,g,b)    ((r)|((g)<<5)|((b)<<10))

#define CHAR_BASE_BLOCK(n)    (((n)*0x4000)+0x6000000)
#define CHAR_BASE_BLOCK_SUB(n)      (((n)*0x4000)+0x6200000)
#define SCREEN_BASE_BLOCK(n)  (((n)*0x800)+0x6000000)
#define SCREEN_BASE_BLOCK_SUB(n)    (((n)*0x800)+0x6200000)

#endif /* VIDEO_H_ */
