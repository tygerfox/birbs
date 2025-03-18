/// "main.h"

#ifndef MAIN_H

// Structs
typedef struct arrow {
	float x, y;
	float angle;
	float angle_buffer;
} Arrow;

typedef struct world {
	float size;
	Arrow* arrows;
	int N;
	// Eta is the stochastic parameter to the birds movement.
	float eta;
	float speed;
	float radius;
} World;

#endif



