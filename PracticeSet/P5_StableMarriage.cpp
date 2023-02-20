#include "P5_StableMarriage.h"

using namespace std;

int main(int argc, char * const argv[]) {

    if(argc!=2) {
        cout <<"ERROR: Must have one argument: input file path" << endl;
        return 0;
    } 

    GaleShapley s(argv[1]);
    cout << "Solving the Stable Matching Problem ... " << endl;
    s.getStableMatching();

    return 0;
}