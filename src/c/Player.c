/*
 * Player.c
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */


/* player.c */
#include "Player.h"

extern Player player;
void player_init(){
	//lynx_load(2);

	player.x =PLAYER_WIDTH*4;//+PLAYER_HEIGHT;
	player.y =PLAYER_HEIGHT*2;//(MAP_HEIGHT * TILE_SIZE)-TILE_SIZE*11;//va infondo alla mappa, considerato che lo y0 e in alto

	player.width=PLAYER_WIDTH;
	player.height=PLAYER_HEIGHT;
	player.width_half=PLAYER_WIDTH>>1;
	player.height_half=PLAYER_HEIGHT>>1;

	//quindi sottrae l'altezza della tail del terreno e l'altezza della tail del personaggio
	player.vx = 0;
	player.vy = 0;
	player.ground_level = 0;//(MAP_HEIGHT * TILE_SIZE)-TILE_SIZE*2;


	player.state = PLAYER_IDLE;
	player.direction = DIR_RIGHT;
	player.current_frame = 0;
	player.animation_timer =  -100;
	player.animation_speed = 0;

	player.health = 100;
	player.lives = 3;
	player.score = 0;

	//	player.width = 4;  // Dimensione sprite
	//	player.height = 6;
	player.jump_force = 12;
	player.gravity = 1;
	player.is_grounded = 1;
	player.is_jumping = 0;

	player.invincibility_timer = 0;
	player.shoot_cooldown = 0;

	// Inizializza lo SCB
	player.visible_spc.sprite.sprctl0 = BPP_4 | TYPE_NORMAL;
	player.visible_spc.sprite.sprctl1 = REHV ;//| PACKED
	player.visible_spc.sprite.sprcoll = NO_COLLIDE;//32;
	player.visible_spc.sprite.next = (void*)0;
	player.visible_spc.sprite.data = sonic_idle_tiles[0];  // Frame iniziale
	player.visible_spc.sprite.hpos = 0;
	player.visible_spc.sprite.vpos = 0;
	player.visible_spc.sprite.hsize = 0x0100*SCALE/SCALE_DIVIDER;//*5/6;  // 1:1 scale
	player.visible_spc.sprite.vsize = 0x0100*SCALE/SCALE_DIVIDER;//*5/6;



	/* Inizializza palette - senza array temporaneo */
	player.visible_spc.sprite.penpal[0] = 0x01;
	player.visible_spc.sprite.penpal[1] = 0x23;
	player.visible_spc.sprite.penpal[2] = 0x45;
	player.visible_spc.sprite.penpal[3] = 0x67;
	player.visible_spc.sprite.penpal[4] = 0x89;
	player.visible_spc.sprite.penpal[5] = 0xAB;
	player.visible_spc.sprite.penpal[6] = 0xCD;
	player.visible_spc.sprite.penpal[7] = 0xEF;



	// Setup animazioni
	player.idle_frames = sonic_idle_tiles;
	player.walk_frames = sonic_run_tiles;  // Per ora stesso set
	player.brake_frames = sonic_brake_tiles;  // Per ora stesso set
	player.jump_frames = sonic_jump_tiles;  // Per ora stesso set


	player.idle_frame_count = 4;         // Solo il primo frame per idle
	player.walk_frame_count = 4;         // 4 frame per camminata
	player.brake_frame_count = 2;         // 4 frame per camminata
	player.jump_frame_count = 2;         // 2 frame per salto


	player.xOnSprite=0;

}


