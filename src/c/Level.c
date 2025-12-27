/* level.c */
#include "Level.h"
#include "Player.h"



extern Level level;
extern Player player;





static inline void rle_rowcache_reset(
		RLE_RowCache *c,
		const u8 *data,
		u16 offset
) {
	c->ptr = data + offset;
	c->next_col = 0;
	c->value = 0;
}


static inline u8 rle_rowcache_get(
		RLE_RowCache *c,
		u16 col
) {
	while (col >= c->next_col) {
		c->value = *c->ptr++;
		c->next_col += *c->ptr++;
	}
	return c->value;
}


/* Prende uno sprite libero dal pool */
static SCB_REHV_PAL* get_free_sprite(void) {
	u8 i;
	for(i = 0; i < MAX_ACTIVE_SPRITES; i++) {
		if(!sprite_used[i]) {
			sprite_used[i] = 1;
			return &sprite_pool[i];
		}
	}
	return NULL;  /* Nessuno sprite libero */
}

/* Rilascia tutti gli sprite (chiamare all'inizio di ogni frame) */
static void release_all_sprites(void) {
	u8 i;
	for(i = 0; i < MAX_ACTIVE_SPRITES; i++) {
		sprite_used[i] = 0;
	}
}


u8 level_get_tile_abs(u16 tile_x, u16 tile_y)
{
	s16 bx = tile_x - level.map_buf_origin_x;
	s16 by = tile_y - level.map_buf_origin_y;

	if ((u16)bx >= MAP_BUF_W || (u16)by >= MAP_BUF_H)
		return TILE_EMPTY;

	// applica circolarità
	bx += level.map_buf_col0;
	if (bx >= MAP_BUF_W) bx -= MAP_BUF_W;

	by += level.map_buf_row0;
	if (by >= MAP_BUF_H) by -= MAP_BUF_H;

	return map_buf[by][bx];
}




static inline u8 decompress_rle_get_value(
		const u8 *data,
		u16 offset,
		u16 col_idx
) {
	const u8 *p = data + offset;
	u16 col = 0;

	for (;;) {
		u8 val = *p++;
		u8 cnt = *p++;

		col += cnt;
		if (col > col_idx)
			return val;
	}
}



static inline void level_init_map_streaming(void)
{
	u8 i, y;
	u16 start_tile_x = level.camera.x >> TILE_SHIFT;/// TILE_SIZE;

	/* sbordo simmetrico */
	level.map_buf_origin_x = start_tile_x - ((MAP_BUF_W - TILES_X)  >>1);
	if ((s16)level.map_buf_origin_x < 0)
		level.map_buf_origin_x = 0;

	level.map_buf_col0 = 0;

	for (i = 0; i < MAP_BUF_W; i++) {
		u16 world_col = level.map_buf_origin_x + i; // colonna del mondo

		for (y = 0; y < MAP_BUF_H; y++) {
			u16 world_row =  y;// se scroll verticaley; // riga del mondo (aggiusta con map_buf_origin_y se scroll verticale)

			if (world_row < level.map_h && world_col < level.map_w) {
				map_buf[y][i] = decompress_rle_get_value(
						Z1L1_FG_MAP_RLE_DATA,
						Z1L1_FG_MAP_RLE_OFFS[world_row],
						world_col
				);
			} else {
				map_buf[y][i] = TILE_EMPTY;
			}
		}
	}
}


static inline void stream_scroll_right(void)
{
	u8 write_col;
	u8 y;
	u16 new_world_col;
	u16 world_row;
	u8 by;

	/* Avanza origine mondo */
	level.map_buf_origin_x++;

	/* Avanza colonna circolare */
	level.map_buf_col0 = (level.map_buf_col0 + 1) & MAP_BUF_MASK;

	/* Colonna fisica da scrivere */
	write_col = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;

	/* Colonna mondo da caricare */
	new_world_col = level.map_buf_origin_x + MAP_BUF_W - 1;

	if (new_world_col >= level.map_w)
		return;

	for (y = 0; y < MAP_BUF_H; y++) {

		world_row = level.map_buf_origin_y + y;
		if (world_row >= level.map_h) {
			by = (level.map_buf_row0 + y) & MAP_BUF_MASK;
			map_buf[by][write_col] = TILE_EMPTY;
			continue;
		}

		by = (level.map_buf_row0 + y) & MAP_BUF_MASK;

		map_buf[by][write_col] = decompress_rle_get_value(
				Z1L1_FG_MAP_RLE_DATA,
				Z1L1_FG_MAP_RLE_OFFS[world_row],
				new_world_col
		);
	}
}



