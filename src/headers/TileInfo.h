/*
 * TileInfo.h
 *
 *  Created on: Dec 15, 2025
 *      Author: giovanni-gds
 */




/* TileInfo.h */
#ifndef TILEINFO_H
#define TILEINFO_H

#include "Utils.h"

/* Tipi di tile */
typedef enum {
	TILE_EMPTY = 0,        // 0: Vuoto
	TILE_PLATFORM =   1,     // 1-99: Piattaforma (solo collisione dall'alto)
	TILE_SOLID =      2,       // >=200: Solido (tutti i lati)
	TILE_BACKGROUND = 3 // 100-199: Sfondo (non collidibile)
} TileType;





/* Struttura informazioni tile semplificata */
typedef struct {
	u8 colorDepth;
	u8 type;               // TileType
	u8 height_map[16];     // Mappa altezze per ogni colonna (0-15, 0 = vuoto, 15 = pieno)
	u8 is_mirrored;        // 1 = tile specchiata orizzontalmente
	u8 is_flipped;         // 1 = tile specchiata verticalmente
	u8 layer;              //definisce il layer per il foreground //0 dietro al player , 1 in linea player , 2 davanti al player
	u8 overlay_tile;
	unsigned char* bitmap; // Puntatore ai dati bitmap
	char* palette;

} TileInfo;

/* Dichiarazioni pubbliche */
void tileinfo_init_table(void);
TileInfo* tileinfo_get(u16 tile_index);
u8 tileinfo_get_height_at(TileInfo* tile, u8 x);

#endif /* TILEINFO_H */
