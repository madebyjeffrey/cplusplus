
GTEST_DIR=../gtest

CXX_FLAGS=-std=c++11 -stdlib=libc++ -g -Wall -Wextra
CXX=clang++

SRC=sudoku.cpp
TST=test.cpp
 
.PHONY: all clean test

all: sudoku

clean:
	rm -f $(all) *.o sudoku test 
	rm -dR *.dSYM
	
sudoku: $(SRC) sudoku.h
	$(CXX) $(CXX_FLAGS) $< -o $@
	
test: $(TST)
	$(CXX) $(CXX_FLAGS) -Wno-unused-private-field $? -o $@
	./test
