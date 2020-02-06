#include <stdio.h>

#include "core8080.h"

void main() {
	struct state_8080 *state = make_state(200, 0);
	load_bin_file(state, 0, "code.bin");

	state->sp = 150;

	int running = 0;
	uint16_t w = 0x403;
	uint8_t hb = w >> 8; 
	printf("hb = %x\n", hb);

	while (!running) {
		running = cpu_update(state);
		print_state(state);
		//printf("sp=%x\n", state->sp);
		//printf("mem[sp] is %x\n", state->memory[state->sp]);
	}	
	free(state->memory);
	free(state);
}