// Gestione animazione
void  player_animate() {
	int frame_count = 0;
	char** current_animation =  (void*) 0;
	player.animation_timer++;

	//printU8As2Nibble(player.sprite.sprcoll,10,10,1);
	// Seleziona animazione in base allo stato
	switch(player.state) {
	case PLAYER_IDLE:
		current_animation = player.idle_frames;
		frame_count = player.idle_frame_count;
		player.animation_speed = 10;  // Più lento
		//player.sprite.data = (unsigned char*)current_animation[0];
		break;
	case PLAYER_WALKING:
		if(ABS(player.vx)<MAX_WALK_SPEED){
			current_animation = player.jump_frames;
			frame_count = player.jump_frame_count;

		}
		else{

			current_animation = player.walk_frames;
			frame_count = player.walk_frame_count;

		}
		player.animation_speed = 2;  // Normale
		break;
	case PLAYER_JUMPING:
	case PLAYER_RUN_JUMPING:
		current_animation = player.jump_frames;
		frame_count = player.jump_frame_count;
		player.animation_speed = 2;  // Più veloce
		break;
	case PLAYER_BRAKING:
		current_animation = player.brake_frames;
		frame_count = player.brake_frame_count;
		player.animation_speed = 4;  // Più veloce
		break;
	default:
		current_animation = player.idle_frames;
		frame_count = player.idle_frame_count;
		break;
	}

	// Cambia frame se è il momento
	if(player.animation_timer >= player.animation_speed)
	{
		player.animation_timer = 0;
		player.current_frame = (player.current_frame + 1 ) % frame_count;
		player.visible_spc.sprite.data = (unsigned char*)current_animation[player.current_frame];

		//resetto il timer impazienza di sonic, dopo che ha battuto il piedino
		if(player.state==PLAYER_IDLE && player.current_frame == player.idle_frame_count-1)player.animation_timer=WAIT_BEFORE_IDLE_ANIMATION;
	}
	else if(player.animation_timer < 0 )
		player.visible_spc.sprite.data = (unsigned char*)current_animation[0];



	// Gestione flip orizzontale in base alla direzione
	if(player.direction == DIR_LEFT) {
		// Imposta flip orizzontale se supportato dal tuo hardware
		player.visible_spc.sprite.sprctl0 =  BPP_4 | TYPE_NORMAL | HFLIP;  // Se c'è un flag per reverse
	} else {
		player.visible_spc.sprite.sprctl0 = BPP_4 | TYPE_NORMAL ;
	}



}








