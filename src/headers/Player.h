/*
 * Player.h
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */
#ifndef PLAYER_H
#define PLAYER_H




#include "Level.h"



// Dichiarazioni dei frame (definite altrove)
extern unsigned char sonic_run_1[];
extern unsigned char sonic_run_2[];
extern unsigned char sonic_run_3[];
extern unsigned char sonic_run_4[];

extern unsigned char sonic_brake_1[];
extern unsigned char sonic_brake_2[];

extern unsigned char sonic_jump_1[];
extern unsigned char sonic_jump_2[];


extern unsigned char sonic_idle_1[];
extern unsigned char sonic_idle_2[];
extern unsigned char sonic_idle_3[];
extern unsigned char sonic_idle_4[];


static const s8 DEFAULT_SPEED = 2;
static const s8 DEFAULT_JUMP_POWER = -15;
static const s8 WAIT_BEFORE_IDLE_ANIMATION = -100;

// Array di puntatori ai frame per animazione
static unsigned char* sonic_run_tiles[] = {
		sonic_run_1,
		sonic_run_2,
		sonic_run_3,
		sonic_run_4,
};


static unsigned char* sonic_brake_tiles[] = {
		sonic_brake_1,
		sonic_brake_2
};

static unsigned char* sonic_jump_tiles[] = {
		sonic_jump_1,
		sonic_jump_2
};


// Array di puntatori ai frame per animazione
static unsigned char* sonic_idle_tiles[] = {
		sonic_idle_1,
		sonic_idle_2,
		sonic_idle_3,
		sonic_idle_4,
};


// Stati del player
typedef enum {
    PLAYER_IDLE = 0,
    PLAYER_WALKING,
	PLAYER_RUNNING,
	PLAYER_BRAKING,
    PLAYER_JUMPING,
	PLAYER_RUN_JUMPING,
    PLAYER_SHOOTING,
	PLAYER_SPINDASH,
    PLAYER_HURT,
    PLAYER_DEAD
} PlayerState;

// Direzioni
typedef enum {
    DIR_RIGHT = 0,
    DIR_LEFT,
    DIR_UP,
    DIR_DOWN
} PlayerDirection;

typedef struct {
    u8 deposit;
    u8 last_deposit;
    u8 collision_frame_delay;
} CollisionState;

// Struttura principale del player
typedef struct Player{
    // Posizione (world o screen)
    u16 x;
    u16 y;

    // Velocità
    s8 vx;
    s8 vy;

    // Stato e animazione
    PlayerState state;
    PlayerDirection direction;
    u8 current_frame;
    s8 animation_timer;
    s8 animation_speed;  // frame di gioco per cambio frame animazione

    // Vita e punteggio
    u8 health;
    u8 lives;
    u16 score;

    // Proprietà fisiche
    u8 width;
    u8 height;
    u8 jump_force;
    u8 gravity;
    u8 is_grounded;
    u8 is_jumping;

    // Contatori/timer
    u8 invincibility_timer;
    u8 shoot_cooldown;

    sprite_collidabile visible_spc;
//    sprite_collidabile ghost_spc;


    // Array di frame per animazioni diverse
    unsigned char** idle_frames;
    unsigned char** walk_frames;
    unsigned char** brake_frames;
    unsigned char** jump_frames;
    u8 idle_frame_count;
    u8 walk_frame_count;
    u8 brake_frame_count;
    u8 jump_frame_count;

    u8 spindash_power;
    u8 rings;
    u8 ground_level;

    CollisionState collision;

    s16 xOnSprite;
    s16 tx;
    s16 newX;

    s16 yOnSprite;
    s16 ty;
    s16 newY;
    s16 collisionByteVal;
} Player;


// Funzioni di gestione
void player_init();
void player_update();
void player_animate();
void player_handle_user_input(u8 key);




#endif /* PLAYER_H */
