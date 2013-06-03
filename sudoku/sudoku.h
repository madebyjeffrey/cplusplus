//
//  sudoku.h
//  Sudoku
//
//  Created by Jeffrey Drake on 2013-06-01.
//  Copyright (c) 2013 Jeffrey Drake. All rights reserved.
//

#ifndef Sudoku_sudoku_h
#define Sudoku_sudoku_h

#include <array>
#include <random>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <numeric>

using Board = std::array<char, 81>;
unsigned const board_size = 9;

auto board_column_has(Board const &b1, unsigned c, char n) -> bool {
	for (unsigned r = 0; r < board_size; ++r) {
        if (b1[r * board_size + c] == n) return true;
    }
    return false;
}

auto board_row_has(Board const &b1, unsigned r, char n) -> bool {
    for (unsigned c = 0; c < board_size; ++c) {
        if (b1[r * board_size + c] == n) return true;
    }
    return false;
}

auto board_third_has(Board const &b1, unsigned r, unsigned c, char n) -> bool {
    unsigned rt = r / 3 * 3, ct = c / 3 * 3;    // row and col third origin
    
    for (r = 0; r < 3; ++r)
        for (c = 0; c < 3; ++c)
            if (b1[(rt + r) * board_size + ct + c] == n) return true;
    return false;
}

auto board_allowed_at(Board const &b1, unsigned r, unsigned c) -> std::vector<char> {
    // assume cell is empty

    std::vector<char> allowed;
	for (char n = 1; n <= 9; ++n) {
        if (not board_column_has(b1, c, n) and
            not board_row_has(b1, r, n) and
            not board_third_has(b1, r, c, n)) {
            allowed.push_back(n);
        } 
    }
    return allowed;
}

auto row(unsigned n) -> unsigned {
	return n / board_size;
}

auto col(unsigned n) -> unsigned {
	return n % board_size;
}

auto random_seq(unsigned min, unsigned max) -> std::vector<unsigned> {
	std::vector<unsigned> range(max-min+1);

	std::iota(std::begin(range), std::end(range), min);
	std::random_device rd;
	std::mt19937 gen(rd());

	std::shuffle(std::begin(range), std::end(range), gen);
	
	return range;
}


auto create_board2() -> Board {
	Board b { { 0 } };
	
	auto seq = random_seq(1, 9);
	unsigned index = 0;
	unsigned const max_index = board_size * board_size - 1;
	
	for (index = 0; index < max_index; ++index) {
		if (seq.empty()) seq = random_seq(1,9);
		
		char n = seq.back(); seq.push_back();
		if (board_allowed_at(b, row(index), col(index), n)) {
			b[index] = n;
		}
	}
		
	
	
}

auto create_board() -> Board {
    Board b1 { {0} };
    
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dis(1,9);
    
    for (unsigned r = 0; r < board_size; ++r) {
        unsigned conflicts = 0;
        for (unsigned c = 0; c < board_size; ++c) {
            do {
                unsigned val = dis(gen);
                if (!board_row_has(b1, r, val)    &&
                    !board_column_has(b1, c, val) &&
                    !board_third_has(b1, r, c, val)) {
                    b1[r * board_size + c] = val;
                    std::cout << r << ", " << c << " : " << val << std::endl;
                    conflicts = 0;
                }
                else {
                        //                    std::cout << "conflict"  << r << ", " << c << " : " << val << std::endl;
                    conflicts++;
                }
                    
                if (conflicts > 9) {
                    std::cout << "Terminated early" << std::endl;
                    return b1;
                }
                    
            } while ( conflicts > 0 );
            
        }
    }
                    
    return b1;
}
                
auto html_board(Board const &b1) -> std::string {
    std::stringstream buf;
    
    buf << R"(
             <!doctype html>
             <html>
             <head>
             <style>
             	table {
             		border: 1px black solid;
             		border-collapse: collapse;
             	}
             	td {
             		width: 2em;
             		height: 2em;
             		text-align: center;
             		vertical-align: middle;
             		font-size: 16pt;
             		border: 1px black solid;
             	}
				tr:first-child td { 
             		border-top: 3px black solid;
             	}
             	tr:nth-child(3n) td { 
             		border-bottom: 3px black solid;
             	}
				col:first-child {
             		border-left: 3px black solid;
             	}

             	col:nth-child(3n) {
             		border-right: 3px black solid;
             	}
             	
             </style>
             <title>Sudoku Test</title>
             </head>
             <body>
                <table>)";
    for (unsigned r = 0; r < board_size; ++r) {
    	buf << "<col>";
    }
    for (unsigned r = 0; r < board_size; ++r) {
        buf << "<tr>";
        for (unsigned c = 0; c < board_size; ++c) {
            buf << "<td>" << std::to_string(b1[r * board_size + c]) << "</td>" << std::endl;
        }
        buf << "</tr>";
    }
    
    buf << R"(
                </table>
             </body>
             </html>)";
    
    return buf.str();
}
                    
template<typename T> 
auto operator<<(std::ostream &os, std::vector<T> const &v) 
	-> typename std::enable_if<std::is_integral<T>::value, std::ostream &>::type {
	std::for_each(std::begin(v), std::end(v), [&](T c) {
		os << (long)c << " ";
	});
	return os;
}
                
                
#endif
