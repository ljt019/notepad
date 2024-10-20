all: build run

build:
	gcc -o target/main src/main.c -mwindows

run:
	.\target\main.exe