build:
	gcc -o nown src/main.c -lncurses

run:
	./nown

dev: build run