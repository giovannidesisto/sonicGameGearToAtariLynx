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

#include <6502.h>
#include <peekpoke.h>
#include <string.h>
#include <tgi.h>
#include "AgCommon.h"


// ----------------------------------------------------------------------------
// DEFINES
// ----------------------------------------------------------------------------

// Calculates the font character index from the ASCII character input.
// All lower case characters are mapped to their upper case equivalents.
// No out of bounds checks are done for performance reasons.
#define ASCII2FONTIDX(c) c >= '`' ? c-64 : c-32


// ----------------------------------------------------------------------------
// ZERO-PAGE VARIABLES
// ----------------------------------------------------------------------------

#pragma bss-name (push, "ZEROPAGE")
  extern u8 _agFontStrIdx;
  #pragma zpsym("_agFontStrIdx")

  extern u8 _agFontStrLen;
  #pragma zpsym("_agFontStrLen")

  extern u8 _agFontStrChar;
  #pragma zpsym("_agFontStrChar")

  extern u8 _agTmpIdx8_1;
  #pragma zpsym("_agTmpIdx8_1")
#pragma bss-name (pop)


// ----------------------------------------------------------------------------
// LOCAL VARIABLES
// ----------------------------------------------------------------------------

// Font image data and SCB sprite definition. Literal (non packed) data is used
// to simplify offset calculations. Image data is 1-bit i.e. either background
// or foreground.
extern char img_ag_font5x5[];
static SCB_REHV_PAL sprFont = {
	BPP_1 | TYPE_NORMAL,                      // sprctl0
  REHV | LITERAL,                           // sprctl1
  0,                                        // sprcoll
  0,                                        // next SCB PTR
  &img_ag_font5x5[0],                       // SPR data PTR
  0, 0,                                     // offset (hpos, vpos)
  0x0100, 0x0100,                           // size (hsize, vsize)
  {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF} // penpal
};
SCB_REHV_PAL* sprFontPtr = &sprFont;


/******************************************************************************
 * Initialises the graphics system, sets the refresh rate and clears the
 * palette.
 */
void agInitGfx() {
  // install and init TGI driver
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_setframerate(AG_LCD_REFRESH_RATE);
  memset((void *)0xFDA0, 0, 32); // blank out the palette
  tgi_clear();
  CLI();
}


/******************************************************************************
 * Sets the current palette by copying 16, 16bit values to the Lynx palette
 * memory. Input data must be a pointer to a 16x 16bit array of BGR values.
 * No bounds checks are performed on the array.
 */
void __fastcall__ agSetPalette(const u16* palette) {
  for (_agTmpIdx8_1 = 0; _agTmpIdx8_1 < 0x10; _agTmpIdx8_1++) {
    POKE(AG_PAL_ADDR_GREEN + _agTmpIdx8_1, palette[_agTmpIdx8_1] >> 8);
    POKE(AG_PAL_ADDR_BLUERED + _agTmpIdx8_1, palette[_agTmpIdx8_1] & 0xFF);
  }
}


/******************************************************************************
 * Sets one value in the current palette at the given colour index. The colour
 * value is a 16 bit BGR value.
 * No bound checks are performed on the index position and its value should be
 * between 0 and 15.
 */
void __fastcall__ agSetPaletteColour(const u8 colourIndex, const u16 colour) {
  POKE(AG_PAL_ADDR_GREEN + colourIndex, colour >> 8);
  POKE(AG_PAL_ADDR_BLUERED + colourIndex, colour & 0xFF);
}


/******************************************************************************
 * Sets the font background and foreground colour at the same time. The input
 * value is a 2x 4bit palette index offset for the current palette. For example
 * a value of 0x0F will set the background colour to 0 and foreground colour to
 * F.
 * No bounds checks are performed on the input value.
 */
void __fastcall__ agSetFontColor(const u8 c) {
  sprFontPtr->penpal[0] = c;
}


/******************************************************************************
 * Draws at most maxLength characters of a text string at the given X and Y
 * locations. If end of string is reached before maxLength, drawing will stop.
 * Some bounds checking is performed to ensure tha characters are not drawn
 * outside of the screen.
 */
void agDrawTextN(const u8 x, const u8 y, const char* str, const u8 maxLength) {
  if (y > 101) return; // don't draw beyond screen height

  sprFontPtr->vpos = y;
  sprFontPtr->hpos = x;

  for (_agFontStrIdx = 0; ; _agFontStrIdx++) {
    _agFontStrChar = str[_agFontStrIdx];
    
    // terminate on null char or if past end of screen
    if (_agFontStrChar == 0 || sprFontPtr->hpos > 159 || _agFontStrIdx >= maxLength) { break; } 

    #if AG_FONT_SKIP_SPACE_CHAR == 1
    if (_agFontStrChar != ' ') {
    #endif
      _agFontStrChar = ASCII2FONTIDX(_agFontStrChar);
      sprFontPtr->data = &img_ag_font5x5[11 * (_agFontStrChar)];    
      tgi_sprite(sprFontPtr);
    #if AG_FONT_SKIP_SPACE_CHAR == 1
    }
    #endif

    sprFontPtr->hpos += 6;
  }
}


/******************************************************************************
 * Draws a text string at the given X and Y locations.
 * Some bounds checking is performed to ensure tha characters are not drawn
 * outside of the screen.
 */
void __fastcall__ agDrawText(const u8 x, const u8 y, const char* str) {
  agDrawTextN(x, y, str, 255);
}


/******************************************************************************
 * Draws a text string at the given Y location, centred in the X axis.
 * Some bounds checking is performed to ensure tha characters are not drawn
 * outside of the screen.
 */
void __fastcall__ agDrawTextCenter(const u8 y, const char* str) {
  _agFontStrLen = strlen(str);
  agDrawText((160 - (_agFontStrLen * 6)) / 2, y, str);
}
