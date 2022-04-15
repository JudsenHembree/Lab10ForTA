3:
	gcc -g -D threebythree *.c -o test

5:
	gcc -g *.c -o test

run3: 3
	./test doggo.ppm

run5: 5
	./test doggo.ppm

runTest: all
	./test test.ppm

leaks:
	valgrind ./test doggo.ppm
debug: 
	gdb --args test doggo.ppm