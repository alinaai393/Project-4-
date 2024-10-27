CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

run_fast:
	./app.exe -f

run_fast_quiet:
	./app.exe -f -q

run_year:
	./app.exe -y 2020

valgrind:
	rm -f app.exe
	gcc -g app.c MinPopVote.c -o app.exe
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe -f

build_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe

run_test:
	./test.exe

clean:
	rm -f app.exe test.exe
