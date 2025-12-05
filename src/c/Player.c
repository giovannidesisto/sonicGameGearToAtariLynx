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
	player.x = TILE_SIZE;
	player.y = (MAP_HEIGHT * TILE_SIZE)-TILE_SIZE/2;
	player.vx = 0;
	player.vy = 0;
	player.ground_level = (MAP_HEIGHT * TILE_SIZE)-TILE_SIZE/2;


	player.state = PLAYER_IDLE;
	player.direction = DIR_RIGHT;
	player.current_frame = 0;
	player.animation_timer =  -100;
	player.animation_speed = 0;

	player.health = 100;
	player.lives = 3;
	player.score = 0;

	player.width = 4;  // Dimensione sprite
	player.height = 6;
	player.jump_force = 12;
	player.gravity = 1;
	player.is_grounded = 1;
	player.is_jumping = 0;

	player.invincibility_timer = 0;
	player.shoot_cooldown = 0;

	// Inizializza lo SCB
	player.sprite.sprctl0 = BPP_4 | TYPE_NORMAL;
	player.sprite.sprctl1 = REHV | PACKED;
	player.sprite.sprcoll = 0;
	player.sprite.next = (void*)0;
	player.sprite.data = sonic_idle_tiles[0];  // Frame iniziale
	player.sprite.hpos = 0;
	player.sprite.vpos = 0;
	player.sprite.hsize = 0x0100;  // 1:1 scale
	player.sprite.vsize = 0x0100;

	/* Inizializza palette - senza array temporaneo */
	player.sprite.penpal[0] = 0x01;
	player.sprite.penpal[1] = 0x23;
	player.sprite.penpal[2] = 0x45;
	player.sprite.penpal[3] = 0x67;
	player.sprite.penpal[4] = 0x89;
	player.sprite.penpal[5] = 0xAB;
	player.sprite.penpal[6] = 0xCD;
	player.sprite.penpal[7] = 0xEF;

	// Setup animazioni
	player.idle_frames = sonic_idle_tiles;
	player.walk_frames = sonic_run_tiles;  // Per ora stesso set
	player.brake_frames = sonic_brake_tiles;  // Per ora stesso set
	player.jump_frames = sonic_jump_tiles;  // Per ora stesso set


	player.idle_frame_count = 4;         // Solo il primo frame per idle
	player.walk_frame_count = 4;         // 4 frame per camminata
	player.brake_frame_count = 2;         // 4 frame per camminata
	player.jump_frame_count = 2;         // 2 frame per salto
}

// Aggiorna la posizione dello sprite
void player_update_sprite_position() {
	// Converti coordinate world in coordinate schermo
	// (dipende dal tuo sistema di coordinate)
	player.sprite.hpos = (u16)player.x;
	player.sprite.vpos = (u16)player.y;
}

// Gestione animazione
 void  player_animate() {
	int frame_count = 0;
	char** current_animation =  (void*) 0;
	player.animation_timer++;


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
		player.sprite.data = (unsigned char*)current_animation[player.current_frame];

		//resetto il timer impazienza di sonic, dopo che ha battuto il piedino
		if(player.state==PLAYER_IDLE && player.current_frame == player.idle_frame_count-1)player.animation_timer=WAIT_BEFORE_IDLE_ANIMATION;
	}
	else if(player.animation_timer < 0 )
				player.sprite.data = (unsigned char*)current_animation[0];



	// Gestione flip orizzontale in base alla direzione
	if(player.direction == DIR_LEFT) {
		// Imposta flip orizzontale se supportato dal tuo hardware
		player.sprite.sprctl0 =  BPP_4 | TYPE_NORMAL | HFLIP;  // Se c'è un flag per reverse
	} else {
		player.sprite.sprctl0 = BPP_4 | TYPE_NORMAL ;
	}




}

// Aggiorna il player (chiamato ogni frame)
void  player_update() {//, Level* level

	static s16 new_x,new_y;
	/* Controlla collisioni con il livello */

	new_x = player.x + player.vx;
	new_y = player.y + player.vy;


	/**
	 * TODO da inserire nel controllo collisioni?
	 */
	//controllo per non uscire dallo schermo
	//limite bordo SX
	if(player.x < TILE_SIZE) player.x=TILE_SIZE;
	else
	//limite bordo DX
	if(player.x > level.end_x - (TILE_SIZE))player.x=level.end_x - (TILE_SIZE);

	// Applica gravità
	if(!player.is_grounded) {
		player.vy += player.gravity;
		if(player.vy > 16) player.vy = 16;  // Velocità massima di caduta
	}

	/**
	 * TODO verificare collisioni
	 */

	// Aggiorna posizione
	player.x += player.vx;
	player.y += player.vy;


	// Controlla se è a terra
	if(player.y > player.ground_level ) {
		player.y = player.ground_level;
		player.vy = 0;


		//quando atterra, se arriva da un salto angolato,
		if(player.vx!=0)
			player.state = PLAYER_BRAKING;



		player.is_grounded = 1;
		player.is_jumping = 0;

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


				if(player.state == PLAYER_WALKING && player.vx !=0)
				{
					player.state = PLAYER_BRAKING;
				}
				else
				{
					//frena con scivolata quando sta correndo e si mollano i tasti
					if(player.state == PLAYER_BRAKING ){
						if(player.vx > 0)player.vx--;
						else player.vx++;
					}

					if(player.state != PLAYER_IDLE && player.vx==0 &&  player.vy==0 )
					{
						//caso fermo
						player.state = PLAYER_IDLE;
						player.current_frame = 0;
						player.animation_timer = WAIT_BEFORE_IDLE_ANIMATION;//tempo di attesa prima di fare battere il piede a sonic, non è poi così impaziente :)
					}
				}
			}
			break;
		}

}
