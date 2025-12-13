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
	player.x =TILE_SIZE+16;
	player.y = 0;//(MAP_HEIGHT * TILE_SIZE)-TILE_SIZE*2;//va infondo alla mappa, considerato che lo y0 e in alto

	player.width=PLAYER_WIDTH;
	player.height=TILE_SIZE;


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
	s16 new_x,
	new_y,
	x_boundary,
	y_boundary,
	tile_x,
	tile_y,
	tile_x1,
	tile_x2,
	tile_y1,
	tile_y2,
	tx,
	ty,
	tile_index,
	srpite_y_offset,
	sprite_y,sprite_x,sprite_x_offset;
	//collision_detected,
	//corner_x,
	//corner_y,
	//check_y,
	//mid_x,corner;
	char buffer[DEBUG_BUFFER_SIZE];
	u8 y_rebased;
	s16 transparentPixelIndex,playerXOnSprite,playerYOnSprite;
	new_x = player.x + player.vx;
	new_y = player.y + player.vy;
	// Reset dello stato ground - verrà reimpostato se troviamo collisione sotto
	player.is_grounded = 0;


	/**
	 * TODO
	 * occorre segmentare le sprite del foregound incategorie
	 * - piattaforme (implementano solo la collisione di caduta (quindi ci si può saltare sopra da sotto senza battere la testa)
	 * - roof, tetti: implementa la collisione dal basso verso l'alto, battendo la testa stoppando il salto
	 * - muri, fermano il movimento orizzontale
	 */


	/**
	 * il controllo AS IS funziona molto bene per i muri ma va in contrasto netto con la gestione piattaforma comprensiva di background
	 * (salite, discese, avvallamenti)
	 */
	// 1. Prima controlla collisioni lungo l'asse X
	if (player.vx != 0) {

		// Controlla per tutta l'altezza del player
		tile_y1 = player.y / TILE_SIZE;
		tile_y2 = (player.y + TILE_SIZE - 1) / player.height;


		//calcolare i boundary con meta player size solo per i muri, così da arrivare a livello
		if (player.vx > 0) {
			// Controlla il lato destro del player
			x_boundary = new_x+player.width/2 ;
			tile_x = x_boundary / TILE_SIZE;
		} else {
			// Controlla il lato sinistro del player
			x_boundary = new_x-player.width/2;
			tile_x = x_boundary / TILE_SIZE;
		}


		/**
		 * FIXME gestione disabilitata per testare il comportamento verticale dovuto alla gravità
		 */
		// Verifica se c'è una tile solida lungo il percorso
		for (ty = tile_y1; ty <= tile_y2; ty++)
		{
			if (ty >= 0 && ty < MAP_HEIGHT && tile_x >= 0 && tile_x < MAP_WIDTH)
			{
				tile_index = level_foregound_map[ty][tile_x];

				// Se la tile è solida (1-99)
				if (tile_index != 0 && tile_index >= 200) {
					// Regola la posizione X per essere tangente alla tile
					if (player.vx > 0)
					{


						player.x = tile_x * TILE_SIZE - (PLAYER_WIDTH/2);
					}
					else
					{

						player.x = (tile_x + 1) * TILE_SIZE +  (PLAYER_WIDTH/2) ;
					}
					player.vx = 0;
					new_x = player.x;
					break;
				}
			}
		}


		//calcolare i boundary con meta player size solo per i muri, così da arrivare a livello
		if (player.vx > 0) {
			// Controlla il lato destro del player
			x_boundary = new_x;//+player.width/2 ;
			tile_x = x_boundary / TILE_SIZE;
		} else {
			// Controlla il lato sinistro del player
			x_boundary = new_x;//-player.width/2;
			tile_x = x_boundary / TILE_SIZE;
		}

		//sale sulla prossima tile se non è un muro, serve per gestire le salite o discese
		for (ty = tile_y1; ty <= tile_y2; ty++) {
			if (ty >= 0 && ty < MAP_HEIGHT && tile_x >= 0 && tile_x < MAP_WIDTH)
			{
				tile_index = level_foregound_map[ty][tile_x];

				// Se la tile è solida (1-99)
				if (tile_index != 0 && tile_index < 100)
				{
					player.y = ty * TILE_SIZE - TILE_SIZE;
					new_y=player.y;
					//y_rebased=1;
					break;
				}
			}
		}

	}




	// 2. Poi controlla collisioni lungo l'asse y
	if (player.vy != 0)
	{
		// Determina quale lato stiamo controllando in base alla direzione
		if (player.vy >= 0) {
			// Controlla il lato inferiore del player (caduta)
			y_boundary = new_y + player.height;
			tile_y = y_boundary / TILE_SIZE;
		} else {
			// Controlla il lato superiore del player (salto)
			y_boundary = new_y;
			tile_y = y_boundary / TILE_SIZE;
		}

		// Controlla per tutta la larghezza del player
		tile_x1 = (new_x / TILE_SIZE) ;
		tile_x2 = (new_x + TILE_SIZE - 1) / TILE_SIZE;


		for (tx = tile_x1; tx <= tile_x2; tx++) {
			if (tile_y >= 0 && tile_y < MAP_HEIGHT && tx >= 0 && tx < MAP_WIDTH) {
				tile_index = level_foregound_map[tile_y][tx];

				// Se la tile è solida (1-99) e la x del player ricade nalla tail corrente
				if (tx * TILE_SIZE <= new_x &&  (tx * TILE_SIZE)+TILE_SIZE >= new_x && tile_index != 0 && (tile_index < 100 || tile_index >= 200))
				{
					playerXOnSprite=(new_x-(tx * TILE_SIZE));

					if(tile_index >= 200){
						player.y = tile_y * TILE_SIZE -TILE_SIZE;
						player.is_grounded = 1;
						player.is_jumping = 0;
						player.vy = 0;
						new_y = player.y;

					break;
					}
					// Regola la posizione Y per essere tangente alla tile
					else if (player.vy >=0)
					{
						// Collisione con il terreno sotto
						//posizione il player tangente alla srpite sottostante
						if(y_rebased==0)
							player.y = tile_y * TILE_SIZE - TILE_SIZE;



						for(sprite_y=0;sprite_y<TILE_SIZE;sprite_y++)
						{


							transparentPixelIndex = (sprite_y*((TILE_SIZE/2)+2)) + 1 + (playerXOnSprite/2);

							player.collisionByteVal = (playerXOnSprite%2!=0)?
									LEVEL_1_PLATFORM[tile_index-1][transparentPixelIndex] & 0x0F
									:
									LEVEL_1_PLATFORM[tile_index-1][transparentPixelIndex] >>4 ;


							//implementa la caduta fuori dallo schermo, es trappole
							if(player.collisionByteVal == 0x00)
							{
								player.y++;
							}
							else
							{
								//atterro solo se incontro un pixel solido
								player.is_grounded = 1;
								player.is_jumping = 0;
								player.vy = 0;
								break;
							}

						}




					}
					else
					{
						// Collisione con il soffitto sopra
						player.y = (tile_y + 1) * TILE_SIZE;
						player.vy = 0; // Annulla la velocità ascensionale, cadrà con la gravità
					}


					new_y = player.y;
					break;
				}
			}
		}
	}

	// 3. Applica il movimento residuo se non ci sono state collisioni
	player.x = new_x;
	player.y = new_y;

}

