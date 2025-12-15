/*
 * Level_1_bck.h
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */

#ifndef ZONE_1_TILES_H
#define ZONE_1_TILES_H

extern unsigned char for_001[];
extern unsigned char for_002[];
extern unsigned char for_003[];


extern unsigned char bck_001[];


extern unsigned char wall_001[];

extern unsigned char prx_001[];
extern unsigned char prx_002[];

//PIATTAFORME, MURI
static unsigned char* LEVEL_1_PLATFORM[] = {
		for_001,
		for_002,
		for_003

};
//ALBERI, FIORI DI SFONDO
static unsigned char* LEVEL_1_BACKGROUND[] = {
		bck_001
};


//TILE SOLIDE, su tutti i 4 lati
static unsigned char* LEVEL_1_WALL[] = {
		wall_001
};


//SFONDO IN PARALLASSE
static unsigned char* LEVEL_1_PRX[] = {
		prx_001,
		prx_002
};


#endif /* ZONE_1_TILES */
