#include <tgi.h>
#include "Utils.h"
#include "Player.h"
#include "Level.h"
// ----------------------------------------------------------------------------
// DEFINES
// ----------------------------------------------------------------------------

#define DELAY_LOGO_TIME 50
#define FONT_COLOUR_LOGO 0x0F
#define FONT_COLOUR_MENU 0x04
#define FONT_COLOUR_STORY 0x0F


// ----------------------------------------------------------------------------
// READ-ONLY DATA
// ----------------------------------------------------------------------------

// Default palette. This palette is loaded when the GFX system is initialised.
// https://atarigamer.com/pages/atari-lynx-palette-generator
#pragma bss-name (push, "RODATA")
u16 myPalette[] = {
		0x0BE9,
		0x0000,
		0x0104,
		0x0400,
		0x0591,
		0x040A,
		0x05F5,
		0x045D,
		0x0833,
		0x0AF3,
		0x0E11,
		0x0A0F,
		0x0B7F,
		0x0BAF,
		0x0F0D,
		0x0FFF
		
		};

#pragma bss-name (pop)

// ----------------------------------------------------------------------------
// Local variables
// ----------------------------------------------------------------------------
static clock_t startTime;

// Dichiarazione del player (sostituisce le vecchie variabili)

Level level={0};
Player player={0};
static u8 game_state;  // 0=menu, 1=playing, 2=paused, 3=gameover

// ----------------------------------------------------------------------------
// FORWARD FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

static void __fastcall__ logoScreen();
static void __fastcall__ gameMenuScreen();
static void __fastcall__ storyScreen();
static void __fastcall__ resetGame();
static void __fastcall__ updateAndDrawGame();

static void __fastcall__ initPlayerFrames();


// ----------------------------------------------------------------------------
// CODE
// ----------------------------------------------------------------------------



void main(void) {

	agInitGfx();       // initialise graphics
	agSetPalette(myPalette);
	agInitRand8(42);   // initialise RNG (use your own seed other than 42)
	startTime = clock2();



	//logoScreen();      // startup logo screen
	//gameMenuScreen();  // game menu screen, before you begin a game
	//storyScreen();     // game story/instructions screen before gameplay starts

	// Inizializza i frame del player
	//initPlayerFrames();

	// Inizializza il gioco
	resetGame();       // resets the game state to a clean state


	//agSetPalette(myPaletteCustom);

	while(1) {
		updateAndDrawGame();
	}
}

// ----------------------------------------------------------------------------

static void __fastcall__ logoScreen() {
	AG_WAIT_LCD();

	/*
  set background image palette to show 'magenta' i.e. the 6th (index 5) colour
  from the palette above in this 1-bit image; then draw the sprite it's linked to
	 */
	agSprBackground.penpal[0] = 0x05;
	tgi_sprite(&agSprBackground);

	// draw some text after setting it's colour
	agSetFontColor(FONT_COLOUR_LOGO);

	agDrawTextCenter(AG_LCD_MID_Y, "MY LOGO" );

	// update the LCD
	tgi_updatedisplay();

	// wait some time before continuing to the next screen
	while (clock2() - startTime < DELAY_LOGO_TIME) {}
}

// ----------------------------------------------------------------------------

static void __fastcall__ gameMenuScreen() {
	AG_WAIT_LCD();

	/*
  set background image palette to show 'red' i.e. the 2nd (index 1) colour
  from the palette above in this 1-bit image; then draw the sprite it's linked to
	 */
	agSprBackground.penpal[0] = 0x01;
	tgi_sprite(&agSprBackground);

	// draw some text after setting it's colour
	agSetFontColor(FONT_COLOUR_MENU);
	agDrawTextCenter(1, "GAME OPTIONS");
	agDrawTextCenter(AG_LCD_MAX_Y - AG_FONT_LINE_HEIGHT, "ANY KEY TO START");

	// update the LCD
	tgi_updatedisplay();

	// wait for any input before continuing
	AG_JOY_WAIT_INPUT();
}

// ----------------------------------------------------------------------------

static void __fastcall__ storyScreen() {
	AG_WAIT_LCD();

	/*
  set background image palette to show 'dark yellow' i.e. the 10th (index A) colour
  from the palette above in this 1-bit image; then draw the sprite it's linked to
	 */
	agSprBackground.penpal[0] = 0x0A;
	tgi_sprite(&agSprBackground);

	// draw some text after setting it's colour
	agSetFontColor(FONT_COLOUR_STORY);
	agDrawTextCenter(10, "YOUR MISSION, SHOULD YOU");
	agDrawTextCenter(10 + AG_FONT_LINE_HEIGHT, "CHOOSE TO ACCEPT IT...");
	agDrawTextCenter(AG_LCD_MAX_Y - AG_FONT_LINE_HEIGHT, "ANY KEY TO START");

	// update the LCD
	tgi_updatedisplay();

	// wait for any input before continuing
	AG_JOY_WAIT_INPUT();
}

// ----------------------------------------------------------------------------



static void __fastcall__ resetGame() {


	// Inizializza il livello
	level_init();

	/* Carica il livello 1 */
	level_load(1);
	tgi_setcollisiondetection(1);
	/* Inizializza il player alla posizione di partenza */
	player_init();//level.start_x, level.start_y);

}

// ----------------------------------------------------------------------------

