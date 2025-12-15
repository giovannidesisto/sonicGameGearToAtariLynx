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

//extern unsigned char tile1_100[];
//extern unsigned char tile1_129[];
//extern unsigned char tile1_143[];
//extern unsigned char tile1_158[];
//extern unsigned char tile1_173[];
//extern unsigned char tile1_179[];
//extern unsigned char tile1_180[];
//extern unsigned char tile1_181[];
//extern unsigned char tile1_182[];
//extern unsigned char tile1_183[];
//extern unsigned char tile1_184[];
//extern unsigned char tile2_101[];
//extern unsigned char tile2_102[];
//extern unsigned char tile2_103[];
//extern unsigned char tile2_104[];
//extern unsigned char tile2_105[];
//extern unsigned char tile2_106[];
//extern unsigned char tile2_107[];
//extern unsigned char tile2_108[];
//extern unsigned char tile2_109[];
//extern unsigned char tile2_110[];
//extern unsigned char tile2_111[];
//extern unsigned char tile2_112[];
//extern unsigned char tile2_113[];
//extern unsigned char tile2_117[];
//extern unsigned char tile2_119[];
//extern unsigned char tile2_125[];
//extern unsigned char tile2_126[];
//extern unsigned char tile2_127[];
//extern unsigned char tile2_128[];
//extern unsigned char tile2_130[];
//extern unsigned char tile2_133[];
//extern unsigned char tile2_135[];
//extern unsigned char tile2_136[];
//extern unsigned char tile2_137[];
//extern unsigned char tile2_138[];
//extern unsigned char tile2_139[];
//extern unsigned char tile2_140[];
//extern unsigned char tile2_141[];
//extern unsigned char tile2_142[];
//extern unsigned char tile2_144[];
//extern unsigned char tile2_145[];
//extern unsigned char tile2_146[];
//extern unsigned char tile2_147[];
//extern unsigned char tile2_148[];
//extern unsigned char tile2_151[];
//extern unsigned char tile2_156[];
//extern unsigned char tile2_159[];
//extern unsigned char tile2_160[];
//extern unsigned char tile2_161[];
//extern unsigned char tile2_167[];
//extern unsigned char tile2_175[];
//extern unsigned char tile2_176[];
//extern unsigned char tile3_114[];
//extern unsigned char tile3_115[];
//extern unsigned char tile3_116[];
//extern unsigned char tile3_118[];
//extern unsigned char tile3_120[];
//extern unsigned char tile3_121[];
//extern unsigned char tile3_122[];
//extern unsigned char tile3_123[];
//extern unsigned char tile3_124[];
//extern unsigned char tile3_131[];
//extern unsigned char tile3_132[];
//extern unsigned char tile3_134[];
//extern unsigned char tile3_149[];
//extern unsigned char tile3_150[];
//extern unsigned char tile3_152[];
//extern unsigned char tile3_153[];
//extern unsigned char tile3_154[];
//extern unsigned char tile3_155[];
//extern unsigned char tile3_157[];
//extern unsigned char tile3_162[];
//extern unsigned char tile3_163[];
//extern unsigned char tile3_164[];
//extern unsigned char tile3_165[];
//extern unsigned char tile3_166[];
//extern unsigned char tile3_168[];
//extern unsigned char tile3_169[];
//extern unsigned char tile3_170[];
//extern unsigned char tile3_171[];
//extern unsigned char tile3_172[];
//extern unsigned char tile3_174[];
//extern unsigned char tile3_177[];
//extern unsigned char tile3_178[];

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
