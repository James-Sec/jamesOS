#ifndef SCREEN_H
#define SCREEN_H
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#include "mem.h"
#include "ports.h"
#include "types.h"

//Screen device I/O ports
#define PORT_SCREEN_CTRL 0x3d4
#define PORT_SCREEN_DATA 0x3d5

void clear_screen ();
void kprint_at (char *message,int offset);
void kprint (char *message);
#endif
