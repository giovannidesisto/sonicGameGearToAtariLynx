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
//#include <string.h>
//#include <stdio.h>
#include "TileInfo.h"


#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) > (b) ? (b) : (a))



#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 102


#define RATIO 1
#define SCALE 1
#define SCALE_DIVIDER 1
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32

#define TILE_SIZE 16// 16

#define TILES_X  11//11
#define TILES_Y  8//8

#define MAP_BUF_W  16// 16          // POTENZA DI 2 (OBBLIGATORIO)
#define MAP_BUF_H  16//16//TILES_Y
#define TILE_SHIFT 4//4 //log2(TILE_SIZE)

#define MAP_BUF_MASK (MAP_BUF_W - 1)



#define MAX_ACTIVE_SPRITES  (TILES_Y*TILES_X)



typedef  struct{
  u8 deposit;
  SCB_REHV_PAL sprite;
} sprite_collidabile;





extern u8 frame_count;
extern u8 effect_counter;

int u16_to_ascii(unsigned short value, char *dest);
void printCoordsToScreen(u16 x, u16 y,u8 print_x, u8 print_y,u8 color);
void printU16(u16 value,u8 print_x, u8 print_y,u8 color);
void printS16(s16 value,u8 print_x, u8 print_y,u8 color);
void printExadec(s16 value,u8 print_x, u8 print_y,u8 color);

#endif /* UTILS_H */