//static RLE_RowCache cache[MAP_BUF_H];
//static inline void stream_scroll_right(void)
//{
//    u8 write_col, y;
//    u16 new_world_col;
//    u16 world_row;
//
//
//    level.map_buf_origin_x++;
//    level.map_buf_col0 = (level.map_buf_col0 + 1) & MAP_BUF_MASK;
//
//    write_col = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;
//    new_world_col = level.map_buf_origin_x + MAP_BUF_W - 1;
//
//    if (new_world_col >= level.map_w)
//        return;
//
//    /* RESET CACHE PER QUESTA COLONNA */
//    for (y = 0; y < MAP_BUF_H; y++) {
//        world_row = level.map_buf_origin_y + y;
//        rle_rowcache_reset(
//            &cache[y],
//            Z1L1_FG_MAP_RLE_DATA,
//            Z1L1_FG_MAP_RLE_OFFS[world_row]
//        );
//    }
//
//    /* FETCH */
//    for (y = 0; y < MAP_BUF_H; y++) {
//        map_buf[y][write_col] =
//            rle_rowcache_get(&cache[y], new_world_col);
//    }
//}


static inline void stream_scroll_left(void)
{
	u8 write_col;
	u8 y;
	u16 new_world_col;

	/* Retrocede origine mondo */
	if (level.map_buf_origin_x == 0)
		return;

	level.map_buf_origin_x--;
	level.map_buf_col0 = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;

	/* Colonna fisica da scrivere (prima logica) */
	write_col = level.map_buf_col0;
	new_world_col = level.map_buf_origin_x;

	for (y = 0; y < MAP_BUF_H; y++) {

		u16 world_row = level.map_buf_origin_y + y;
		u8  phys_row  = (y + level.map_buf_row0) & MAP_BUF_MASK;

		if (world_row < level.map_h) {
			map_buf[phys_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							new_world_col
					);
		} else {
			map_buf[phys_row][write_col] = TILE_EMPTY;
		}
	}
}




static inline void stream_scroll_up(void)
{
	u8 x, write_row, write_col;
	u16 world_row, world_col;

	if (level.map_buf_origin_y == 0)
		return;

	level.map_buf_origin_y--;
	level.map_buf_row0 = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;

	write_row = level.map_buf_row0;
	world_row = level.map_buf_origin_y;

	for (x = 0; x < MAP_BUF_W; x++) {
		world_col = level.map_buf_origin_x + x;
		write_col = (x + level.map_buf_col0) & MAP_BUF_MASK;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							world_col
					);
		} else {
			map_buf[write_row][write_col] = TILE_EMPTY;
		}
	}
}




static inline void stream_scroll_down(void)
{
	u8 x, write_row, write_col;
	u16 world_row, world_col;

	level.map_buf_origin_y++;
	level.map_buf_row0 = (level.map_buf_row0 + 1) & MAP_BUF_MASK;

	write_row = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;
	world_row = level.map_buf_origin_y + MAP_BUF_H - 1;

	for (x = 0; x < MAP_BUF_W; x++) {
		world_col = level.map_buf_origin_x + x;
		write_col = (x + level.map_buf_col0) & MAP_BUF_MASK;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							world_col
					);
		} else {
			map_buf[write_row][write_col] = TILE_EMPTY;
		}
	}
}

