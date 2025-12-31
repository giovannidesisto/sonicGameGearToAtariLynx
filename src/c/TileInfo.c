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

static const unsigned char AAA_TERRENO   [2]=   		{0x01,0x28};//MARRONE CHIARO / MARRONE SCURO
static const unsigned char AAA_ERBA_CELO [2]=   		{0x01,0xC6};//VERDE CHIARO,VERDE SCURO
static const unsigned char AAA_TERRENO_ERBA_OMBRA[4]=   {0x01,0x82,0x6C,0xF0};// MARRONE SCURO / MARRONE CHIARO / VERDE SCURO / VERDE CHIARO / GRIGIO BIANCO
static const unsigned char AAA_MONTAGNE[4]=   {0x01,0xE2,0x8A,0x00};// MARRONE SCURO / MARRONE CHIARO / VERDE SCURO / VERDE CHIARO / GRIGIO BIANCO
static const unsigned char AAA_SONIC[4]= {0x01,0x4D,0xB8,0x2F};

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
		tile_info_table[i].xy_offset = 0x00;
		tile_info_table[i].layer = 0;
		tile_info_table[i].colorDepth = BPP_4;
		/* Inizializza height_map a 0 (vuoto) per tutte */
		for ( x = 0; x < 16; x++) {
			tile_info_table[i].height_map[x] = 0;
		}
	}


	tile_info_table[1].type = TILE_BACKGROUND;
	tile_info_table[1].colorDepth = BPP_2;
	tile_info_table[1].bitmap = &s2fg_001[0];
	tile_info_table[1].palette = AAA_TERRENO;


	tile_info_table[2].type = TILE_SOLID;
	tile_info_table[2].colorDepth = BPP_3;
	tile_info_table[2].bitmap = &s3fg_002[0];
	tile_info_table[2].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[3].type = TILE_BACKGROUND;
	tile_info_table[3].colorDepth = BPP_2;
	tile_info_table[3].bitmap = &s2fg_003[0];
	tile_info_table[3].layer=2;
	tile_info_table[3].xy_offset= 0x0E;// 14;
	tile_info_table[3].palette= AAA_ERBA_CELO;





	tile_info_table[51].type = TILE_BACKGROUND;
	tile_info_table[51].colorDepth = BPP_3;
	tile_info_table[51].bitmap = &s3fg_palm_001[0];
	tile_info_table[51].palette =AAA_TERRENO_ERBA_OMBRA;

	tile_info_table[52].type = TILE_BACKGROUND;
	tile_info_table[52].colorDepth = BPP_3;
	tile_info_table[52].bitmap = &s3fg_palm_002[0];
	tile_info_table[52].xy_offset= 0x10;// 14;
	tile_info_table[52].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[53].type = TILE_BACKGROUND;
	tile_info_table[53].colorDepth = BPP_3;
	tile_info_table[53].bitmap = &s3fg_palm_003[0];
	tile_info_table[53].overlay_tile = 3;
	tile_info_table[53].palette =AAA_TERRENO_ERBA_OMBRA;

	tile_info_table[54].type = TILE_BACKGROUND;
	tile_info_table[54].colorDepth = BPP_3;
	tile_info_table[54].bitmap = &s3fg_palm_003[0];
	tile_info_table[54].palette =AAA_TERRENO_ERBA_OMBRA;

	tile_info_table[55].type = TILE_BACKGROUND;
	tile_info_table[55].colorDepth = BPP_3;
	tile_info_table[55].bitmap = &s3fg_palm_005[0];
	tile_info_table[55].palette =AAA_TERRENO_ERBA_OMBRA;

	tile_info_table[56].type = TILE_BACKGROUND;
	tile_info_table[56].colorDepth = BPP_3;
	tile_info_table[56].bitmap = &s3fg_palm_006[0];
	tile_info_table[56].palette =AAA_TERRENO_ERBA_OMBRA;
	tile_info_table[56].xy_offset =0x01;

	tile_info_table[57].type = TILE_BACKGROUND;
	tile_info_table[57].colorDepth = BPP_2;
	tile_info_table[57].bitmap = &s2fg_palm_007[0];
	tile_info_table[57].palette =AAA_ERBA_CELO;
	tile_info_table[57].xy_offset =0x10;

	tile_info_table[58].type = TILE_BACKGROUND;
	tile_info_table[58].colorDepth = BPP_3;
	tile_info_table[58].bitmap = &s3fg_palm_008[0];
	tile_info_table[58].palette =AAA_TERRENO_ERBA_OMBRA;
	tile_info_table[58].xy_offset= 0x06;






	tile_info_table[61].type = TILE_BACKGROUND;
	tile_info_table[61].colorDepth = BPP_3;
	tile_info_table[61].bitmap = &s3fg_palm_001[0];
	tile_info_table[61].layer=2;
	tile_info_table[61].palette =AAA_TERRENO_ERBA_OMBRA;

	tile_info_table[62].type = TILE_BACKGROUND;
	tile_info_table[62].colorDepth = BPP_3;
	tile_info_table[62].bitmap = &s3fg_palm_002[0];
	tile_info_table[62].layer=2;
	tile_info_table[62].xy_offset= 0x10;// 14;
	tile_info_table[62].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[63].type = TILE_BACKGROUND;
	tile_info_table[63].colorDepth = BPP_3;
	tile_info_table[63].bitmap = &s3fg_palm_003[0];
	tile_info_table[63].overlay_tile = 3;
	tile_info_table[63].layer=2;
	tile_info_table[63].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[64].type = TILE_BACKGROUND;
	tile_info_table[64].colorDepth = BPP_3;
	tile_info_table[64].bitmap = &s3fg_palm_003[0];
	tile_info_table[64].layer=2;
	tile_info_table[64].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[65].type = TILE_BACKGROUND;
	tile_info_table[65].colorDepth = BPP_3;
	tile_info_table[65].bitmap = &s3fg_palm_005[0];
	tile_info_table[65].layer=2;
	tile_info_table[65].palette =AAA_TERRENO_ERBA_OMBRA;


	tile_info_table[66].type = TILE_BACKGROUND;
	tile_info_table[66].colorDepth = BPP_3;
	tile_info_table[66].bitmap = &s3fg_palm_006[0];
	tile_info_table[66].layer=2;
	tile_info_table[66].palette =AAA_TERRENO_ERBA_OMBRA;
	tile_info_table[66].xy_offset =0x01;


	tile_info_table[67].type = TILE_BACKGROUND;
	tile_info_table[67].colorDepth = BPP_2;
	tile_info_table[67].bitmap = &s2fg_palm_007[0];
	tile_info_table[67].layer=2;
	tile_info_table[67].palette =AAA_ERBA_CELO;
	tile_info_table[67].xy_offset = 0x10;

	tile_info_table[68].type = TILE_BACKGROUND;
	tile_info_table[68].colorDepth = BPP_3;
	tile_info_table[68].bitmap = &s3fg_palm_008[0];
	tile_info_table[68].layer=2;
	tile_info_table[68].palette =AAA_TERRENO_ERBA_OMBRA;
	tile_info_table[68].xy_offset= 0x06;






    tile_info_table[100].type = TILE_BACKGROUND;
    tile_info_table[100].colorDepth = BPP_3;
    tile_info_table[100].bitmap = &s3bg_prx_001;
	tile_info_table[100].palette =AAA_SONIC;

    tile_info_table[101].type = TILE_BACKGROUND;
    tile_info_table[101].colorDepth = BPP_3;
    tile_info_table[101].bitmap =&s3bg_prx_002;
	tile_info_table[101].palette =AAA_SONIC;



    tile_info_table[102].type = TILE_BACKGROUND;
    tile_info_table[102].colorDepth = BPP_3;
    tile_info_table[102].bitmap = &s3bg_prx_001;
	tile_info_table[102].palette =AAA_SONIC;
	tile_info_table[102].is_mirrored=1;


    tile_info_table[103].type = TILE_BACKGROUND;
    tile_info_table[103].colorDepth = BPP_3;
    tile_info_table[103].bitmap =&s3bg_prx_002;
	tile_info_table[103].palette =AAA_SONIC;
	tile_info_table[103].is_mirrored=1;




    tile_info_table[104].type = TILE_BACKGROUND;
    tile_info_table[104].colorDepth = BPP_3;
    tile_info_table[104].bitmap =&s3bg_prx_003;
	tile_info_table[104].palette =AAA_MONTAGNE;
	tile_info_table[104].xy_offset=0x0C;



    tile_info_table[105].type = TILE_BACKGROUND;
    tile_info_table[105].colorDepth = BPP_3;
    tile_info_table[105].bitmap =&s3bg_prx_004;
	tile_info_table[105].palette =AAA_MONTAGNE;
	tile_info_table[105].xy_offset=0x01;

    tile_info_table[106].type = TILE_BACKGROUND;
    tile_info_table[106].colorDepth = BPP_3;
    tile_info_table[106].bitmap =&s3bg_prx_005;
	tile_info_table[106].palette =AAA_MONTAGNE;


    tile_info_table[107].type = TILE_BACKGROUND;
    tile_info_table[107].colorDepth = BPP_3;
    tile_info_table[107].bitmap =&s3bg_prx_006;
	tile_info_table[107].palette =AAA_MONTAGNE;


    tile_info_table[108].type = TILE_BACKGROUND;
    tile_info_table[108].colorDepth = BPP_3;
    tile_info_table[108].bitmap =&s3bg_prx_007;
	tile_info_table[108].palette =AAA_MONTAGNE;

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

