/*
 * Player.c
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */


/* player.c */
#include "Player.h"
#include "Level.h"

//#define GROUND_LEVEL 144//80  // base dello schermo
#define PLAYER_START_X 80//80
//static void __fastcall__ player_animate();

extern Level level;
extern Player player;
// Inizializzazione del player
void player_init(){//, u16 x, u16 y) {
	//player.level = level;
	player.x = TILE_SIZE;// level.start_x+PLAYER_START_X;
	player.y = (MAP_HEIGHT * TILE_SIZE);//level.start_y;//-16-8;
	player.vx = 0;
	player.vy = 0;
	player.ground_level = (MAP_HEIGHT * TILE_SIZE);//level.start_y;//-16-8 ;


	player.state = PLAYER_IDLE;
	player.direction = DIR_RIGHT;
	player.current_frame = 0;
	player.animation_timer = 0;
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
	player.sprite.data = player_frames[0];  // Frame iniziale
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
	player.idle_frames = player_frames;
	player.walk_frames = player_frames;  // Per ora stesso set
	player.jump_frames = player_frames;  // Per ora stesso set
	player.idle_frame_count = 1;         // Solo il primo frame per idle
	player.walk_frame_count = 4;         // 4 frame per camminata
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
	u8 frame_count = 0;
	char** current_animation =  (void*) 0;
	player.animation_timer++;




	// Seleziona animazione in base allo stato
	switch(player.state) {
	case PLAYER_IDLE:
		current_animation = player.idle_frames;
		frame_count = player.idle_frame_count;
		player.animation_speed = 4;  // Più lento
		break;
	case PLAYER_WALKING:
		current_animation = player.walk_frames;
		frame_count = player.walk_frame_count;
		player.animation_speed = 0;  // Normale
		break;
	case PLAYER_JUMPING:
		current_animation = player.jump_frames;
		frame_count = player.jump_frame_count;
		player.animation_speed = 2;  // Più veloce
		break;
	default:
		current_animation = player.idle_frames;
		frame_count = player.idle_frame_count;
		break;
	}

	// Cambia frame se è il momento
	if(player.animation_timer >= player.animation_speed) {
		player.animation_timer = 0;
		player.current_frame = (player.current_frame + 1) % frame_count;
		player.sprite.data = (unsigned char*)current_animation[player.current_frame];
	}

	// Gestione flip orizzontale in base alla direzione
	if(player.direction == DIR_LEFT) {
		// Imposta flip orizzontale se supportato dal tuo hardware
		player.sprite.sprctl0 =  BPP_4 | TYPE_NORMAL | HFLIP;  // Se c'è un flag per reverse
	} else {
		player.sprite.sprctl0 = BPP_4 | TYPE_NORMAL ;
	}




}

// Aggiorna il player (chiamato ogni frame)
void  player_update(Level * level) {//, Level* level
	// Applica gravità
	if(!player.is_grounded) {
		player.vy += player.gravity;
		if(player.vy > 16) player.vy = 16;  // Velocità massima di caduta
	}

	// Aggiorna posizione
	player.x += player.vx;
	player.y += player.vy;



	// Gestisci invincibilità
	if(player.invincibility_timer > 0) {
		player.invincibility_timer--;
		// Lampeggio (alterna visibilità)
		if((player.invincibility_timer / 4) % 2 == 0) {
			player.sprite.sprctl0 &= ~0x0F;  // Nascondi
		} else {
			player.sprite.sprctl0 |= 0x0F;   // Mostra
		}
	}

	// Aggiorna cooldown
	if(player.shoot_cooldown > 0) {
		player.shoot_cooldown--;
	}



	// Controlla se è a terra
	if(player.y >= player.ground_level ) {

		player.y = player.ground_level;
		player.vy = 0;
		player.is_grounded = 1;
		player.is_jumping = 0;

		if(player.state == PLAYER_JUMPING) {
			player.state = PLAYER_IDLE;
		}
	}



	// Aggiorna animazione
	player_animate();
}

// Movimento del player
void player_move(s8 dx, s8 dy) {
	player.vx = dx;

	if(dx > 0) {
		player.direction = DIR_RIGHT;
		player.state = PLAYER_WALKING;
	} else if(dx < 0) {
		player.direction = DIR_LEFT;
		player.state = PLAYER_WALKING;
	} else {
		if(player.is_grounded) {
			player.state = PLAYER_IDLE;
		}
	}
}

// Salto
void player_jump() {
	if(player.is_grounded && !player.is_jumping) {
		player.vy = -player.jump_force;
		player.is_grounded = 0;
		player.is_jumping = 1;
		player.state = PLAYER_JUMPING;
		player.current_frame = 0;  // Frame iniziale del salto
		player.animation_timer = 0;
	}
}

// Sparo
void player_shoot() {
	if(player.shoot_cooldown == 0) {
		// Logica per creare un proiettile
		player.state = PLAYER_SHOOTING;
		player.shoot_cooldown = 10;  // 10 frame di cooldown

		// Animazione di sparo
		player.current_frame = 0;
		player.animation_timer = 0;
		player.animation_speed = 2;
	}
}

// Danno al player
void player_hurt(u8 damage) {
	if(player.invincibility_timer == 0) {
		player.health -= damage;
		player.invincibility_timer = 60;  // 60 frame di invincibilità

		if(player.health <= 0) {
			player.state = PLAYER_DEAD;
			player.lives--;
			player.health = 0;
		} else {
			player.state = PLAYER_HURT;
		}
	}
}

