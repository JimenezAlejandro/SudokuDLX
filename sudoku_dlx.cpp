#include "sudoku_dlx.hpp"
#include <iomanip>
#include <iostream>
#include <cmath>
#include <climits>

sudokuSolver::sudokuSolver(int size) : size(size == 9 || size == 4 ? size : 9), solved(false)
{
    createHeader();
    createMatrix();
}

void sudokuSolver::appendNodeVertical(rowNode* n)
{
    n->up = n->head->up;
    n->down = n->head;
    n->head->up->down = n;
    n->head->up = n;

    n->head->size++;
}

void sudokuSolver::cover(node* col)
{
    for (node* down = col->down; down != col; down = down->down)
    {
        for (node* right = down->right; right != down; right = right->right)
        {
            right->up->down = right->down;
            right->down->up = right->up;

            right->head->size--;
        }
    }

    col->left->right = col->right;
    col->right->left = col->left;
}

columnNode* sudokuSolver::chooseColumn()
{
    int minSize = INT_MAX;
    columnNode* chosen = static_cast<columnNode*>(this->h->right);

    for (columnNode* col = chosen; minSize > 1 && col != this->h; col = static_cast<columnNode*>(col->right))
    {
        if (col->size < minSize)
        {
            chosen = col;
            minSize = col->size;
        }
    }
    return chosen;
}

void sudokuSolver::createHeader()
{
    this->h = new columnNode();
    this->h->left = h;
    this->h->right = h;

    this->cols = new columnNode*[this->size * this->size * NUM_CONSTRAINTS];

    columnNode* prev = this->h;

    for (int headerCol = 0; headerCol < this->size * this->size * NUM_CONSTRAINTS; ++headerCol)
    {
        int constraint = headerCol / (this->size * this->size);
        int pos = (headerCol / this->size) % 4;
        int num = headerCol % this->size;

        columnNode* current = new columnNode(columnData(constraint, pos, num));
        linkHorizontal(prev, current);
        current->up = current;
        current->down = current;

        this->cols[headerCol] = current;
        prev = current;
    }
}

void sudokuSolver::createMatrix()
{
    for (int row = 0; row < this->size; ++row)
    {
        for (int col = 0; col < this->size; ++col)
        {
            for (int num = 0; num < this->size; ++num)
            {
                rowData data = rowData(row, col, num);
                rowNode* prev = new rowNode(data, findHead(data, ROWCOL));
                prev->left = prev;
                prev->right = prev;
                appendNodeVertical(prev);

                for (int constraint = ROWNUM; constraint < NUM_CONSTRAINTS; ++constraint)
                {
                    rowNode* n = new rowNode(data, findHead(data, constraint));
                    appendNodeVertical(n);
                    linkHorizontal(prev, n);
                    prev = n;
                }
            }
        }
    }
}

columnNode* sudokuSolver::findHead(rowData& data, int constraint)
{
    columnNode* found = nullptr;

    switch (constraint)
    {
        case ROWCOL:
        {
            int pos = (constraint * this->size * this->size) + (data.row * this->size) + data.col;
            found = this->cols[pos];
            break;
        }
        case ROWNUM:
        {
            int pos = (constraint * this->size * this->size) + (data.row * this->size) + data.num;
            found = this->cols[pos];
            break;
        }
        case COLNUM:
        {
            int pos = (constraint * this->size * this->size) + (data.col * this->size) + data.num;
            found = this->cols[pos];
            break;
        }
        case REGNUM:
        {
            int region = getRegion(data.row, data.col);
            int pos = (constraint * this->size * this->size) + (region * this->size) + data.num;
            found = this->cols[pos];
            break;
        }

        default:
            break;
    }

    return found;
}

int sudokuSolver::findHeadPos(columnNode* col)
{
    return (col->data.constraint * this->size * this->size) + (col->data.pos * this->size) + col->data.num;
}

rowNode* sudokuSolver::findRow(rowData& data)
{
    rowNode* found = nullptr;
    
    for (int constraint = 0; !found && constraint < NUM_CONSTRAINTS; ++constraint)
    {
        columnNode* head = findHead(data, constraint);

        for (node* row = head->down; row != head; row = row->down)
        {
            if (static_cast<rowNode*>(row)->data == data)
            {
                found = static_cast<rowNode*>(row);
                break;
            }
        }
    }

    return found;
}

int sudokuSolver::getRegion(int row, int col)
{
    int bound = std::sqrt(this->size);
    int region = (row / bound) * bound + (col / bound);
    return region;
}

bool sudokuSolver::isValid(const std::string& puzzle)
{
    bool valid = true;

    if (puzzle.size() != this->size * this->size)
    {
        std::cout << "Puzzle is not valid. Size must match a " << this->size << "x" << this->size << " sudoku." << std::endl;
        valid = false;
    }

    for (auto it = puzzle.begin(); it != puzzle.end(); ++it)
    {
        if (*it != '.' && (*it < '1' || (*it - '0') > this->size))
        {
            std::cout << "Puzzle is not valid. Use the numbers 1 to " << this->size << " and a '.' for empty cells." << std::endl;
            valid = false;
            break;
        }
    }
    return valid;
}

void sudokuSolver::linkHorizontal(node* left, node* right)
{
    right->right = left->right;
    left->right->left = right;
    right->left = left;
    left->right = right;
}

void sudokuSolver::populateExisting(const std::string& puzzle)
{
    for (unsigned int pos = 0; pos < puzzle.size(); ++pos)
    {
        if (puzzle[pos] == '.')
        {
            continue;
        }

        rowData data = rowData(pos / this->size, pos % this->size, puzzle[pos] - '0' - 1);
        rowNode* row = findRow(data);
        columnNode* col = row->head;
        this->solution[pos] = row;

        for (node* right = row->right; right != row; right = right->right)
        {
            cover(right->head);
        }

        cover(col);
    }
}

