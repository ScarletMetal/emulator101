#include <stdio.h>
#include <argp.h>

#include "core/core8080.h"
#include "core/disassembler.h"

struct arguments {
    int debug;

    char *target;
};

static struct argp_option options[] = {
        {"debug",  'd', 0,           0, "Print Debug Output"},
        {"target", 't', "FILE_NAME", 0, "Binary File The Emulator Will Execute"},
        {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'd': // debug enabled
            arguments->debug = 1;
            break;
        case 't': // target file
            arguments->target = arg;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
static char args_doc[] = "";

static char doc[] = "intel 8080 emulator created as part of my 12th grade final project assignment";

static struct argp argp = {
        options, parse_opt, args_doc, doc
};

int main(int argc, char *argv[]) {
    struct arguments arguments = {0, "rom.bin"};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    printf("target %s\n", arguments.target);

    char *filename = arguments.target;
    int debug = arguments.debug;
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
	return 0;
}

