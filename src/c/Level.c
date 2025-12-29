/* level.c */
#include "Level.h"
#include "Player.h"

extern Player player;




#define BG0_W 4
#define BG0_H 1

#define BG1_W 4
#define BG1_H 2

const u8 bg0_map[BG0_H][BG0_W]={
		{100,100,100,100}
};
const u8 bg1_map[BG1_H][BG1_W]=
{
		{101,  0,101,  0},
		{  0,101,  0,101}
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


/**
 * FIXME non funziona l'inlining
 */
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

	u8 i;//,y;

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





static void draw_parallax_layer(
    const u8 *map,
    u16 map_w,
    u16 map_h,
    s16 parallax_x,
    s16 fixed_y,
    SCB_REHV_PAL **pprev
)
{
    u16 x, y;
    SCB_REHV_PAL *spr;
    u16 start_x = parallax_x >> TILE_SHIFT;
    s16 offset_x = -(parallax_x & (TILE_SIZE - 1));

    for (y = 0; y < map_h; y++) {
        for (x = 0; x <= TILES_X; x++) {

            u16 tx = (start_x + x) % map_w;
            u8 tile = map[y * map_w + tx];
            TileInfo *ti = tileinfo_get(tile);

            if (ti->type == TILE_EMPTY)
                continue;

            spr = get_free_sprite();
            if (!spr) return;

            spr->data = (unsigned char*)ti->bitmap;
            spr->sprctl0 = ti->colorDepth | TYPE_BACKGROUND;
            spr->sprctl1 = REHV | PACKED;

            spr->hpos = offset_x + x * TILE_SIZE;
            spr->vpos = fixed_y + y * TILE_SIZE;

            (*pprev)->next = (char *)spr;
            *pprev = spr;
        }
    }
}




#define PARALLAX_BG0_SHIFT 4   // 1/8
#define PARALLAX_BG1_SHIFT 3   // 1/4
#define BG0_FIXED_Y 0
#define BG1_FIXED_Y 16

void level_draw(void)
{
    u16 x, y,ctr;
    u16 start_tile_x, start_tile_y;
    u16 end_tile_x, end_tile_y;

    s16 screen_x, screen_y;

    SCB_REHV_PAL *sprite;
    SCB_REHV_PAL *first_sprite;
    SCB_REHV_PAL *bg0_head = NULL, *bg0_tail = NULL;
    SCB_REHV_PAL *bg1_head = NULL, *bg1_tail = NULL;
    SCB_REHV_PAL *fg_head  = NULL, *fg_tail  = NULL;

    TileInfo *tile_info;
    u8 tile_index;
	ctr=0;
    /* ===================================================== */
    /* RESET SPRITE POOL */
    /* ===================================================== */
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
    agSprBackground.penpal[0] = 0x09;
    first_sprite = &agSprBackground;

    /* ===================================================== */
    /* ================= BG0 (montagne) ==================== */
    /* ===================================================== */
    {
        s16 par_px   = level.camera.x >> PARALLAX_BG0_SHIFT;
        s16 tile_x0  = par_px >> TILE_SHIFT;
        s16 px_off   = -(par_px & (TILE_SIZE - 1));

        for (x = 0; x < TILES_X + 2; x++)
        {
            u16 map_x = (tile_x0 + x) % BG0_W;
            tile_index = bg0_map[0][map_x];
            tile_info  = tileinfo_get(tile_index);

            if (tile_info->type != TILE_EMPTY)
            {
                sprite = get_free_sprite();
                if (!sprite) return;
                ctr++;
                sprite->sprctl0 = tile_info->colorDepth | TYPE_BACKGROUND;
                sprite->sprctl1 = REHV | PACKED;
                sprite->data    = tile_info->bitmap;
                sprite->hpos    = px_off + (x << TILE_SHIFT);
                sprite->vpos    = BG0_FIXED_Y;
                sprite->next    = NULL;

                if (!bg0_head) bg0_head = sprite;
                else bg0_tail->next = (char *)sprite;
                bg0_tail = sprite;
            }
        }
    }

    /* ===================================================== */
    /* ================= BG1 (foresta) ===================== */
    /* ===================================================== */
    {
        s16 par_px   = level.camera.x >> PARALLAX_BG1_SHIFT;
        s16 tile_x0  = par_px >> TILE_SHIFT;
        s16 px_off   = -(par_px & (TILE_SIZE - 1));

        for (y = 0; y < BG1_H; y++)
        {
            for (x = 0; x < TILES_X + 2; x++)
            {
                u16 map_x = (tile_x0 + x) % BG1_W;
                tile_index = bg1_map[y][map_x];
                tile_info  = tileinfo_get(tile_index);

                if (tile_info->type != TILE_EMPTY)
                {
                    sprite = get_free_sprite();
                    if (!sprite) return;
                    ctr++;
                    sprite->sprctl0 = tile_info->colorDepth | TYPE_BACKGROUND;
                    sprite->sprctl1 = REHV | PACKED;
                    sprite->data    = tile_info->bitmap;
                    sprite->hpos    = px_off + (x << TILE_SHIFT);
                    sprite->vpos    = BG1_FIXED_Y + (y << TILE_SHIFT);
                    sprite->next    = NULL;

                    if (!bg1_head) bg1_head = sprite;
                    else bg1_tail->next =  (char *) sprite;
                    bg1_tail = sprite;
                }
            }
        }
    }

    /* ===================================================== */
    /* ================= FOREGROUND ======================== */
    /* ===================================================== */
    for (y = start_tile_y; y < end_tile_y; y++)
    {
        for (x = start_tile_x; x < end_tile_x; x++)
        {
            tile_index = level_get_tile_abs(x, y);
            tile_info  = tileinfo_get(tile_index);

            if (tile_info->type != TILE_EMPTY)
            {
                screen_x = (x << TILE_SHIFT) - level.camera.x;
                screen_y = (y << TILE_SHIFT) - level.camera.y;

                sprite = get_free_sprite();
                if (!sprite) return;
                ctr++;
                sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL;
                sprite->sprctl1 = REHV | PACKED;
                sprite->data    = tile_info->bitmap;
                sprite->hpos    = screen_x;
                sprite->vpos    = screen_y;
                sprite->next    = NULL;

				if(tile_info->is_mirrored) {
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL | HFLIP;
					sprite->hpos = screen_x + TILE_SIZE - 1;
				} else {
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL;
					sprite->hpos = screen_x;
				}


                if (!fg_head) fg_head = sprite;
                else fg_tail->next = (char *)sprite;
                fg_tail = sprite;
            }
        }
    }

    /* ===================================================== */
    /* CONCATENAZIONE */
    /* ===================================================== */
    agSprBackground.next = bg0_head ? bg0_head :
                           bg1_head ? bg1_head :
                           fg_head  ? fg_head  :
                        		   &player.visible_spc.sprite;

    if (bg0_tail)
        bg0_tail->next = bg1_head ? bg1_head :
                         fg_head  ? fg_head  :
                         &player.visible_spc.sprite;

    if (bg1_tail)
        bg1_tail->next = fg_head ? fg_head :
                         &player.visible_spc.sprite;

    if (fg_tail)
        fg_tail->next = &player.visible_spc.sprite;

    player.visible_spc.sprite.next = NULL;

    /* ===================================================== */
    /* DRAW */
    /* ===================================================== */
    tgi_sprite(first_sprite);
	printU16(ctr,0,0,0x01);
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
			tile_info = tileinfo_get(tile_index);

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
				tile_info = tileinfo_get(tile_index);

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
				tile_info = tileinfo_get(tile_index);

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
//	if(player.x < player.width) player.x = player.width;
//	else if(player.x > level.end_x-player.width) player.x = level.end_x-player.width;


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
