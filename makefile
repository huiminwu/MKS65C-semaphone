all: ctl.c sema.c
	gcc -o Setup ctl.c
	gcc -o Run sema.c

setup: ctl.c
	./Setup $(args)

run: 
	./Run

debug: ctl.c sema.c
	gcc -g -o setup_debug ctl.c
	gcc -g -o run_debug   sema.c
