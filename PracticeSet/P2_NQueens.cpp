#include <bits/stdc++.h>
#include "P2_NQueens.h"


using namespace std;
int main() {

    int boardSize;
    cout << "Enter Board Size: " << endl;
    cin >> boardSize;

    Board b = Board(boardSize);
    cout << "Solving N-Queens ... " << endl;
    b.solveNQueens();

    return 0;
}