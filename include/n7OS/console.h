#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <inttypes.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define SCREEN_ADDR 0xB8000

#define PORT_CMD 0x3D4
#define PORT_DATA 0x3D5

#define CMD_HIGH 0xE
#define CMD_LOW 0xF

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define PURPLE 0x5
#define BROWN 0x6
#define GRAY 0x7
#define D_GRAY 0x8
#define L_BLUE 0x9
#define L_GREEN 0xA
#define L_CYAN 0xB
#define L_RED 0xC
#define L_PURPLE 0xD
#define YELLOW 0xE
#define WHITE 0xF

// Character color: blink|back|text
#define BLINK (0 << 7)
#define BACK (WHITE << 4)
#define TEXT BLACK
#define CHAR_COLOR (BLINK | BACK | TEXT)

#define HEADER_SIZE 16
#define HEADER_BLINK (0 << 7)
#define HEADER_BACK (D_GRAY << 4)
#define HEADER_TEXT WHITE
#define HEADER_CHAR_COLOR (HEADER_BLINK | HEADER_BACK | HEADER_TEXT)

void init_console();

void console_putchar(const char c);

/**
 * vide l'écran
 */
void clear_screen(int full);

void console_puttime();
void console_putheader();

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

#endif
