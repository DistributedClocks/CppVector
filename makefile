CC := g++
WARNINGS := -W -Wall -Werror
USE_11 := -std=c++11
SOURCES := ./src/CppVec.cpp ./src/VClock/VClock.cpp

TEST_OBJ := cppvec-tests.o

cppvec-client-server: $(SOURCES) $(TEST_OBJ)
	./$(TEST_OBJ)
	$(CC) $(WARNINGS) $(USE_11) -o cppvec-client-server.o ./example/client-server.cpp $(SOURCES)

cppvec-tests.o: $(SOURCES)
	$(CC) $(WARNINGS) $(USE_11) -o $(TEST_OBJ) tests/test-cppvec.cpp $(SOURCES)