/* level.c */
#include "Level.h"
#include "Player.h"



extern Level level;
extern Player player;



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



//u8 level_get_tile(u16 world_x, u16 world_y)
//{
//	u16 tile_x = world_x >> TILE_SHIFT;
//	u16 tile_y = world_y >> TILE_SHIFT;
//
//	s16 bx = tile_x - level.map_buf_origin_x;
//	s16 by = tile_y - level.map_buf_origin_y;
//
//	if ((u16)bx >= MAP_BUF_W || (u16)by >= MAP_BUF_H)
//		return TILE_EMPTY;
//
//	// applica circolarità
//	bx += level.map_buf_col0;
//	if (bx >= MAP_BUF_W) bx -= MAP_BUF_W;
//
//	by += level.map_buf_row0;
//	if (by >= MAP_BUF_H) by -= MAP_BUF_H;
//
//	return map_buf[by][bx];
//}

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
			u16 world_row = y; // riga del mondo (aggiusta con map_buf_origin_y se scroll verticale)

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
	u8 write_col,y;
	u16 new_world_col;

	level.map_buf_origin_x++;
	level.map_buf_col0 = (level.map_buf_col0 + 1) & MAP_BUF_MASK;

	/* scriviamo nell'ultima colonna logica */
	write_col = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;
	new_world_col = level.map_buf_origin_x + MAP_BUF_W - 1;

	if (new_world_col < level.map_w) {
		for ( y = 0; y < MAP_BUF_H; y++) {
			u16 world_row = y; // oppure level.map_buf_origin_y + y se scroll verticale
			map_buf[y][write_col] = decompress_rle_get_value(
					Z1L1_FG_MAP_RLE_DATA,
					Z1L1_FG_MAP_RLE_OFFS[world_row],
					new_world_col
			);
		}
	}
}

static inline void stream_scroll_left(void)
{
	u8 write_col,y;
	u16 new_world_col;

	level.map_buf_origin_x--;
	level.map_buf_col0 = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;

	write_col = level.map_buf_col0;
	new_world_col = level.map_buf_origin_x;

	if ((s16)new_world_col >= 0) {
		for ( y = 0; y < MAP_BUF_H; y++) {
			u16 world_row = y; // oppure level.map_buf_origin_y + y
			map_buf[y][write_col] = decompress_rle_get_value(
					Z1L1_FG_MAP_RLE_DATA,
					Z1L1_FG_MAP_RLE_OFFS[world_row],
					new_world_col
			);
		}
	}
}


static inline void stream_scroll_up(void)
{
	u8 x,write_row;
	// decrementa la posizione della mappa nel mondo
	if (level.map_buf_origin_y == 0)
		return; // già al bordo superiore

	level.map_buf_origin_y--;
	level.map_buf_row0 = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;

	// la riga logica da aggiornare nel buffer circolare
	write_row = level.map_buf_row0;

	for ( x = 0; x < MAP_BUF_W; x++) {
		u16 world_col = level.map_buf_origin_x + x;
		u16 world_row = level.map_buf_origin_y;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][x] = decompress_rle_get_value(
					Z1L1_FG_MAP_RLE_DATA,
					Z1L1_FG_MAP_RLE_OFFS[world_row],
					world_col
			);
		} else {
			map_buf[write_row][x] = TILE_EMPTY;
		}
	}
}


static inline void stream_scroll_down(void)
{

	u8 x,write_row;
	level.map_buf_origin_y++;
	level.map_buf_row0 = (level.map_buf_row0 + 1) & MAP_BUF_MASK;

	write_row = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;

	for ( x = 0; x < MAP_BUF_W; x++) {
		u16 world_col = level.map_buf_origin_x + x;
		u16 world_row = level.map_buf_origin_y + MAP_BUF_H - 1;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][x] = decompress_rle_get_value(
					Z1L1_FG_MAP_RLE_DATA,
					Z1L1_FG_MAP_RLE_OFFS[world_row],
					world_col
			);
		} else {
			map_buf[write_row][x] = TILE_EMPTY;
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


	level.start_x =0;
	level.start_y =level.map_h * TILE_SIZE;//level.map_h * TILE_SIZE ;

	level.end_x =  level.map_w * TILE_SIZE;
	level.end_y =  level.map_h * TILE_SIZE ;


	/* Inizializza la camera */
	level_init_camera();

	/* Posiziona la camera sul player iniziale */
	level_update_camera( );


	level_init();
}







