/*
 * Player.h
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */
#ifndef PLAYER_H
#define PLAYER_H
#define PLAYER_FRAME_COUNT 7
#define PLAYER_WIDTH 12
#define PLAYER_HEIGHT 24
#include <AgCommon.h>  

// Dichiarazioni dei frame (definite altrove)
extern char player_frame_00[];
extern char player_frame_01[];
extern char player_frame_02[];
extern char player_frame_03[];
extern char player_frame_04[];
extern char player_frame_05[];
extern char player_frame_06[];
// Array di puntatori ai frame per animazione
static unsigned char* player_frames[] = {
    player_frame_00,
    player_frame_01,
    player_frame_02,
    player_frame_03,
    player_frame_04,
    player_frame_05,
    player_frame_06
};


// Stati del player
typedef enum {
    PLAYER_IDLE = 0,
    PLAYER_WALKING,
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