//
//void check_world_collision(void)
//{
//    /* DICHIARAZIONE DI TUTTE LE VARIABILI ALL'INIZIO */
//    s16 new_x;
//    s16 new_y;
//    s16 old_x;
//    s16 old_y;
//    s16 old_feet;
//    s16 left_x;
//    s16 new_feet;
//    s16 right_x;
//    s16 tx;
//    s16 ty;
//    TileInfo* ti;
//    u8 local_x;
//    u8 h;
//    s16 ground_y;
//    s16 i;
//    s16 dir;
//    s16 check_x;
//    s16 player_left;
//    s16 player_right;
//    s16 player_top;
//    s16 player_bottom;
//    s16 tile_left;
//    s16 tile_right;
//    s16 tile_top;
//    s16 tile_bottom;
//    s16 start_tx;
//    s16 end_tx;
//    s16 start_ty;
//    s16 end_ty;
//    s16 check_points[3];
//    s16 steps;
//    s16 step;
//    s16 current_x;
//    s16 current_y;
//    s16 current_left;
//    s16 current_right;
//    s16 test_tile;
//    s16 check_ty_start;
//    s16 check_ty_end;
//    s16 temp;
//    s16 platform_checks[5];
//    s16 check_ty;
//
//    /* INIZIALIZZAZIONE */
//    old_x = player.x;
//    old_y = player.y;
//    old_feet = old_y + player.height;
//
//    new_x = old_x + player.vx;
//    new_y = old_y + player.vy;
//    new_feet = new_y + player.height;
//
//    player.is_grounded = 0;
//
//    /* ============================================== */
//    /* 1. COLLISIONI ORIZZONTALI (ENTRAMBE LE DIREZIONI) */
//    /* ============================================== */
//    if (player.vx != 0) {
//        dir = (player.vx > 0) ? 1 : -1;
//        steps = (player.vx < 0) ? -player.vx : player.vx;
//
//        /* Controlla ogni pixel nel percorso */
//        for (step = 1; step <= steps; step++) {
//            current_x = old_x + (dir * step);
//            current_left = current_x - player.width_half;
//            current_right = current_x + player.width_half - 1;
//
//            /* A seconda della direzione, controlla il bordo corretto */
//            if (dir > 0) {
//                /* DESTRA: controlla bordo destro */
//                test_tile = current_right >> TILE_SHIFT;
//            } else {
//                /* SINISTRA: controlla bordo sinistro */
//                test_tile = current_left >> TILE_SHIFT;
//            }
//
//            /* Controlla 3 punti verticali (alto, centro, basso) */
//            check_points[0] = old_y;
//            check_points[1] = old_y + (player.height_half);
//            check_points[2] = old_y + player.height - 1;
//
//            for (i = 0; i < 3; i++) {
//                ty = check_points[i] >> TILE_SHIFT;
//
//                if (test_tile < 0 || test_tile >= level.map_w ||
//                    ty < 0 || ty >= level.map_h)
//                    continue;
//
//                ti = tileinfo_get(level_get_tile_abs(test_tile, ty));
//
//                if (ti->type == TILE_SOLID) {
//                    if (dir > 0) {
//                        /* Collisione a DESTRA */
//                        new_x = (test_tile <<TILE_SHIFT) - player.width_half - 1;
//                    } else {
//                        /* Collisione a SINISTRA */
//                        new_x = ((test_tile + 1) <<TILE_SHIFT) + player.width_half;
//                    }
//                    player.vx = 0;
//                    goto h_done;
//                }
//            }
//        }
//    }
//
//h_done:
//
//    /* ============================================== */
//    /* 2. COLLISIONI VERTICALI SOLID */
//    /* ============================================== */
//    if (player.vy != 0) {
//        dir = (player.vy > 0) ? 1 : -1;
//        steps = (player.vy < 0) ? -player.vy : player.vy;
//
//        /* Controlla ogni pixel nel percorso */
//        for (step = 1; step <= steps; step++) {
//            current_y = old_y + (dir * step);
//
//            /* A seconda della direzione, controlla il bordo corretto */
//            if (dir > 0) {
//                /* CADUTA: controlla i piedi */
//                test_tile = (current_y + player.height - 1) >> TILE_SHIFT;
//            } else {
//                /* SALTO: controlla la testa */
//                test_tile = current_y >> TILE_SHIFT;
//            }
//
//            /* Controlla 3 punti orizzontali (sinistra, centro, destra) */
//            check_points[0] = new_x - player.width_half;
//            check_points[1] = new_x;
//            check_points[2] = new_x + player.width_half - 1;
//
//            for (i = 0; i < 3; i++) {
//                tx = check_points[i] >> TILE_SHIFT;
//
//                if (tx < 0 || tx >= level.map_w ||
//                    test_tile < 0 || test_tile >= level.map_h)
//                    continue;
//
//                ti = tileinfo_get(level_get_tile_abs(tx, test_tile));
//
//                if (ti->type == TILE_SOLID) {
//                    if (dir > 0) {
//                        /* Atterraggio su tile solida */
//                        new_y = (test_tile <<TILE_SHIFT) - player.height;
//                        player.vy = 0;
//                        player.is_grounded = 1;
//                        player.is_jumping = 0;
//                    } else {
//                        /* Colpisce il soffitto */
//                        new_y = ((test_tile + 1) <<TILE_SHIFT);
//                        player.vy = 0;
//                    }
//                    goto v_done;
//                }
//            }
//        }
//    }
//
//v_done:
//
//    /* ============================================== */
//    /* 3. COLLISIONI CON PIATTAFORME (FIXED) */
//    /* ============================================== */
//    if (player.vy >= 0) {
//        /* Ricalcola new_feet dopo eventuali correzioni */
//        new_feet = new_y + player.height;
//
//        /* Controlla la tile direttamente sotto i piedi */
//        ty = (new_feet >> TILE_SHIFT);
//
//        left_x = new_x - player.width_half;
//        right_x = new_x + player.width_half - 1;
//
//        /* Controlla anche la tile sopra, nel caso di movimento veloce */
//        check_ty_start = ((old_feet) >> TILE_SHIFT);
//        check_ty_end = ty;
//
//        /* Assicurati che start <= end */
//        if (check_ty_start > check_ty_end) {
//            temp = check_ty_start;
//            check_ty_start = check_ty_end;
//            check_ty_end = temp;
//        }
//
//        /* Controlla ogni tile verticale nel percorso */
//        for (check_ty = check_ty_start; check_ty <= check_ty_end; check_ty++) {
//            start_tx = (left_x >> TILE_SHIFT);
//            end_tx = (right_x >> TILE_SHIFT);
//
//            for (tx = start_tx; tx <= end_tx; tx++) {
//                if (tx < 0 || tx >= level.map_w || check_ty < 0 || check_ty >= level.map_h)
//                    continue;
//
//                ti = tileinfo_get(level_get_tile_abs(tx, check_ty));
//
//                if (ti->type == TILE_PLATFORM) {
//                    /* Controlla 5 punti lungo la larghezza */
//                    platform_checks[0] = left_x;
//                    platform_checks[1] = left_x + (player.width_half >> 1);
//                    platform_checks[2] = new_x;
//                    platform_checks[3] = right_x - (player.width_half >> 1);
//                    platform_checks[4] = right_x;
//
//                    for (i = 0; i < 5; i++) {
//                        check_x = platform_checks[i];
//
//                        /* Verifica che questo punto sia nella tile corrente */
//                        if ((check_x >> TILE_SHIFT) != tx) continue;
//
//                        local_x = check_x & 0x0F;
//                        h = ti->height_map[local_x];
//
//                        if (h > 0) {
//                            ground_y = (check_ty <<TILE_SHIFT) + (15 - h);
//
//                            /* CONDIZIONE CORRETTA per piattaforme:
//                               1. I piedi erano STRETTAMENTE SOPRA la piattaforma
//                               2. I piedi sono ORA SOTTO o SULLA piattaforma
//                            */
//                            if (old_feet < ground_y) {
//                                /* Prima era sopra: continua */
//                            } else if (new_feet >= ground_y) {
//                                /* Ora è sotto/sulla: ATTERRA */
//                                new_y = ground_y - player.height;
//                                player.vy = 0;
//                                player.is_grounded = 1;
//                                player.is_jumping = 0;
//                                goto p_done;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//p_done:
//
//    /* ============================================== */
//    /* 4. APPLICA POSIZIONE FINALE */
//    /* ============================================== */
//    player.x = new_x;
//    player.y = new_y;
//}




