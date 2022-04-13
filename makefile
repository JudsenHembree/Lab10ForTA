all:
	gcc -g *.c -o test

run: all
	./test doggo.ppm

runTest: all
	./test test.ppm

leaks:
	valgrind ./test doggo.ppm
debug: 
	gdb --args test doggo.ppm