/* Inizializza il sistema di sprite */
void level_init(void) {

	u8 i,y;

	lynx_load(1);//livello
	lynx_load(2);//player
	lynx_load(3);//sfondo
	lynx_load(4);//background




	//identifica le coordinate x/y mondo nel buffer attuale
	level.map_buf_origin_x = 0;
	level.map_buf_origin_y = 0;


	level.fg_map = map_buf;
	level.camera.prev_tile_x=0;
	level.camera.prev_tile_y=0;
	level.map_h= MAP_HEIGHT;
	level.map_w= MAP_WIDTH;

	level.start_x =0;
	level.start_y =level.map_h * TILE_SIZE;//level.map_h * TILE_SIZE ;

	level.end_x =  level.map_w * TILE_SIZE;
	level.end_y =  level.map_h * TILE_SIZE ;




	/* Limita la camera ai bordi del livello */
	level.level_width_px =  level.map_w * TILE_SIZE;
	level.level_height_px = level.map_h * TILE_SIZE;

	level_init_map_streaming();

	/* Inizializza il pool di sprite */
	for(i = 0; i < MAX_ACTIVE_SPRITES; i++) {
		sprite_used[i] = 0;  /* Tutti liberi */

		/* Imposta valori di default */
		sprite_pool[i].sprctl0 = BPP_4 | TYPE_BACKGROUND;
		sprite_pool[i].sprctl1 = REHV | PACKED;
		sprite_pool[i].sprcoll = 0;
		sprite_pool[i].next = (void*)0;
		sprite_pool[i].hsize = 0x0100;
		sprite_pool[i].vsize = 0x0100;

		/* Copia la palette */
		{
			u8 j;
			for(j = 0; j < 8; j++) {
				sprite_pool[i].penpal[j] = tile_palette[j];
			}
		}
	}


}

/* Carica un livello */
void level_load(u8 level_num) {
	u8 x, y;
	/* Per ora carica solo la mappa di default */
	(void)level_num;  /* Ignorato per ora */




	level_init();

	/* Inizializza la camera */
	level_init_camera();

	/* Posiziona la camera sul player iniziale */
	level_update_camera( );


}






void level_draw() {
	u16 x, y, start_tile_x, start_tile_y, end_tile_x, end_tile_y, tile_index,dx,dy,ty,tx;


	SCB_REHV_PAL* sprite;
	SCB_REHV_PAL* prev_sprite ;
	SCB_REHV_PAL* first_sprite;
	TileInfo* tile_info;

	release_all_sprites();

	// Calcola la porzione visibile
	start_tile_x = level.camera.x >> TILE_SHIFT;//  /TILE_SIZE;
	start_tile_y = level.camera.y >> TILE_SHIFT; // TILE_SIZE;

	end_tile_x   = start_tile_x + TILES_X;
	end_tile_y   = start_tile_y + TILES_Y;

	// Limita ai bordi della mappa
	if(end_tile_x > level.map_w) end_tile_x = level.map_w;
	if(end_tile_y > level.map_h) end_tile_y = level.map_h;

	// Aggiornamento dinamico colonne se la camera si è mossa
	dx = start_tile_x - level.camera.prev_tile_x;
	dy = start_tile_y - level.camera.prev_tile_y;


	ty = level.map_buf_origin_y;
	tx = level.map_buf_origin_x;

	while (start_tile_x > level.map_buf_origin_x )//+ MAP_SBORDA_X)
		stream_scroll_right();

	while (start_tile_x < level.map_buf_origin_x )//+ MAP_SBORDA_X)
		stream_scroll_left();

	while (start_tile_y > level.map_buf_origin_y)
		stream_scroll_down();

	while (start_tile_y  < level.map_buf_origin_y )
		stream_scroll_up();

	level.camera.prev_tile_y = start_tile_y;
	level.camera.prev_tile_x = start_tile_x;


	// Inizia con lo sfondo
	agSprBackground.penpal[0] = 0x09;
	prev_sprite = &agSprBackground;
	first_sprite = prev_sprite;

	// Render foreground tiles visibili
	for(y = start_tile_y; y < end_tile_y; y++) {
		for(x = start_tile_x; x < end_tile_x; x++) {
			tile_index =   level_get_tile_abs(x,y);//level.fg_map[y*MAP_BUF_W + x]; // accesso row-major
			tile_info = tileinfo_get(tile_index);

			if(tile_info->type != TILE_EMPTY) {
				int world_x = x * TILE_SIZE;
				int world_y = y * TILE_SIZE;
				int screen_x = world_x - level.camera.x;
				int screen_y = world_y - level.camera.y;

				sprite = get_free_sprite();
				if(!sprite) return;

				sprite->data = (unsigned char*) tile_info->bitmap;
				sprite->vpos = screen_y;
				if(tile_info->is_mirrored) {
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL | HFLIP;
					sprite->hpos = screen_x + TILE_SIZE - 1;
				} else {
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL;
					sprite->hpos = screen_x;
				}

				// Aggiungi alla lista sprite
				prev_sprite->next = sprite;
				prev_sprite = sprite;
			}
		}
	}

	// Aggiungi il player sopra tutto
	player.visible_spc.sprite.next = NULL;
	prev_sprite->next = &player.visible_spc.sprite;

	// Disegna tutta la lista
	tgi_sprite(first_sprite);

	//	printCoordsToScreen(level.map_buf_origin_x,level.map_buf_origin_y,0,0,0x0F);
	//	printCoordsToScreen(start_tile_x,start_tile_y,0,20,0x0D);
	//printCoordsToScreen(player.x,level.end_x,0,30,0x0C);

}


