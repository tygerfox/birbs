debug:
	gcc -Og main.c args.c -lm -lSDL2 -Wall -g -o program
release:
	gcc -march=native -mtune=native -O3 main.c args.c -lm -lSDL2 -o program_release
generic:
	gcc -O2 main.c args.c -lm -lSDL2 -Wall -o program_generic
