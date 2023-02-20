#include "P7_CoinToss.h"

using namespace std;

int main(int argc, char * const argv[]) {

    if(argc!=3) {
        cout <<"ERROR: Must have two arguments: q (Alice) and p (Bob)." << endl;
        return 0;
    } 

    double q = stod(argv[1]);
    double p = stod(argv[2]);

    if(q>p) {
        cout << "Alice wins for any number of trials as q > p";
        return 0;
    }

    cout << "Alice's coin: P(H) = " << q << " P(T) = " << 1-q << endl;
    cout << "Bob's coin: P(H) = " << p <<  " P(T) = " << 1-p << endl;

    CoinTossInstance c(q, p);

    int optimalTosses = c.getOptimalNumberOfTosses();
    cout << "Optimal Number of Coin Tosses for Alice: " << optimalTosses << endl;

    // commenting this code since I used library "Sciplot" to plot the graph
    cout << "\n************************************************************\n";
    cout << "NOTE: Code for plotting the graph has been commented to avoid compilation errors.\n\tGraph plotted using the Sciplot package has been added to submissions" << endl;
    cout << "\n************************************************************\n";
    // c.drawVerificationCurve(optimalTosses);


    return 0;
}