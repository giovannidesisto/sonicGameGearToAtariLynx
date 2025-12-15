/*
 * TileInfo.c
 *
 *  Created on: Dec 15, 2025
 *      Author: giovanni-gds
 */




/* TileInfo.c */
#include "TileInfo.h"

#include "Zone_1_tiles.h"

/* Tabella informazioni tile */
static TileInfo tile_info_table[256];

/* Inizializza la tabella delle tile */
void tileinfo_init_table(void) {
    int i,x;

    /* Inizializza tutto a vuoto */
    for (i = 0; i < 256; i++) {
        tile_info_table[i].type = TILE_EMPTY;
        tile_info_table[i].bitmap = NULL;
        tile_info_table[i].is_mirrored = 0;
        tile_info_table[i].colorDepth = BPP_1;
        /* Inizializza height_map a 0 (vuoto) per tutte */
        for ( x = 0; x < 16; x++) {
            tile_info_table[i].height_map[x] = 0;
        }
    }

    /* ESEMPI: Configura alcune tile */


    /* Tile 1: Piattaforma semplice (piena) GIALLO */
    tile_info_table[1].type = TILE_PLATFORM;
    tile_info_table[1].colorDepth = BPP_4;
    tile_info_table[1].bitmap = (unsigned char*)LEVEL_1_PLATFORM[0];
    /* Height map per tile piena (tutta a 15) */
    for ( x = 0; x < 16; x++) {
        tile_info_table[1].height_map[x] = 15;
    }

    /* Tile 1: Piattaforma semplice (piena) VERDE*/
    tile_info_table[2].type = TILE_PLATFORM;
    tile_info_table[2].colorDepth = BPP_4;
    tile_info_table[2].bitmap = (unsigned char*)LEVEL_1_PLATFORM[1];

    /* Height map per salita: da 0 a 15 in 16 passi */
    for ( x = 0; x < 16; x++) {
        tile_info_table[2].height_map[x] = 15; /* 0, 1, 2, ..., 15 */
    }

    /* Tile 3: Piattaforma con discesa  */
    tile_info_table[3].type = TILE_PLATFORM;
    tile_info_table[3].colorDepth = BPP_4;
    tile_info_table[3].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2]; /* Stessa bitmap di 2 */
    tile_info_table[3].is_mirrored = 0;
    /* Height map per salita: da 0 a 15  */
    for ( x = 0; x < 16; x++) {
        tile_info_table[3].height_map[x] = x; /* 0, 1, 2, ..., 15 */
    }

    /* Tile 3: Piattaforma con discesa (specchiata) */
    tile_info_table[4].type = TILE_PLATFORM;
    tile_info_table[4].colorDepth = BPP_4;
    tile_info_table[4].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2]; /* Stessa bitmap di 2 */
    tile_info_table[4].is_mirrored = 1; /* Specchiata */
    /* Height map per discesa: da 15 a 0 (verrà invertita da is_mirrored) */
    for ( x = 0; x < 16; x++) {
        tile_info_table[4].height_map[x] = x; /* 0, 1, 2, ..., 15 */
    }


    /* Tile 200: Solida (muro) - nessuna height_map necessaria */
    tile_info_table[5].type = TILE_SOLID;
    tile_info_table[5].colorDepth = BPP_4;
    tile_info_table[5].bitmap = (unsigned char*)LEVEL_1_WALL[0];



    /* Tile 100: Sfondo (nuvole) */
    tile_info_table[100].type = TILE_BACKGROUND;
    tile_info_table[100].colorDepth = BPP_4;
    tile_info_table[100].bitmap = (unsigned char*)LEVEL_1_PRX[0];


}

/* Restituisce le informazioni per una tile */
TileInfo* tileinfo_get(u16 tile_index) {
    if (tile_index < 256) {
        return &tile_info_table[tile_index];
    }
    return &tile_info_table[0]; /* Tile vuota */
}

/* Restituisce l'altezza a una data coordinata X, considerando lo specchiamento */
u8 tileinfo_get_height_at(TileInfo* tile, u8 x) {
	u8 actual_x;
	if (tile == NULL) return 0;

    actual_x = x;
    if (tile->is_mirrored) {
        actual_x = 15 - x;
    }

    if (actual_x < 16) {
        return tile->height_map[actual_x];
    }
    return 0;
}
