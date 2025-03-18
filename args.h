/// "args.h"

#ifndef ARGS_H

// GNU libs
#include <argp.h>

// Headers
#include "main.h"


// Structs
struct arguments {
	char *cargs[2];
	int num_birds;
	float eta, speed, radius, size;
};

// Prototypes
struct arguments get_args(int argc, char *argv[]);


#endif



