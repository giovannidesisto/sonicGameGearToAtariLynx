/* level.c */
#include "Level.h"
#include "Player.h"

extern Level level;
extern Player player;

/* Mappa di esempio (Green Hill Zone style) */
static int level_foregound_map[MAP_HEIGHT][MAP_WIDTH] = {
		//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		/*
		{0,0,00,0,0,0,0,0,0,0,00,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,00,0,0,0,0,0,0,0,00,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,00,0,0,0,0,0,0,0,00,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,00,0,0,0,0,0,0,0,00,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,00,0,0,0,0,0,0,0,8,9,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,00,0,0,0,0,0,0,0,7,10,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0,0,06,0,0,0,0,0,0,4,0,0,0,0,0},
		{1,1,1,1,1,1,2,2,2,2,05,2,2,2,2,2,2,3,2,2,2,2,2},
		 */
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}


};






/* Inizializza il sistema di sprite */
void level_init(void) {
	short x;
	short y;
	short j;
	//short s=1;
	for(y = 0; y < TILES_Y; y++)
		for(x = 0; x < TILES_X; x++)
		{
			SCB_MATRIX[y][x].sprctl0 = BPP_4 | TYPE_NORMAL;
			SCB_MATRIX[y][x].sprctl1 = REHV  ;//| PACKED
			SCB_MATRIX[y][x].sprcoll =  0x00;// 32;
			SCB_MATRIX[y][x].next = (void*)0;
			SCB_MATRIX[y][x].data = (void*)0;// &LEVEL_1_FOREGROUND_TILES[0];
			SCB_MATRIX[y][x].hsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_MATRIX[y][x].vsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_MATRIX[y][x].vpos = 0;// (-5 + (y*TILE_SIZE));
			SCB_MATRIX[y][x].hpos = 0;//(-8 + (x*TILE_SIZE));
			for(j = 0; j < 8; j++) {
				SCB_MATRIX[y][x].penpal[j] = tile_palette[j];
			}

			//s++;
		}








}

/* Carica un livello */
void level_load(u8 level_num) {
	u8 x, y;
	/* Per ora carica solo la mappa di default */
	(void)level_num;  /* Ignorato per ora */


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
	int x, y, j,start_tile_x,start_tile_y,end_tile_x,end_tile_y,tile_index;

	/* Calcola quale porzione della mappa è visibile */
	start_tile_x = level.camera.x / TILE_SIZE  ;//+ (level.camera.x % TILE_SIZE==0?0:-1);
	start_tile_y =  (level.camera.y / TILE_SIZE) ;


	end_tile_x = start_tile_x + TILES_X; /*  per sicurezza */
	end_tile_y =  start_tile_y + TILES_Y ;

	/* Limita ai bordi della mappa */
	if (end_tile_x > MAP_WIDTH) end_tile_x = MAP_WIDTH;
	if (end_tile_y > MAP_HEIGHT) end_tile_y = MAP_HEIGHT;


	//printCoordsToScreen(start_tile_x,start_tile_y,1,30,0x02);
	//printCoordsToScreen(end_tile_x,end_tile_y,1,40,0x02);


	for (y = start_tile_y; y < end_tile_y; y++) {
		for (x = start_tile_x; x < end_tile_x; x++) {



			tile_index = level_foregound_map[y][x];//level.foregound_map

			if (tile_index != 0) {
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

					//indici 0-99 tail collidable
					//100-199 non collidable, es nuvole
					if(tile_index<100)
					{
						SCB_MATRIX[sprite_y][sprite_x].data =   (unsigned char*) LEVEL_1_FOREGROUND_TILES[tile_index-1];
						//la tail è sopra il player
						if(player.y<=world_y)SCB_MATRIX[sprite_y][sprite_x].sprcoll=  TPVP_DOWN;
						else
							if(player.y+TILE_SIZE>=world_y)SCB_MATRIX[sprite_y][sprite_x].sprcoll=  TPVP_UP;
						/**
						 * probabilmente occorrerà gestire anche le casistiche combinate
						 * tipo tocco tail basso+lato etc
						 *
						 * attenzione alle cordinate del player, la sua x è centrale per
						 * gestire il cambio direzione
						 */
						else
							if(player.x+TILE_SIZE/2<world_x  || world_x<player.x-TILE_SIZE/2 )SCB_MATRIX[sprite_y][sprite_x].sprcoll=  TPVP_SIDE;

					}
					else
				    {
						SCB_MATRIX[sprite_y][sprite_x].data =   (unsigned char*) LEVEL_1_BACKGROUND_TILES[100-tile_index];
						SCB_MATRIX[sprite_y][sprite_x].sprcoll=  NO_COLLIDE;
				    }


					SCB_MATRIX[sprite_y][sprite_x].hpos =   +screen_x; //-TILE_SIZE -1
					SCB_MATRIX[sprite_y][sprite_x].vpos =   +screen_y; //+TILE_SIZE -1

					/* Disegna lo sprite */
					tgi_sprite(&SCB_MATRIX[sprite_y][sprite_x]);
					//printU8As2Nibble(FORE_GROUND_SCB_MATRIX[sprite_y][sprite_x].sprcoll,screen_x,screen_y,1);
				}
			}
		}
	}
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


	/* Imposta la posizione dello sprite del player */
	if(player.collision.collision_frame_delay == 0){
		player.visible_spc.sprite.hpos = level_world_to_screen_x(player.x);
		player.visible_spc.sprite.vpos = level_world_to_screen_y(player.y);
		player.collision.collision_frame_delay=1;
		//player.collision.collision_frame_delay = 1;
	}
	//player.ghost_spc.sprite.hpos = level_world_to_screen_x(player.x);
	//player.ghost_spc.sprite.vpos = level_world_to_screen_y(player.y);
}

/* Converti coordinate world a screen X */
int level_world_to_screen_x( int world_x) {
	return (int)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
int level_world_to_screen_y( int world_y) {
	return (int)(world_y - level.camera.y);
}

