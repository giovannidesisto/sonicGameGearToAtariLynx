/* level.c */
#include "Level.h"
#include <tgi.h>
#include "AgCommon.h"
#include "Utils.h"
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
static u8 level_foregound_map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,4,3,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0},
		{3,2,3,2,2,2,1,1,2,2,2,2,2,2,2,1,2,2,1,1,1,2,2},
		{3,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

static u8 level_back_ground_map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0},
		{0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

};


/* Inizializza il sistema di sprite */
void level_init(void) {
	short x;
	short y;
	short j;
	for(y = 0; y < TILES_Y; y++)
		for(x = 0; x < TILES_X; x++)
		{
			FORE_GROUND_SCB_MATRIX[y][x].sprctl0 = BPP_4 | TYPE_BACKGROUND;
			FORE_GROUND_SCB_MATRIX[y][x].sprctl1 = REHV | PACKED;
			FORE_GROUND_SCB_MATRIX[y][x].sprcoll = 0;
			FORE_GROUND_SCB_MATRIX[y][x].next = (void*)0;
			FORE_GROUND_SCB_MATRIX[y][x].data =  &tile_ground[0];
			FORE_GROUND_SCB_MATRIX[y][x].hsize = 0x0100;
			FORE_GROUND_SCB_MATRIX[y][x].vsize = 0x0100;
			FORE_GROUND_SCB_MATRIX[y][x].vpos = 0;// (-5 + (y*TILE_SIZE));
			FORE_GROUND_SCB_MATRIX[y][x].hpos = 0;//(-8 + (x*TILE_SIZE));
			for(j = 0; j < 8; j++) {
				FORE_GROUND_SCB_MATRIX[y][x].penpal[j] = tile_palette[j];
			}



			BACK_GROUND_SCB_MATRIX[y][x].sprctl0 = BPP_4 | TYPE_BACKGROUND;
			BACK_GROUND_SCB_MATRIX[y][x].sprctl1 = REHV | PACKED;
			BACK_GROUND_SCB_MATRIX[y][x].sprcoll = 0;
			BACK_GROUND_SCB_MATRIX[y][x].next = (void*)0;
			BACK_GROUND_SCB_MATRIX[y][x].data =  &tile_ground[0];
			BACK_GROUND_SCB_MATRIX[y][x].hsize = 0x0100;
			BACK_GROUND_SCB_MATRIX[y][x].vsize = 0x0100;
			BACK_GROUND_SCB_MATRIX[y][x].vpos = 0;// (-5 + (y*TILE_SIZE));
			BACK_GROUND_SCB_MATRIX[y][x].hpos = 0;//(-8 + (x*TILE_SIZE));
			for(j = 0; j < 8; j++) {
				BACK_GROUND_SCB_MATRIX[y][x].penpal[j] = tile_palette[j];
			}


		}
}

/* Carica un livello */
void level_load(u8 level_num) {
	u8 x, y;
	/* Per ora carica solo la mappa di default */
	(void)level_num;  /* Ignorato per ora */

	/**
	 * non copio su level gli array già presenti.
	 * ho provato con puntatore 2D o puntatore di puntatore ma non
	 * funziona la lettura del dato
	 */
	/*
	for(y = 0; y < MAP_HEIGHT; y++)
		for(x = 0; x < MAP_WIDTH; x++) {
			{
				level.foregound_map[y][x] = level_foregound_map[y][x];
			}
		}
	*/
	//level.foregound_map =level_foregound_map;
	/* Imposta posizioni di partenza e fine */

	level.start_x =0;
	level.start_y =MAP_HEIGHT * TILE_SIZE;//MAP_HEIGHT * TILE_SIZE ;

	level.end_x =  MAP_WIDTH * TILE_SIZE;
	level.end_y =  MAP_HEIGHT * TILE_SIZE ;


	 /* Inizializza la camera */
	 level_init_camera();

	 /* Posiziona la camera sul player iniziale */
	 level_update_camera( level.start_x, level.start_y);
}



void level_draw() {
    int x, y, j,start_tile_x,start_tile_y,end_tile_x,end_tile_y;

    /* Calcola quale porzione della mappa è visibile */
    start_tile_x = level.camera.x / TILE_SIZE  ;//+ (level.camera.x % TILE_SIZE==0?0:-1);
    start_tile_y =  (level.camera.y / TILE_SIZE) ;


    end_tile_x = start_tile_x + TILES_X; /*  per sicurezza */
    end_tile_y =  start_tile_y + TILES_Y ;

    /* Limita ai bordi della mappa */
    if (end_tile_x > MAP_WIDTH) end_tile_x = MAP_WIDTH;
    if (end_tile_y > MAP_HEIGHT) end_tile_y = MAP_HEIGHT;


	printCoordsToScreen(start_tile_x,start_tile_y,1,30,0x02);
	printCoordsToScreen(end_tile_x,end_tile_y,1,40,0x02);


    for (y = start_tile_y; y < end_tile_y; y++) {
        for (x = start_tile_x; x < end_tile_x; x++) {
            u8 tile_type = level_foregound_map[y][x];//level.foregound_map

            if (tile_type != TILE_EMPTY && tile_data[tile_type] != (void*)0) {
                /* Calcola l'indice nella matrice di sprite */
                int sprite_x = x - start_tile_x;
                int sprite_y =  y - start_tile_y;

                /* Calcola la posizione sullo schermo */
                int world_x = x * TILE_SIZE;
                int world_y = y * TILE_SIZE;

                int screen_x = level_world_to_screen_x( world_x);
                int screen_y = level_world_to_screen_y( world_y);
                //if(screen_y>=-TILE_SIZE)
                {
					/* Imposta i dati dello sprite */
					FORE_GROUND_SCB_MATRIX[sprite_y][sprite_x].data = tile_data[tile_type];
					FORE_GROUND_SCB_MATRIX[sprite_y][sprite_x].hpos =   +screen_x; //-TILE_SIZE -1
					FORE_GROUND_SCB_MATRIX[sprite_y][sprite_x].vpos =   +screen_y; //+TILE_SIZE -1

					/* Disegna lo sprite */
					tgi_sprite(&FORE_GROUND_SCB_MATRIX[sprite_y][sprite_x]);
                }
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

    if(level.camera.x>level_width_px)level.camera.x=0;
    if(level.camera.y>level_height_px)level.camera.y=0;
}

/* Converti coordinate world a screen X */
int level_world_to_screen_x( int world_x) {
    return (int)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
int level_world_to_screen_y( int world_y) {
    return (int)(world_y - level.camera.y);
}