void level_draw() {
	u16 x, y, start_tile_x, start_tile_y, end_tile_x, end_tile_y, tile_index,dx,dy;


	SCB_REHV_PAL* sprite;
	SCB_REHV_PAL* prev_sprite ;
	SCB_REHV_PAL* first_sprite;
	TileInfo* tile_info;

	release_all_sprites();

	// Calcola la porzione visibile
	start_tile_x = level.camera.x / TILE_SIZE;
	start_tile_y = level.camera.y / TILE_SIZE;

	end_tile_x   = start_tile_x + TILES_X;
	end_tile_y   = start_tile_y + TILES_Y;

	// Limita ai bordi della mappa
	if(end_tile_x > level.map_w) end_tile_x = level.map_w;
	if(end_tile_y > level.map_h) end_tile_y = level.map_h;

	// Aggiornamento dinamico colonne se la camera si è mossa
	dx = start_tile_x - level.camera.prev_tile_x;
	//aggiorno il buffer solo se necessario
	if(dx != 0  && ABS(dx)>MAP_SBORDA_X)
	{

		while (start_tile_x > level.map_buf_origin_x + MAP_SBORDA_X)
			stream_scroll_right();

		while (start_tile_x < level.map_buf_origin_x + MAP_SBORDA_X)
			stream_scroll_left();

		level.camera.prev_tile_x = start_tile_x;

	}



	// Calcola lo scostamento verticale
	dy = start_tile_y - level.camera.prev_tile_y;
	if (dy != 0)
	{
		while (start_tile_y > level.map_buf_origin_y)
			stream_scroll_down();  // aggiorna riga intera, stride non necessario

		while (start_tile_y < level.map_buf_origin_y   )
			stream_scroll_up();    // aggiorna riga intera, stride non necessario

		level.camera.prev_tile_y = start_tile_y;
	}

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



}


void level_init_camera() {
	level.camera.x = 0;
	level.camera.y = 0;
	level.camera.width = SCREEN_WIDTH;
	level.camera.height = SCREEN_HEIGHT;

	/* Calcola il centro dello schermo */
	level.camera.center_x = level.camera.width>>1;//>>1;/// 2;
	level.camera.center_y = level.camera.height>>2;//;>>2;/// 2;

}



/* Aggiorna la posizione della camera per seguire il player */
void level_update_camera( ) {
	u16 center_x,center_y;//,level_width_px,level_height_px;


	/* Segui il player mantenendolo al centro */
	level.camera.x = player.x - level.camera.center_x;
	level.camera.y =  player.y  - level.camera.center_y;


	/* Limiti orizzontali */
	if (level.camera.x < 0) {
		level.camera.x = 0;
	}
	if (level.camera.x + level.camera.width > level.level_width_px) {
		level.camera.x = level.level_width_px - level.camera.width;
	}

	/* Limiti verticali */
	if (level.camera.y < 0) {
		level.camera.y = 0;
	}
	if (level.camera.y + level.camera.height > level.level_height_px) {
		level.camera.y = level.level_height_px - level.camera.height;
	}

	if(level.camera.x>level.level_width_px)level.camera.x=0;
	if(level.camera.y>level.level_height_px)level.camera.y=0;

	player.visible_spc.sprite.hpos = (int)(player.x - level.camera.x);//level_world_to_screen_x(player.x);
	player.visible_spc.sprite.vpos = (int)(player.y - level.camera.y);//level_world_to_screen_y(player.y);

}



/* Converti coordinate world a screen X */
int level_world_to_screen_x( int world_x) {
	return (int)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
int level_world_to_screen_y( int world_y) {
	return (int)(world_y - level.camera.y);
}



