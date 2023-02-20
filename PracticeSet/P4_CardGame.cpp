#include "P4_CardGame.h"

using namespace std;

int main(int argc, char * const argv[]) {

    // Justification for the equation:
    //
    // So when we are left with lets say X red and Y black cards in the deck still, we can either (1) stop or (2) continue the game.
    //
    // if we choose (2):
    // -> i.e. we choose to continue the game, the next card can either be black or red with probabilities based on number of cards left in the deck.
    // -> If it turns out to be red, we are now at a game where we have X-1 red and Y black and similarly for black. By conditional Expectation, 
    // -> our expected value E($won | X red, Y black ) = P(red drawn)*E($won | X-1 red, Y black) + P(black drawn)*E($won | X red, Y-1 black)
    //
    // if we choose (1):
    // -> Since the deck initially had equal red and black, (D - X) - (D - Y) = (Y - X) = [red cards dealt - black card dealt]*$1 = profit
    //
    // We choose the path that gives us a higher expected profit/value.

    if(argc!=2) {
        cout <<"ERROR: Must have one argument: Total Deck Size" << endl;
        return 0;
    }
    int deckSize = atoi(argv[1]);
    if(deckSize%2==1) {
        cout <<"ERROR: Must be even number of cards" << endl;
        return 0;
    }

    Deck d(deckSize);
    cout << "Computing Value ... " << endl;
    d.computeValue();

    return 0;
}