void level_init_camera() {
	level.camera.x = 0;
	level.camera.y = 0;
	level.camera.width = SCREEN_WIDTH;
	level.camera.height = SCREEN_HEIGHT;

	/* Calcola il centro dello schermo */
	level.camera.center_x = level.camera.width>>1;//>>1;/// 2;
	level.camera.center_y = level.camera.height>>2;//;>>2;/// 2;


	level.camera.dead_zone_left = 32;
	level.camera.dead_zone_right = 32;
	level.camera.max_inertia_pixels = 8;  /* 16 pixel di inerzia */
	level.camera.inertia_counter = 0;
	level.camera.inertia_x = 0;
	level.camera.last_player_dir = 0;

}



/* Aggiorna la posizione della camera per seguire il player */
//versione base senza dead zone
//void level_update_camera( ) {
//	u16 center_x,center_y;
//
//
//	/* Segui il player mantenendolo al centro */
//	level.camera.x = player.x - level.camera.center_x;
//	level.camera.y =  player.y  - level.camera.center_y;
//
//
//	/* Limiti orizzontali */
//	if (level.camera.x < 0) {
//		level.camera.x = 0;
//	}
//	if (level.camera.x + level.camera.width > level.level_width_px) {
//		level.camera.x = level.level_width_px - level.camera.width;
//	}
//
//	/* Limiti verticali */
//	if (level.camera.y < 0) {
//		level.camera.y = 0;
//	}
//	if (level.camera.y + level.camera.height > level.level_height_px) {
//		level.camera.y = level.level_height_px - level.camera.height;
//	}
//
//	if(level.camera.x>level.level_width_px)level.camera.x=0;
//	if(level.camera.y>level.level_height_px)level.camera.y=0;
//
//	player.visible_spc.sprite.hpos = (int)(player.x - level.camera.x);
//	player.visible_spc.sprite.vpos = (int)(player.y - level.camera.y);
//}

/* Aggiorna la posizione della camera per seguire il player */
//versione con dead zone senza ineriza
//void level_update_camera_old( ) {
//	u16 player_screen_x, player_screen_y;
//	u16 dead_zone_left, dead_zone_right;
//	u16 dead_zone_top, dead_zone_bottom;
//	/* Calcola la posizione del player rispetto alla camera attuale */
//	player_screen_x = player.x - level.camera.x;
//	player_screen_y = player.y - level.camera.y;
//
//	/* Definisci le dead zone (puoi renderle configurabili) */
//	dead_zone_left = level.camera.center_x - level.camera.dead_zone_left;  /* 40px a sinistra del centro */
//	dead_zone_right = level.camera.center_x + level.camera.dead_zone_right; /* 40px a destra del centro */
//
//	/* Regola la camera orizzontalmente solo se il player esce dalle dead zone */
//	if (player_screen_x < dead_zone_left) {
//		/* Player troppo a sinistra: sposta la camera a sinistra */
//		level.camera.x = player.x - dead_zone_left;
//	}
//	else if (player_screen_x > dead_zone_right) {
//		/* Player troppo a destra: sposta la camera a destra */
//		level.camera.x = player.x - dead_zone_right;
//	}
//
//
//	level.camera.y = player.y - level.camera.center_y;
//
//	/* Limiti orizzontali */
//	if (level.camera.x < 0) {
//		level.camera.x = 0;
//	}
//	if (level.camera.x + level.camera.width > level.level_width_px) {
//		level.camera.x = level.level_width_px - level.camera.width;
//	}
//
//	/* Limiti verticali */
//	if (level.camera.y < 0) {
//		level.camera.y = 0;
//	}
//	if (level.camera.y + level.camera.height > level.level_height_px) {
//		level.camera.y = level.level_height_px - level.camera.height;
//	}
//
//	/* Aggiorna posizione sprite del player (relative alla camera) */
//	player.visible_spc.sprite.hpos = (int)(player.x - level.camera.x);
//	player.visible_spc.sprite.vpos = (int)(player.y - level.camera.y);
//}



