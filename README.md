
# Sudoku Solver using Dancing Links

This Sudoku solver uses Donald Knuth's Algorithm X and the dancing links technique to efficiently solve Sudoku puzzles.

![Example 1](/img/example_01.png)
![Example 2](/img/example_02.png)

## Getting Started

### Prerequisites

* C++ compiler

### Running

First compile and link the source and header files

```
g++ -o sudokudlx main.cpp sudoku_dlx.cpp
```

Then run on Windows with

```
.\sudokudlx
```
or on Linux
```
./sudokudlx
```

## Improvements

* Use arrays to store the nodes in each row as described in [Donald Knuth's The Art of Computer Programming Volume 4 Fascicle 5C](https://www-cs-faculty.stanford.edu/~knuth/fasc5c.ps.gz)
* Allow users to create an NxN sudoku solver where N is a perfect square
* Create a GUI

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* [Jonathan Chu's explanation of a sudoku solver implementation using dancing links](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/sudoku.paper.html)
* [Donald Knuth's paper on dancing links](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf)
* [Wikipedia article on Donald Knuth's Algorithm X](https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X)