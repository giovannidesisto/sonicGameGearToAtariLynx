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
        tile_info_table[i].colorDepth = BPP_1;
        /* Inizializza height_map a 0 (vuoto) per tutte */
        for ( x = 0; x < 16; x++) {
            tile_info_table[i].height_map[x] = 0;
        }
    }




    /* Tile 1: Piattaforma erba */
    tile_info_table[Z1L1_PLANE_GRASS].type = TILE_PLATFORM;
    tile_info_table[Z1L1_PLANE_GRASS].colorDepth = BPP_4;
    tile_info_table[Z1L1_PLANE_GRASS].bitmap = (unsigned char*)LEVEL_1_PLATFORM[0];
    /* Height map per tile piena (tutta a 15) */
    for ( x = 0; x < 16; x++) {
        tile_info_table[Z1L1_PLANE_GRASS].height_map[x] = 10;
    }
    /* Tile 1: ombra erba  0-1 */
    tile_info_table[2].type = TILE_BACKGROUND;
    tile_info_table[2].colorDepth = BPP_4;
    tile_info_table[2].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[0];


    /* Tile 3: ombra erba 0-2  */
    tile_info_table[3].type = TILE_BACKGROUND;
    tile_info_table[3].colorDepth = BPP_4;
    tile_info_table[3].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[1];

    /* Tile 3: quadrettato scuro */
    tile_info_table[4].type = TILE_BACKGROUND;
    tile_info_table[4].colorDepth = BPP_4;
    tile_info_table[4].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[2];


    /* Tile 3: quadrettato muro scuro con erba  solido*/
    tile_info_table[5].type = TILE_SOLID;
    tile_info_table[5].colorDepth = BPP_4;
    tile_info_table[5].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[3];


    /* Tile 3: quadrettato muro scuro con erba  piattaforma*/
    tile_info_table[6].type = TILE_PLATFORM;
    tile_info_table[6].colorDepth = BPP_4;
    tile_info_table[6].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[3];
    /* Height map per tile piena (tutta a 15) */
    for ( x = 0; x < 16; x++) {
        tile_info_table[6].height_map[x] = 10;
    }





    /* muro scuro destro solido intero */
    tile_info_table[7].type = TILE_SOLID;
    tile_info_table[7].colorDepth = BPP_4;
    tile_info_table[7].bitmap = (unsigned char*)LEVEL_1_WALL[0];


    /* muro scuro destro attraversabile  intero */
    tile_info_table[8].type = TILE_BACKGROUND;
    tile_info_table[8].colorDepth = BPP_4;
    tile_info_table[8].bitmap = (unsigned char*)LEVEL_1_WALL[0];



    /* muro scuro destro con erba  solido intero */
    tile_info_table[8].type = TILE_SOLID;
    tile_info_table[8].colorDepth = BPP_4;
    tile_info_table[8].bitmap = (unsigned char*)LEVEL_1_WALL[1];


    /* muro scuro destro con erba attraversabile  intero */
    tile_info_table[9].type = TILE_BACKGROUND;
    tile_info_table[9].colorDepth = BPP_4;
    tile_info_table[9].bitmap = (unsigned char*)LEVEL_1_WALL[1];


    /* TERMINALE ERBA ALTO DX */
    tile_info_table[10].type = TILE_PLATFORM;
    tile_info_table[10].colorDepth = BPP_4;
    tile_info_table[10].bitmap = (unsigned char*)LEVEL_1_PLATFORM[1];
    /* Height map per tile piena (tutta a 15) */

    for ( x = 0; x < 16; x++) {
        tile_info_table[10].height_map[x] = 10;
    }




    //avvallamento da due sx parte prima
    tile_info_table[11].type = TILE_PLATFORM;
    tile_info_table[11].colorDepth = BPP_4;
    tile_info_table[11].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2];
    /* Height map per tile piena (tutta a 15) */
    y=0;
    for ( x = 0; x < 16; x++) {
    	if(x<8)y=2;
    	else y=4;
        tile_info_table[11].height_map[x] = 10-y;
    }


    //avvallamento da due sx parte seconda
    tile_info_table[12].type = TILE_PLATFORM;
    tile_info_table[12].colorDepth = BPP_4;
    tile_info_table[12].bitmap = (unsigned char*)LEVEL_1_PLATFORM[3];
    /* Height map per tile piena (tutta a 15) */
    //tile_info_table[12].height_map = {8,8,8,8,8,8,8,8,6,6,6,6,6,6,6,6};

    y=0;
    for ( x = 0; x < 16; x++) {
    	if(x<8)y=6;
    	else y=8;
        tile_info_table[12].height_map[x] = 10-y;
    }


    //avvallamento da due dx parte seconda
    tile_info_table[13].type = TILE_PLATFORM;
    tile_info_table[13].colorDepth = BPP_4;
    tile_info_table[13].bitmap = (unsigned char*)LEVEL_1_PLATFORM[3];
    tile_info_table[13].is_mirrored=1;
    /* Height map per tile piena (tutta a 15) */
    y=0;
    for ( x = 0; x < 16; x++) {
      	if(x<8)y=6;
        else y=8;
        tile_info_table[13].height_map[x] = 10-y;
    }


    //avvallamento da due dx parte seconda
    tile_info_table[14].type = TILE_PLATFORM;
    tile_info_table[14].colorDepth = BPP_4;
    tile_info_table[14].bitmap = (unsigned char*)LEVEL_1_PLATFORM[2];
    tile_info_table[14].is_mirrored=1;
    /* Height map per tile piena (tutta a 15) */
    y=0;
    for ( x = 0; x < 16; x++) {
      	if(x<8)y=2;
        else y=4;
        tile_info_table[14].height_map[x] = 10-y;
    }



    /* erba sotto avvallamento da due sx*/
    tile_info_table[15].type = TILE_BACKGROUND;
    tile_info_table[15].colorDepth = BPP_4;
    tile_info_table[15].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[4];

    /* erba sotto avvallamento da due dx*/
    tile_info_table[16].type = TILE_BACKGROUND;
    tile_info_table[16].colorDepth = BPP_4;
    tile_info_table[16].is_mirrored=1;
    tile_info_table[16].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[4];




    /* muro chiaro SX solido intero */
       tile_info_table[17].type = TILE_SOLID;
       tile_info_table[17].colorDepth = BPP_4;
       tile_info_table[17].bitmap = (unsigned char*)LEVEL_1_WALL[2];


       /* muro chiaro SX attraversabile  intero */
       tile_info_table[18].type = TILE_SOLID;
       tile_info_table[18].colorDepth = BPP_4;
       tile_info_table[18].bitmap = (unsigned char*)LEVEL_1_WALL[2];


       /* TERMINALE ERBA ALTO DX */
       tile_info_table[19].type = TILE_PLATFORM;
       tile_info_table[19].colorDepth = BPP_4;
       tile_info_table[19].bitmap = (unsigned char*)LEVEL_1_PLATFORM[1];
       tile_info_table[19].is_mirrored=1;
       /* Height map per tile piena (tutta a 15) */

       for ( x = 0; x < 16; x++) {
           tile_info_table[19].height_map[x] = 10;
       }




       /* TERMINALE ERBA ALTO DX */
       tile_info_table[20].type = TILE_PLATFORM;
       tile_info_table[20].colorDepth = BPP_4;
       tile_info_table[20].bitmap = (unsigned char*)LEVEL_1_WALL[3];
       ;
       /* Height map per tile piena (tutta a 15) */

       for ( x = 0; x < 16; x++) {
           tile_info_table[20].height_map[x] = 10;
       }



    //totem
    tile_info_table[99].type = TILE_BACKGROUND;
    tile_info_table[99].colorDepth = BPP_4;
    tile_info_table[99].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[5];

