#include <stdio.h>

#include "core8080.h"

void main() {
	struct state_8080 *state = make_state(200, 0);
	load_bin_file(state, 0, "code.bin");

	state->sp = 150;

	int running = 0;

	while (!running) {
		running = cpu_update(state);
		//print_state(state);
	}
	printf("calculation result is %x\n", state->a);
	free(state->memory);
	free(state);
}