// In player_update():
void player_update() {


	if(player.x < TILE_SIZE) player.x=TILE_SIZE;
	else
		//limite bordo DX
		if(player.x > level.end_x - (TILE_SIZE))player.x=level.end_x - (TILE_SIZE);
	//
	////	// Applica gravità
	if(!player.is_grounded) {
		player.vy += player.gravity;
		if(player.vy > 16) player.vy = 16;  // Velocità massima di caduta
	}

	if(player.y> MAP_HEIGHT*TILE_SIZE )
	{
		player.y=0;
		player.x=0;
		player.vx=0;
		player.vy=0;
	}

	check_world_collision();

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


			//			if(player.state == PLAYER_WALKING && player.vx !=0)
			//			{
			//				player.state = PLAYER_BRAKING;
			//			}
			//			else
			//			{
			//frena con scivolata quando sta correndo e si mollano i tasti
			//				if(player.state == PLAYER_BRAKING ){
			//					if(player.vx > 0)player.vx--;
			//					else player.vx++;
			//				}
			player.vx=0;player.vy=0;
			if(player.state != PLAYER_IDLE)
			{
				//caso fermo
				player.state = PLAYER_IDLE;
				player.current_frame = 0;
				player.animation_timer = WAIT_BEFORE_IDLE_ANIMATION;//tempo di attesa prima di fare battere il piede a sonic, non è poi così impaziente :)
			}
			//			}
		}
		break;
	}

}