static void  __fastcall__  updateAndDrawGame() {
	// Processa l'input usando le funzioni del player
	u8 pos = 0;
	u8 joy = SUZY.joystick;
	/* Controlla collisioni con il livello */
	u16 new_x;
	u16 new_y;
	/* Gestione combinazioni dirette */
	switch (joy & (AG_JOY_LEFT | AG_JOY_RIGHT | AG_JOY_UP | AG_JOY_DOWN)) {
	case (AG_JOY_LEFT | AG_JOY_UP):
	            						/* Salto con rincorsa a sinistra */
	            						if (!player.is_jumping && player.is_grounded) {
	            							player.is_jumping = 1;
	            							player.is_grounded = 0;
	            							player.state = PLAYER_RUN_JUMPING;
	            							player.direction = DIR_LEFT;
	            							player.vx = -2;
	            							player.vy = -8;  // Salto potente
	            							player.current_frame = 0;
	            							player.animation_timer = 0;
	            						}
	break;

	case (AG_JOY_RIGHT | AG_JOY_UP):
	            						/* Salto con rincorsa a destra */
	            						if (!player.is_jumping && player.is_grounded) {
	            							player.is_jumping = 1;
	            							player.is_grounded = 0;
	            							player.state = PLAYER_RUN_JUMPING;
	            							player.direction = DIR_RIGHT;
	            							player.vx = 2;
	            							player.vy = -8;  // Salto potente
	            							player.current_frame = 0;
	            							player.animation_timer = 0;
	            						}
	break;

	case AG_JOY_UP:
		/* Salto verticale */
		if (!player.is_jumping && player.is_grounded) {
			player.is_jumping = 1;
			player.is_grounded = 0;
			player.state = PLAYER_JUMPING;
			player.vy = -6;  // Salto normale
			player.current_frame = 0;
			player.animation_timer = 0;
		}
		break;
	/*
	 * FIXME
	 */
//	case AG_JOY_DOWN:
//		/* Salto verticale */
//		 {
//			player.is_jumping = 0;
//			player.is_grounded = 1;
//			player.state = PLAYER_IDLE;
//			player.vy = 0;  // Salto normale
//			//player.current_frame = 0;
//			//player.animation_timer = 0;
//		}
//		break;


	case AG_JOY_LEFT:
		/* Cammina a sinistra */
		player.vx = -4;
		player.direction = DIR_LEFT;
		if (player.is_grounded) {
			player.state = PLAYER_WALKING;
			//player.current_frame = 0;
			//player.animation_timer = 0;
		}

		break;

	case AG_JOY_RIGHT:
		/* Cammina a destra */
		player.vx = 4;
		player.direction = DIR_RIGHT;
		if (player.is_grounded) {
			player.state = PLAYER_WALKING;
			//player.current_frame = 0;d
			//player.animation_timer = 0;
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
				//frena con scivolata
				if(player.state == PLAYER_BRAKING ){//&& player.vx !=0
					if(player.vx > 0)player.vx--;
					else player.vx++;

				}

				if(player.vx==0 &&  player.vy==0 )
				{
					//caso fermo
					player.state = PLAYER_IDLE ;
					player.current_frame = 0;
					player.animation_timer = player.animation_speed;
				}
			}
		}
		break;
	}

	/* Gestione pulsanti */
	if (joy & AG_JOY_A) {
		player_shoot();
	}



	/* Controlla collisioni con il livello */
	new_x = player.x + player.vx;
	new_y = player.y + player.vy;

//	if(new_x == player.x && new_y == player.y)
//	{
//		player.current_frame = 0;
//		player.animation_timer = 0;
//		player.state = PLAYER_IDLE;
//	}

	/* Collisione orizzontale */
	if(level_check_collision(new_x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT)) {
		player.vx = 0;  /* Ferma movimento orizzontale */
	} else {
		player.x = new_x;
	}

	/* Collisione verticale */
	if(level_check_collision( player.x, new_y, PLAYER_WIDTH, PLAYER_HEIGHT)) {
		if(player.vy > 0) {
			/* Collisione dal basso (atterraggio) */
			player.is_grounded = 1;
			player.is_jumping = 0;
		}
		player.vy = 0;
	} else {
		player.y = new_y;
	}



	//controllo per non uscire dallo schermo
	//limite bordo SX
	if(player.x < TILE_SIZE) player.x=TILE_SIZE;
	else
	//limite bordo DX
	if(player.x > level.end_x - (TILE_SIZE))player.x=level.end_x - (TILE_SIZE);


	/* Aggiorna il player (senza chiamare player_update_sprite_position) */
	player_update();
	/* Aggiorna la camera per seguire il player */
	level_update_camera(player.x, player.y);
	/* Imposta la posizione dello sprite del player */
	player.sprite.hpos = level_world_to_screen_x(player.x);
	player.sprite.vpos = level_world_to_screen_y(player.y);


	AG_WAIT_LCD();

	/* Disegna lo sfondo */
	agSprBackground.penpal[0] = 0x09;
	tgi_sprite(&agSprBackground);

	/* Disegna il livello usando la camera */
	level_draw();

	/* Disegna il player */
	tgi_sprite(&player.sprite);



	//printCoordsToScreen(player.x,player.y,1,0,0x0E);
    //printCoordsToScreen(level.camera.x,level.camera.y,1,10,0x0D);
	//printCoordsToScreen(level.end_x,level.end_y,1,20,0x0C);

	// update the LCD
	tgi_updatedisplay();
}

