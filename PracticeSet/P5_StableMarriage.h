#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>

using namespace std;

const char separator    = ' ';
const int nameWidth     = 6;
const int numWidth      = 8;

class GaleShapley {

    // params
    int numberOfCouples;
    vector< vector<int> > mensPreference;
    vector< vector<int> > womensPreference;
    vector<int> mensMatch;
    vector<int> womensMatch;

    // find and return the first unmatched man, if none, return -1
    int findUnmatchedMan() {
        for(int i=0; i< numberOfCouples; i++) {
            if(mensMatch[i]==-1) {
                return i;
            }   
        }
        return -1;
    }

    // return true if a man prefers woman A over woman B
    bool doesManPreferAoverB(int man, int womanA, int womanB) {
        for(int i=0; i<numberOfCouples; i++) {
            if(mensPreference[man][i]==womanA) {
                return true;
            }
            else if(mensPreference[man][i]==womanB) {
                return false;
            }
        }
    }

    // return true if a woman prefers man A over man B
    bool doesWomanPreferAoverB(int woman, int manA, int manB) {
        for(int i=0; i<numberOfCouples; i++) {
            if(womensPreference[woman][i]==manA) {
                return true;
            }
            else if(womensPreference[woman][i]==manB) {
                return false;
            }
        }
    }

    // print the final Matches
    void printMatches() {
        cout << "-----------------" << endl;
        cout << left << setw(nameWidth) << setfill(separator) << "Man";
        cout << left << setw(nameWidth) << setfill(separator) << "-->";
        cout << left << setw(nameWidth) << setfill(separator) << "Woman" << endl;
        cout << "-----------------" << endl;
        for(int i = 0; i < numberOfCouples; i++) {
            cout << left << setw(nameWidth) << setfill(separator) << i;
            cout << left << setw(nameWidth) << setfill(separator) << "-->";
            cout << left << setw(nameWidth) << setfill(separator) << mensMatch[i] << endl;
        }
        cout << "-----------------" << endl;
    }

    // solve the Stable Matching Problem
    void findStableMarriage() {

        // keep track of which woman the man has already proposed, this helps the algorithm to not cycle
        vector< vector<bool> > hasManProposedWoman(numberOfCouples, vector<bool> (numberOfCouples, false));
        
        while(findUnmatchedMan()!=-1) {

            int m = findUnmatchedMan();

            //search for a woman on order of man's preference
            for(auto j : mensPreference[m]) {
                if(hasManProposedWoman[m][j]==false) {
                    
                    // if the woman is not matched to anyone yet, match it to the unmatched man
                    if(womensMatch[j]==-1){
                        mensMatch[m] = j;
                        womensMatch[j] = m;
                        // move on to match the next man
                        break;
                    }
                    else if(womensMatch[j]!=-1) {

                        // if the woman prefers the unmatched man over her match, match them, mark previous match = unmatched
                        if(doesWomanPreferAoverB(j, m, womensMatch[j])==true) {
                            mensMatch[womensMatch[j]] = -1;
                            womensMatch[j] = m;
                            mensMatch[m] = j;
                            // move on to match the next man
                            break;  
                        }
                    }
                    hasManProposedWoman[m][j] = true;
                }
            }

        }

        printMatches();

    }

public:

    // default constructor taking filepath as a parameter from main
    GaleShapley(string filePath) {

        ifstream inputFile(filePath);
        inputFile >> numberOfCouples;

        int value = 0;
        for(int i = 0; i < numberOfCouples; i++) {
            inputFile >> value;
            mensPreference.push_back({value});
            for(int j = 1; j < numberOfCouples; j++) {
                inputFile >> value;
                mensPreference[i].push_back(value);
            }
            mensMatch.push_back(-1);
        }

        for(int i = 0; i < numberOfCouples; i++) {
            inputFile >> value;
            womensPreference.push_back({value});
            for(int j = 1; j < numberOfCouples; j++) {
                inputFile >> value;
                womensPreference[i].push_back(value);
            }
            womensMatch.push_back(-1);
        }
    }

    // main public function
    void getStableMatching() {
        findStableMarriage();
    }
};