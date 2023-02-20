#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace std;

class Sudoku {

    // params
    int sudokuSize = 9;
    vector<vector<int>> sudoku;

    // print the sudoku
    void printSudoku() {
        cout << "Board Position: " << endl;
        cout << "-----------------" << endl;
        for(int i = 0; i < sudokuSize; i++) {
            for(int j = 0; j < sudokuSize; j++) {
                cout << sudoku[i][j] << " ";
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }

    // find an empty box
    bool findEmptyBox(int &row, int &col, int r, int c) {
        for(row = r; row < sudokuSize; row++) {
            for(col = 0; col < sudokuSize; col++) {
                if(sudoku[row][col]==0)
                    return true;
            }
        }
        return false;
    }

    // check if the position i,j will be valid given an inital config of the chess sudoku
    bool is_this_position_safe(int row, int col, bool print) {

        // if print bool is true, then print where the problem came, else dont

        // no element repeats in row
        unordered_set<int> m;
        for(int i = 0; i < sudokuSize;i++) {
            if(sudoku[row][i]!=0) {
                if(m.find(sudoku[row][i])==m.end())
                    m.insert(sudoku[row][i]);
                else {
                    (print)?(cout << "Row error: " << sudoku[row][i] << " repeats in the row " << row << endl):(cout << "");
                    return false;
                }
            }
        }

        // no element repeats in column
        m.clear();
        for(int i = 0; i < sudokuSize;i++) {
            if(sudoku[i][col]!=0) {
                if(m.find(sudoku[i][col])==m.end())
                    m.insert(sudoku[i][col]);
                else {
                    (print)?(cout << "Column error: " << sudoku[i][col] << " repeats in the column " << col << endl):(cout << "");
                    return false; 
                }
            }
        }

        // no element repeats in the 3x3 box
        m.clear();
        for(int i = (row/3)*3; i < (row/3)*3+3; i++) {
            for(int j = (col/3)*3; j < (col/3)*3+3; j++) {
                if(sudoku[i][j]!=0) {
                    if(m.find(sudoku[i][j])==m.end())
                        m.insert(sudoku[i][j]);
                    else {
                        (print)?(cout << "3x3 box error: " << sudoku[i][j] << " repeats in the 3x3 box enclosing index [" << i << "," << j << "]" << endl):(cout << "");
                        return false;
                    } 
                }
            }
        }

        // no element repeats along the positive diagonals + difference of 4 between adjacent elements in the diagonal
        m.clear();
        m.insert(sudoku[row][col]);
        int k = 1;
        while(k<sudokuSize) {

            // accessing right half of positive diagonal
            if(row-k>=0 && col+k<sudokuSize && sudoku[row-k][col+k]!=0) {
                // if element does not repeat
                if(m.find(sudoku[row-k][col+k])==m.end()) {
                    // if adjacent element entry is filled and the difference between adjacent elements is <4
                    if(sudoku[row-k+1][col+k-1]!=0 && abs(sudoku[row-k+1][col+k-1]-sudoku[row-k][col+k])<4) {
                        (print)?(cout << "Positive Diagonal difference error: sudoku[" << row-k << "][" << col+k << "] - "
                                           << "sudoku[" << row-k+1 << "][" << col+k-1 << "] < 4" << endl):(cout << "");
                        return false;
                    }
                    // else enter it in the set
                    else
                        m.insert(sudoku[row-k][col+k]);
                }
                // if element repeats along diagonal
                else {
                    (print)?(cout << "Positive Diagonal error: " << sudoku[row-k][col+k] << " repeats in diagonal of index [" << row << "," << col << "]" << endl):(cout << "");
                    return false;
                } 
            }
            
            // accessing left half of positive diagonal   
            if(row+k<sudokuSize && col-k>=0 && sudoku[row+k][col-k]!=0) { 
                // if element does not repeat
                if(m.find(sudoku[row+k][col-k])==m.end()) {
                    // if adjacent element entry is filled and the difference between adjacent elements is <4
                    if(sudoku[row+k-1][col-k+1]!=0 && abs(sudoku[row+k-1][col-k+1]-sudoku[row+k][col-k])<4) {
                        (print)?(cout << "Positive Diagonal difference error: sudoku[" << row+k << "][" << col-k << "] - "
                                           << "sudoku[" << row+k-1 << "][" << col-k+1 << "] < 4" << endl):(cout << "");
                        return false;
                    }
                    // else enter it in the set
                    else
                        m.insert(sudoku[row+k][col-k]);
                }
                // if element repeats along diagonal
                else {
                    (print)?(cout << "Positive Diagonal error: " << sudoku[row+k][col-k] << " repeats in diagonal of index [" << row << "," << col << "]" << endl):(cout << "");
                    return false;
                } 
            }
            k++;
        }

        return true;
    }

    // solve the Dutch Sudoku
    void solve(int r, int c) {

        // checking if there are unfilled spaces, if not, return, since we have already solved the puzzle
        int row, col;
        if(findEmptyBox(row, col, r, c)==false) {
            // if any solution is found, print the solution and backtrack
            printSudoku();
            return;
        }
        
        // iterating over all possible values a cell can take
        for(int k = 1; k <= sudokuSize; k++) {

            // put k in that cell temperorily
            sudoku[row][col] = k;

            // if assigning value k for that space was a valid assignement
            if(is_this_position_safe(row, col, false)) {
                solve(row, col);
                // empty that cell so more values can be tried and all solutions can be generated
                sudoku[row][col] = 0;
            }

        }

        // if no value of k was a valid assignment, simply empty that space and backtrack to try more combinations for previous recursors
        sudoku[row][col] = 0;
    }

public:

    // default constructor taking filepath as a parameter from main
    Sudoku(string filePath) {
        ifstream sudokuFile(filePath);
        int value = 0;
        for(int i = 0; i < sudokuSize; i++) {
            sudokuFile >> value;
            sudoku.push_back({value});
            for(int j = 1; j < sudokuSize; j++) {
                sudokuFile >> value;
                sudoku[i].push_back(value);
            }
        }
        cout << "Initial position: " << endl;
        printSudoku();
    }

    // main public function
    void solveSudoku() {

        // check if the initial state of the sudoku is valid or not
        for(int i = 0; i < sudokuSize; i++) {
            if(is_this_position_safe(0, i, true)==false || is_this_position_safe(i, i, true)==false) {
                cout << "EXIT: Partially Filled Sudoku does not meet the Dutch Sudoku requirements" << endl;
                return;
            }
        }

        solve(0,0);
	    cout << "No more solution to the Dutch Sudoku Problem" << endl;
    }
};