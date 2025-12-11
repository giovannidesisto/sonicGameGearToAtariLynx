//#include <tgi.h>
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
//__fastcall__
static void  updateAndDrawGame();

static void __fastcall__ initPlayerFrames();


// ----------------------------------------------------------------------------
// CODE
// ----------------------------------------------------------------------------



void main(void) {

	agInitGfx();       // initialise graphics
	agSetPalette(myPalette);
	agInitRand8(42);   // initialise RNG (use your own seed other than 42)
	startTime = clock2();
	tgi_setcollisiondetection(1);


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

	/* Inizializza il player alla posizione di partenza */
	player_init();//level.start_x, level.start_y);

}

// ----------------------------------------------------------------------------
//__fastcall__
static void   updateAndDrawGame() {
	/* Controlla collisioni con il livello */

//	if(0)
	{
		player_handle_user_input(SUZY.joystick);
		AG_WAIT_LCD();
		/* Aggiorna il player (senza chiamare player_update_sprite_position) */
		player_update();
		// Aggiorna animazione
		player_animate();
		/* Aggiorna la camera per seguire il player */
		level_update_camera(player.x, player.y);
		//tgi_clear();
		/* Disegna lo sfondo */
		agSprBackground.penpal[0] = 0x09;
		tgi_sprite(&agSprBackground);
		/* Disegna il livello usando la camera */
		level_draw();
		/* Disegna il player */
		tgi_sprite(&player.visible_spc.sprite);
		tgi_updatedisplay();
	}
//	else{
//		AG_WAIT_LCD();
//		agSprBackground.penpal[0] = 0x01;
//		//tgi_sprite(&agSprBackground);
//		SCB_MATRIX[0][0].data =(unsigned char*) bck_100;
//		SCB_MATRIX[0][0].vpos= 60;
//		SCB_MATRIX[0][0].hpos= 60;
//		tgi_sprite(&SCB_MATRIX[0][0]);
//		tgi_updatedisplay();
//	}
}
