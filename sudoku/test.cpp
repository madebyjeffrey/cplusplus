//
//  main.cpp
//  SudokuTest
//
//  Created by Jeffrey Drake on 2013-06-01.
//  Copyright (c) 2013 Jeffrey Drake. All rights reserved.
//

#define CATCH_CONFIG_MAIN // provide main
#include "catch.hpp"

#include <iostream>
#include <algorithm>
#include <vector>


#include "../Sudoku/sudoku.h"

TEST_CASE("integer/division", "Testing assumption of n/3*3") {
	REQUIRE((0/3*3)==0);
	REQUIRE((1/3*3)==0);
	REQUIRE((2/3*3)==0);
	REQUIRE((3/3*3)==3);
	REQUIRE((4/3*3)==3);
	REQUIRE((5/3*3)==3);
	REQUIRE((6/3*3)==6);
	REQUIRE((7/3*3)==6);
	REQUIRE((8/3*3)==6);
}

TEST_CASE("sudoku/board_allowed_at", "The numbers allowed at a square") {
    Board b { { 0 } };
    
    auto allowed = board_allowed_at(b, 0, 0);
    decltype(board_allowed_at(b, 0, 0)) compare = { 1,2,3,4,5,6,7,8,9 };

	std::sort(std::begin(allowed), std::end(allowed));
	
    REQUIRE(std::equal(std::begin(allowed), std::end(allowed), std::begin(compare)));
}


TEST_CASE("sudoku/board_allowed_at2", "The numbers allowed at a square") {
    Board b { { 4, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    auto allowed = board_allowed_at(b, 0, 0);

	REQUIRE(allowed.empty());
}

TEST_CASE("sudoku/board_allowed_at3", "The numbers allowed at a square") {
    Board b { { 0, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    auto allowed = board_allowed_at(b, 0, 0);

	REQUIRE(allowed.size() == 1);
	REQUIRE(allowed[0] == 4);
}

TEST_CASE("sudoku/board_allowed_at4", "The numbers allowed at a square") {
    Board b { { 4, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 0, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    auto allowed = board_allowed_at(b, 3, 3);

	REQUIRE(allowed.size() == 1);
	REQUIRE(allowed[0] == 5);
}

TEST_CASE("sudoku/board_allowed_at5", "The numbers allowed at a square") {
    Board b { { 0, 0, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    auto allowed = board_allowed_at(b, 0, 0);
    std::sort(std::begin(allowed), std::end(allowed));
    decltype(board_allowed_at(b, 0, 0)) should_be { 4 };
    
//     std::cerr << allowed << std::endl;

	REQUIRE(allowed.size() == 1);
	REQUIRE(std::equal(std::begin(allowed), std::end(allowed), std::begin(should_be)));
	
	allowed = board_allowed_at(b, 0, 1);
	should_be = { 5 };
	REQUIRE(allowed.size() == 1);
	REQUIRE(std::equal(std::begin(allowed), std::end(allowed), std::begin(should_be)));
}

//board_third_has(Board const &b1, unsigned r, unsigned c, char n) -> bool {
TEST_CASE("sudoku/board_third_has", "The numbers allowed at a third part of board") {
    Board b { { 4, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
    	for (unsigned j = 0; j < board_size; ++j)
    		REQUIRE(board_third_has(b, i, j, b[i * board_size + j]));
}

TEST_CASE("sudoku/board_third_has9", "The numbers allowed at a third part of board, removed 9s") {
    Board b { { 4, 5, 2, 3, 0, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 0, 4,
                6, 7, 0, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 0,
                2, 4, 5, 0, 8, 7, 1, 6, 3,
                0, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 0, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 0, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 0, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
    	for (unsigned j = 0; j < board_size; ++j)
    		REQUIRE_FALSE(board_third_has(b, i, j, 9));
}

// board_row_has(
TEST_CASE("sudoku/board_row_has", "The numbers allowed on a row of board") {
    Board b { { 4, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
		for (unsigned j = 0; j < board_size; ++j) {
//     		std::cerr << "i, j: " << i << ", " << j << " > " << (int)b[i * board_size + j] << std::endl;
    	
    		REQUIRE(board_row_has(b, i, b[i * board_size + j]));
    	}
}

TEST_CASE("sudoku/board_row_has9", "The numbers allowed on a row of board, removed 9s") {
    Board b { { 4, 5, 2, 3, 0, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 0, 4,
                6, 7, 0, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 0,
                2, 4, 5, 0, 8, 7, 1, 6, 3,
                0, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 0, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 0, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 0, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
    	for (unsigned j = 0; j < board_size; ++j)
    		REQUIRE_FALSE(board_row_has(b, i, 9));
}

TEST_CASE("sudoku/board_col_has", "The numbers allowed on a col of board") {
    Board b { { 4, 5, 2, 3, 9, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 9, 4,
                6, 7, 9, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 9,
                2, 4, 5, 9, 8, 7, 1, 6, 3,
                9, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 9, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 9, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 9, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
		for (unsigned j = 0; j < board_size; ++j) {
//     		std::cerr << "i, j: " << i << ", " << j << " > " << (int)b[i * board_size + j] << std::endl;
    	
    		REQUIRE(board_column_has(b, j, b[i * board_size + j]));
    	}
}

TEST_CASE("sudoku/board_col_has9", "The numbers allowed on a col of board, removed 9s") {
    Board b { { 4, 5, 2, 3, 0, 1, 8, 7, 6, 
                3, 1, 8, 6, 7, 5, 2, 0, 4,
                6, 7, 0, 4, 2, 8, 3, 1, 5,
                8, 3, 1, 5, 6, 4, 7, 2, 0,
                2, 4, 5, 0, 8, 7, 1, 6, 3,
                0, 6, 7, 2, 1, 3, 5, 4, 8,
                7, 0, 6, 8, 5, 2, 4, 3, 1,
                1, 8, 3, 7, 4, 0, 6, 5, 2,
                5, 2, 4, 1, 3, 6, 0, 8, 7 } };
    
    for (unsigned i = 0; i < board_size; ++i)
    	for (unsigned j = 0; j < board_size; ++j)
    		REQUIRE_FALSE(board_column_has(b, j, 9));
}

TEST_CASE("random_seq", "Should ensure all numbers are represented") {
	auto res = random_seq(1, 9);
	std::sort(std::begin(res), std::end(res));
	decltype(random_seq(1,9)) should_be { 1,2,3,4,5,6,7,8,9 };
	
	REQUIRE(std::equal(std::begin(res), std::end(res), std::begin(should_be)));
}

TEST_CASE("rowcol", "Ensure row and col are correct for the indexes") {
	REQUIRE(row(0) == 0);
	REQUIRE(row(1) == 0);
	REQUIRE(row(1 * board_size) == 1);
	REQUIRE(row(board_size * board_size - 1) == board_size - 1);
	REQUIRE(col(0) == 0);
	REQUIRE(col(1) == 1);
	REQUIRE(col(1 * board_size) == 0);
	REQUIRE(col(board_size * board_size - 1) == board_size - 1);
}