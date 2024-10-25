# Build the main application
build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe -I.

# Run the application with default settings
run:
	./app.exe

# Run the application in quiet mode
run_quiet:
	./app.exe --quiet

# Run the application in fast mode
run_fast:
	./app.exe --fast

# Run the application with both quiet and fast modes ON
run_quiet_fast:
	./app.exe --quiet --fast

# Run the application for a specific year (example: year 2020)
run_year_2020:
	./app.exe --year 2020

# Valgrind check with fast mode ON
valgrind:
	rm -f app.exe
	gcc -g app.c MinPopVote.c -o app.exe -I.
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe --fast

# Build the testing suite
build_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe -I.

# Run the testing suite
run_test: build_test
	./test.exe

# Clean up the build (remove executables)
clean:
	rm -f app.exe test.exe

# Build both the main application and the testing suite
build_all: build build_test
