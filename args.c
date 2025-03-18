/// "args.c", GNU arguments handling

// C standard libs
#include <stdlib.h>

// Headers
#include "args.h"


// Constants
static struct argp_option options[] = {
	{"num_birds", 'n', "NUM_BIRDS", 0, "number of birds(default: 1000)"},
	{"eta", 'e', "ETA", 0, "stochastic parameter(default: 1.0)"},
	{"speed", 's', "SPEED", 0, "speed(default: 0.03)"},
	{"radius", 'r', "RADIUS", 0, "radius(default: 1.0)"},
	{"world_size", 'l', "WORLD_SIZE", 0, "world size(default: 31.6)"},
};
static char doc[] = "Simulation of \"flocking\" \"birds\"";
static char args_doc[] = "TABLE FILE";
const int NUM_BIRDS = 1000;
const float ETA = 1.0;
const float SPEED = 0.03;
const float RADIUS = 1.0;
const float WORLD_SIZE = 31.6;


// Functions
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *args = state->input;
	switch (key) {
		case 'n':
			args->num_birds = atoi(arg);
			printf("Number of \"birds\" set to: %d\n", args->num_birds);
			break;
		case 'e':
			args->eta = atof(arg);
			printf("Stochastic term set to: %d\n", args->num_birds);
			break;
		case 's':
			args->speed = atof(arg);
			printf("Speed set to: %d\n", args->num_birds);
			break;
		case 'r':
			args->radius = atof(arg);
			printf("Radius set to: %d\n", args->num_birds);
			break;
		case 'l':
			args->size = atof(arg);
			printf("World size set to: %d\n", args->num_birds);
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 2)
				// Too many arguments
				argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };
struct arguments get_args(int argc, char *argv[]) {
	struct arguments args;
	args.num_birds = NUM_BIRDS;
	args.eta = ETA;
	args.speed = SPEED;
	args.radius = RADIUS;
	args.size = WORLD_SIZE;

	argp_parse(&argp, argc, argv, 0, 0, &args);
	
	return args;
}



