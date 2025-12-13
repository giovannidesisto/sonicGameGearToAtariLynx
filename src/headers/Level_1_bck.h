/*
 * Level_1_bck.h
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */

#ifndef LEVEL_1_BCK_H
#define LEVEL_1_BCK_H
extern unsigned char for_001[];
extern unsigned char for_002[];
extern unsigned char for_003[];
extern unsigned char for_004[];

extern unsigned char bck_001[];


extern unsigned char wall_001[];

//PIATTAFORME, MURI
static unsigned char* LEVEL_1_PLATFORM[] = {
		for_001,
		for_002,
		for_003,
		for_004

};
//ALBERI, FIORI DI SFONDO
static unsigned char* LEVEL_1_BACKGROUND[] = {
		bck_001
};

static unsigned char* LEVEL_1_WALL[] = {
		wall_001
};



#endif /* LEVEL_1_BCK_H */
