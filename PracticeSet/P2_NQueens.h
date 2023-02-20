#include <bits/stdc++.h>

using namespace std;

class Board {

    // params
    int boardSize;
    vector<vector<int>> board;

    // print the board
    void printBoard() {
        cout << "Solution: " << endl;
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
    bool solve(int col) {
        if(col >= boardSize)
            return true;
        else {
            for(int i = 0; i < boardSize; i++) {
                if(is_this_position_safe(i, col)) {
                    board[i][col] = 1;
                    if(solve(col+1))
                        return true;
                    else {
                        board[i][col] = 0;
                    } 
                }
            }
        }
        return false;
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
        if (solve(0))
			printBoard();
		else 
			cout << "No solution to the " << boardSize << "-Queens Problem" << endl;
    }
};