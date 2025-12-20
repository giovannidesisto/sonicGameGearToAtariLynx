/* level.c */
#include "Level.h"
#include "Player.h"

extern Level level;
extern Player player;
#define MAX_ACTIVE_SPRITES  TILES_Y*TILES_X


static SCB_REHV_PAL sprite_pool[MAX_ACTIVE_SPRITES];
static u8 sprite_used[MAX_ACTIVE_SPRITES];  /* 0 = libero, 1 = in uso */


/* Inizializza il sistema di sprite */
void level_init(void) {

//	short x;
//	short y;
//	short j;
    u8 i;

	lynx_load(1);//livello
	lynx_load(2);//player
	lynx_load(3);//sfondo
	lynx_load(4);//background

	level.fg_map = &Z1L1_FG_MAP[0];// //Z1L1_FG_ADDR;
	level.bg_map = &Z1L1_BG_MAP[0];//  Z1L1_BG_ADDR;
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
//
//	for(y = 0; y < TILES_Y; y++)
//		for(x = 0; x < TILES_X; x++)
//		{
//			SCB_MATRIX[y][x].sprctl0 = BPP_1 | TYPE_NORMAL;
//			SCB_MATRIX[y][x].sprctl1 = REHV  | PACKED ;//LITERAL per sprite non compresse, SOLO PIATTAFORME!!! le altre sprite devono essere compresse
//			SCB_MATRIX[y][x].sprcoll =  NO_COLLIDE;
//			SCB_MATRIX[y][x].next = (void*)0;
//			SCB_MATRIX[y][x].data = (void*)0;
//			SCB_MATRIX[y][x].hsize = 0x0100*SCALE/SCALE_DIVIDER;
//			SCB_MATRIX[y][x].vsize = 0x0100*SCALE/SCALE_DIVIDER;
//			SCB_MATRIX[y][x].vpos = 0;
//			SCB_MATRIX[y][x].hpos = 0;
//			for(j = 0; j < 8; j++) {
//				SCB_MATRIX[y][x].penpal[j] = tile_palette[j];
//			}
//
//
//		}
//
//
//
//
//	// In Level.c, inizializzala in level_init()
//	for(y = 0; y < TILES_Y; y++)
//		for(x = 0; x < TILES_X; x++) {
//			SCB_PRX_MATRIX[y][x].sprctl0 = BPP_1 | TYPE_NORMAL;
//			SCB_PRX_MATRIX[y][x].sprctl1 = REHV  | PACKED ;//LITERAL per sprite non compresse, SOLO PIATTAFORME!!! le altre sprite devono essere compresse
//			SCB_PRX_MATRIX[y][x].sprcoll =  NO_COLLIDE;
//			SCB_PRX_MATRIX[y][x].next = (void*)0;
//			SCB_PRX_MATRIX[y][x].data = (void*)0;
//			SCB_PRX_MATRIX[y][x].hsize = 0x0100*SCALE/SCALE_DIVIDER;
//			SCB_PRX_MATRIX[y][x].vsize = 0x0100*SCALE/SCALE_DIVIDER;
//			SCB_PRX_MATRIX[y][x].vpos = 0;
//			SCB_PRX_MATRIX[y][x].hpos = 0;
//			for(j = 0; j < 8; j++) {
//				SCB_PRX_MATRIX[y][x].penpal[j] = tile_palette[j];
//			}
//		}
//


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
	level_update_camera( level.start_x, level.start_y);
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

void level_draw() {
	int x, y, start_tile_x, start_tile_y, end_tile_x, end_tile_y, tile_index,
	parallax_offset_x,parallax_offset_y,prx_start_tile_x ,prx_start_tile_y ;
	int prx_end_tile_x;
	int prx_end_tile_y;
	int sprite_count = 0;
	TileInfo* tile_info;


	SCB_REHV_PAL* sprite;
	release_all_sprites();


	prev_sprite = NULL;
	first_sprite = NULL;
	/* Calcola quale porzione della mappa è visibile */
	start_tile_x = level.camera.x / TILE_SIZE;
	start_tile_y = level.camera.y / TILE_SIZE;

	end_tile_x = start_tile_x + TILES_X;
	end_tile_y = start_tile_y + TILES_Y;

	/* Limita ai bordi della mappa */
	if (end_tile_x > level.map_w) end_tile_x = level.map_w;
	if (end_tile_y > level.map_h) end_tile_y = level.map_h;

	// 1. COMINCIA CON LO SFONDO
	agSprBackground.penpal[0] = 0x09;
	first_sprite = &agSprBackground;  // Sfondo è il primo
	prev_sprite = first_sprite;



	//aggiunge le tile dello sfondo che si muove in parallasse orizzontale
//	parallax_offset_x = level.camera.x / 2;
//	parallax_offset_y = level.camera.y / 2;
//
//	// Calcola quali tile di parallasse sono visibili
//	prx_start_tile_x = parallax_offset_x / TILE_SIZE;
//	prx_start_tile_y = parallax_offset_y / TILE_SIZE;
//	prx_end_tile_x = prx_start_tile_x + TILES_X;
//	prx_end_tile_y = prx_start_tile_y + TILES_Y;
//
//	// Limita ai bordi della mappa parallasse
//	if (prx_end_tile_x > level.map_w) prx_end_tile_x = level.map_w;
//	if (prx_end_tile_y > level.map_h) prx_end_tile_y = level.map_h;

//	// Disegna le tile di parallasse
//	for (y = prx_start_tile_y; y < prx_end_tile_y; y++) {
//		for (x = prx_start_tile_x; x < prx_end_tile_x; x++) {
//			tile_index = level.bg_map[y*level.map_w+x];// BCKG_MAP[y][x];
//
//			if (tile_index != 0) {
//				int sprite_x = x - prx_start_tile_x;
//				int sprite_y = y - prx_start_tile_y;
//
//				// Posizione mondo della tile (senza effetto parallasse)
//				int world_x = x * TILE_SIZE;
//				int world_y = y * TILE_SIZE;
//
//				// Posizione schermo con effetto parallasse
//				int screen_x = world_x - parallax_offset_x;
//				int screen_y = world_y - parallax_offset_y;
//				tile_info = tileinfo_get(tile_index);
//				// Setup sprite parallasse
//				SCB_PRX_MATRIX[sprite_y][sprite_x].data =(unsigned char*)  tile_info->bitmap;//; (unsigned char*) LEVEL_1_PRX[effect_counter< EFFECT_TOGGLE_VALUE==0 ? tile_index-1:tile_index];
//
//				SCB_PRX_MATRIX[sprite_y][sprite_x].next = (void*)0;
//				//SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl1 = REHV | PACKED;
//
//				SCB_PRX_MATRIX[sprite_y][sprite_x].vpos = screen_y;
//				if(tile_info->is_mirrored)
//				{
//					SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
//					SCB_PRX_MATRIX[sprite_y][sprite_x].hpos = screen_x+TILE_SIZE-1;
//				}
//				else
//				{
//					SCB_PRX_MATRIX[sprite_y][sprite_x].hpos = screen_x;
//					SCB_PRX_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
//				}
//
//
//
//				// Controlla se la tile è visibile
//				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
//						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
//					continue;  // Tile non visibile
//				} else {
//					// Aggiungi alla lista
//					prev_sprite->next = &SCB_PRX_MATRIX[sprite_y][sprite_x];
//					prev_sprite = &SCB_PRX_MATRIX[sprite_y][sprite_x];
//					sprite_count++;
//				}
//			}
//		}
//	}
//
//
//
//	// ALBERI,PALME,FIORI
//	for (y = start_tile_y; y < end_tile_y; y++) {
//		for (x = start_tile_x; x < end_tile_x; x++) {
//			tile_index =  level.fg_map[y*level.map_w+x];//FG_MAP[y][x];
//			tile_info = tileinfo_get(tile_index);
//
//
//			if ( tile_info->type == TILE_BACKGROUND)//tile_info->type != TILE_EMPTY)//
//			{  // Background tiles
//				int sprite_x = x - start_tile_x;
//				int sprite_y = y - start_tile_y;
//
//				int world_x = x * TILE_SIZE;
//				int world_y = y * TILE_SIZE;
//				int screen_x = level_world_to_screen_x(world_x);
//				int screen_y = level_world_to_screen_y(world_y);
//
//
//				// Setup sprite background
//				SCB_MATRIX[sprite_y][sprite_x].data = (unsigned char*)  tile_info->bitmap;//  LEVEL_1_BACKGROUND[tile_index-100];
//				SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
//				SCB_MATRIX[sprite_y][sprite_x].next = (void*)0;  // Termina per ora
//
//				SCB_MATRIX[sprite_y][sprite_x].vpos = screen_y;
//
//				if(tile_info->is_mirrored)
//				{
//					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
//					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x+TILE_SIZE-1;
//				}
//				else
//				{
//					SCB_MATRIX[sprite_y][sprite_x].hpos = screen_x;
//					SCB_MATRIX[sprite_y][sprite_x].sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
//				}
//
//
//
//				// Aggiungi in level_draw(), prima di setup sprite:
//				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
//						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
//					continue;  // Tile non visibile, salta
//				}else{
//					// Aggiungi alla lista
//					prev_sprite->next = &SCB_MATRIX[sprite_y][sprite_x];
//					prev_sprite = &SCB_MATRIX[sprite_y][sprite_x];
//					sprite_count++;
//				}
//			}
//		}
//	}


	// PIATTAFORME E MURI

	for (y = start_tile_y; y < end_tile_y; y++) {
		for (x = start_tile_x; x < end_tile_x; x++) {
			tile_index =level.fg_map[y*level.map_w+x];//; FG_MAP[y][x];
			tile_info = tileinfo_get(tile_index);
			if (tile_info->type != TILE_EMPTY){//(tile_info->type == TILE_PLATFORM || tile_info->type == TILE_SOLID) {  // Foreground tiles
				int sprite_x = x - start_tile_x;
				int sprite_y = y - start_tile_y;

				int world_x = x * TILE_SIZE;
				int world_y = y * TILE_SIZE;
				int screen_x = (int)(world_x - level.camera.x);//;level_world_to_screen_x(world_x);
				int screen_y = (int)(world_y - level.camera.y);// level_world_to_screen_y(world_y);


				   sprite = get_free_sprite();
				                if(!sprite) return;  /* Pool esaurito */

				// Setup sprite background
				sprite->data = (unsigned char*)  tile_info->bitmap;//  LEVEL_1_BACKGROUND[tile_index-100];
				sprite->hpos = screen_x;
				sprite->next = (void*)0;  // Termina per ora

				sprite->vpos = screen_y ;
				if(tile_info->is_mirrored)
				{
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL| HFLIP;
					sprite->hpos = screen_x+TILE_SIZE-1;
				}
				else
				{
					sprite->hpos = screen_x;
					sprite->sprctl0 = tile_info->colorDepth | TYPE_NORMAL ;
				}




				// Aggiungi in level_draw(), prima di setup sprite:
//				if (screen_x + TILE_SIZE < 0 || screen_x >= SCREEN_WIDTH ||
//						screen_y + TILE_SIZE < 0 || screen_y >= SCREEN_HEIGHT) {
//					continue;  // Tile non visibile, salta
//				}
//				else
				{
					// Aggiungi alla lista
					prev_sprite->next = sprite;//&SCB_MATRIX[sprite_y][sprite_x];
					prev_sprite = sprite;//&SCB_MATRIX[sprite_y][sprite_x];
					//sprite_count++;
				}
			}
		}
	}

	// 4. AGGIUNGI IL PLAYER (ULTIMO, SOPRA TUTTO)
	player.visible_spc.sprite.next = (void*)0;
	prev_sprite->next = &player.visible_spc.sprite;

	// 5. DISEGNA TUTTA LA LISTA CON UNA SOLA CHIAMATA
	tgi_sprite(first_sprite);
	//sprite_count++;
	// Debug: mostra quanti sprite stai disegnando
	//printU16(sprite_count, 00, 0, 0x04);
}



void level_init_camera() {
	level.camera.x = 0;
	level.camera.y = 0;
	level.camera.width = SCREEN_WIDTH;
	level.camera.height = SCREEN_HEIGHT;

	/* Calcola il centro dello schermo */
	level.camera.center_x = level.camera.width >>1;/// 2;
	level.camera.center_y = level.camera.height >>2;/// 2;

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



//void level_update_camera()
//{
//    static s16 max_cam_step;
//    static s16 player_screen_x;
//    static s16 target_cam_x;
//    static s16 delta;
//    static u16 level_width_px, level_height_px;
//    static s16 dead_left;
//    static s16 dead_right;
//    static s16 cam_min_x, cam_max_x;
//    static s16 screen_x ;
//    #define DEAD_LEFT   60
//    #define DEAD_RIGHT  100
//    #define CAM_MARGIN  1
//
//    level_width_px  = level.map_w * TILE_SIZE;
//    level_height_px = level.map_h * TILE_SIZE;
//
//    cam_min_x = 0;
//    cam_max_x = level_width_px - level.camera.width;
//
//    dead_left  = DEAD_LEFT;
//    dead_right = DEAD_RIGHT;
//
//    if (level.camera.x <= cam_min_x)
//        dead_left = 0;
//
//    if (level.camera.x >= cam_max_x)
//        dead_right = level.camera.width;
//
//    player_screen_x = player.x - level.camera.x;
//    target_cam_x = level.camera.x;
//
//    if (player_screen_x > dead_right)
//        target_cam_x += (player_screen_x - dead_right);
//    else if (player_screen_x < dead_left)
//        target_cam_x += (player_screen_x - dead_left);
//
//    delta = target_cam_x - level.camera.x;
//    max_cam_step = ABS(player.vx) + CAM_MARGIN;
//
//    if (delta >  max_cam_step) delta =  max_cam_step;
//    if (delta < -max_cam_step) delta = -max_cam_step;
//
//    level.camera.x += delta;
//
//    if (level.camera.x < cam_min_x)
//        level.camera.x = cam_min_x;
//
//    if (level.camera.x > cam_max_x)
//        level.camera.x = cam_max_x;
//
//    /* --- VERTICALE COME PRIMA --- */
//
//    level.camera.y = player.y - (level.camera.height / 2) + 16;
//
//    if (level.camera.y < 0)
//        level.camera.y = 0;
//
//    if (level.camera.y + level.camera.height > level_height_px)
//        level.camera.y = level_height_px - level.camera.height;
//
//    /* --- CLAMP PLAYER IN SCREEN SPACE --- */
//
//   screen_x = player.x - level.camera.x;
//
//    if (screen_x < 0)
//        screen_x = 0;
//
//    if (screen_x > level.camera.width - player.width)
//        screen_x = level.camera.width - player.width;
//
//    player.visible_spc.sprite.hpos = screen_x;
//    player.visible_spc.sprite.vpos = (int)(player.y - level.camera.y);
//}


/* Converti coordinate world a screen X */
int level_world_to_screen_x( int world_x) {
	return (int)(world_x - level.camera.x);
}

/* Converti coordinate world a screen Y */
int level_world_to_screen_y( int world_y) {
	return (int)(world_y - level.camera.y);
}



