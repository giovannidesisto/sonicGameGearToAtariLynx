/* level.h */
#ifndef LEVEL_H
#define LEVEL_H

#include "Utils.h"
#include "Zone_1_tiles.h"
#include "maps/Level1_maps.h"


//#define MAP_BUF_PAD 4
//#define MAP_BUF_PAD_HALF MAP_BUF_PAD/2
//#define MAP_BUF_W      TILES_X+MAP_BUF_PAD   // sbordo dx/sx



static u8 map_buf[MAP_BUF_H][MAP_BUF_W]={0};
static SCB_REHV_PAL sprite_pool[MAX_ACTIVE_SPRITES];
static u8 sprite_used[MAX_ACTIVE_SPRITES];


//typedef struct {
//    const u8 *ptr;   // puntatore nello stream RLE
//    u16 next_col;         // colonna raggiunta (esclusiva)
//    u8 value;        // valore corrente
//} RLE_RowCache;


//typedef struct {
//	u16 prev_tile_x;
//	u16 prev_tile_y;
//	u8 center_x;
//	u8 center_y;
//	s16 x;        /* Posizione X della camera (in pixel world) */
//	s16 y;        /* Posizione Y della camera (in pixel world) */
//	u8 width;    /* Larghezza della viewport (solitamente SCREEN_WIDTH) */
//	u8 height;   /* Altezza della viewport (solitamente SCREEN_HEIGHT) */
//} Camera;


/* Struttura Camera estesa */
typedef struct {
    u16 prev_tile_x;
    u16 prev_tile_y;

    u8 center_x;
    u8 center_y;
    s16 x;                   /* Posizione X della camera (in pixel world) */
    s16 y;                   /* Posizione Y della camera (in pixel world) */

    u8 width;               /* Larghezza della viewport */
    u8 height;              /* Altezza della viewport */

    /* Nuovi campi per dead zone e inerzia */
    u8 dead_zone_left;      /* Dead zone sinistra dal centro */
    u8 dead_zone_right;     /* Dead zone destra dal centro */
    s8 inertia_x;           /* Inerzia orizzontale (-127 a +127) */
    u8 inertia_counter;     /* Contatore per l'inerzia */
    u8 max_inertia_pixels;  /* Pixel massimi di inerzia (default 16) */
    u8 last_player_dir;     /* Ultima direzione del player (0=fermo, 1=dx, 2=sx) */
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
	u8* fg_map; //punta alla porzione decompressa di mappa
	u8* bg_map;

	/* Limita la camera ai bordi del livello */
	u16 level_width_px; //=  level.map_w * TILE_SIZE;
	u16 level_height_px ;//= level.map_h * TILE_SIZE;

	s16 map_buf_origin_x;
	s16 map_buf_origin_y;
	u8 map_buf_col0;
	u8 map_buf_row0;

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
//u8 level_get_tile(u16 world_x, u16 world_y);
u8 level_get_tile_abs(u16 world_x, u16 world_y);
#endif /* LEVEL_H */
