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
	player.visible_spc.sprite.sprctl0 = BPP_3 | TYPE_NORMAL;
	player.visible_spc.sprite.sprctl1 = REHV ;//| PACKED
	player.visible_spc.sprite.sprcoll = NO_COLLIDE;//32;
	player.visible_spc.sprite.next = (void*)0;
	player.visible_spc.sprite.data = sonic_idle_tiles[0];  // Frame iniziale
	player.visible_spc.sprite.hpos = 0;
	player.visible_spc.sprite.vpos = 0;
	player.visible_spc.sprite.hsize = 0x0100;//*5/6;  // 1:1 scale
	player.visible_spc.sprite.vsize = 0x0100;//*5/6;



	/* Inizializza palette - senza array temporaneo */
	player.visible_spc.sprite.penpal[0] = 0x01;
	player.visible_spc.sprite.penpal[1] = 0x4D;
	player.visible_spc.sprite.penpal[2] = 0xB8;
	player.visible_spc.sprite.penpal[3] = 0x2F;
	//	player.visible_spc.sprite.penpal[4] = 0x89;
	//	player.visible_spc.sprite.penpal[5] = 0xAB;
	//	player.visible_spc.sprite.penpal[6] = 0xCD;
	//	player.visible_spc.sprite.penpal[7] = 0xEF;



	// Setup animazioni
	player.idle_frames = sonic_idle_tiles;
	player.walk_frames = sonic_run_tiles;  // Per ora stesso set
	player.brake_frames = sonic_brake_tiles;  // Per ora stesso set
	player.jump_frames = sonic_jump_tiles;  // Per ora stesso set


	player.idle_frame_count = 5;         // Solo il primo frame per idle
	player.walk_frame_count = 4;         // 4 frame per camminata
	player.brake_frame_count = 1;         // 4 frame per camminata
	player.jump_frame_count = 6;         // 2 frame per salto


	player.patienceCounter=8;



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
		player.animation_speed = 3;  // Più lento
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
		player.animation_speed = 1;  // Normale
		break;
	case PLAYER_JUMPING:
	case PLAYER_RUN_JUMPING:
		current_animation = player.jump_frames;
		frame_count = player.jump_frame_count;
		player.animation_speed = 1;  // Più veloce
		break;
	case PLAYER_BRAKING:
		current_animation = player.brake_frames;
		frame_count = player.brake_frame_count;
		player.animation_speed = 1;  // Più veloce
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



		//resetto il timer impazienza di sonic, dopo che ha battuto il piedino
		if(player.state==PLAYER_IDLE && player.current_frame > 2){//player.idle_frame_count-2

			if(player.patienceCounter != 0)
			{
				if(player.current_frame == 3 )
					player.current_frame = 4;
				else
					player.current_frame = 3;

				player.patienceCounter--;
			}

			else
			{
				player.patienceCounter = 8;
				player.current_frame = 0;
				player.animation_timer = WAIT_BEFORE_IDLE_ANIMATION;
			}
		}else{

			player.current_frame++;

			if (player.current_frame >= frame_count) {
				player.current_frame = 0;
			}

		}



		player.visible_spc.sprite.data = (unsigned char*)current_animation[player.current_frame];

	}
	else if(player.animation_timer < 0 )
		player.visible_spc.sprite.data = (unsigned char*)current_animation[0];



	// Gestione flip orizzontale in base alla direzione
	if(player.direction == DIR_LEFT) {
		// Imposta flip orizzontale se supportato dal tuo hardware
		player.visible_spc.sprite.sprctl0 =  BPP_3 | TYPE_NORMAL | HFLIP;  // Se c'è un flag per reverse
	} else {
		player.visible_spc.sprite.sprctl0 = BPP_3 | TYPE_NORMAL ;
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


