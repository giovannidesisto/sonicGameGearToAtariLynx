/*
 * Utils.c
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */
#include "Utils.h"

int u16_to_ascii(unsigned short value, char *dest) {
	int min_digits=5;
    char temp[5];         // 65535 max 5 cifre
    int i = 0, len = 0;

    // estraggo le cifre al contrario
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while(value > 0);

    // aggiungo padding fino a min_digits
    while(i < min_digits) {
        temp[i++] = '0';
    }

    // ora ribalto temp nel buffer finale
    len = i;
    while(i > 0) {
        *dest++ = temp[--i];
    }

    return len;
}
void printCoordsToScreen(u16 x, u16 y,u8 print_x, u8 print_y,u8 color){
	char arrToPrint[16]={0};
    short pos=0;
    arrToPrint[pos++]='x';
	pos += u16_to_ascii(x, (char*)&arrToPrint[pos]);
	arrToPrint[pos++]='y';
	pos += u16_to_ascii(y, (char*)&arrToPrint[pos]);
	agSetFontColor(color);
    agDrawText(print_x, print_y, arrToPrint);
}

