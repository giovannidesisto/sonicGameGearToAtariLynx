/*
 * Player.h
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */
#ifndef PLAYER_H
#define PLAYER_H
#define PLAYER_FRAME_COUNT 4
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32
#include <AgCommon.h>  

// Dichiarazioni dei frame (definite altrove)
extern char sonic_run_1[];
extern char sonic_run_2[];
extern char sonic_run_3[];
extern char sonic_run_4[];


extern char sonic_idle_1[];
extern char sonic_idle_2[];
extern char sonic_idle_3[];
extern char sonic_idle_4[];

// Array di puntatori ai frame per animazione
static unsigned char* sonic_run_tiles[] = {
		sonic_run_1,
		sonic_run_2,
		sonic_run_3,
		sonic_run_4,
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
    u8 animation_timer;
    u8 animation_speed;  // frame di gioco per cambio frame animazione

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

    // Sprite Control Block
    SCB_REHV_PAL sprite;

    // Array di frame per animazioni diverse
    char** idle_frames;
    char** walk_frames;
    char** jump_frames;
    u8 idle_frame_count;
    u8 walk_frame_count;
    u8 jump_frame_count;

    u8 spindash_power;
    u8 rings;
    u8 ground_level;


} Player;


// Funzioni di gestione
void player_init();
void player_update();
//void player_draw();
void player_move( s8 dx, s8 dy);
void player_jump();
void player_shoot();
void player_hurt( u8 damage);
 void player_animate();
// Nuove funzioni
void player_run_jump(s8 direction);
void player_spindash();
void player_hurt( u8 damage);
#endif /* PLAYER_H */