void sudokuSolver::printMatrix()
{
    for (node* head = this->h->right; head->head->data.constraint < ROWNUM; head = head->right)
    {
        for (node* row = head->down; row != head; row = row->down)
        {
            rowData data = static_cast<rowNode*>(row)->data;
            std::cout << '(' << data.num << ',' << data.row << ',' << data.col << ')' << ' ';

            node* col = row;

            do
            {
                std::cout << '[' << col->head->data.constraint << ',' << col->head->data.pos << ',' << col->head->data.num << ']' << ' ';
                col = col->right;
            } while (col != row);

            std::cout << std::endl;
        }
    }
}

void sudokuSolver::printReadableMatrix()
{
    for (node* head = this->h->right; head != this->h && head->head->data.constraint < ROWNUM; head = head->right)
    {
        for (node* row = head->down; row != head; row = row->down)
        {
            rowData data = static_cast<rowNode*>(row)->data;
            std::cout << '(' << data.num << ',' << data.row << ',' << data.col << ')' << ' ';

            node* col = row;

            do
            {
                int offset = findHeadPos(col->head) - findHeadPos(col->left->head);
                for (int spaces = offset > 0 ? offset : findHeadPos(col->head); spaces > 0; --spaces)
                {
                    std::cout << "   ";
                }

                std::cout << '[' << col->head->data.constraint << ',' << col->head->data.pos << ',' << col->head->data.num << ']' << ' ';
                col = col->right;
            } while (col != row);

            std::cout << std::endl;
        }
    }
}

void sudokuSolver::printSolution()
{
    int width = 0;
    int remaining = this->size;
    while (remaining > 0)
    {
        width++;
        remaining /= 10;
    }
    
    for (auto it = this->solution.begin(); it != this->solution.end(); ++it)
    {
        if ((*it).first > 0 && (*it).first % this->size == 0)
        {
            std::cout << std::endl;
        }

        rowNode* row = static_cast<rowNode*>((*it).second);
        if (row->data.row > 0 && row->data.col == 0 && row->data.row % (int) std::sqrt(this->size) == 0)
        {
            std::cout << std::endl;
        }
        if (row->data.col > 0 && row->data.col % (int) std::sqrt(this->size) == 0)
        {
            std::cout << "   ";
        }
        std::cout << std::setw(2) << std::setiosflags(std::ios::left) << "[";
        std::cout << std::setw(width) << row->data.num + 1;
        std::cout << std::setw(2) << std::setiosflags(std::ios::right) << "]";
        std::cout << std::resetiosflags(std::ios::right);
    }

    std::cout << std::endl;
}

void sudokuSolver::reset()
{
    for (auto it = this->solution.begin(); it != this->solution.end(); ++it)
    {
        node* row = (*it).second;
        for (node* left = row->left; left != row; left = left->left)
        {
            uncover(left->head);
        }
        
        uncover(row->head);
        this->solution.erase((*it).first);
    }
}

void sudokuSolver::search()
{
    if (!solved)
    {
        if (this->h->right == this->h)
        {
            solved = true;
            printSolution();
            return;
        }

        columnNode* col = chooseColumn();
        cover(col);

        for (node* row = col->down; row != col; row = row->down)
        {
            int pos = static_cast<rowNode*>(row)->data.row * this->size + static_cast<rowNode*>(row)->data.col;
            this->solution[pos] = row;

            for(node* right = row->right; right != row; right = right->right)
            {
                cover(right->head);
            }

            search();
            row = this->solution[pos];
            this->solution.erase(pos);
            col = row->head;

            for(node* left = row->left; left != row; left = left->left)
            {
                uncover(left->head);
            }
        }

        uncover(col);
    }
}

void sudokuSolver::solve(const std::string& puzzle)
{
    if (isValid(puzzle))
    {
        if (this->solved)
        {
            reset();
            solved = false;
        }
        populateExisting(puzzle);
        search();
    }
}

void sudokuSolver::uncover(node* col)
{
    col->left->right = col;
    col->right->left = col;

    for (node* up = col->up; up != col; up = up->up)
    {
        for(node* left = up->left; left != up; left = left->left)
        {
            left->up->down = left;
            left->down->up = left;

            left->head->size++;
        }
    }
}

columnData::columnData() : constraint(0), pos(0), num(0)
{ }

columnData::columnData(int constraint, int pos, int num) : constraint(constraint), pos(pos), num(num)
{ }

columnNode::columnNode() : node(this), size(0), data(columnData())
{ }

columnNode::columnNode(columnData& data) : node(this), size(0), data(data)
{ }

columnNode::columnNode(columnData&& data) : node(this), size(0), data(data)
{ }

rowData::rowData(int row, int col, int num) : row(row), col(col), num(num)
{ }

bool rowData::operator==(const rowData& rhs)
{
    return this->row == rhs.row && this->col == rhs.col && this->num == rhs.num;
}

rowNode::rowNode(rowData& data) : data(data)
{ }

rowNode::rowNode(rowData&& data) : data(data)
{ }

rowNode::rowNode(rowData& data, columnNode* head) : node(head), data(data)
{ }

rowNode::rowNode(rowData&& data, columnNode* head) : node(head), data(data)
{ }

node::node() : head(nullptr), left(nullptr), right(nullptr), up(nullptr), down(nullptr)
{ }

node::node(columnNode* head) : head(head), left(nullptr), right(nullptr), up(nullptr), down(nullptr)
{ }