#ifndef SCREEN_H
#define SCREEN_H
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define YELLOW_ON_BLACK 0x0e

// some colors
#define BLACK           0x0
#define BLUE            0x1
#define GREEN           0x2
#define CYAN            0x3
#define RED             0x4
#define MAGENTA         0x5
#define BROWN           0x6
#define LIGHT_GREY      0x7
#define DARK_GREY       0x8
#define LIGHT_BLUE      0x9
#define LIGHT_GREEN     0xa
#define LIGHT_CYAN      0xb
#define LIGHT_RED       0xc
#define LIGHT_MAGENTA   0xd
#define LIGHT_BROWN     0xe
#define WHITE           0xf

// vga I/O ports
#define VGA_CTRL_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5

#include <stdint.h>
#include <stdarg.h>
#include "mem.h"
#include "ports.h"

void clear_screen ();
void kprint_at (uint8_t *message, int32_t offset);
void kprint (uint8_t *message);
void kprint_debug (uint8_t *message, uint8_t color);
#endif
