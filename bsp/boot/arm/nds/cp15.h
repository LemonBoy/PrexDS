#ifndef CP15_H_
#define CP15_H_

#define CP15_ITCM_ENABLE     (1 << 18)
#define CP15_DTCM_ENABLE     (1 << 16)
#define CP15_ICACHE_ENABLE   (1 << 12)
#define CP15_DCACHE_ENABLE   (1 <<  2)
#define CP15_PROTECT_ENABLE  (1 <<  0)

#define CP15_REGION_4K     (0b01011 << 1)
#define CP15_REGION_8K     (0b01100 << 1)
#define CP15_REGION_16K    (0b01101 << 1)
#define CP15_REGION_32K    (0b01110 << 1)
#define CP15_REGION_64K    (0b01111 << 1)
#define CP15_REGION_128K   (0b10000 << 1)
#define CP15_REGION_256K   (0b10001 << 1)
#define CP15_REGION_512K   (0b10010 << 1)
#define CP15_REGION_1M     (0b10011 << 1)
#define CP15_REGION_2M     (0b10100 << 1)
#define CP15_REGION_4M     (0b10101 << 1)
#define CP15_REGION_8M     (0b10110 << 1)
#define CP15_REGION_16M    (0b10111 << 1)
#define CP15_REGION_32M    (0b11000 << 1)
#define CP15_REGION_64M    (0b11001 << 1)
#define CP15_REGION_128M   (0b11010 << 1)
#define CP15_REGION_256M   (0b11011 << 1)
#define CP15_REGION_512M   (0b11100 << 1)
#define CP15_REGION_1G     (0b11101 << 1)
#define CP15_REGION_2G     (0b11110 << 1)
#define CP15_REGION_4G     (0b11111 << 1)

#endif /* CP15_H_ */
