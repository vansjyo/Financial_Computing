#include <bits/stdc++.h>

using namespace std;

class Board {

    // params
    int boardSize;
    int numberOfSoln = 0;
    vector<vector<int>> board;

    // print the board
    void printBoard() {
        cout << "-----------------" << endl;
        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board.size(); j++) {
                cout << (board[i][j]?"Q":"x") << " ";
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }

    // check if the position i,j will be valid given an inital config of the chess board
    bool is_this_position_safe(int row, int col) {

        // iterate over all positions where a queen is found and check if [row, col] does not conflict with any of them
        int k = 1;
        while(k<boardSize) {
            if( board[row][col] || 
                (col-k >= 0 && board[row][col-k]) ||
                (row-k >= 0 && board[row-k][col]) ||
                (row+k < boardSize && col+k < boardSize && board[row+k][col+k]) ||
                (row-k >= 0 && col-k >= 0 && board[row-k][col-k]) ||
                (row-k >= 0 && col+k < boardSize && board[row-k][col+k]) ||
                (row+k < boardSize && col-k >= 0 && board[row+k][col-k]) )
                return false;
            k++;
        }

        return true;
    }

    // solve the NQueens
    void solve(int col) {
        /* 
            Idea here is to simply keep printing solution if the recursion is able to reach col==boardSize and try a different row for the previous step
            and see if any other solution is possible keeping all previous config same and only iterating to the next row instead. Before iterating to the
            next row, we also remove the queen from the previous [row,col] irrespective of whether we found a solution or not, because in both conditions
            we remove the queen from the previously placed position. This way we are basically trying all posssible combinations in which we can get a 
            solution and not returning as soon as we find "a" possible solution. When we find a solution, we assert that by simply printing it.
            Took some help from: https://www.techiedelight.com/print-possible-solutions-n-queens-problem/
        */
        if(col >= boardSize) {
            cout << "Solution #" << numberOfSoln+1 << ": " << endl;
            printBoard();
            numberOfSoln++;
            return;
        }

        for(int i = 0; i < boardSize; i++) {
            if(is_this_position_safe(i, col)) {
                board[i][col] = 1;
                solve(col+1);
                board[i][col] = 0;
            }
        }
    
    }

public:

    // default constructor taking [boardDim = size of board] as a parameter from main
    Board(int boardDim) {
        boardSize = boardDim;
        for(int i = 0; i < boardSize; i++) {
            board.push_back({0});
            for(int j = 0; j < boardSize; j++) {
                board[i].push_back(0);
            }
        }
    }

    // main public function
    void solveNQueens() {
        solve(0);
		cout << "No more solutions to the " << boardSize << "-Queens Problem" << endl;
        cout << "Total number of solutions: " << numberOfSoln << endl;
    }
};