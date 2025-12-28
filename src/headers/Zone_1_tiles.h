/*
 * Level_1_bck.h
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */

#ifndef ZONE_1_TILES_H
#define ZONE_1_TILES_H
#include "SpriteRepo.h"

//PIATTAFORME e muri solidi
static unsigned char* LEVEL_1_PLATFORM[] = {
		for_001,
		for_002,
		for_003,
		for_004
};

//ALBERI, FIORI DI SFONDO
static unsigned char* LEVEL_1_BACKGROUND[] = {
		bck_003,
		bck_002,
		bck_001,
		bck_004,
		bck_005
};


//TILE SOLIDE, su tutti i 4 lati
static unsigned char* LEVEL_1_WALL[] = {
		wall_001,
		wall_002,
		wall_003,
		wall_004
};


//SFONDO IN PARALLASSE
static unsigned char* LEVEL_1_PRX[] = {
		prx_001,
		prx_002
};


#endif /* ZONE_1_TILES */
