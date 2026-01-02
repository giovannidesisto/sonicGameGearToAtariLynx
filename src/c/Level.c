/* level.c */
#include "Level.h"


#define BG0_W 16
#define BG0_H 1
#define BG1_W 11
#define BG1_H 4


#define PARALLAX_BG0_SHIFT 4   // 1/8
#define PARALLAX_BG1_SHIFT 3   // 1/4
#define BG0_FIXED_Y 0
#define BG1_FIXED_Y 16


static u8 prev_bg0_tile_index;
static u8 prev_bg1_tile_index;
static u8 prev_fg_tile_index;
static TileInfo *prev_bg0_tile_info;
static TileInfo *prev_bg1_tile_info;
static TileInfo *prev_fg_tile_info;



#define TILE_CACHE_SIZE 24

typedef struct {
	u8 tile_index;
	TileInfo* info;
	u8 hit_count;
	u8 age;
} TileCacheEntry;

#pragma bss-name (push, "ZEROPAGE")
static TileCacheEntry tile_cache[TILE_CACHE_SIZE];
static TileInfo* bg0_tile_cache[BG0_W] = {NULL};
static TileInfo* bg1_tile_cache[BG1_H][BG1_W] = {NULL};

#pragma bss-name (pop)


static u8 cache_clock = 0;
// Funzione di cache


static inline TileInfo* get_tile_cached(u8 tile_index) {
	u8 i, lru_idx;

	/* 1. Cerca nella cache (hit) */
	for (i = 0; i < TILE_CACHE_SIZE; i++) {
		if (tile_cache[i].tile_index == tile_index) {
			/* Hit: aggiorna LRU e ritorna */
			tile_cache[i].hit_count = ++cache_clock;
			return tile_cache[i].info;
		}
	}

	/* 2. Miss: trova la entry meno recentemente usata (LRU) */
	lru_idx = 0;
	for (i = 1; i < TILE_CACHE_SIZE; i++) {
		if (tile_cache[i].hit_count < tile_cache[lru_idx].hit_count) {
			lru_idx = i;
		}
	}

	/* 3. Sostituisci entry LRU */
	tile_cache[lru_idx].tile_index = tile_index;
	tile_cache[lru_idx].info = tileinfo_get(tile_index);
	tile_cache[lru_idx].hit_count = ++cache_clock;

	return tile_cache[lru_idx].info;
}

//static  TileInfo* get_tile_cached(u8 tile_index) {
//u8 i,replace_idx;
//	cache_clock++;
//
//    // 1. Cerca nella cache
//    for ( i = 0; i < TILE_CACHE_SIZE; i++) {
//        if (tile_cache[i].tile_index == tile_index) {
//            tile_cache[i].hit_count++;
//            tile_cache[i].age = cache_clock;
//            return tile_cache[i].info;
//        }
//    }
//
//    // 2. Non trovato: cerca slot da sostituire (LFU)
//    replace_idx = 0;
//    for ( i = 1; i < TILE_CACHE_SIZE; i++) {
//        // LFU: meno hit, o se pari hit, più vecchio
//        if (tile_cache[i].hit_count < tile_cache[replace_idx].hit_count ||
//           (tile_cache[i].hit_count == tile_cache[replace_idx].hit_count
//        		   &&tile_cache[i].age < tile_cache[replace_idx].age)
//			)
//        	{
//            replace_idx = i;
//        }
//    }
//
//    // 3. Rimpiazza e ritorna
//    tile_cache[replace_idx].tile_index = tile_index;
//    tile_cache[replace_idx].info = tileinfo_get(tile_index);
//    tile_cache[replace_idx].hit_count = 1;
//    tile_cache[replace_idx].age = cache_clock;
//
//    return tile_cache[replace_idx].info;
//}

const u8 bg0_map[BG0_H][BG0_W]={
		{100,101,0,0,0,0,103,102,0,0,0,0,0,0,0,0}
};
const u8 bg1_map[BG1_H][BG1_W]=
{
		//	{0,0,0},
		//		{0,0,0},
		//	{0,0,0},
		{110,111,112,113,114,115,116,112,106,105,104},
		{0},
		{0},
		{109,109,108,107,109,109,109,108,109,109,107}
};



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





static void level_init_map_streaming(void)
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








