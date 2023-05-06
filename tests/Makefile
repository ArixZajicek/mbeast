


main: main.c
	@gcc main.c -o main -I../libcostume -L../libcostume -lcostume -lm

run: main
	@export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../libcostume
	./main