//    //palma
//    tile_info_table[100].type = TILE_BACKGROUND;
//    tile_info_table[100].colorDepth = BPP_4;
//    tile_info_table[100].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[6];

    /* Tile 6: Sfondo (nuvole) */
//    tile_info_table[6].type = TILE_BACKGROUND;
//    tile_info_table[6].colorDepth = BPP_4;
//    tile_info_table[6].bitmap = (unsigned char*)LEVEL_1_BACKGROUND[0];





    /* Tile 100: Sfondo (nuvole) */
//    tile_info_table[100].type = TILE_BACKGROUND;
//    tile_info_table[100].colorDepth = BPP_4;
//    tile_info_table[100].bitmap = (unsigned char*)LEVEL_1_PRX[0];


    // === AUTO-GENERATED TILE INFO (COMPILER SAFE) ===

//    tile_info_table[100].type = TILE_BACKGROUND;
//    tile_info_table[100].colorDepth = BPP_1;
//    tile_info_table[100].bitmap = (unsigned char*)tile1_100;
//    tile_info_table[100].is_mirrored = 0;
//    tile_info_table[100].is_flipped = 0;
//    tile_info_table[100].palette[0] = 0x60;
//
//    tile_info_table[101].type = TILE_BACKGROUND;
//    tile_info_table[101].colorDepth = BPP_2;
//    tile_info_table[101].bitmap = (unsigned char*)tile2_101;
//    tile_info_table[101].is_mirrored = 0;
//    tile_info_table[101].is_flipped = 0;
//    tile_info_table[101].palette[0] = 0x06;
//    tile_info_table[101].palette[1] = 0x9F;
//
//    tile_info_table[102].type = TILE_BACKGROUND;
//    tile_info_table[102].colorDepth = BPP_2;
//    tile_info_table[102].bitmap = (unsigned char*)tile2_102;
//    tile_info_table[102].is_mirrored = 0;
//    tile_info_table[102].is_flipped = 0;
//    tile_info_table[102].palette[0] = 0x06;
//    tile_info_table[102].palette[1] = 0x9F;
//
//    tile_info_table[103].type = TILE_BACKGROUND;
//    tile_info_table[103].colorDepth = BPP_2;
//    tile_info_table[103].bitmap = (unsigned char*)tile2_103;
//    tile_info_table[103].is_mirrored = 0;
//    tile_info_table[103].is_flipped = 0;
//    tile_info_table[103].palette[0] = 0x06;
//    tile_info_table[103].palette[1] = 0x9F;
//
//    tile_info_table[104].type = TILE_BACKGROUND;
//    tile_info_table[104].colorDepth = BPP_2;
//    tile_info_table[104].bitmap = (unsigned char*)tile2_104;
//    tile_info_table[104].is_mirrored = 0;
//    tile_info_table[104].is_flipped = 0;
//    tile_info_table[104].palette[0] = 0x06;
//    tile_info_table[104].palette[1] = 0x9F;
//
//    tile_info_table[105].type = TILE_BACKGROUND;
//    tile_info_table[105].colorDepth = BPP_2;
//    tile_info_table[105].bitmap = (unsigned char*)tile2_105;
//    tile_info_table[105].is_mirrored = 0;
//    tile_info_table[105].is_flipped = 0;
//    tile_info_table[105].palette[0] = 0x06;
//    tile_info_table[105].palette[1] = 0x9F;
//
//    tile_info_table[106].type = TILE_BACKGROUND;
//    tile_info_table[106].colorDepth = BPP_2;
//    tile_info_table[106].bitmap = (unsigned char*)tile2_106;
//    tile_info_table[106].is_mirrored = 0;
//    tile_info_table[106].is_flipped = 0;
//    tile_info_table[106].palette[0] = 0x06;
//    tile_info_table[106].palette[1] = 0x9F;
//
//    tile_info_table[107].type = TILE_BACKGROUND;
//    tile_info_table[107].colorDepth = BPP_2;
//    tile_info_table[107].bitmap = (unsigned char*)tile2_107;
//    tile_info_table[107].is_mirrored = 0;
//    tile_info_table[107].is_flipped = 0;
//    tile_info_table[107].palette[0] = 0x06;
//    tile_info_table[107].palette[1] = 0x9F;
//
//    tile_info_table[108].type = TILE_BACKGROUND;
//    tile_info_table[108].colorDepth = BPP_2;
//    tile_info_table[108].bitmap = (unsigned char*)tile2_108;
//    tile_info_table[108].is_mirrored = 0;
//    tile_info_table[108].is_flipped = 0;
//    tile_info_table[108].palette[0] = 0x06;
//    tile_info_table[108].palette[1] = 0x9F;
//
//    tile_info_table[109].type = TILE_BACKGROUND;
//    tile_info_table[109].colorDepth = BPP_2;
//    tile_info_table[109].bitmap = (unsigned char*)tile2_109;
//    tile_info_table[109].is_mirrored = 0;
//    tile_info_table[109].is_flipped = 0;
//    tile_info_table[109].palette[0] = 0x06;
//    tile_info_table[109].palette[1] = 0x9F;
//
//    tile_info_table[110].type = TILE_BACKGROUND;
//    tile_info_table[110].colorDepth = BPP_2;
//    tile_info_table[110].bitmap = (unsigned char*)tile2_110;
//    tile_info_table[110].is_mirrored = 0;
//    tile_info_table[110].is_flipped = 0;
//    tile_info_table[110].palette[0] = 0x06;
//    tile_info_table[110].palette[1] = 0x9F;
//
//    tile_info_table[111].type = TILE_BACKGROUND;
//    tile_info_table[111].colorDepth = BPP_2;
//    tile_info_table[111].bitmap = (unsigned char*)tile2_111;
//    tile_info_table[111].is_mirrored = 0;
//    tile_info_table[111].is_flipped = 0;
//    tile_info_table[111].palette[0] = 0x06;
//    tile_info_table[111].palette[1] = 0x9F;
//
//    tile_info_table[112].type = TILE_BACKGROUND;
//    tile_info_table[112].colorDepth = BPP_2;
//    tile_info_table[112].bitmap = (unsigned char*)tile2_112;
//    tile_info_table[112].is_mirrored = 0;
//    tile_info_table[112].is_flipped = 0;
//    tile_info_table[112].palette[0] = 0x06;
//    tile_info_table[112].palette[1] = 0x9F;
//
//    tile_info_table[113].type = TILE_BACKGROUND;
//    tile_info_table[113].colorDepth = BPP_2;
//    tile_info_table[113].bitmap = (unsigned char*)tile2_113;
//    tile_info_table[113].is_mirrored = 0;
//    tile_info_table[113].is_flipped = 0;
//    tile_info_table[113].palette[0] = 0x06;
//    tile_info_table[113].palette[1] = 0x90;
//
//    tile_info_table[114].type = TILE_BACKGROUND;
//    tile_info_table[114].colorDepth = BPP_3;
//    tile_info_table[114].bitmap = (unsigned char*)tile3_114;
//    tile_info_table[114].is_mirrored = 0;
//    tile_info_table[114].is_flipped = 0;
//    tile_info_table[114].palette[0] = 0x12;
//    tile_info_table[114].palette[1] = 0x56;
//    tile_info_table[114].palette[2] = 0xB0;
//
//    tile_info_table[115].type = TILE_BACKGROUND;
//    tile_info_table[115].colorDepth = BPP_3;
//    tile_info_table[115].bitmap = (unsigned char*)tile3_115;
//    tile_info_table[115].is_mirrored = 0;
//    tile_info_table[115].is_flipped = 0;
//    tile_info_table[115].palette[0] = 0x12;
//    tile_info_table[115].palette[1] = 0x56;
//    tile_info_table[115].palette[2] = 0xB0;
//
//    tile_info_table[116].type = TILE_BACKGROUND;
//    tile_info_table[116].colorDepth = BPP_3;
//    tile_info_table[116].bitmap = (unsigned char*)tile3_116;
//    tile_info_table[116].is_mirrored = 0;
//    tile_info_table[116].is_flipped = 0;
//    tile_info_table[116].palette[0] = 0x12;
//    tile_info_table[116].palette[1] = 0x56;
//    tile_info_table[116].palette[2] = 0xB0;
//
//    tile_info_table[117].type = TILE_BACKGROUND;
//    tile_info_table[117].colorDepth = BPP_2;
//    tile_info_table[117].bitmap = (unsigned char*)tile2_117;
//    tile_info_table[117].is_mirrored = 0;
//    tile_info_table[117].is_flipped = 0;
//    tile_info_table[117].palette[0] = 0x25;
//    tile_info_table[117].palette[1] = 0x6B;
//
//    tile_info_table[118].type = TILE_BACKGROUND;
//    tile_info_table[118].colorDepth = BPP_3;
//    tile_info_table[118].bitmap = (unsigned char*)tile3_118;
//    tile_info_table[118].is_mirrored = 0;
//    tile_info_table[118].is_flipped = 0;
//    tile_info_table[118].palette[0] = 0x12;
//    tile_info_table[118].palette[1] = 0x56;
//    tile_info_table[118].palette[2] = 0xB0;
//
//    tile_info_table[119].type = TILE_BACKGROUND;
//    tile_info_table[119].colorDepth = BPP_2;
//    tile_info_table[119].bitmap = (unsigned char*)tile2_119;
//    tile_info_table[119].is_mirrored = 0;
//    tile_info_table[119].is_flipped = 0;
//    tile_info_table[119].palette[0] = 0x12;
//    tile_info_table[119].palette[1] = 0x50;
//
//    tile_info_table[120].type = TILE_BACKGROUND;
//    tile_info_table[120].colorDepth = BPP_3;
//    tile_info_table[120].bitmap = (unsigned char*)tile3_120;
//    tile_info_table[120].is_mirrored = 0;
//    tile_info_table[120].is_flipped = 0;
//    tile_info_table[120].palette[0] = 0x12;
//    tile_info_table[120].palette[1] = 0x56;
//    tile_info_table[120].palette[2] = 0xB0;
//
//    tile_info_table[121].type = TILE_BACKGROUND;
//    tile_info_table[121].colorDepth = BPP_3;
//    tile_info_table[121].bitmap = (unsigned char*)tile3_121;
//    tile_info_table[121].is_mirrored = 0;
//    tile_info_table[121].is_flipped = 0;
//    tile_info_table[121].palette[0] = 0x12;
//    tile_info_table[121].palette[1] = 0x56;
//    tile_info_table[121].palette[2] = 0xB0;
//
//    tile_info_table[122].type = TILE_BACKGROUND;
//    tile_info_table[122].colorDepth = BPP_3;
//    tile_info_table[122].bitmap = (unsigned char*)tile3_122;
//    tile_info_table[122].is_mirrored = 0;
//    tile_info_table[122].is_flipped = 0;
//    tile_info_table[122].palette[0] = 0x12;
//    tile_info_table[122].palette[1] = 0x56;
//    tile_info_table[122].palette[2] = 0xB0;
//
//    tile_info_table[123].type = TILE_BACKGROUND;
//    tile_info_table[123].colorDepth = BPP_3;
//    tile_info_table[123].bitmap = (unsigned char*)tile3_123;
//    tile_info_table[123].is_mirrored = 0;
//    tile_info_table[123].is_flipped = 0;
//    tile_info_table[123].palette[0] = 0x12;
//    tile_info_table[123].palette[1] = 0x56;
//    tile_info_table[123].palette[2] = 0xB0;
//
//    tile_info_table[124].type = TILE_BACKGROUND;
//    tile_info_table[124].colorDepth = BPP_3;
//    tile_info_table[124].bitmap = (unsigned char*)tile3_124;
//    tile_info_table[124].is_mirrored = 0;
//    tile_info_table[124].is_flipped = 0;
//    tile_info_table[124].palette[0] = 0x12;
//    tile_info_table[124].palette[1] = 0x56;
//    tile_info_table[124].palette[2] = 0xB0;
//
//    tile_info_table[125].type = TILE_BACKGROUND;
//    tile_info_table[125].colorDepth = BPP_2;
//    tile_info_table[125].bitmap = (unsigned char*)tile2_125;
//    tile_info_table[125].is_mirrored = 0;
//    tile_info_table[125].is_flipped = 0;
//    tile_info_table[125].palette[0] = 0x12;
//    tile_info_table[125].palette[1] = 0x56;
//
//    tile_info_table[126].type = TILE_BACKGROUND;
//    tile_info_table[126].colorDepth = BPP_2;
//    tile_info_table[126].bitmap = (unsigned char*)tile2_126;
//    tile_info_table[126].is_mirrored = 0;
//    tile_info_table[126].is_flipped = 0;
//    tile_info_table[126].palette[0] = 0x12;
//    tile_info_table[126].palette[1] = 0x50;
//
//    tile_info_table[127].type = TILE_BACKGROUND;
//    tile_info_table[127].colorDepth = BPP_2;
//    tile_info_table[127].bitmap = (unsigned char*)tile2_127;
//    tile_info_table[127].is_mirrored = 0;
//    tile_info_table[127].is_flipped = 0;
//    tile_info_table[127].palette[0] = 0x12;
//    tile_info_table[127].palette[1] = 0x50;
//
//    tile_info_table[128].type = TILE_BACKGROUND;
//    tile_info_table[128].colorDepth = BPP_2;
//    tile_info_table[128].bitmap = (unsigned char*)tile2_128;
//    tile_info_table[128].is_mirrored = 0;
//    tile_info_table[128].is_flipped = 0;
//    tile_info_table[128].palette[0] = 0x12;
//    tile_info_table[128].palette[1] = 0x50;
//
//    tile_info_table[129].type = TILE_BACKGROUND;
//    tile_info_table[129].colorDepth = BPP_1;
//    tile_info_table[129].bitmap = (unsigned char*)tile1_129;
//    tile_info_table[129].is_mirrored = 0;
//    tile_info_table[129].is_flipped = 0;
//    tile_info_table[129].palette[0] = 0x15;
//
//    tile_info_table[130].type = TILE_BACKGROUND;
//    tile_info_table[130].colorDepth = BPP_2;
//    tile_info_table[130].bitmap = (unsigned char*)tile2_130;
//    tile_info_table[130].is_mirrored = 0;
//    tile_info_table[130].is_flipped = 0;
//    tile_info_table[130].palette[0] = 0x12;
//    tile_info_table[130].palette[1] = 0x50;
//
//    tile_info_table[131].type = TILE_BACKGROUND;
//    tile_info_table[131].colorDepth = BPP_3;
//    tile_info_table[131].bitmap = (unsigned char*)tile3_131;
//    tile_info_table[131].is_mirrored = 0;
//    tile_info_table[131].is_flipped = 0;
//    tile_info_table[131].palette[0] = 0x01;
//    tile_info_table[131].palette[1] = 0x25;
//    tile_info_table[131].palette[2] = 0x90;
//
//    tile_info_table[132].type = TILE_BACKGROUND;
//    tile_info_table[132].colorDepth = BPP_3;
//    tile_info_table[132].bitmap = (unsigned char*)tile3_132;
//    tile_info_table[132].is_mirrored = 0;
//    tile_info_table[132].is_flipped = 0;
//    tile_info_table[132].palette[0] = 0x01;
//    tile_info_table[132].palette[1] = 0x25;
//    tile_info_table[132].palette[2] = 0x90;
//
//    tile_info_table[133].type = TILE_BACKGROUND;
//    tile_info_table[133].colorDepth = BPP_2;
//    tile_info_table[133].bitmap = (unsigned char*)tile2_133;
//    tile_info_table[133].is_mirrored = 0;
//    tile_info_table[133].is_flipped = 0;
//    tile_info_table[133].palette[0] = 0x12;
//    tile_info_table[133].palette[1] = 0x50;
//
//    tile_info_table[134].type = TILE_BACKGROUND;
//    tile_info_table[134].colorDepth = BPP_3;
//    tile_info_table[134].bitmap = (unsigned char*)tile3_134;
//    tile_info_table[134].is_mirrored = 0;
//    tile_info_table[134].is_flipped = 0;
//    tile_info_table[134].palette[0] = 0x12;
//    tile_info_table[134].palette[1] = 0x38;
//    tile_info_table[134].palette[2] = 0xA0;
//
//    tile_info_table[135].type = TILE_BACKGROUND;
//    tile_info_table[135].colorDepth = BPP_2;
//    tile_info_table[135].bitmap = (unsigned char*)tile2_135;
//    tile_info_table[135].is_mirrored = 0;
//    tile_info_table[135].is_flipped = 0;
//    tile_info_table[135].palette[0] = 0x13;
//    tile_info_table[135].palette[1] = 0x8A;
//
//    tile_info_table[136].type = TILE_BACKGROUND;
//    tile_info_table[136].colorDepth = BPP_2;
//    tile_info_table[136].bitmap = (unsigned char*)tile2_136;
//    tile_info_table[136].is_mirrored = 0;
//    tile_info_table[136].is_flipped = 0;
//    tile_info_table[136].palette[0] = 0x13;
//    tile_info_table[136].palette[1] = 0x8A;
//
//    tile_info_table[137].type = TILE_BACKGROUND;
//    tile_info_table[137].colorDepth = BPP_2;
//    tile_info_table[137].bitmap = (unsigned char*)tile2_137;
//    tile_info_table[137].is_mirrored = 0;
//    tile_info_table[137].is_flipped = 0;
//    tile_info_table[137].palette[0] = 0x13;
//    tile_info_table[137].palette[1] = 0x8A;
//
//    tile_info_table[138].type = TILE_BACKGROUND;
//    tile_info_table[138].colorDepth = BPP_2;
//    tile_info_table[138].bitmap = (unsigned char*)tile2_138;
//    tile_info_table[138].is_mirrored = 0;
//    tile_info_table[138].is_flipped = 0;
//    tile_info_table[138].palette[0] = 0x13;
//    tile_info_table[138].palette[1] = 0x8A;
//
//    tile_info_table[139].type = TILE_BACKGROUND;
//    tile_info_table[139].colorDepth = BPP_2;
//    tile_info_table[139].bitmap = (unsigned char*)tile2_139;
//    tile_info_table[139].is_mirrored = 0;
//    tile_info_table[139].is_flipped = 0;
//    tile_info_table[139].palette[0] = 0x13;
//    tile_info_table[139].palette[1] = 0x8A;
//
//    tile_info_table[140].type = TILE_BACKGROUND;
//    tile_info_table[140].colorDepth = BPP_2;
//    tile_info_table[140].bitmap = (unsigned char*)tile2_140;
//    tile_info_table[140].is_mirrored = 0;
//    tile_info_table[140].is_flipped = 0;
//    tile_info_table[140].palette[0] = 0x01;
//    tile_info_table[140].palette[1] = 0x90;
//
//    tile_info_table[141].type = TILE_BACKGROUND;
//    tile_info_table[141].colorDepth = BPP_2;
//    tile_info_table[141].bitmap = (unsigned char*)tile2_141;
//    tile_info_table[141].is_mirrored = 0;
//    tile_info_table[141].is_flipped = 0;
//    tile_info_table[141].palette[0] = 0x12;
//    tile_info_table[141].palette[1] = 0x50;
//
//    tile_info_table[142].type = TILE_BACKGROUND;
//    tile_info_table[142].colorDepth = BPP_2;
//    tile_info_table[142].bitmap = (unsigned char*)tile2_142;
//    tile_info_table[142].is_mirrored = 0;
//    tile_info_table[142].is_flipped = 0;
//    tile_info_table[142].palette[0] = 0x12;
//    tile_info_table[142].palette[1] = 0x50;
//
//    tile_info_table[143].type = TILE_BACKGROUND;
//    tile_info_table[143].colorDepth = BPP_1;
//    tile_info_table[143].bitmap = (unsigned char*)tile1_143;
//    tile_info_table[143].is_mirrored = 0;
//    tile_info_table[143].is_flipped = 0;
//    tile_info_table[143].palette[0] = 0x15;
//
//    tile_info_table[144].type = TILE_BACKGROUND;
//    tile_info_table[144].colorDepth = BPP_2;
//    tile_info_table[144].bitmap = (unsigned char*)tile2_144;
//    tile_info_table[144].is_mirrored = 0;
//    tile_info_table[144].is_flipped = 0;
//    tile_info_table[144].palette[0] = 0x12;
//    tile_info_table[144].palette[1] = 0x50;
//
//    tile_info_table[145].type = TILE_BACKGROUND;
//    tile_info_table[145].colorDepth = BPP_2;
//    tile_info_table[145].bitmap = (unsigned char*)tile2_145;
//    tile_info_table[145].is_mirrored = 0;
//    tile_info_table[145].is_flipped = 0;
//    tile_info_table[145].palette[0] = 0x12;
//    tile_info_table[145].palette[1] = 0x50;
//
//    tile_info_table[146].type = TILE_BACKGROUND;
//    tile_info_table[146].colorDepth = BPP_2;
//    tile_info_table[146].bitmap = (unsigned char*)tile2_146;
//    tile_info_table[146].is_mirrored = 0;
//    tile_info_table[146].is_flipped = 0;
//    tile_info_table[146].palette[0] = 0x12;
//    tile_info_table[146].palette[1] = 0x50;
//
//    tile_info_table[147].type = TILE_BACKGROUND;
//    tile_info_table[147].colorDepth = BPP_2;
//    tile_info_table[147].bitmap = (unsigned char*)tile2_147;
//    tile_info_table[147].is_mirrored = 0;
//    tile_info_table[147].is_flipped = 0;
//    tile_info_table[147].palette[0] = 0x12;
//    tile_info_table[147].palette[1] = 0x50;
//
//    tile_info_table[148].type = TILE_BACKGROUND;
//    tile_info_table[148].colorDepth = BPP_2;
//    tile_info_table[148].bitmap = (unsigned char*)tile2_148;
//    tile_info_table[148].is_mirrored = 0;
//    tile_info_table[148].is_flipped = 0;
//    tile_info_table[148].palette[0] = 0x12;
//    tile_info_table[148].palette[1] = 0x50;
//
//    tile_info_table[149].type = TILE_BACKGROUND;
//    tile_info_table[149].colorDepth = BPP_3;
//    tile_info_table[149].bitmap = (unsigned char*)tile3_149;
//    tile_info_table[149].is_mirrored = 0;
//    tile_info_table[149].is_flipped = 0;
//    tile_info_table[149].palette[0] = 0x01;
//    tile_info_table[149].palette[1] = 0x25;
//    tile_info_table[149].palette[2] = 0x90;
//
//    tile_info_table[150].type = TILE_BACKGROUND;
//    tile_info_table[150].colorDepth = BPP_3;
//    tile_info_table[150].bitmap = (unsigned char*)tile3_150;
//    tile_info_table[150].is_mirrored = 0;
//    tile_info_table[150].is_flipped = 0;
//    tile_info_table[150].palette[0] = 0x01;
//    tile_info_table[150].palette[1] = 0x25;
//    tile_info_table[150].palette[2] = 0x90;
//
//    tile_info_table[151].type = TILE_BACKGROUND;
//    tile_info_table[151].colorDepth = BPP_2;
//    tile_info_table[151].bitmap = (unsigned char*)tile2_151;
//    tile_info_table[151].is_mirrored = 0;
//    tile_info_table[151].is_flipped = 0;
//    tile_info_table[151].palette[0] = 0x12;
//    tile_info_table[151].palette[1] = 0x50;
//
//    tile_info_table[152].type = TILE_BACKGROUND;
//    tile_info_table[152].colorDepth = BPP_3;
//    tile_info_table[152].bitmap = (unsigned char*)tile3_152;
//    tile_info_table[152].is_mirrored = 0;
//    tile_info_table[152].is_flipped = 0;
//    tile_info_table[152].palette[0] = 0x12;
//    tile_info_table[152].palette[1] = 0x35;
//    tile_info_table[152].palette[2] = 0x8A;
//
//    tile_info_table[153].type = TILE_BACKGROUND;
//    tile_info_table[153].colorDepth = BPP_3;
//    tile_info_table[153].bitmap = (unsigned char*)tile3_153;
//    tile_info_table[153].is_mirrored = 0;
//    tile_info_table[153].is_flipped = 0;
//    tile_info_table[153].palette[0] = 0x12;
//    tile_info_table[153].palette[1] = 0x35;
//    tile_info_table[153].palette[2] = 0x8A;
//
//    tile_info_table[154].type = TILE_BACKGROUND;
//    tile_info_table[154].colorDepth = BPP_3;
//    tile_info_table[154].bitmap = (unsigned char*)tile3_154;
//    tile_info_table[154].is_mirrored = 0;
//    tile_info_table[154].is_flipped = 0;
//    tile_info_table[154].palette[0] = 0x12;
//    tile_info_table[154].palette[1] = 0x35;
//    tile_info_table[154].palette[2] = 0x8A;
//
//    tile_info_table[155].type = TILE_BACKGROUND;
//    tile_info_table[155].colorDepth = BPP_3;
//    tile_info_table[155].bitmap = (unsigned char*)tile3_155;
//    tile_info_table[155].is_mirrored = 0;
//    tile_info_table[155].is_flipped = 0;
//    tile_info_table[155].palette[0] = 0x12;
//    tile_info_table[155].palette[1] = 0x38;
//    tile_info_table[155].palette[2] = 0xA0;
//
//    tile_info_table[156].type = TILE_BACKGROUND;
//    tile_info_table[156].colorDepth = BPP_2;
//    tile_info_table[156].bitmap = (unsigned char*)tile2_156;
//    tile_info_table[156].is_mirrored = 0;
//    tile_info_table[156].is_flipped = 0;
//    tile_info_table[156].palette[0] = 0x13;
//    tile_info_table[156].palette[1] = 0x8A;
//
//    tile_info_table[157].type = TILE_BACKGROUND;
//    tile_info_table[157].colorDepth = BPP_3;
//    tile_info_table[157].bitmap = (unsigned char*)tile3_157;
//    tile_info_table[157].is_mirrored = 0;
//    tile_info_table[157].is_flipped = 0;
//    tile_info_table[157].palette[0] = 0x12;
//    tile_info_table[157].palette[1] = 0x35;
//    tile_info_table[157].palette[2] = 0x8A;
//
//    tile_info_table[158].type = TILE_BACKGROUND;
//    tile_info_table[158].colorDepth = BPP_1;
//    tile_info_table[158].bitmap = (unsigned char*)tile1_158;
//    tile_info_table[158].is_mirrored = 0;
//    tile_info_table[158].is_flipped = 0;
//    tile_info_table[158].palette[0] = 0x09;
//
//    tile_info_table[159].type = TILE_BACKGROUND;
//    tile_info_table[159].colorDepth = BPP_2;
//    tile_info_table[159].bitmap = (unsigned char*)tile2_159;
//    tile_info_table[159].is_mirrored = 0;
//    tile_info_table[159].is_flipped = 0;
//    tile_info_table[159].palette[0] = 0x12;
//    tile_info_table[159].palette[1] = 0x50;
//
//    tile_info_table[160].type = TILE_BACKGROUND;
//    tile_info_table[160].colorDepth = BPP_2;
//    tile_info_table[160].bitmap = (unsigned char*)tile2_160;
//    tile_info_table[160].is_mirrored = 0;
//    tile_info_table[160].is_flipped = 0;
//    tile_info_table[160].palette[0] = 0x12;
//    tile_info_table[160].palette[1] = 0x50;
//
//    tile_info_table[161].type = TILE_BACKGROUND;
//    tile_info_table[161].colorDepth = BPP_2;
//    tile_info_table[161].bitmap = (unsigned char*)tile2_161;
//    tile_info_table[161].is_mirrored = 0;
//    tile_info_table[161].is_flipped = 0;
//    tile_info_table[161].palette[0] = 0x12;
//    tile_info_table[161].palette[1] = 0x50;
//
//    tile_info_table[162].type = TILE_BACKGROUND;
//    tile_info_table[162].colorDepth = BPP_3;
//    tile_info_table[162].bitmap = (unsigned char*)tile3_162;
//    tile_info_table[162].is_mirrored = 0;
//    tile_info_table[162].is_flipped = 0;
//    tile_info_table[162].palette[0] = 0x01;
//    tile_info_table[162].palette[1] = 0x25;
//    tile_info_table[162].palette[2] = 0x90;
//
//    tile_info_table[163].type = TILE_BACKGROUND;
//    tile_info_table[163].colorDepth = BPP_3;
//    tile_info_table[163].bitmap = (unsigned char*)tile3_163;
//    tile_info_table[163].is_mirrored = 0;
//    tile_info_table[163].is_flipped = 0;
//    tile_info_table[163].palette[0] = 0x01;
//    tile_info_table[163].palette[1] = 0x25;
//    tile_info_table[163].palette[2] = 0x90;
//
//    tile_info_table[164].type = TILE_BACKGROUND;
//    tile_info_table[164].colorDepth = BPP_3;
//    tile_info_table[164].bitmap = (unsigned char*)tile3_164;
//    tile_info_table[164].is_mirrored = 0;
//    tile_info_table[164].is_flipped = 0;
//    tile_info_table[164].palette[0] = 0x01;
//    tile_info_table[164].palette[1] = 0x25;
//    tile_info_table[164].palette[2] = 0x90;
//
//    tile_info_table[165].type = TILE_BACKGROUND;
//    tile_info_table[165].colorDepth = BPP_3;
//    tile_info_table[165].bitmap = (unsigned char*)tile3_165;
//    tile_info_table[165].is_mirrored = 0;
//    tile_info_table[165].is_flipped = 0;
//    tile_info_table[165].palette[0] = 0x01;
//    tile_info_table[165].palette[1] = 0x25;
//    tile_info_table[165].palette[2] = 0x90;
//
//    tile_info_table[166].type = TILE_BACKGROUND;
//    tile_info_table[166].colorDepth = BPP_3;
//    tile_info_table[166].bitmap = (unsigned char*)tile3_166;
//    tile_info_table[166].is_mirrored = 0;
//    tile_info_table[166].is_flipped = 0;
//    tile_info_table[166].palette[0] = 0x01;
//    tile_info_table[166].palette[1] = 0x25;
//    tile_info_table[166].palette[2] = 0x90;
//
//    tile_info_table[167].type = TILE_BACKGROUND;
//    tile_info_table[167].colorDepth = BPP_2;
//    tile_info_table[167].bitmap = (unsigned char*)tile2_167;
//    tile_info_table[167].is_mirrored = 0;
//    tile_info_table[167].is_flipped = 0;
//    tile_info_table[167].palette[0] = 0x01;
//    tile_info_table[167].palette[1] = 0x29;
//
//    tile_info_table[168].type = TILE_BACKGROUND;
//    tile_info_table[168].colorDepth = BPP_3;
//    tile_info_table[168].bitmap = (unsigned char*)tile3_168;
//    tile_info_table[168].is_mirrored = 0;
//    tile_info_table[168].is_flipped = 0;
//    tile_info_table[168].palette[0] = 0x01;
//    tile_info_table[168].palette[1] = 0x25;
//    tile_info_table[168].palette[2] = 0x90;
//
//    tile_info_table[169].type = TILE_BACKGROUND;
//    tile_info_table[169].colorDepth = BPP_3;
//    tile_info_table[169].bitmap = (unsigned char*)tile3_169;
//    tile_info_table[169].is_mirrored = 0;
//    tile_info_table[169].is_flipped = 0;
//    tile_info_table[169].palette[0] = 0x01;
//    tile_info_table[169].palette[1] = 0x25;
//    tile_info_table[169].palette[2] = 0x90;
//
//    tile_info_table[170].type = TILE_BACKGROUND;
//    tile_info_table[170].colorDepth = BPP_3;
//    tile_info_table[170].bitmap = (unsigned char*)tile3_170;
//    tile_info_table[170].is_mirrored = 0;
//    tile_info_table[170].is_flipped = 0;
//    tile_info_table[170].palette[0] = 0x01;
//    tile_info_table[170].palette[1] = 0x23;
//    tile_info_table[170].palette[2] = 0x89;
//    tile_info_table[170].palette[3] = 0xA0;
//
//    tile_info_table[171].type = TILE_BACKGROUND;
//    tile_info_table[171].colorDepth = BPP_3;
//    tile_info_table[171].bitmap = (unsigned char*)tile3_171;
//    tile_info_table[171].is_mirrored = 0;
//    tile_info_table[171].is_flipped = 0;
//    tile_info_table[171].palette[0] = 0x01;
//    tile_info_table[171].palette[1] = 0x23;
//    tile_info_table[171].palette[2] = 0x58;
//    tile_info_table[171].palette[3] = 0x9A;
//
//    tile_info_table[172].type = TILE_BACKGROUND;
//    tile_info_table[172].colorDepth = BPP_3;
//    tile_info_table[172].bitmap = (unsigned char*)tile3_172;
//    tile_info_table[172].is_mirrored = 0;
//    tile_info_table[172].is_flipped = 0;
//    tile_info_table[172].palette[0] = 0x01;
//    tile_info_table[172].palette[1] = 0x23;
//    tile_info_table[172].palette[2] = 0x58;
//    tile_info_table[172].palette[3] = 0x9A;
//
//    tile_info_table[173].type = TILE_BACKGROUND;
//    tile_info_table[173].colorDepth = BPP_1;
//    tile_info_table[173].bitmap = (unsigned char*)tile1_173;
//    tile_info_table[173].is_mirrored = 0;
//    tile_info_table[173].is_flipped = 0;
//    tile_info_table[173].palette[0] = 0x09;
//
//    tile_info_table[174].type = TILE_BACKGROUND;
//    tile_info_table[174].colorDepth = BPP_3;
//    tile_info_table[174].bitmap = (unsigned char*)tile3_174;
//    tile_info_table[174].is_mirrored = 0;
//    tile_info_table[174].is_flipped = 0;
//    tile_info_table[174].palette[0] = 0x01;
//    tile_info_table[174].palette[1] = 0x25;
//    tile_info_table[174].palette[2] = 0x90;
//
//    tile_info_table[175].type = TILE_BACKGROUND;
//    tile_info_table[175].colorDepth = BPP_2;
//    tile_info_table[175].bitmap = (unsigned char*)tile2_175;
//    tile_info_table[175].is_mirrored = 0;
//    tile_info_table[175].is_flipped = 0;
//    tile_info_table[175].palette[0] = 0x01;
//    tile_info_table[175].palette[1] = 0x90;
//
//    tile_info_table[176].type = TILE_BACKGROUND;
//    tile_info_table[176].colorDepth = BPP_2;
//    tile_info_table[176].bitmap = (unsigned char*)tile2_176;
//    tile_info_table[176].is_mirrored = 0;
//    tile_info_table[176].is_flipped = 0;
//    tile_info_table[176].palette[0] = 0x01;
//    tile_info_table[176].palette[1] = 0x90;
//
//    tile_info_table[177].type = TILE_BACKGROUND;
//    tile_info_table[177].colorDepth = BPP_3;
//    tile_info_table[177].bitmap = (unsigned char*)tile3_177;
//    tile_info_table[177].is_mirrored = 0;
//    tile_info_table[177].is_flipped = 0;
//    tile_info_table[177].palette[0] = 0x01;
//    tile_info_table[177].palette[1] = 0x25;
//    tile_info_table[177].palette[2] = 0x90;
//
//    tile_info_table[178].type = TILE_BACKGROUND;
//    tile_info_table[178].colorDepth = BPP_3;
//    tile_info_table[178].bitmap = (unsigned char*)tile3_178;
//    tile_info_table[178].is_mirrored = 0;
//    tile_info_table[178].is_flipped = 0;
//    tile_info_table[178].palette[0] = 0x01;
//    tile_info_table[178].palette[1] = 0x25;
//    tile_info_table[178].palette[2] = 0x90;
//
//    tile_info_table[179].type = TILE_BACKGROUND;
//    tile_info_table[179].colorDepth = BPP_1;
//    tile_info_table[179].bitmap = (unsigned char*)tile1_179;
//    tile_info_table[179].is_mirrored = 0;
//    tile_info_table[179].is_flipped = 0;
//    tile_info_table[179].palette[0] = 0x09;
//
//    tile_info_table[180].type = TILE_BACKGROUND;
//    tile_info_table[180].colorDepth = BPP_1;
//    tile_info_table[180].bitmap = (unsigned char*)tile1_180;
//    tile_info_table[180].is_mirrored = 0;
//    tile_info_table[180].is_flipped = 0;
//    tile_info_table[180].palette[0] = 0x09;
//
//    tile_info_table[181].type = TILE_BACKGROUND;
//    tile_info_table[181].colorDepth = BPP_1;
//    tile_info_table[181].bitmap = (unsigned char*)tile1_181;
//    tile_info_table[181].is_mirrored = 0;
//    tile_info_table[181].is_flipped = 0;
//    tile_info_table[181].palette[0] = 0x09;
//
//    tile_info_table[182].type = TILE_BACKGROUND;
//    tile_info_table[182].colorDepth = BPP_1;
//    tile_info_table[182].bitmap = (unsigned char*)tile1_182;
//    tile_info_table[182].is_mirrored = 0;
//    tile_info_table[182].is_flipped = 0;
//    tile_info_table[182].palette[0] = 0x09;
//
//    tile_info_table[183].type = TILE_BACKGROUND;
//    tile_info_table[183].colorDepth = BPP_1;
//    tile_info_table[183].bitmap = (unsigned char*)tile1_183;
//    tile_info_table[183].is_mirrored = 0;
//    tile_info_table[183].is_flipped = 0;
//    tile_info_table[183].palette[0] = 0x09;
//
//    tile_info_table[184].type = TILE_BACKGROUND;
//    tile_info_table[184].colorDepth = BPP_1;
//    tile_info_table[184].bitmap = (unsigned char*)tile1_184;
//    tile_info_table[184].is_mirrored = 0;
//    tile_info_table[184].is_flipped = 0;
//    tile_info_table[184].palette[0] = 0x09;


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
