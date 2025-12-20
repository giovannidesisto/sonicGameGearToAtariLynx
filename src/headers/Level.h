/* level.h */
#ifndef LEVEL_H
#define LEVEL_H

#include "Utils.h"
#include "Zone_1_tiles.h"
#include "maps/Level1_maps.h"


/* Definizioni costanti */


/*
numero di sprite necessarie per disegnare il livello
sbordano di 8 pixel per lato orizzontalmente e di 5 pixel per lato verticalmente,
nell'ottica di gestire in continuità l'aggiornamento della capera che insegue il player
non solo durante lo scorrimeto verticale del gioco ma anche durante lo scorrimento verticale (es se sale
su piattaforme che lo portano in alto)
 */


//extern u16 FG_MAP[MAP_HEIGHT][MAP_WIDTH];

//extern u16 BCKG_MAP[MAP_HEIGHT][MAP_WIDTH];

//static SCB_REHV_PAL SCB_MATRIX[TILES_Y][TILES_X];
//static SCB_REHV_PAL SCB_PRX_MATRIX[TILES_Y][TILES_X];
/*
typedef enum {
    TPVP_UP = 2,
	TPVP_DOWN = 3,
	TPVP_SIDE = 4,
	TPVP_SAME_DESC=5, //gestione discese
	TPVP_SAME_ASC=6 //gestione salite
} TailPositionVsPlayer;
 */




typedef struct {
	u8 center_x;
	u8 center_y;
	u16 x;        /* Posizione X della camera (in pixel world) */
	u16 y;        /* Posizione Y della camera (in pixel world) */
	u8 width;    /* Larghezza della viewport (solitamente SCREEN_WIDTH) */
	u8 height;   /* Altezza della viewport (solitamente SCREEN_HEIGHT) */
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
	u8* fg_map;
	u8* bg_map;

	/* Limita la camera ai bordi del livello */
	u16 level_width_px; //=  level.map_w * TILE_SIZE;
	u16 level_height_px ;//= level.map_h * TILE_SIZE;


	Camera camera;                            /* Camera per lo scroll */
} Level;






/* Dichiarazioni pubbliche */
void level_init(void);
void level_load( u8 level_num);
void level_draw();


void level_init_camera();
void level_update_camera();
int level_world_to_screen_x (int world_x);
int level_world_to_screen_y( int world_y);


#endif /* LEVEL_H */
