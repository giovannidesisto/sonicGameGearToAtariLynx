/*
 * Atari Gamer Common Lynx Library.
 * Copyright 2022 Igor Kromin (atarigamer.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

#ifndef __LYNX_AG_COMMON__
#define __LYNX_AG_COMMON__

#include <time.h>
#include <_suzy.h>

// ----------------------------------------------------------------------------
// DEFINES
// ----------------------------------------------------------------------------

// LCD related constants
#define AG_LCD_REFRESH_RATE 75
#define AG_LCD_MAX_X 159
#define AG_LCD_MAX_Y 101
#define AG_LCD_WIDTH 160
#define AG_LCD_HEIGHT 102
#define AG_LCD_MID_Y 50
#define AG_WAIT_LCD() while (tgi_busy()) {}

// font related constants
#define AG_FONT_LINE_HEIGHT 6
#define AG_FONT_CHAR_WIDTH 6
#define AG_FONT_LAST_LINE_Y AG_FONT_LINE_HEIGHT * 16
#define AG_FONT_SKIP_SPACE_CHAR 1

// Palette address locations
// Green colours:    FDA0 - FDAF
// Blue/Red colours: FDB0 - FDBF
#define AG_PAL_ADDR_GREEN 0xFDA0
#define AG_PAL_ADDR_BLUERED 0xFDB0

// joystick constants and macros
#define AG_JOY_NONE 0
#define AG_JOY_A 1
#define AG_JOY_B 2
#define AG_JOY_OPT2 4
#define AG_JOY_OPT1 8
#define AG_JOY_OPT12 12
#define AG_JOY_RIGHT 16
#define AG_JOY_LEFT 32
#define AG_JOY_DOWN 64
#define AG_JOY_UP 128
// waits for a joystick button to be released
#define AG_JOY_WAIT(button) while (SUZY.joystick == button) {}
// waits for all joystick input to stop
#define AG_JOY_WAIT_NOINPUT() while (SUZY.joystick != AG_JOY_NONE) {}
// wait for any joystick input to continue
#define AG_JOY_WAIT_INPUT() while (SUZY.joystick == AG_JOY_NONE) {}

// ----------------------------------------------------------------------------
// TYPES
// ----------------------------------------------------------------------------

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;


// ----------------------------------------------------------------------------
// EXPORTED FUNCTIONS
// ----------------------------------------------------------------------------

void agInitGfx();
void __fastcall__ agSetPalette(const u16* palette);
void __fastcall__ agSetPaletteColour(const u8 colourIndex, const u16 colour);
void __fastcall__ agSetFontColor(const u8 c);
void __fastcall__ agDrawText(const u8 x, const u8 y, const char* str);
void __fastcall__ agDrawTextCenter(const u8 y, const char* str);
void __fastcall__ agDrawTextN(const u8 x, const u8 y, const char* str, const u8 maxLength);


// ----------------------------------------------------------------------------
// IMPORTED DATA
// ----------------------------------------------------------------------------

/*
Single pixel, 1-bit colour image, with the colour set to 1 i.e. the second
colour in the pallete of possible colours.
*/
extern char img_pixel2col[];


// ----------------------------------------------------------------------------
// IMPORTED FUNCTIONS
// ----------------------------------------------------------------------------

extern time_t clock2();
extern u8 agRand8(u8 bitMask);
extern void agInitRand8(u32 seed);

typedef  struct{
  u8 deposit;
  SCB_REHV_PAL sprite;
} sprite_collidabile;

/* Palette condivisa per tutti i tile */
static const u8 tile_palette[8] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

static SCB_REHV_PAL agSprBackground = {
  BPP_1 | TYPE_BACKGROUND,                  // sprctl0
  REHV | LITERAL,                           // sprctl1
  0,                                        // sprcoll
  (void*) 0,                                // next SCB PTR
  &img_pixel2col[0],                        // SPR data PTR
  0, 0,                                     // offset (hpos, vpos)
  0x0100 * AG_LCD_WIDTH,                    // hsize
  0x0100 * AG_LCD_HEIGHT,                   // vsize
  {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF} // penpal
};


static SCB_REHV_PAL agSprPixel = {
  BPP_1 | TYPE_BACKGROUND,                  // sprctl0
  REHV | LITERAL,                           // sprctl1
  0,                                        // sprcoll
  (void*) 0,                                // next SCB PTR
  &img_pixel2col[0],                        // SPR data PTR
  0, 0,                                     // offset (hpos, vpos)
  0x0100,                    // hsize
  0x0100,                   // vsize
  {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF} // penpal
};


#endif // __LYNX_AG_COMMON__
