#include "sudoku_dlx.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string s;
    sudokuSolver sudoku = sudokuSolver();
    sudokuSolver sudokuFour = sudokuSolver(4);

    while (true)
    {
        std::cout << "1. 9x9 sudoku" << std::endl;
        std::cout << "2. 4x4 sudoku" << std::endl;
        std::cout << "3. Quit\n" << std::endl;
        std::cout << "Enter option: ";
        std::cin >> s;
        std::cout << std::endl;
        std::string puzzle;

        if (s.compare("3") == 0)
        {
            break;
        }
        else if (s.compare("1") == 0)
        {
            std::cout << "Enter 9x9 sudoku: ";
            std::cin >> puzzle;
            std::cout << std::endl;
            sudoku.solve(puzzle);
            std::cout << std::endl;
        }
        else if (s.compare("2") == 0)
        {
            std::cout << "Enter 4x4 sudoku: ";
            std::cin >> puzzle;
            std::cout << std::endl;
            sudokuFour.solve(puzzle);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Use one of the options above, example format for sudoku:" << std::endl;
            std::cout << "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......\n" << std::endl;
        }
    }

    return 0;
}
