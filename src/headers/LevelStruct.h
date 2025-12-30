/*
 * LevelStruct.h
 *
 *  Created on: Dec 30, 2025
 *      Author: giovanni-gds
 */

#include "Utils.h"
#ifndef LEVEL_STRUCT_H
#define LEVEL_STRUCT_H

/* Struttura Camera estesa */
typedef struct {
    u16 prev_tile_x;
    u16 prev_tile_y;

    u8 center_x;
    u8 center_y;
    s16 x;                   /* Posizione X della camera (in pixel world) */
    s16 y;                   /* Posizione Y della camera (in pixel world) */

    u8 width;               /* Larghezza della viewport */
    u8 height;              /* Altezza della viewport */

    /* Nuovi campi per dead zone e inerzia */
    u8 dead_zone_left;      /* Dead zone sinistra dal centro */
    u8 dead_zone_right;     /* Dead zone destra dal centro */
    s8 inertia_x;           /* Inerzia orizzontale (-127 a +127) */
    u8 inertia_counter;     /* Contatore per l'inerzia */
    u8 max_inertia_pixels;  /* Pixel massimi di inerzia (default 16) */
    u8 last_player_dir;     /* Ultima direzione del player (0=fermo, 1=dx, 2=sx) */
} Camera;


/* Struttura livello */
typedef struct {
	//u8* foregound_map;//[MAP_HEIGHT][MAP_WIDTH];  /* Mappa di tile */
	//u8 background_map[MAP_HEIGHT][MAP_WIDTH];  /* Mappa di tile */
	u16 start_x;                     		  /* Posizione di partenza player */
	u16 start_y;
	u16 end_x;                                /* Posizione fine livello */
	u16 end_y;

	u16 map_w;
	u8  map_h;
	u8 current_level;
	//u8* fg_map; //punta alla porzione decompressa di mappa
	//u8* bg_map;

	/* Limita la camera ai bordi del livello */
	u16 level_width_px; //=  level.map_w * TILE_SIZE;
	u16 level_height_px ;//= level.map_h * TILE_SIZE;

	s16 map_buf_origin_x;
	s16 map_buf_origin_y;
	u8 map_buf_col0;
	u8 map_buf_row0;

	Camera camera;                            /* Camera per lo scroll */
} Level;


#endif

