/*
 * Utils.h
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */
#ifndef UTILS_H
#define UTILS_H
#include "AgCommon.h"
#include <tgi.h>
int u16_to_ascii(unsigned short value, char *dest);
void printCoordsToScreen(u16 x, u16 y,u8 print_x, u8 print_y,u8 color);
void printU8As2Nibble(unsigned char value,u8 print_x, u8 print_y,u8 color);
int iabs(int v);
#endif /* UTILS_H */
