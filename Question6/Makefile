# Name of the executable
TARGET = mynetcat

# Source files
SRC = mynetcat.c 

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g -ftest-coverage -fprofile-arcs

# Default rule - compile the program
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean up generated files
clean:
	rm -f $(TARGET) *.gcno *.gcda *.gcov input_file.txt client_output.txt

# Rule to generate code coverage
coverage: all
	./run_tests.sh
	gcov mynetcat.c

# Declare clean and coverage as phony targets
.PHONY: all clean coverage

# chmod +x run_tests.sh
# make clean
# make coverage
