/*
 * Player.h
 *
 *  Created on: Dec 2, 2025
 *      Author: giovanni-gds
 */
#ifndef PLAYER_STRUCT_H
#define PLAYER_STRUCT_H
#include "Utils.h"



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

    u8 width_half;
    u8 height_half;

    u8 jump_force;
    u8 gravity;
    u8 is_grounded;
    u8 is_jumping;

    // Contatori/timer
    u8 invincibility_timer;
    u8 shoot_cooldown;

    sprite_collidabile visible_spc;


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

    //CollisionState collision;

    s16 xOnSprite;
    s16 tx;
    s16 newX;

    s16 yOnSprite;
    s16 ty;
    s16 newY;
    s16 collisionByteVal;
} Player;




#endif /* PLAYER_H */
