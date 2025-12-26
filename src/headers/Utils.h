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
#include <string.h>
#include <stdio.h>
#include "TileInfo.h"
#define DEBUG_BUFFER_SIZE 512
#define DEBUG_BUFFER ((char*)0xFA00)
//extern char DEBUG_BUFFER[DEBUG_BUFFER_SIZE] ;

#define RES32
//#define RES24 1
//#define RES16 1

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) > (b) ? (b) : (a))



#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 102


#ifdef RES32
	#define RATIO 1
	#define SCALE 1
	#define SCALE_DIVIDER 1
	#define TILE_SIZE 16
	#define PLAYER_WIDTH 16
	#define PLAYER_HEIGHT 32
	#define TILES_X  11 //11//11
	#define TILES_Y  8 //8//8
#elif RES24
	#define RATIO 1
	#define SCALE 3
	#define SCALE_DIVIDER 4
	#define TILE_SIZE 24
	#define PLAYER_WIDTH 12
	#define TILES_X  9//11//11
	#define TILES_Y  6 //8//8
#elif RES16
	#define RATIO 1
	#define SCALE 1
	#define SCALE_DIVIDER 2
	#define TILE_SIZE 16
	#define PLAYER_WIDTH 8
	#define TILES_X  11//11//11
	#define TILES_Y  8 //8//8

#endif









//#define EFFECT_TOGGLE_VALUE 10
//#define FRAME_DIVIDER 2



extern SCB_REHV_PAL* prev_sprite;
extern SCB_REHV_PAL* first_sprite;

extern u8 frame_count;
extern u8 effect_counter;

int u16_to_ascii(unsigned short value, char *dest);
void printCoordsToScreen(u16 x, u16 y,u8 print_x, u8 print_y,u8 color);
void printU16(u16 value,u8 print_x, u8 print_y,u8 color);
void printS16(s16 value,u8 print_x, u8 print_y,u8 color);
void printExadec(s16 value,u8 print_x, u8 print_y,u8 color);
int iabs(int v);
void debug_init(void);
void debug_print(const char *str);
#endif /* UTILS_H */


