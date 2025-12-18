/*
 * Player.c
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */


/* player.c */
#include "Player.h"
extern Level level;

extern Player player;
void player_init(){
	lynx_load(2);

	player.x =TILE_SIZE+PLAYER_HEIGHT;
	player.y = 0;//(MAP_HEIGHT * TILE_SIZE)-TILE_SIZE*2;//va infondo alla mappa, considerato che lo y0 e in alto

	player.width=PLAYER_WIDTH;
	player.height=PLAYER_HEIGHT;


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
		current_animation = player.walk_frames;
		frame_count = player.walk_frame_count;
		player.animation_speed = 4;  // Normale
		break;
	case PLAYER_JUMPING:
	case PLAYER_RUN_JUMPING:
		current_animation = player.jump_frames;
		frame_count = player.jump_frame_count;
		player.animation_speed = 4;  // Più veloce
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
		s16 check_x = (player.vx > 0) ? (new_x + player.width/2) : (new_x - player.width/2);
		tile_x = check_x / TILE_SIZE;
		tile_y = (player.y + player.height/2) / TILE_SIZE;

		if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w)
		{
			tile_index =  level.fg_map[tile_y*level.map_w+tile_x];//  level_foregound_map[tile_y][tile_x];
			tile_info = tileinfo_get(tile_index);

			if (tile_info->type == TILE_SOLID)
			{
				if (player.vx > 0) {
					player.x = tile_x * TILE_SIZE - (player.width/2);
				} else {
					player.x = (tile_x + 1) * TILE_SIZE + (player.width/2);
				}
				player.vx = 0;
				new_x = player.x;
			}
			else if(tile_info->type == TILE_PLATFORM)
			{
				s16 feet_y = new_y + player.height;

				u8 local_x = new_x - (tile_x * TILE_SIZE);
				u8 terrain_height = tileinfo_get_height_at(tile_info, local_x);


				if (terrain_height > 0) {
					/* IMPORTANTE:
					   - terrain_height = 15: cima della tile (y = tile_top)
					   - terrain_height = 0: fondo della tile (y = tile_top + 15)
					   ground_y = tile_top + (15 - terrain_height)
					 */
					u16 ground_y = (tile_y * TILE_SIZE) + (15 - terrain_height);

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
	}





	/* 2. COLLISIONI VERTICALI */
	if (player.vy != 0) {
		s16 center_x = new_x;

		if (player.vy > 0) {
			/* CADUTA - Controlla sotto */
			s16 feet_y = new_y + player.height;
			tile_x = center_x / TILE_SIZE;
			tile_y = feet_y / TILE_SIZE;

			if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w) {
				tile_index =level.fg_map[tile_y*level.map_w+tile_x];// level_foregound_map[tile_y][tile_x];
				tile_info = tileinfo_get(tile_index);

				if (tile_info->type == TILE_SOLID) {
					player.y = tile_y * TILE_SIZE - player.height;
					player.is_grounded = 1;
					player.is_jumping = 0;
					player.vy = 0;
					new_y = player.y;
				}
				else if (tile_info->type == TILE_PLATFORM) {
					u8 local_x = center_x - (tile_x * TILE_SIZE);
					u8 terrain_height = tileinfo_get_height_at(tile_info, local_x); // 0-15

					if (terrain_height > 0) {
						/* IMPORTANTE:
                           - terrain_height = 15: cima della tile (y = tile_top)
                           - terrain_height = 0: fondo della tile (y = tile_top + 15)

                           ground_y = tile_top + (15 - terrain_height)
						 */
						u16 ground_y = (tile_y * TILE_SIZE) + (15 - terrain_height);

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
			tile_x = center_x / TILE_SIZE;
			tile_y = head_y / TILE_SIZE;

			if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w) {
				tile_index = level.fg_map[tile_y*level.map_w+tile_x];//level_foregound_map[tile_y][tile_x];
				tile_info = tileinfo_get(tile_index);

				if (tile_info->type == TILE_SOLID) {
					player.y = (tile_y + 1) * TILE_SIZE;
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

void player_adjust_to_ground(void) {
	s16 tile_x ,tile_y;
	if (!player.is_grounded) return;

	tile_x = player.x / TILE_SIZE;
	tile_y = (player.y + player.height) / TILE_SIZE;

	if (tile_y >= 0 && tile_y < level.map_h && tile_x >= 0 && tile_x < level.map_w) {
		s16 tile_index =level.fg_map[tile_y*level.map_w+tile_x];// level_foregound_map[tile_y][tile_x];
		TileInfo* tile_info = tileinfo_get(tile_index);

		if (tile_info->type == TILE_PLATFORM) {
			u8 local_x = player.x - (tile_x * TILE_SIZE);
			u8 terrain_height = tileinfo_get_height_at(tile_info, local_x);

			if (terrain_height > 0) {
				u16 correct_y = (tile_y * TILE_SIZE) + (15 - terrain_height) - player.height;

				if (player.y != correct_y) {
					player.y = correct_y;
				}
			}
		}
	}
}

void player_update() {
	if(player.x < TILE_SIZE) player.x = TILE_SIZE;
	else if(player.x > level.end_x - (TILE_SIZE)) player.x = level.end_x - (TILE_SIZE);

	/* Applica gravità */
	if(!player.is_grounded) {
		player.vy += player.gravity;
		if(player.vy > 16) player.vy = 16;
	}

	check_world_collision();

	//	/* Se il player è a terra, regola l'altezza in base alla piattaforma */
	if (player.is_grounded) {
		player_adjust_to_ground();
	}
}

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
		player.vx = -1 * DEFAULT_SPEED ;
		player.direction = DIR_LEFT;
		if (player.is_grounded) {
			if(player.state !=PLAYER_WALKING)player.animation_timer=player.animation_speed;
			player.state = PLAYER_WALKING;
		}
		break;
	case AG_JOY_RIGHT:
		/* Cammina a destra */
		player.vx = DEFAULT_SPEED;
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
		player.vx = -1*DEFAULT_SPEED;
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
		player.vx = DEFAULT_SPEED;
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
