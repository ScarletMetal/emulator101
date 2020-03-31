#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct state_8080;
struct io_8080;

int run_gui(char *filename, int debug);