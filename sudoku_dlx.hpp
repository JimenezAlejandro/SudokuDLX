#ifndef SUDOKU_DLX_H
#define SUDOKU_DLX_H

#include <map>
#include <string>

const int NUM_CONSTRAINTS = 4;

class sudokuSolver;
struct columnData;
struct columnNode;
struct rowData;
struct rowNode;
struct node;

class sudokuSolver
{
public:
    explicit sudokuSolver(int size = 9);
    void solve(const std::string& puzzle);
private:
    columnNode* h;
    columnNode** cols;
    std::map<int, node*> solution;
    int size;
    bool solved;

    enum Constraint { ROWCOL = 0, ROWNUM = 1, COLNUM = 2, REGNUM = 3 };

    void appendNodeVertical(rowNode* n);
    columnNode* chooseColumn();
    void cover(node* col);
    void createHeader();
    void createMatrix();
    columnNode* findHead(rowData& data, int constraint);
    int findHeadPos(columnNode* col);
    rowNode* findRow(rowData& data);
    int getRegion(int row, int col);
    bool isValid(const std::string& puzzle);
    void linkHorizontal(node* left, node* right);
    void populateExisting(const std::string& puzzle);
    void printMatrix();
    void printReadableMatrix();
    void printSolution();
    void reset();
    void search();
    void uncover(node* col);
};

struct node
{
    node* left;
    node* right;
    node* up;
    node* down;
    columnNode* head;

    node();
    explicit node(columnNode* head);
};

struct columnData
{
    int constraint;
    int pos;
    int num;

    columnData();
    columnData(int constraint, int pos, int num);
};

struct columnNode : node
{
    int size;
    columnData data;

    columnNode();
    explicit columnNode(columnData& data);
    explicit columnNode(columnData&& data);
};

struct rowData
{
    int row;
    int col;
    int num;

    rowData(int row, int col, int num);
    bool operator==(const rowData& rhs);
};

struct rowNode : node
{
    rowData data;

    explicit rowNode(rowData& data);
    explicit rowNode(rowData&& data);
    rowNode(rowData& data, columnNode* head);
    rowNode(rowData&& data, columnNode* head);
};

#endif