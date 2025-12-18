/*
 * LevelMap.h
 *
 *  Created on: Dec 16, 2025
 *      Author: giovanni-gds
 */
#ifndef LEVEL1_MAPS_H
#define LEVEL1_MAPS_H

#define MAP_WIDTH  24
#define MAP_HEIGHT 17
#define Z1L1_FG_ADDR 0x0200
#define Z1L1_BG_ADDR Z1L1_FG_ADDR +  (MAP_WIDTH*Z1L1_BG_ADDR)
typedef unsigned char u8;

extern const u8 Z1L1_FG_MAP[MAP_HEIGHT][MAP_WIDTH];
extern const u8 Z1L1_BG_MAP[MAP_HEIGHT][MAP_WIDTH];

/* Definizioni costanti */
#endif	
