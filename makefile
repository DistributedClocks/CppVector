# Commands
CC := g++
WARNINGS := -W -Wall -Werror
USE_11 := -std=c++11
RM := rm -f

# Files
SOURCES := ./src/CppVec.cpp ./src/VClock/VClock.cpp
TEST_OBJ := cppvec-tests.o
EXAMPLE_OBJ := cppvec-client-server.o

# Run to clean and rebuild example project
all: clean cppvec-client-server

# Run all tests
test: $(TEST_OBJ)
	./$(TEST_OBJ)

cppvec-client-server: clean $(SOURCES)	
	$(CC) $(WARNINGS) $(USE_11) -o $(EXAMPLE_OBJ) ./example/client-server.cpp $(SOURCES)

cppvec-tests.o: clean $(SOURCES)
	$(CC) $(WARNINGS) $(USE_11) -o $(TEST_OBJ) tests/test-cppvec.cpp $(SOURCES)

clean:
	$(RM) *.o