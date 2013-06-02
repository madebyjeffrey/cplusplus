//
//  main.cpp
//  Sudoku
//
//  Created by Jeffrey Drake on 2013-06-01.
//  Copyright (c) 2013 Jeffrey Drake. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "sudoku.h"

int main(int, const char *[])
{
    auto b = create_board();
    std::ofstream of("/tmp/board.html");
    
    of << html_board(b) << std::endl;
    of.close();
    
    system("open /tmp/board.html");
    
    // insert code here...
//    std::cout << "Hello, World!\n";
    return 0;
}

