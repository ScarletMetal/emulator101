#include <stdio.h>

#include "cli.h"

#include "../core/core8080.h"
#include "../core/disassembler.h"

int run_cli(char *filename, int debug) {
    struct state_8080 *state = make_state(200, 0);
    load_bin_file(state, 0, filename);

    state->sp = 150;

    int running = 0;

    while (!running) {
        running = cpu_update(state);
        if (debug) {
            disassemble_8080(state->memory, state->pc);
            print_state(state);
        }
    }
    printf("result in a is %x\n", state->a);
    free(state->memory);
    free(state);
}