void player_handle_user_input(u8 key){

	switch (key & (AG_JOY_A|AG_JOY_LEFT|AG_JOY_RIGHT)) {
	case AG_JOY_A:
		/* Salto verticale */
		if (!player.is_jumping && player.is_grounded) {
			player.is_jumping = 1;
			player.is_grounded = 0;
			if(player.state !=PLAYER_JUMPING)player.animation_timer=player.animation_speed;
			player.state = PLAYER_JUMPING;
			player.vy = DEFAULT_JUMP_POWER;  // Salto normale
		}
		break;

	case AG_JOY_LEFT:
		/* Cammina a sinistra */
		if(player.vx>0)player.vx=0;
		if(ABS(player.vx)<MAX_RUN_SPEED)player.vx--;
		//player.vx = -1 * DEFAULT_SPEED ;
		player.direction = DIR_LEFT;
		if (player.is_grounded) {
			if(player.state !=PLAYER_WALKING)player.animation_timer=player.animation_speed;
			player.state = PLAYER_WALKING;
		}
		break;
	case AG_JOY_RIGHT:
		/* Cammina a destra */

		if(player.vx<0)player.vx=0;
		if(ABS(player.vx)<MAX_RUN_SPEED)player.vx++;

		player.direction = DIR_RIGHT;
		if (player.is_grounded) {
			if(player.state !=PLAYER_WALKING)player.animation_timer=player.animation_speed;
			player.state = PLAYER_WALKING;
		}
		break;




	case AG_JOY_A | AG_JOY_LEFT:
	/* Salto con rincorsa vs SX*/
	if (!player.is_jumping && player.is_grounded) {
		player.is_jumping = 1;
		player.is_grounded = 0;
		if(player.state !=PLAYER_JUMPING)player.animation_timer=player.animation_speed;
		player.state = PLAYER_JUMPING;
		player.vy = DEFAULT_JUMP_POWER;
		/* Cammina a sinistra */
		if(player.vx>0)player.vx=0;
		if(ABS(player.vx)<MAX_RUN_SPEED)player.vx--;
	}
	break;
	case AG_JOY_A | AG_JOY_RIGHT:
	/* Salto con rincorsa vs DX*/
	if (!player.is_jumping && player.is_grounded) {
		player.is_jumping = 1;
		player.is_grounded = 0;
		if(player.state !=PLAYER_JUMPING)player.animation_timer=player.animation_speed;
		player.state = PLAYER_JUMPING;
		player.vy = DEFAULT_JUMP_POWER;
		if(player.vx<0)player.vx=0;
		if(ABS(player.vx)<MAX_RUN_SPEED)player.vx++;
	}
	break;

	default:
		if (player.is_grounded && !player.is_jumping)
		{


			player.vx=0;player.vy=0;
			if(player.state != PLAYER_IDLE)
			{
				//caso fermo
				player.state = PLAYER_IDLE;
				player.current_frame = 0;
				player.animation_timer = WAIT_BEFORE_IDLE_ANIMATION;//tempo di attesa prima di fare battere il piede a sonic, non è poi così impaziente :)
			}
		}
		break;
	}

}
