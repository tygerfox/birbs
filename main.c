/// "./main.c"
/// Simulation of "flocking" "birds". See "Novel type of Phase Transition in a System of Self-Driven Particles" (1995), T. Vicsek et al.

// C standard
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// GNU
#include <argp.h>

// Libs
#include <SDL2/SDL.h>

// Headers
#include "args.h"


// Resolution of window
const int WIDTH = 512;
const int HEIGHT = 512;
const int DOTSIZE = 3;


// Protoypes
SDL_Window* init_window(int width, int height);
SDL_Renderer* init_renderer(SDL_Window* window);
void clear_frame(SDL_Renderer* renderer, int red, int blue, int green);
void draw_dot(SDL_Renderer* renderer, int x, int y);
float rand_uniform(float a, float b);
void anim_loop(SDL_Renderer* renderer, int width, int height, World* world);


// Entrypoint
int main(int argc, char *argv[]) {
	SDL_Window* window = init_window(WIDTH, HEIGHT);
	SDL_Renderer* renderer = init_renderer(window);

	srand(0);

	World world = {0};
	struct arguments args = get_args(argc, argv);
	world.N = args.num_birds;
	world.eta = args.eta;
	world.size = args.size;
	world.radius = args.radius;
	world.speed = args.speed;
	world.arrows = calloc(world.N, sizeof(Arrow));

	for (int i=0; i<world.N; ++i) {
		world.arrows[i].x = rand_uniform(0, world.size);
		world.arrows[i].y = rand_uniform(0, world.size);
		world.arrows[i].angle = rand_uniform(-M_PI, M_PI);
	}

	float framerate = 24;
	int running = 1;
	SDL_Event event;
	Uint32 t0 = SDL_GetTicks();

	while (running) {
		anim_loop(renderer, WIDTH, HEIGHT, &world);
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				running = 0;

		Uint32 t1 = SDL_GetTicks();
		Uint32 dt = t1 - t0;
		t0 = t1;
		if (dt < 1000 / framerate)
			SDL_Delay(1000 / framerate - dt);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


SDL_Window* init_window(int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window* window = SDL_CreateWindow("SDL Example",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	return window;
}

SDL_Renderer* init_renderer(SDL_Window* window) {
	return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void clear_frame(SDL_Renderer* renderer, int red, int blue, int green) {
	SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void draw_dot(SDL_Renderer* renderer, int x, int y) {
	SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, DOTSIZE, DOTSIZE});
}

float rand_uniform(float a, float b) {
	return rand() / (float) RAND_MAX * (b - a) + a;
}

void move_arrows(World world) {
	for (int i=0; i < world.N; ++i) {
		float angle = world.arrows[i].angle;
		float dx = cosf(angle);
		float dy = sinf(angle);
		float x = world.arrows[i].x;
		float y = world.arrows[i].y;
		x += world.speed * dx;
		y += world.speed * dy;
		// Keep within boundaries
		if (x < 0)
			x += world.size;
		else if (x > world.size)
			x -= world.size;
		if (y < 0)
			y += world.size;
		else if (y > world.size)
			y -= world.size;
		world.arrows[i].x = x;
		world.arrows[i].y = y;
	}
}

void point_arrows(World world) {
	for (int i=0; i < world.N; ++i) {
		Arrow *birb = &world.arrows[i];
		float xsum = 0, ysum = 0;
		for (int j=0; j < world.N; ++j) {
			if (j == i)
				continue;
			Arrow *neighbour = &world.arrows[j];
			float dx = fabsf(neighbour->x - birb->x);
			float dy = fabsf(neighbour->y - birb->y);
			// Distance in recursive space
			if (dx > world.size)
				dx -= world.size;
			if (dy > world.size)
				dy -= world.size;
			float r2 = world.radius * world.radius;
			if (dx > world.radius || dy > world.radius || dx*dx + dy*dy > r2)
				continue;
			xsum += cosf(neighbour->angle);
			ysum += sinf(neighbour->angle);
		}
		float mean_angle = atan2f(ysum, xsum);
		if ((ysum == 0) & (xsum == 0))
			mean_angle = world.arrows[i].angle;
		float deflection = rand_uniform(-world.eta / 2, world.eta / 2);
		world.arrows[i].angle_buffer = mean_angle + deflection;
	}
	for (int i=0; i < world.N; ++i)
		world.arrows[i].angle = world.arrows[i].angle_buffer;
}

void gravity(World world) {
	for (int i=0; i<world.N; ++i) {
		Arrow *birb = &world.arrows[i];
		float xvel = world.speed * cosf(birb->angle);
		float yvel = world.speed * sinf(birb->angle);
		float xdist = birb->x - world.size / 2;
		float ydist = birb->y - world.size / 2;
		xvel -= xdist / 1000;
		yvel -= ydist / 1000;
		float norm = sqrtf(xvel*xvel + yvel*yvel);
		xvel *= world.speed / norm;
		yvel *= world.speed / norm;
		world.arrows[i].angle = atan2f(yvel, xvel);
	}
}

void anim_loop(SDL_Renderer* renderer, int width, int height, World* world) {
	move_arrows(*world);
	point_arrows(*world);
	//gravity(*world); // experimental

	clear_frame(renderer, 255, 255, 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i=0; i < world->N; ++i) {
		int x = width * (world->arrows[i].x / world->size);
		int y = width * (world->arrows[i].y / world->size);
		draw_dot(renderer, x, y);
	}

	SDL_RenderPresent(renderer);
}



