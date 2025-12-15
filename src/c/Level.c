/* level.c */
#include "Level.h"
#include "Player.h"

extern Level level;
extern Player player;




/* Mappa di esempio (Green Hill Zone style) */
u16 level_foregound_map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,3,2,2,4,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,6,0,0,0,0,0,3,2,0,0,2,4,0,0,0,0,0,0,0},
		{0,  0,0,  2,2,2,2,0,0,0,3,2,0,0,0,0,2,4,0,0,0,0,0,0},
		{0,  5,0,  0,0,0,0,0,0,3,2,0,0,0,0,0,0,2,4,0,0,0,0,0},
		{1,  1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


u16 level_prx_map[MAP_HEIGHT][MAP_WIDTH]={
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,  0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

};


/* Inizializza il sistema di sprite */
void level_init(void) {
	short x;
	short y;
	short j;

	for(y = 0; y < TILES_Y; y++)
		for(x = 0; x < TILES_X; x++)
		{
			SCB_MATRIX[y][x].sprctl0 = BPP_4 | TYPE_NORMAL;
			SCB_MATRIX[y][x].sprctl1 = REHV  | PACKED ;//LITERAL per sprite non compresse, SOLO PIATTAFORME!!! le altre sprite devono essere compresse
			SCB_MATRIX[y][x].sprcoll =  NO_COLLIDE;
			SCB_MATRIX[y][x].next = (void*)0;
			SCB_MATRIX[y][x].data = (void*)0;
			SCB_MATRIX[y][x].hsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_MATRIX[y][x].vsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_MATRIX[y][x].vpos = 0;
			SCB_MATRIX[y][x].hpos = 0;
			for(j = 0; j < 8; j++) {
				SCB_MATRIX[y][x].penpal[j] = tile_palette[j];
			}


		}




	// In Level.c, inizializzala in level_init()
	for(y = 0; y < TILES_Y; y++)
		for(x = 0; x < TILES_X; x++) {
			SCB_PRX_MATRIX[y][x].sprctl0 = BPP_4 | TYPE_NORMAL;
			SCB_PRX_MATRIX[y][x].sprctl1 = REHV  | PACKED ;//LITERAL per sprite non compresse, SOLO PIATTAFORME!!! le altre sprite devono essere compresse
			SCB_PRX_MATRIX[y][x].sprcoll =  NO_COLLIDE;
			SCB_PRX_MATRIX[y][x].next = (void*)0;
			SCB_PRX_MATRIX[y][x].data = (void*)0;
			SCB_PRX_MATRIX[y][x].hsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_PRX_MATRIX[y][x].vsize = 0x0100*SCALE/SCALE_DIVIDER;
			SCB_PRX_MATRIX[y][x].vpos = 0;
			SCB_PRX_MATRIX[y][x].hpos = 0;
			for(j = 0; j < 8; j++) {
				SCB_PRX_MATRIX[y][x].penpal[j] = tile_palette[j];
			}
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
	int x, y, start_tile_x, start_tile_y, end_tile_x, end_tile_y, tile_index,
	parallax_offset_x,parallax_offset_y,prx_start_tile_x ,prx_start_tile_y ;
	int prx_end_tile_x;
	int prx_end_tile_y;
	int sprite_count = 0;
	TileInfo* tile_info;
	prev_sprite = NULL;
	first_sprite = NULL;
	/* Calcola quale porzione della mappa è visibile */
	start_tile_x = level.camera.x / TILE_SIZE;
	start_tile_y = level.camera.y / TILE_SIZE;

	end_tile_x = start_tile_x + TILES_X;
	end_tile_y = start_tile_y + TILES_Y;

	/* Limita ai bordi della mappa */
	if (end_tile_x > MAP_WIDTH) end_tile_x = MAP_WIDTH;
	if (end_tile_y > MAP_HEIGHT) end_tile_y = MAP_HEIGHT;

	// 1. COMINCIA CON LO SFONDO
	agSprBackground.penpal[0] = 0x09;
	first_sprite = &agSprBackground;  // Sfondo è il primo
	prev_sprite = first_sprite;



	//aggiunge le tile dello sfondo che si muove in parallasse orizzontale
	parallax_offset_x = level.camera.x / 2;
	parallax_offset_y = level.camera.y / 2;

	// Calcola quali tile di parallasse sono visibili
	prx_start_tile_x = parallax_offset_x / TILE_SIZE;
	prx_start_tile_y = parallax_offset_y / TILE_SIZE;
	prx_end_tile_x = prx_start_tile_x + TILES_X;
	prx_end_tile_y = prx_start_tile_y + TILES_Y;

	// Limita ai bordi della mappa parallasse
	if (prx_end_tile_x > MAP_WIDTH) prx_end_tile_x = MAP_WIDTH;
	if (prx_end_tile_y > MAP_HEIGHT) prx_end_tile_y = MAP_HEIGHT;

	// Disegna le tile di parallasse
	for (y = prx_start_tile_y; y < prx_end_tile_y; y++) {
		for (x = prx_start_tile_x; x < prx_end_tile_x; x++) {
			tile_index = level_prx_map[y][x];

			if (tile_index != 0) {
				int sprite_x = x - prx_start_tile_x;
				int sprite_y = y - prx_start_tile_y;

				// Posizione mondo della tile (senza effetto parallasse)
				int world_x = x * TILE_SIZE;
				int world_y = y * TILE_SIZE;

				// Posizione schermo con effetto parallasse
				int screen_x = world_x - parallax_offset_x;
				int screen_y = world_y - parallax_offset_y;
				tile_info = tileinfo_get(tile_index);
				// Setup sprite parallasse
				SCB_PRX_MATRIX[sprite_y][sprite_x].data =(unsigned char*)  tile_info->bitmap;//; (unsigned char*) LEVEL_1_PRX[effect_counter< EFFECT_TOGGLE_VALUE==0 ? tile_index-1:tile_index];

				SCB_PRX_MATRIX[sprite_y][sprite_x].next = (void*)0;
				//SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl1 = REHV | PACKED;

				SCB_PRX_MATRIX[sprite_y][sprite_x].vpos = screen_y;
				if(tile_info->is_mirrored)
				{
					SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
					SCB_PRX_MATRIX[sprite_y][sprite_x].hpos = screen_x+TILE_SIZE-1;
				}
				else
				{
					SCB_PRX_MATRIX[sprite_y][sprite_x].hpos = screen_x;
					SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
				}



				// Controlla se la tile è visibile
				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
					continue;  // Tile non visibile
				} else {
					// Aggiungi alla lista
					prev_sprite->next = &SCB_PRX_MATRIX[sprite_y][sprite_x];
					prev_sprite = &SCB_PRX_MATRIX[sprite_y][sprite_x];
					sprite_count++;
				}
			}
		}
	}





	//////////////////////////////////////////////////////////////////////






	// ALBERI,PALME,FIORI
	for (y = start_tile_y; y < end_tile_y; y++) {
		for (x = start_tile_x; x < end_tile_x; x++) {
			tile_index = level_foregound_map[y][x];
			tile_info = tileinfo_get(tile_index);


			if (tile_info->type == TILE_BACKGROUND) {  // Background tiles
				int sprite_x = x - start_tile_x;
				int sprite_y = y - start_tile_y;

				int world_x = x * TILE_SIZE;
				int world_y = y * TILE_SIZE;
				int screen_x = level_world_to_screen_x(world_x);
				int screen_y = level_world_to_screen_y(world_y);


				// Setup sprite background
				SCB_MATRIX[sprite_y][sprite_x].data = (unsigned char*)  tile_info->bitmap;//  LEVEL_1_BACKGROUND[tile_index-100];
				SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
				SCB_MATRIX[sprite_y][sprite_x].next = (void*)0;  // Termina per ora

				SCB_MATRIX[sprite_y][sprite_x].vpos = screen_y;

				if(tile_info->is_mirrored)
				{
					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x+TILE_SIZE-1;
				}
				else
				{
					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
				}



				// Aggiungi in level_draw(), prima di setup sprite:
				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
					continue;  // Tile non visibile, salta
				}else{
					// Aggiungi alla lista
					prev_sprite->next = &SCB_MATRIX[sprite_y][sprite_x];
					prev_sprite = &SCB_MATRIX[sprite_y][sprite_x];
					sprite_count++;
				}
			}
		}
	}


	// PIATTAFORME E MURI
	for (y = start_tile_y; y < end_tile_y; y++) {
		for (x = start_tile_x; x < end_tile_x; x++) {
			tile_index = level_foregound_map[y][x];
			tile_info = tileinfo_get(tile_index);
			if (tile_info->type == TILE_PLATFORM || tile_info->type == TILE_SOLID) {  // Foreground tiles
				int sprite_x = x - start_tile_x;
				int sprite_y = y - start_tile_y;

				int world_x = x * TILE_SIZE;
				int world_y = y * TILE_SIZE;
				int screen_x = level_world_to_screen_x(world_x);
				int screen_y = level_world_to_screen_y(world_y);

				// Setup sprite background
				SCB_MATRIX[sprite_y][sprite_x].data = (unsigned char*)  tile_info->bitmap;//  LEVEL_1_BACKGROUND[tile_index-100];
				SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
				SCB_MATRIX[sprite_y][sprite_x].next = (void*)0;  // Termina per ora

				SCB_MATRIX[sprite_y][sprite_x].vpos = screen_y;
				if(tile_info->is_mirrored)
				{
					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x+TILE_SIZE-1;
				}
				else
				{
					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
				}




				// Aggiungi in level_draw(), prima di setup sprite:
				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
					continue;  // Tile non visibile, salta
				}
				else{
					// Aggiungi alla lista
					prev_sprite->next = &SCB_MATRIX[sprite_y][sprite_x];
					prev_sprite = &SCB_MATRIX[sprite_y][sprite_x];
					sprite_count++;
				}
			}
		}
	}

	// 4. AGGIUNGI IL PLAYER (ULTIMO, SOPRA TUTTO)
	player.visible_spc.sprite.next = (void*)0;
	prev_sprite->next = &player.visible_spc.sprite;

	// 5. DISEGNA TUTTA LA LISTA CON UNA SOLA CHIAMATA
	tgi_sprite(first_sprite);
	sprite_count++;
	// Debug: mostra quanti sprite stai disegnando
	printU16(sprite_count, 00, 0, 0x04);
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
	center_y = (level.camera.height / 2);

	/* Segui il player mantenendolo al centro */
	level.camera.x = player_x - center_x;
	level.camera.y =  player_y  - center_y ;

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

	player.visible_spc.sprite.hpos = level_world_to_screen_x(player.x);
	player.visible_spc.sprite.vpos = level_world_to_screen_y(player.y);

}

/* Converti coordinate world a screen X */
int level_world_to_screen_x( int world_x) {
	return (int)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
int level_world_to_screen_y( int world_y) {
	return (int)(world_y - level.camera.y);
}



