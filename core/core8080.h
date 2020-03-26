#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "constants.h"

struct io_8080;

struct flags_8080 {
    uint8_t z:1;
    uint8_t s:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t p:1;
    uint8_t pad:3;
};


struct state_8080 {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

    uint8_t *memory;

    uint8_t int_enable;

    uint16_t ram_offset;

    struct flags_8080 flags;
    struct io_8080 *io;

    uint8_t screen_buffer[SCREEN_WIDTH][SCREEN_HEIGHT][4];
    void (* update_screen) (struct state_8080 *state);
};

int cpu_update(struct state_8080 *state);

int load_bin_file(struct state_8080 *state, int offset, char *file_name);
struct state_8080 *make_state(int mem_size, uint16_t ram_offset);
void print_state(struct state_8080 *state);
