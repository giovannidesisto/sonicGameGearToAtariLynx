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
void decompress_column(const u8 *data, u16 offset, u8 *out, u8 start, u8 height, u8 stride) {
    u8 val, cnt, i;
    u16 idx = offset;
    u8 out_y = start;
    while (out_y < start + height) {
        val = data[idx++];
        cnt = data[idx++];
        for (i = 0; i < cnt && out_y < start + height; i++) {
            out[out_y * stride] = val;
            out_y++;
        }
    }
}


void update_map_columns(u8 start_col, u8 end_col) {
    u8 i;
    for(i = start_col; i < end_col; i++) {
        decompress_column(
            Z1L1_FG_MAP_RLE_DATA,
            Z1L1_FG_MAP_RLE_OFFS[i],
			//FIXME 23
            &map_buf[0][i], // modulo se buffer circolare
            0,
            MAP_BUF_H,
            MAP_BUF_W   // stride
        );
    }
}


u8 level_get_tile(u16 world_x, u16 world_y)
{
    /* 1. world_x/world_y sono world VISIBILI (relativi alla viewport) */
    u16 abs_world_x = world_x;
    u16 abs_world_y = world_y;

    /* 2. converti in coordinate tile world */
    u16 tile_x = abs_world_x / TILE_SIZE;
    u16 tile_y = abs_world_y / TILE_SIZE;

    /* 3. traduci in coordinate buffer */
    s16 bx = (s16)tile_x - (s16)level.map_buf_origin_x;
    s16 by = (s16)tile_y - (s16)level.map_buf_origin_y;

    /* 4. fuori dal buffer */
    if (bx < 0 || bx >= MAP_BUF_W ||
        by < 0 || by >= MAP_BUF_H)
        return TILE_EMPTY;

    /* 5. fuori dalla mappa */
    if (tile_x >= level.map_w || tile_y >= level.map_h)
        return TILE_EMPTY;

    /* 6. accesso al buffer */
    return map_buf[by][bx];

}


static void shift_buffer_right_and_load_prev(void)
{
    s16 x, y;
    s16 new_world_col;

    /* 1. shift a destra */
    for (y = 0; y < MAP_BUF_H; y++) {
        for (x = MAP_BUF_W - 1; x > 0; x--) {
            map_buf[y][x] = map_buf[y][x - 1];
        }
    }

    /* 2. nuova colonna world da caricare */
    new_world_col = level.map_buf_origin_x - 1;

    if (new_world_col >= 0) {
        decompress_column(
            Z1L1_FG_MAP_RLE_DATA,
            Z1L1_FG_MAP_RLE_OFFS[new_world_col],
            &map_buf[0][0],
            0,
            MAP_BUF_H,
            MAP_BUF_W
        );
    } else {
        /* fuori mappa → riempi di TILE_EMPTY */
        for (y = 0; y < MAP_BUF_H; y++) {
            map_buf[y][0] = TILE_EMPTY;
        }
    }

    /* 3. aggiorna origine */
    level.map_buf_origin_x--;
}



static void shift_buffer_left_and_load_next(void)
{
    u8 x, y;
    u16 new_world_col;

    /* 1. shift a sinistra */
    for (y = 0; y < MAP_BUF_H; y++) {
        for (x = 0; x < MAP_BUF_W - 1; x++) {
            map_buf[y][x] = map_buf[y][x + 1];
        }
    }

    /* 2. nuova colonna world da caricare */
    new_world_col = level.map_buf_origin_x + MAP_BUF_W;

    if (new_world_col < level.map_w) {
        decompress_column(
            Z1L1_FG_MAP_RLE_DATA,
            Z1L1_FG_MAP_RLE_OFFS[new_world_col],
            &map_buf[0][MAP_BUF_W - 1],
            0,
            MAP_BUF_H,
            MAP_BUF_W
        );
    } else {
        /* fuori mappa → riempi di TILE_EMPTY */
        for (y = 0; y < MAP_BUF_H; y++) {
            map_buf[y][MAP_BUF_W - 1] = TILE_EMPTY;
        }
    }

    /* 3. aggiorna origine */
    level.map_buf_origin_x++;
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
	for(i = 0; i < MAP_BUF_W; i++) {
	    decompress_column(
	        Z1L1_FG_MAP_RLE_DATA,
	        Z1L1_FG_MAP_RLE_OFFS[i],
	        &map_buf[0][i], //il primo indice indica la Y, si deve usare la y del view port per ottimizzare la  dimensione della mappa in memoria.
	        0,
	        MAP_BUF_H,
	        MAP_BUF_W   // stride per andare giù nella colonna
	    );
	}



	level.fg_map = map_buf;

	level.camera.prev_tile_x=0;
	level.camera.prev_tile_y=0;
	level.map_h= MAP_HEIGHT;
	level.map_w= MAP_WIDTH;


	/* Limita la camera ai bordi del livello */
	level.level_width_px =  level.map_w * TILE_SIZE;
	level.level_height_px = level.map_h * TILE_SIZE;


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
}







void level_draw() {
    u16 x, y, start_tile_x, start_tile_y, end_tile_x, end_tile_y, tile_index,dx;


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
    if(dx != 0)
    {
            /* scroll a destra */
            while (start_tile_x > level.map_buf_origin_x+MAP_BUF_PAD_HALF)// + 2)
                shift_buffer_left_and_load_next();

            // Scorrimento a sinistra: decomprimi nuove colonne a sx
           // update_map_columns(start_tile_x, level.camera.prev_tile_x);
            /* scroll a sinistra */
            while (start_tile_x < level.map_buf_origin_x+MAP_BUF_PAD_HALF)// + 2)
                shift_buffer_right_and_load_prev();



        level.camera.prev_tile_x = start_tile_x;

    }



    // Inizia con lo sfondo
    agSprBackground.penpal[0] = 0x09;
    prev_sprite = &agSprBackground;
    first_sprite = prev_sprite;

    // Render foreground tiles visibili
    for(y = start_tile_y; y < end_tile_y; y++) {
        for(x = start_tile_x; x < end_tile_x; x++) {
            tile_index =   level_get_tile(x*TILE_SIZE,y*TILE_SIZE);//level.fg_map[y*MAP_BUF_W + x]; // accesso row-major
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