/* Inizializza il sistema di sprite */
void level_init(void) {

	u8 i,x,y;//,y;

	lynx_load(1);//livello
	lynx_load(2);//player
	lynx_load(3);//sfondo
	lynx_load(4);//background




	//identifica le coordinate x/y mondo nel buffer attuale
	level.map_buf_origin_x = 0;
	level.map_buf_origin_y = 0;


	//level.fg_map = map_buf;
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


	prev_bg0_tile_info=0;
	prev_bg1_tile_info=0;
	prev_fg_tile_info=0;

	prev_bg0_tile_index=0;
	prev_bg1_tile_index=0;
	prev_fg_tile_index=0;
	for(i = 0; i < TILE_CACHE_SIZE; i++) {
		tile_cache[i].tile_index = 0xFF;  // Invalida
		tile_cache[i].hit_count = 0;
		tile_cache[i].age = 0;
		tile_cache[i].info = tileinfo_get(0);  // Tile vuota
	}


	// Pre-calcola BG0 (solo una volta)
	for( x = 0; x < BG0_W; x++) {
		bg0_tile_cache[x] = tileinfo_get(bg0_map[0][x]);
	}

	// Pre-calcola BG1 (solo una volta)
	for( y = 0; y < BG1_H; y++) {
		for( x = 0; x < BG1_W; x++) {
			bg1_tile_cache[y][x] = tileinfo_get(bg1_map[y][x]);
		}
	}
}

/* Carica un livello */
void level_load(u8 level_num) {
	//u8 x, y;
	/* Per ora carica solo la mappa di default */
	(void)level_num;  /* Ignorato per ora */




	level_init();

	/* Inizializza la camera */
	level_init_camera();

	/* Posiziona la camera sul player iniziale */
	level_update_camera( );
}













