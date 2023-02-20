// #include <bits/stdc++.h>
#include "P3_DutchSudoku.h"

using namespace std;

int main(int argc, char * const argv[]) {

    if(argc!=2) {
        cout <<"ERROR: Must have one argument: input file path" << endl;
        return 0;
    } 

    Sudoku s(argv[1]);
    cout << "Solving the Dutch Sudoku ... " << endl;
    s.solveSudoku();

    return 0;
}