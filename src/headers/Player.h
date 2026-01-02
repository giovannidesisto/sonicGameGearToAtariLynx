/*
 * Player.h
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "SpriteRepo.h"
#include "PlayerStruct.h"



static const u8 MAX_RUN_SPEED = 16;//14;
static const u8 MAX_WALK_SPEED = 12;//8;
static const s8 DEFAULT_JUMP_POWER = -12;
static const s8 WAIT_BEFORE_IDLE_ANIMATION = -5	;




// Array di puntatori ai frame per animazione
static const unsigned char* sonic_run_tiles[] = {
		sonic_fast_1,
		sonic_fast_2,
		sonic_fast_3,
		sonic_fast_4,
};


static const unsigned char* sonic_brake_tiles[] = {
		sonic_brake_1
};

static const unsigned char* sonic_jump_tiles[] = {
		sonic_run_1,
		sonic_run_2,
		sonic_run_3,
		sonic_run_4,
		sonic_run_5,
		sonic_run_6
};


// Array di puntatori ai frame per animazione
static const  unsigned char* sonic_idle_tiles[] = {
		sonic_idle_1,
		sonic_idle_2,
		sonic_idle_3,
		sonic_idle_4,
		sonic_idle_5
};


// Funzioni di gestione
void player_init();
void player_update();
void player_animate();
void player_handle_user_input(u8 key);




#endif /* PLAYER_H */
