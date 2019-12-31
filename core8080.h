#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
struct Flags {
    uint8_t z:1;
    uint8_t s:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t p:1;
    uint8_t pad:3;
};

struct State8080 {
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
    struct Flags flags;

    uint8_t int_enable;
};

int execute(struct State8080 *state);
int load_bin_file(struct State8080 *state, int offset, char *file_name);

struct State8080 *make_state(int mem_size);
