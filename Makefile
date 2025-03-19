debug:
	gcc -Og -flto main.c args.c -lm -lSDL2 -Wall -g -o birbs
native:
	gcc -march=native -mtune=native -O3 -flto main.c args.c -lm -lSDL2 -o birbs
generic:
	gcc -O3 -flto main.c args.c -lm -lSDL2 -o birbs