static u8 inertia_dir = 0;          /* Direzione inerzia: 0=none, 1=dx, 2=sx */
static u8 inertia_counter = 0;      /* Pixel rimanenti */
static u8 player_was_moving = 0;    /* Player si muoveva nel frame precedente? */
static u8 last_move_dir = 0;        /* Ultima direzione di movimento */
void level_update_camera( ) {

	u8 player_is_moving;
	u8 current_dir;
	u16 player_screen_x;
	u16 dead_zone_left, dead_zone_right;

	/* Calcoli iniziali */
	player_screen_x = player.x - level.camera.x;
	dead_zone_left = level.camera.center_x - level.camera.dead_zone_left;
	dead_zone_right = level.camera.center_x + level.camera.dead_zone_right;

	/* Stato corrente */
	current_dir = (player.vx > 0) ? 1 : (player.vx < 0) ? 2 : 0;
	player_is_moving = (current_dir != 0);

	/* DETECTION: Quando iniziare una nuova inerzia */
	if (player_is_moving && !player_was_moving) {
		/* Player ha INIZIATO a muoversi (da fermo a movimento) */
		inertia_dir = current_dir;
		inertia_counter = level.camera.max_inertia_pixels;
		last_move_dir = current_dir;
	}
	else if (player_is_moving && player_was_moving && current_dir != last_move_dir) {
		/* Player ha CAMBIATO DIREZIONE (da dx a sx o viceversa) */
		inertia_dir = current_dir;
		inertia_counter = level.camera.max_inertia_pixels;
		last_move_dir = current_dir;
	}
	else if (!player_is_moving && player_was_moving) {
		/* Player si è FERMATO */
		inertia_dir = last_move_dir;  /* Continua nell'ultima direzione */
		inertia_counter = level.camera.max_inertia_pixels;
	}

	/* Aggiorna stato per il prossimo frame */
	player_was_moving = player_is_moving;
	if (player_is_moving) {
		last_move_dir = current_dir;
	}

	/* APPLICA INERZIA */
	if (inertia_counter > 0) {
		if (inertia_dir == 1) {  /* Destra */
			if (level.camera.x + level.camera.width < level.level_width_px) {
				if(inertia_counter>6)level.camera.x+=2;
				level.camera.x++;
			}
		}
		else if (inertia_dir == 2) {  /* Sinistra */
			if (level.camera.x > 0) {
				if(inertia_counter>6)level.camera.x-=2;
				level.camera.x--;
			}
		}
		inertia_counter--;
	}
	/* COMPORTAMENTO DEAD ZONE (solo se nessuna inerzia) */
	else if (inertia_counter == 0) {
		inertia_dir = 0;  /* Resetta direzione inerzia */

		if (player_screen_x < dead_zone_left) {
			level.camera.x = player.x - dead_zone_left;
		}
		else if (player_screen_x > dead_zone_right) {
			level.camera.x = player.x - dead_zone_right;
		}
	}



	level.camera.y = player.y - level.camera.center_y;

	/* Limiti orizzontali */
	if (level.camera.x < 0) {
		level.camera.x = 0;
		inertia_counter = 0;
		inertia_dir = 0;
	}
	if (level.camera.x + level.camera.width > level.level_width_px) {
		level.camera.x = level.level_width_px - level.camera.width;
		inertia_counter = 0;
		inertia_dir = 0;
	}

	/* Limiti verticali */
	if (level.camera.y < 0) {
		level.camera.y = 0;
	}
	if (level.camera.y + level.camera.height > level.level_height_px) {
		level.camera.y = level.level_height_px - level.camera.height;
	}

	/* Aggiorna posizione sprite del player */
	player.visible_spc.sprite.hpos = (int)(player.x - level.camera.x);
	player.visible_spc.sprite.vpos = (int)(player.y - level.camera.y);
}