void level_draw(void)
{
	u16 x, y, ctr,color;
	u16 start_tile_x, start_tile_y;
	u16 end_tile_x, end_tile_y;
	s16 screen_x, screen_y;
	SCB_REHV_PAL *head ;
	SCB_REHV_PAL *tail ;




	SCB_REHV_PAL *sprite;
	SCB_REHV_PAL *first_sprite;

	/* BG */
	SCB_REHV_PAL *bg0_head = NULL, *bg0_tail = NULL;
	SCB_REHV_PAL *bg1_head = NULL, *bg1_tail = NULL;

	/* FG layers */
	SCB_REHV_PAL *fg0_head = NULL, *fg0_tail = NULL;
	SCB_REHV_PAL *fg1_head = NULL, *fg1_tail = NULL;
	SCB_REHV_PAL *fg2_head = NULL, *fg2_tail = NULL;

	TileInfo *tile_info;


	u8 tile_index;


	ctr = 0;
	release_all_sprites();

	/* ===================================================== */
	/* CAMERA → TILE */
	/* ===================================================== */
	start_tile_x = level.camera.x >> TILE_SHIFT;
	start_tile_y = level.camera.y >> TILE_SHIFT;

	end_tile_x = start_tile_x + TILES_X;
	end_tile_y = start_tile_y + TILES_Y;

	if (end_tile_x > level.map_w) end_tile_x = level.map_w;
	if (end_tile_y > level.map_h) end_tile_y = level.map_h;

	/* ===================================================== */
	/* STREAMING MAP */
	/* ===================================================== */
	while (start_tile_x > level.map_buf_origin_x) stream_scroll_right();
	while (start_tile_x < level.map_buf_origin_x) stream_scroll_left();
	while (start_tile_y > level.map_buf_origin_y) stream_scroll_down();
	while (start_tile_y < level.map_buf_origin_y) stream_scroll_up();

	level.camera.prev_tile_x = start_tile_x;
	level.camera.prev_tile_y = start_tile_y;

	/* ===================================================== */
	/* ROOT */
	/* ===================================================== */
	agSprBackground.penpal[0] = 0x0D;
	first_sprite = &agSprBackground;

	/* ===================================================== */
	/* BG0 */
	/* ===================================================== */
	{
		s16 par_px  = level.camera.x >> PARALLAX_BG0_SHIFT;
		s16 tile_x0 = par_px >> TILE_SHIFT;
		s16 px_off  = -(par_px & (TILE_SIZE - 1));

		for (x = 0; x < TILES_X + 2; x++) {
			u16 map_x = (tile_x0 + x) % BG0_W;

			//			u8 currentTileIndex  = bg0_map[0][map_x];
			//			if(currentTileIndex==prev_bg0_tile_index && prev_bg0_tile_info != 0)
			//			{
			//				tile_info = prev_bg0_tile_info;
			//			}
			//			else
			//			{
			tile_info =   bg0_tile_cache[map_x];//get_tile_cached(currentTileIndex);//tileinfo_get(currentTileIndex);
			//				prev_bg0_tile_index = currentTileIndex;
			//				prev_bg0_tile_info = tile_info;
			//			}

			if (tile_info->type != TILE_EMPTY) {
				sprite = get_free_sprite();
				if (!sprite) return;

				sprite->sprctl0 = tile_info->colorDepth | TYPE_BACKGROUND;
				sprite->sprctl1 = REHV | PACKED;
				sprite->data    = tile_info->bitmap;
				sprite->hpos    = px_off + (x << TILE_SHIFT)+ (tile_info->xy_offset >> 4);
				sprite->vpos    = BG0_FIXED_Y +(tile_info->xy_offset&0x0F);
				sprite->next    = NULL;


				if (tile_info->is_mirrored) {
					sprite->sprctl0 =  sprite->sprctl0 |HFLIP;// tile_info->colorDepth | TYPE_NORMAL | HFLIP;
					sprite->hpos    =  px_off + (x << TILE_SHIFT) + + TILE_SIZE - 1  + (tile_info->xy_offset >> 4);
				}

				if (tile_info->is_flipped) {
					sprite->sprctl0 =  sprite->sprctl0 |VFLIP;
					sprite->vpos =sprite->vpos +TILE_SIZE;
				}




				if(tile_info->colorDepth==BPP_2){
					sprite->penpal[0]=tile_info->palette[0];
					sprite->penpal[1]=tile_info->palette[1];
				}
				else if(tile_info->colorDepth==BPP_3){
					sprite->penpal[0]=tile_info->palette[0];
					sprite->penpal[1]=tile_info->palette[1];
					sprite->penpal[2]=tile_info->palette[2];
					sprite->penpal[3]=tile_info->palette[3];
				}

				if (!bg0_head) bg0_head = sprite;
				else bg0_tail->next = (char*)sprite;
				bg0_tail = sprite;
#ifdef DEBUG
				ctr++;
#endif
			}
		}
	}

	/* ===================================================== */
	/* BG1 */
	/* ===================================================== */
	{
		s16 par_px  = level.camera.x >> PARALLAX_BG1_SHIFT;
		s16 tile_x0 = par_px >> TILE_SHIFT;
		s16 px_off  = -(par_px & (TILE_SIZE - 1));

		for (y = 0; y < BG1_H; y++) {
			for (x = 0; x < TILES_X + 2; x++) {
				u16 map_x = (tile_x0 + x) % BG1_W;
				tile_info  = bg1_tile_cache[y][map_x]; // tileinfo_get(currentTileIndex);



				if (tile_info->type != TILE_EMPTY) {
					sprite = get_free_sprite();
					if (!sprite) return;

					sprite->sprctl0 = tile_info->colorDepth | TYPE_BACKGROUND;
					sprite->sprctl1 = REHV | PACKED;
					sprite->data    = tile_info->bitmap;
					sprite->hpos    = px_off + (x << TILE_SHIFT) + (tile_info->xy_offset >> 4);
					sprite->vpos    = BG1_FIXED_Y + (y << TILE_SHIFT)  +(tile_info->xy_offset&0x0F);
					sprite->next    = NULL;


					if (tile_info->is_mirrored) {
						sprite->sprctl0 =  sprite->sprctl0 |HFLIP;// tile_info->colorDepth | TYPE_NORMAL | HFLIP;
						sprite->hpos    =  px_off + (x << TILE_SHIFT) + + TILE_SIZE - 1  + (tile_info->xy_offset >> 4);
					}
					if (tile_info->is_flipped) {
						sprite->sprctl0 =  sprite->sprctl0 |VFLIP;
						sprite->vpos =sprite->vpos +TILE_SIZE;
					}



					if(tile_info->colorDepth==BPP_2){
						sprite->penpal[0]=tile_info->palette[0];
						sprite->penpal[1]=tile_info->palette[1];
					}
					else if(tile_info->colorDepth==BPP_3){
						sprite->penpal[0]=tile_info->palette[0];
						sprite->penpal[1]=tile_info->palette[1];
						sprite->penpal[2]=tile_info->palette[2];
						sprite->penpal[3]=tile_info->palette[3];
					}

					if (!bg1_head) bg1_head = sprite;
					else bg1_tail->next = (char*)sprite;
					bg1_tail = sprite;
#ifdef DEBUG
					ctr++;
#endif
				}
			}
		}
	}

	/* ===================================================== */
	/* FOREGROUND (layerizzato) */
	/* ===================================================== */
	for (y = start_tile_y; y < end_tile_y; y++) {
		for (x = start_tile_x; x < end_tile_x; x++) {
			u8 tile_nr;
			u8 tile_bk;
			u8 currentTileIndex;
			tile_nr =level_get_tile_abs(x, y);


			addTile:
			//tile_info = tileinfo_get(tile_nr);//level_get_tile_abs(x, y));

			currentTileIndex  = tile_nr;//bg1_map[y][map_x];
			if(currentTileIndex==prev_fg_tile_index && prev_fg_tile_info != 0)
			{
				tile_info = prev_fg_tile_info;
			}
			else
			{
				tile_info = get_tile_cached(currentTileIndex);//	tileinfo_get(currentTileIndex);
				prev_fg_tile_index = currentTileIndex;
				prev_fg_tile_info = tile_info;
			}




			if (tile_info->type == TILE_EMPTY) continue;

			screen_x = (x << TILE_SHIFT) - level.camera.x;
			screen_y = (y << TILE_SHIFT) - level.camera.y;

			sprite = get_free_sprite();
			if (!sprite) return;

			sprite->sprctl1 = REHV | PACKED;
			sprite->data    = tile_info->bitmap;
			sprite->vpos    = screen_y+ (tile_info->xy_offset & 0X0F);
			sprite->next    = NULL;

			if (tile_info->is_mirrored) {
				sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL | HFLIP;
				sprite->hpos    = screen_x + TILE_SIZE - 1  + (tile_info->xy_offset >> 4);
			} else {
				sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL;
				sprite->hpos    = screen_x + (tile_info->xy_offset >> 4);
			}



			if(tile_info->colorDepth==BPP_2){
				sprite->penpal[0]=tile_info->palette[0];
				sprite->penpal[1]=tile_info->palette[1];
			}
			else if(tile_info->colorDepth==BPP_3){
				sprite->penpal[0]=tile_info->palette[0];
				sprite->penpal[1]=tile_info->palette[1];
				sprite->penpal[2]=tile_info->palette[2];
				sprite->penpal[3]=tile_info->palette[3];
			}

			/* -------- dispatch per layer -------- */
			if (tile_info->layer == 0) {
				if (!fg0_head) fg0_head = sprite;
				else fg0_tail->next = (char*)sprite;
				fg0_tail = sprite;
			}
			else if (tile_info->layer == 1) {
				if (!fg1_head) fg1_head = sprite;
				else fg1_tail->next = (char*)sprite;
				fg1_tail = sprite;
			}
			else {
				if (!fg2_head) fg2_head = sprite;
				else fg2_tail->next = (char*)sprite;
				fg2_tail = sprite;
			}
#ifdef DEBUG
			ctr++;
#endif


			if(tile_info->overlay_tile>0){
				tile_nr=tile_info->overlay_tile;
				goto addTile;
			}

		}
	}

	/* ===================================================== */
	/* CONCATENAZIONE FINALE (ORDINE CORRETTO)                */
	/* bg0 → bg1 → fg0 → fg1 → player → fg2 → NULL           */
	/* ===================================================== */

	head = &agSprBackground;
	tail = &agSprBackground;

	/* BG0 */
	if (bg0_head) {
		tail->next = bg0_head;
		tail = bg0_tail;
	}

	/* BG1 */
	if (bg1_head) {
		tail->next = bg1_head;
		tail = bg1_tail;
	}

	/* FG layer 0 (dietro player) */
	if (fg0_head) {
		tail->next = fg0_head;
		tail = fg0_tail;
	}

	/* FG layer 1 (stesso piano player) */
	if (fg1_head) {
		tail->next = fg1_head;
		tail = fg1_tail;
	}

	/* PLAYER */
	tail->next = &player.visible_spc.sprite;
	tail = &player.visible_spc.sprite;

	/* FG layer 2 (davanti al player) */
	if (fg2_head) {
		tail->next = fg2_head;
		tail = fg2_tail;
	}

	/* Fine lista */
	tail->next = NULL;

	/* ===================================================== */
	/* DRAW */
	/* ===================================================== */
	tgi_sprite(first_sprite);
#ifdef DEBUG
	printU16(ctr, 0, 0, 0x01);
#endif
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




void check_world_collision() {
	s16 new_x, new_y;
	s16 tile_x, tile_y;
	s16 tile_index;
	TileInfo* tile_info;

	new_x = player.x + player.vx;
	new_y = player.y + player.vy;

	player.is_grounded = 0;

	/* 1. COLLISIONI ORIZZONTALI */
	if (player.vx != 0) {
		s16 check_x = (player.vx > 0) ? (new_x + player.width_half) : (new_x - player.width_half);
		tile_x = check_x >> TILE_SHIFT;
		tile_y = (player.y + player.height_half) >> TILE_SHIFT;

		if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w)
		{


			//			u16 bx = tile_x - level.map_buf_origin_x;
			//			u16 by = tile_y - level.map_buf_origin_y;


			tile_index = level_get_tile_abs(tile_x, tile_y );//level.fg_map[tile_y*level.map_w+tile_x];//
			tile_info = get_tile_cached(tile_index);

			if (tile_info->type == TILE_SOLID)
			{
				if (player.vx > 0) {
					player.x = (tile_x << TILE_SHIFT) - (player.width_half);
				} else {
					player.x = ((tile_x + 1) << TILE_SHIFT) + (player.width_half);
				}
				player.vx = 0;
				new_x = player.x;
			}
			else if(tile_info->type == TILE_PLATFORM)
			{
				//		s16 feet_y = new_y + player.height;

				u8 local_x = new_x - (tile_x * TILE_SIZE);
				u8 terrain_height = tileinfo_get_height_at(tile_info, local_x);


				if (terrain_height > 0) {
					/* IMPORTANTE:
					   - terrain_height = 15: cima della tile (y = tile_top)
					   - terrain_height = 0: fondo della tile (y = tile_top + 15)
					   ground_y = tile_top + (15 - terrain_height)
					 */
					u16 ground_y = (tile_y << TILE_SHIFT) + (15 - terrain_height);

					if ((new_y + player.height) >= ground_y) {
						player.y = ground_y - player.height;
						player.is_grounded = 1;
						player.is_jumping = 0;
						player.vy = 0;
						new_y = player.y;
					}
				}
			}
		}
	}





	/* 2. COLLISIONI VERTICALI */
	if (player.vy != 0) {
		s16 center_x = new_x;

		if (player.vy > 0) {
			/* CADUTA - Controlla sotto */
			s16 feet_y = new_y + player.height;
			tile_x = center_x >> TILE_SHIFT;
			tile_y = feet_y >> TILE_SHIFT;

			if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w) {
				tile_index = level_get_tile_abs(tile_x , tile_y);//level.fg_map[tile_y*level.map_w+tile_x];//
				tile_info = get_tile_cached(tile_index);

				if (tile_info->type == TILE_SOLID) {
					player.y = tile_y * TILE_SIZE - player.height;
					player.is_grounded = 1;
					player.is_jumping = 0;
					player.vy = 0;
					new_y = player.y;
				}
				else if (tile_info->type == TILE_PLATFORM) {
					u8 local_x = center_x - (tile_x << TILE_SHIFT);
					u8 terrain_height = tileinfo_get_height_at(tile_info, local_x); // 0-15

					if (terrain_height > 0) {
						/* IMPORTANTE:
                           - terrain_height = 15: cima della tile (y = tile_top)
                           - terrain_height = 0: fondo della tile (y = tile_top + 15)

                           ground_y = tile_top + (15 - terrain_height)
						 */
						u16 ground_y = (tile_y << TILE_SHIFT) + (15 - terrain_height);

						if (feet_y >= ground_y) {
							player.y = ground_y - player.height;
							player.is_grounded = 1;
							player.is_jumping = 0;
							player.vy = 0;
							new_y = player.y;
						}
					}
				}
			}
		} else {
			/* SALTO - Controlla sopra */
			s16 head_y = new_y;
			tile_x = center_x >> TILE_SHIFT;
			tile_y = head_y >> TILE_SHIFT;

			if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w) {
				tile_index =level_get_tile_abs(tile_x , tile_y);// level.fg_map[tile_y*level.map_w+tile_x];//
				tile_info = get_tile_cached(tile_index);

				if (tile_info->type == TILE_SOLID) {
					player.y = (tile_y + 1) << TILE_SHIFT;
					player.vy = 0;
					new_y = player.y;
				}
			}
		}
	}

	/* 3. APPLICA MOVIMENTO */
	player.x = new_x;
	player.y = new_y;
}


void player_update() {


	/* Applica gravità */
	if(!player.is_grounded) {
		player.vy += player.gravity;
		if(player.vy > 16) player.vy = 16;
	}

	check_world_collision();

	/* DOPO: Limita la posizione del player */
	if(player.x < player.width) {
		player.x = player.width;
		player.vx = 0;  /* Importante: ferma anche la velocità orizzontale */
	}
	else if(player.x > level.end_x - player.width) {
		player.x = level.end_x - player.width;
		player.vx = 0;  /* Importante: ferma anche la velocità orizzontale */
	}

}




