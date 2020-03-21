#include <stdio.h>

#include "core8080.h"
#include "disassembler.h"

int main(int argc, char *argv[]) {
    char *filename = "code.bin";
    if (argc > 1)
        filename = argv[1];

	struct state_8080 *state = make_state(200, 0);
	load_bin_file(state, 0, filename);

	state->sp = 150;

	int running = 0;

	while (!running) {
		running = cpu_update(state);
	}
	printf("result in a is %x\n", state->a);
	free(state->memory);
	free(state);
	return 0;
}

