/* level.c */
#include "Level.h"
#include <tgi.h>
#include "AgCommon.h"
extern Level level;

/* Informazioni sui tipi di tile */
static TileInfo tile_info[TILE_MAX_TYPES] = {
		/* TILE_EMPTY */
		{0, 0, 0, 0},
		/* TILE_GROUND - solido */
		{1, 0, 0, 0},
		/* TILE_GRASS - solido */
		{1, 0, 0, 1},
		/* TILE_DIRT - solido */
		{1, 0, 0, 2},
		/* TILE_SPIKE - pericoloso */
		{0, 1, 0, 3},
		/* TILE_SPRING - non solido */
		{0, 0, 0, 4},
		/* TILE_RING - collezionabile */
		{0, 0, 1, 5},
		/* TILE_CHECKPOINT */
		{0, 0, 0, 6},
		/* TILE_WATER */
		{0, 0, 0, 7}
};

/* Array di puntatori ai dati grafici */
static char* tile_data[TILE_MAX_TYPES] = {
		(void*) 0,
		tile_ground,
		tile_grass,
		tile_dirt,
		tile_spike,
		tile_spring,
		tile_ring,
		(void*) 0,  /* Checkpoint - da definire */
		(void*) 0   /* Water - da definire */
};

/* Mappa di esempio (Green Hill Zone style) */
static u8 default_map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

};

/* Inizializza il sistema di sprite */
void level_init(void) {
	short x;
	short y;
	short j;
	for(y = 0; y < TILES_Y; y++)
		for(x = 0; x < TILES_X; x++)
		{
			sprFontMatrix[y][x].sprctl0 = BPP_4 | TYPE_BACKGROUND;
			sprFontMatrix[y][x].sprctl1 = REHV | PACKED;
			sprFontMatrix[y][x].sprcoll = 0;
			sprFontMatrix[y][x].next = (void*)0;
			sprFontMatrix[y][x].data =  &tile_ground[0];
			sprFontMatrix[y][x].hsize = 0x0100;
			sprFontMatrix[y][x].vsize = 0x0100;
			sprFontMatrix[y][x].vpos = 0;// (-5 + (y*TILE_SIZE));
			sprFontMatrix[y][x].hpos = 0;//(-8 + (x*TILE_SIZE));
			for(j = 0; j < 8; j++) {
				sprFontMatrix[y][x].penpal[j] = tile_palette[j];
			}


		}
}

/* Carica un livello */
void level_load(u8 level_num) {
	u8 x, y;
	/* Per ora carica solo la mappa di default */
	(void)level_num;  /* Ignorato per ora */
	for(y = 0; y < MAP_HEIGHT; y++)
		for(x = 0; x < MAP_WIDTH; x++) {
			{
				level.map[y][x] = default_map[y][x];
			}
		}
	/* Imposta posizioni di partenza e fine */

	level.start_x =00;
	level.start_y =MAP_HEIGHT * TILE_SIZE - TILE_SIZE;

	level.end_x =  MAP_WIDTH * TILE_SIZE;
	level.end_y = MAP_HEIGHT * TILE_SIZE;


	 /* Inizializza la camera */
	    level_init_camera();

	    /* Posiziona la camera sul player iniziale */
	    level_update_camera( level.start_x, level.start_y);
}



void level_draw() {
    u8 x, y, j;

    /* Calcola quale porzione della mappa è visibile */
    u8 start_tile_x = level.camera.x / TILE_SIZE;
    u8 start_tile_y =  (level.camera.y / TILE_SIZE);
    u8 end_tile_x = start_tile_x + TILES_X ; /* +1 per sicurezza */
    u8 end_tile_y = start_tile_y + TILES_Y ;

    /* Limita ai bordi della mappa */
    if (end_tile_x > MAP_WIDTH) end_tile_x = MAP_WIDTH;
    if (end_tile_y > MAP_HEIGHT) end_tile_y = MAP_HEIGHT;

    for (y = start_tile_y; y < end_tile_y; y++) {
        for (x = start_tile_x; x < end_tile_x; x++) {
            u8 tile_type = level.map[y][x];

            if (tile_type != TILE_EMPTY && tile_data[tile_type] != (void*)0) {
                /* Calcola l'indice nella matrice di sprite */
                u8 sprite_x = x - start_tile_x;
                u8 sprite_y =  y - start_tile_y;

                /* Calcola la posizione sullo schermo */
                u16 world_x = x * TILE_SIZE;
                u16 world_y = y * TILE_SIZE;

                u8 screen_x = level_world_to_screen_x( world_x);
                u8 screen_y = level_world_to_screen_y( world_y);

                /* Imposta i dati dello sprite */
                sprFontMatrix[sprite_y][sprite_x].data = tile_data[tile_type];
                sprFontMatrix[sprite_y][sprite_x].hpos =  -TILE_SIZE  +screen_x;
                sprFontMatrix[sprite_y][sprite_x].vpos =              +screen_y;

                /* Disegna lo sprite */
                tgi_sprite(&sprFontMatrix[sprite_y][sprite_x]);
            }
        }
    }
}

/* Controlla collisioni (versione semplificata) */
u8 level_check_collision( u16 x, u16 y, u8 width, u8 height) {

	//limit destro dello schermo
	//if(x + width > level.end_x - width)return 1;
	//inizio del livello
	//if(x < 80)return 1;
	return 0;

}

void level_init_camera() {
    level.camera.x = 0;
    level.camera.y = 0;
    level.camera.width = SCREEN_WIDTH;
    level.camera.height = SCREEN_HEIGHT;
}



/* Aggiorna la posizione della camera per seguire il player */
void level_update_camera( u16 player_x, u16 player_y) {
    u16 center_x,center_y,level_width_px,level_height_px;

	/* Calcola il centro dello schermo */
     center_x = level.camera.width / 2;
     center_y = level.camera.height / 2;

    /* Segui il player mantenendolo al centro */
    level.camera.x = player_x - center_x;
    level.camera.y = player_y - center_y;

    /* Limita la camera ai bordi del livello */
     level_width_px =  MAP_WIDTH * TILE_SIZE;
     level_height_px = MAP_HEIGHT * TILE_SIZE;

    /* Limiti orizzontali */
    if (level.camera.x < 0) {
        level.camera.x = 0;
    }
    if (level.camera.x + level.camera.width > level_width_px) {
        level.camera.x = level_width_px - level.camera.width;
    }

    /* Limiti verticali */
    if (level.camera.y < 0) {
        level.camera.y = 0;
    }
    if (level.camera.y + level.camera.height > level_height_px) {
        level.camera.y = level_height_px - level.camera.height;
    }
}

/* Converti coordinate world a screen X */
u8 level_world_to_screen_x( u16 world_x) {
    return (u8)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
u8 level_world_to_screen_y( u16 world_y) {
    return (u8)(world_y - level.camera.y);
}
