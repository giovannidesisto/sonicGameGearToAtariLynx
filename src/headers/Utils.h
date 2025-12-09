/*
 * Utils.h
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */
#ifndef UTILS_H
#define UTILS_H
#include "AgCommon.h"
#include <tgi.h>



//#define RES32 0
#define RES32 1
//#define RES16 0



#ifdef RES32
	#define RATIO 1
	#define SCALE 1
	#define SCALE_DIVIDER 1
	#define TILE_SIZE 32
	#define TILES_X  6 //11//11
	#define TILES_Y  4 //8//8
#elif RES24
	#define RATIO 1
	#define SCALE 2
	#define SCALE_DIVIDER 3
	#define TILE_SIZE 21
	#define TILES_X  9//11//11
	#define TILES_Y  5 //8//8
#elif RES16
	#define RATIO 1
	#define SCALE 1
	#define SCALE_DIVIDER 2
	#define TILE_SIZE 16
	#define TILES_X  11//11//11
	#define TILES_Y  8 //8//8

#endif


int u16_to_ascii(unsigned short value, char *dest);
void printCoordsToScreen(u16 x, u16 y,u8 print_x, u8 print_y,u8 color);
void printU8As2Nibble(unsigned char value,u8 print_x, u8 print_y,u8 color);
int iabs(int v);
#endif /* UTILS_H */
