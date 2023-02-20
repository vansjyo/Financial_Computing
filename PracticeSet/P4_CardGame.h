#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <math.h>
#define EPS 0.00001f

using namespace std;

class Deck {

    int deckSize;

    float solve(int redLeft, int blackLeft, vector<vector<float>>& mem) {
        if(redLeft<0 || blackLeft<0) return 0;
        if(abs(mem[redLeft][blackLeft]-(-1)) > EPS) {
            return mem[redLeft][blackLeft];
        }
        float p = (float)redLeft/(redLeft+blackLeft);
        float val = fmax(p*solve(redLeft-1, blackLeft, mem) + (1-p)*solve(redLeft, blackLeft-1, mem), blackLeft - redLeft);
        mem[redLeft][blackLeft] = val;
        return mem[redLeft][blackLeft];
    }

public:

    // default constructor taking deck size as parameter
    Deck(int size) {
        deckSize = size;
    }

    // main public function
    int computeValue() {
        vector<vector<float>> mem(deckSize, vector<float> (deckSize, -1));
        mem[0][0] = 0;
        cout << "Value is: " << solve(deckSize/2, deckSize/2, mem) << endl;
        return 0;
    }
};