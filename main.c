#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <gui/sdl_ui.h>

#include "cli/cli.h"

enum MODE {
    MODE_CLI = 0,
    MODE_GUI = 1,
    MODE_SERVER = 2,
};
struct arguments {
    int debug;
    int mode;

    char *target;
};

static struct argp_option options[] = {
        {"debug",  'd', 0,           0, "Print Debug Output"},
        {"target", 't', "FILE_NAME", 0, "Binary File The Emulator Will Execute"},
        {"mode",    'm', "MODE",  0, "Sets The Mod Of Execution For This Program"},
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
        case 'm':
            arguments->mode = atoi(arg);
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
    struct arguments arguments = {0, 0, "rom.bin"};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    char *filename = arguments.target;
    int debug = arguments.debug;
	int mode   = arguments.mode;
	if (mode == MODE_CLI)
	    return run_cli(filename, debug);
	if (mode == MODE_GUI)
	    return run_gui(filename, debug);
	return 0;
}

