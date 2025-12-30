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



typedef enum {
	EMPTY			= 0,
	Z1L1_PLANE_GRASS,
	PLAYER_RUNNING,
	PLAYER_BRAKING,
	PLAYER_JUMPING,
	PLAYER_RUN_JUMPING,
	PLAYER_SHOOTING,
	PLAYER_SPINDASH,
	PLAYER_HURT,
	PLAYER_DEAD
} TileName;

/* Inizializza la tabella delle tile */
void tileinfo_init_table(void) {
	int i,x,y;

	/* Inizializza tutto a vuoto */
	for (i = 0; i < 256; i++) {
		tile_info_table[i].type = TILE_EMPTY;
		tile_info_table[i].bitmap = NULL;
		tile_info_table[i].is_mirrored = 0;
		tile_info_table[i].colorDepth = BPP_4;
		/* Inizializza height_map a 0 (vuoto) per tutte */
		for ( x = 0; x < 16; x++) {
			tile_info_table[i].height_map[x] = 0;
		}
	}


	tile_info_table[1].type = TILE_BACKGROUND;
	tile_info_table[1].colorDepth = BPP_4;
	tile_info_table[1].bitmap = &fg_001[0];


	tile_info_table[2].type = TILE_SOLID;
	tile_info_table[2].colorDepth = BPP_4;
	tile_info_table[2].bitmap = &fg_002[0];


	tile_info_table[3].type = TILE_BACKGROUND;
	tile_info_table[3].colorDepth = BPP_4;
	tile_info_table[3].bitmap = &fg_003[0];
	tile_info_table[3].layer=2;






	tile_info_table[51].type = TILE_BACKGROUND;
	tile_info_table[51].colorDepth = BPP_4;
	tile_info_table[51].bitmap = &palm_001[0];

	tile_info_table[52].type = TILE_BACKGROUND;
	tile_info_table[52].colorDepth = BPP_4;
	tile_info_table[52].bitmap = &palm_002[0];

	tile_info_table[53].type = TILE_BACKGROUND;
	tile_info_table[53].colorDepth = BPP_4;
	tile_info_table[53].bitmap = &palm_003[0];
	tile_info_table[53].overlay_tile = 3;

	tile_info_table[54].type = TILE_BACKGROUND;
	tile_info_table[54].colorDepth = BPP_4;
	tile_info_table[54].bitmap = &palm_003[0];

	tile_info_table[55].type = TILE_BACKGROUND;
	tile_info_table[55].colorDepth = BPP_4;
	tile_info_table[55].bitmap = &palm_005[0];

	tile_info_table[56].type = TILE_BACKGROUND;
	tile_info_table[56].colorDepth = BPP_4;
	tile_info_table[56].bitmap = &palm_006[0];

	tile_info_table[57].type = TILE_BACKGROUND;
	tile_info_table[57].colorDepth = BPP_4;
	tile_info_table[57].bitmap = &palm_007[0];

	tile_info_table[58].type = TILE_BACKGROUND;
	tile_info_table[58].colorDepth = BPP_4;
	tile_info_table[58].bitmap = &palm_008[0];







	tile_info_table[61].type = TILE_BACKGROUND;
	tile_info_table[61].colorDepth = BPP_4;
	tile_info_table[61].bitmap = &palm_001[0];
	tile_info_table[61].layer=2;

	tile_info_table[62].type = TILE_BACKGROUND;
	tile_info_table[62].colorDepth = BPP_4;
	tile_info_table[62].bitmap = &palm_002[0];
	tile_info_table[62].layer=2;

	tile_info_table[63].type = TILE_BACKGROUND;
	tile_info_table[63].colorDepth = BPP_4;
	tile_info_table[63].bitmap = &palm_003[0];
	tile_info_table[63].overlay_tile = 3;
	tile_info_table[63].layer=2;


	tile_info_table[64].type = TILE_BACKGROUND;
	tile_info_table[64].colorDepth = BPP_4;
	tile_info_table[64].bitmap = &palm_003[0];
	tile_info_table[64].layer=2;


	tile_info_table[65].type = TILE_BACKGROUND;
	tile_info_table[65].colorDepth = BPP_4;
	tile_info_table[65].bitmap = &palm_005[0];
	tile_info_table[65].layer=2;


	tile_info_table[66].type = TILE_BACKGROUND;
	tile_info_table[66].colorDepth = BPP_4;
	tile_info_table[66].bitmap = &palm_006[0];
	tile_info_table[66].layer=2;



	tile_info_table[67].type = TILE_BACKGROUND;
	tile_info_table[67].colorDepth = BPP_4;
	tile_info_table[67].bitmap = &palm_007[0];
	tile_info_table[67].layer=2;



	tile_info_table[68].type = TILE_BACKGROUND;
	tile_info_table[68].colorDepth = BPP_4;
	tile_info_table[68].bitmap = &palm_008[0];
	tile_info_table[68].layer=2;








    tile_info_table[100].type = TILE_BACKGROUND;
    tile_info_table[100].colorDepth = BPP_4;
    tile_info_table[100].layer = 2;
    tile_info_table[100].bitmap = (unsigned char*)LEVEL_1_PRX[0];


    tile_info_table[101].type = TILE_BACKGROUND;
    tile_info_table[101].colorDepth = BPP_4;
    tile_info_table[101].layer = 2;
    tile_info_table[101].bitmap = (unsigned char*)LEVEL_1_PRX[1];




	//    /* Tile 1: Piattaforma erba */

	//    tile_info_table[Z1L1_PLANE_GRASS].type = TILE_PLATFORM;
	//    tile_info_table[Z1L1_PLANE_GRASS].colorDepth = BPP_4;
	//    tile_info_table[Z1L1_PLANE_GRASS].bitmap = (unsigned char*)LEVEL_1_PLATFORM[0];
	//    /* Height map per tile piena (tutta a 15) */
	//    for ( x = 0; x < 16; x++) {
	//        tile_info_table[Z1L1_PLANE_GRASS].height_map[x] = 10;
	//    }
	//    /* Tile 1: ombra erba  0-1 */
	//    tile_info_table[2].type = TILE_BACKGROUND;
	//    tile_info_table[2].colorDepth = BPP_4;
	//    tile_info_table[2].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[0];
	//
	//
	//    /* Tile 3: ombra erba 0-2  */
	//    tile_info_table[3].type = TILE_BACKGROUND;
	//    tile_info_table[3].colorDepth = BPP_4;
	//    tile_info_table[3].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[1];
	//
	//    /* Tile 3: quadrettato scuro */
	//    tile_info_table[4].type = TILE_BACKGROUND;
	//    tile_info_table[4].colorDepth = BPP_4;
	//    tile_info_table[4].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[2];
	//  //  tile_info_table[4].palette = (unsigned char*)paletta_terreno_chiaro_2_colori;
	//
	//
	//    /* Tile 3: quadrettato muro scuro con erba  solido*/
	//    tile_info_table[5].type = TILE_SOLID;
	//    tile_info_table[5].colorDepth = BPP_4;
	//    tile_info_table[5].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[3];
	//
	//
	//    /* Tile 3: quadrettato muro scuro con erba  piattaforma*/
	//    tile_info_table[6].type = TILE_PLATFORM;
	//    tile_info_table[6].colorDepth = BPP_4;
	//    tile_info_table[6].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[3];
	//    /* Height map per tile piena (tutta a 15) */
	//    for ( x = 0; x < 16; x++) {
	//        tile_info_table[6].height_map[x] = 10;
	//    }
	//
	//
	//
	//
	//
	//    /* muro scuro destro solido intero */
	//    tile_info_table[7].type = TILE_SOLID;
	//    tile_info_table[7].colorDepth = BPP_4;
	//    tile_info_table[7].bitmap = (unsigned char*)LEVEL_1_WALL[0];
	//
	//
	//    /* muro scuro destro attraversabile  intero */
	//    tile_info_table[8].type = TILE_BACKGROUND;
	//    tile_info_table[8].colorDepth = BPP_4;
	//    tile_info_table[8].bitmap = (unsigned char*)LEVEL_1_WALL[0];
	//
	//
	//
	//    /* muro scuro destro con erba  solido intero */
	//    tile_info_table[8].type = TILE_SOLID;
	//    tile_info_table[8].colorDepth = BPP_4;
	//    tile_info_table[8].bitmap = (unsigned char*)LEVEL_1_WALL[1];
	//
	//
	//    /* muro scuro destro con erba attraversabile  intero */
	//    tile_info_table[9].type = TILE_BACKGROUND;
	//    tile_info_table[9].colorDepth = BPP_4;
	//    tile_info_table[9].bitmap = (unsigned char*)LEVEL_1_WALL[1];
	//
	//
	//    /* TERMINALE ERBA ALTO DX */
	//    tile_info_table[10].type = TILE_PLATFORM;
	//    tile_info_table[10].colorDepth = BPP_4;
	//    tile_info_table[10].bitmap = (unsigned char*)LEVEL_1_PLATFORM[1];
	//    /* Height map per tile piena (tutta a 15) */
	//
	//    for ( x = 0; x < 16; x++) {
	//        tile_info_table[10].height_map[x] = 10;
	//    }
	//
	//
	//
	//
	//    //avvallamento da due sx parte prima
	//    tile_info_table[11].type = TILE_PLATFORM;
	//    tile_info_table[11].colorDepth = BPP_4;
	//    tile_info_table[11].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2];
	//    /* Height map per tile piena (tutta a 15) */
	//    y=0;
	//    for ( x = 0; x < 16; x++) {
	//    	if(x<8)y=2;
	//    	else y=4;
	//        tile_info_table[11].height_map[x] = 10-y;
	//    }
	//
	//
	//    //avvallamento da due sx parte seconda
	//    tile_info_table[12].type = TILE_PLATFORM;
	//    tile_info_table[12].colorDepth = BPP_4;
	//    tile_info_table[12].bitmap = (unsigned char*)LEVEL_1_PLATFORM[3];
	//    /* Height map per tile piena (tutta a 15) */
	//    //tile_info_table[12].height_map = {8,8,8,8,8,8,8,8,6,6,6,6,6,6,6,6};
	//
	//    y=0;
    //    for ( x = 0; x < 16; x++) {
	//    	if(x<8)y=6;
	//    	else y=8;
	//        tile_info_table[12].height_map[x] = 10-y;
	//    }
	//
	//
	//    //avvallamento da due dx parte seconda
	//    tile_info_table[13].type = TILE_PLATFORM;
	//    tile_info_table[13].colorDepth = BPP_4;
	//    tile_info_table[13].bitmap = (unsigned char*)LEVEL_1_PLATFORM[3];
	//    tile_info_table[13].is_mirrored=1;
	//    /* Height map per tile piena (tutta a 15) */
	//    y=0;
	//    for ( x = 0; x < 16; x++) {
	//      	if(x<8)y=6;
	//        else y=8;
	//        tile_info_table[13].height_map[x] = 10-y;
	//    }
	//
	//
	//    //avvallamento da due dx parte seconda
	//    tile_info_table[14].type = TILE_PLATFORM;
	//    tile_info_table[14].colorDepth = BPP_4;
	//    tile_info_table[14].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2];
	//    tile_info_table[14].is_mirrored=1;
	//    /* Height map per tile piena (tutta a 15) */
	//    y=0;
	//    for ( x = 0; x < 16; x++) {
	//      	if(x<8)y=2;
	//        else y=4;
	//        tile_info_table[14].height_map[x] = 10-y;
	//    }
	//
	//
	//
	//    /* erba sotto avvallamento da due sx*/
	//    tile_info_table[15].type = TILE_BACKGROUND;
	//    tile_info_table[15].colorDepth = BPP_4;
	//    tile_info_table[15].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[4];
	//
	//    /* erba sotto avvallamento da due dx*/
	//    tile_info_table[16].type = TILE_BACKGROUND;
	//    tile_info_table[16].colorDepth = BPP_4;
	//    tile_info_table[16].is_mirrored=1;
	//    tile_info_table[16].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[4];
	//
	//
	//
	//
	//    /* muro chiaro SX solido intero */
	//       tile_info_table[17].type = TILE_SOLID;
	//       tile_info_table[17].colorDepth = BPP_4;
	//       tile_info_table[17].bitmap = (unsigned char*)LEVEL_1_WALL[2];
	//
	//
	//       /* muro chiaro SX attraversabile  intero */
	//       tile_info_table[18].type = TILE_SOLID;
	//       tile_info_table[18].colorDepth = BPP_4;
	//       tile_info_table[18].bitmap = (unsigned char*)LEVEL_1_WALL[2];
	//
	//
	//       /* TERMINALE ERBA ALTO DX */
	//       tile_info_table[19].type = TILE_PLATFORM;
	//       tile_info_table[19].colorDepth = BPP_4;
	//       tile_info_table[19].bitmap = (unsigned char*)LEVEL_1_PLATFORM[1];
	//       tile_info_table[19].is_mirrored=1;
	//       /* Height map per tile piena (tutta a 15) */
	//
	//       for ( x = 0; x < 16; x++) {
	//           tile_info_table[19].height_map[x] = 10;
	//       }
	//
	//
	//
	//
	//       /* TERMINALE ERBA ALTO DX */
	//       tile_info_table[20].type = TILE_PLATFORM;
	//       tile_info_table[20].colorDepth = BPP_4;
	//       tile_info_table[20].bitmap = (unsigned char*)LEVEL_1_WALL[3];
	//       ;
	//       /* Height map per tile piena (tutta a 15) */
	//
	//       for ( x = 0; x < 16; x++) {
	//           tile_info_table[20].height_map[x] = 10;
	//       }
	//
	//
	//
	//




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

