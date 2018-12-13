all: ctl.c sema.c
	gcc -o Setup ctl.c
	gcc -o Run sema.c

setup: ctl.c
	./Setup $(args)

run: 
	./Run
