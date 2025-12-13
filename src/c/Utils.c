/*
 * Utils.c
 *
 *  Created on: Dec 4, 2025
 *      Author: giovanni-gds
 */
#include "Utils.h"
#define IMAX(a,b) ((a) > (b) ? (a) : (b))

SCB_REHV_PAL* prev_sprite = NULL;
SCB_REHV_PAL* first_sprite = NULL;

//char DEBUG_BUFFER[DEBUG_BUFFER_SIZE]={0};
int u8_to_hex_ascii(unsigned char value, char *dest) {
    int min_digits = 2;      // un byte = due cifre hex
    char temp[2];            // buffer temporaneo
    int i = 0, len = 0;
    unsigned char nibble;

    // estraggo i due nibble al contrario (prima quello basso)
    nibble = value & 0x0F;
    temp[i++] = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));

    nibble = (value >> 4) & 0x0F;
    temp[i++] = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));

    // padding eventuale (qui non servirà mai, ma rimane per equivalenza)
    while (i < min_digits) {
        temp[i++] = '0';
    }

    // ribalto temp nel buffer finale come nel tuo metodo
    len = i;
    while (i > 0) {
        *dest++ = temp[--i];
    }

    return len;   // sempre 2
}


int u16_to_ascii(unsigned short value, char *dest) {
	int min_digits=3;
	char temp[3];         // 65535 max 5 cifre
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

int s16_to_ascii(short value, char *dest) {
    int min_digits = 5;
    char temp[6];  // 5 cifre + eventuale segno
    int i = 0, len = 0;
    int negative = 0;
    unsigned short v;

    // Gestione negativo
    if (value < 0) {
        negative = 1;
        v = (unsigned short)(-value);
    } else {
        v = (unsigned short)value;
    }

    // estrazione cifre al contrario
    do {
        temp[i++] = '0' + (v % 10);
        v /= 10;
    } while (v > 0);

    // padding fino al minimo richiesto
    while (i < min_digits) {
        temp[i++] = '0';
    }

    // aggiungi il segno se negativo
    if (negative) {
        temp[i++] = '-';
    }

    // ribalta nel buffer finale
    len = i;
    while (i > 0) {
        *dest++ = temp[--i];
    }

    return len;
}
void printS16(s16 value,u8 print_x, u8 print_y,u8 color){
	char arrToPrint[16]={0};
	short pos=0;
	arrToPrint[pos++]='x';
	pos += s16_to_ascii(value, (char*)&arrToPrint[pos]);
	agSetFontColor(color);
	agDrawText(print_x, print_y, arrToPrint);
}

void printExadec(s16 value,u8 print_x, u8 print_y,u8 color){
	char arrToPrint[16]={0};
	short pos=0;
	arrToPrint[pos++]='x';
	pos += u8_to_hex_ascii(value, (char*)&arrToPrint[pos]);
	agSetFontColor(color);
	agDrawText(print_x, print_y, arrToPrint);
}

void printU16(u16 value,u8 print_x, u8 print_y,u8 color){
	char arrToPrint[16]={0};
	short pos=0;
	arrToPrint[pos++]='x';
	pos += u16_to_ascii(value, (char*)&arrToPrint[pos]);
	agSetFontColor(color);
	agDrawText(print_x, print_y, arrToPrint);
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

int iabs(int v) {
	return (v < 0) ? -v : v;
}

void debug_init(void) {
    /* Pulisci il buffer */
   memset(DEBUG_BUFFER,0, DEBUG_BUFFER_SIZE);
	//DEBUG_BUFFER={(unsigned char)0xFF};
}

void debug_print(const char *str) {
    strcpy(DEBUG_BUFFER, str);
}

