#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random>
#include <sciplot/sciplot.hpp>

#define EPS 0.0001f

using namespace std;

class CoinTossInstance {

private:

    double q;
    double p;
    int upperBound;
    int lowerBound;
    vector< vector<int> > mem;
    
    // Compute lower bound of N(p,q) using equation in the paper
    int getLowerBound(double q, double p) {
        double val = 1/(2*(p-q)) - 1.0/2;
        return floor(val);
    }

    // Compute Upper bound of N(p,q) using equation in the paper
    int getUpperBound(double q, double p) {
        double val = fmax(1-p,q)/(p-q);
        return ceil(val);
    }

    // Compute nCr for given n and r
    int nCr(int n, int r) {
        if(n<r) return 0;
        if(mem[n][r]!=-1) return mem[n][r];

        if(r==0) {
            mem[n][r] = 1;
            return mem[n][r];
        }

        mem[n][r] = nCr(n-1, r) + nCr(n-1, r-1);
        return mem[n][r];
    }

    // get probability of Alice winning for n tosses
    double getFunctionValue(int n, double q, double p) {
        double functionVal = 0;
        for(int r = 0; r <= n; r++) {
            double outerFactor = nCr(n, r) * pow(p,r) * pow(1-p, n-r);
            double innerSum = 0;
            for(int s = r+1; s <= n; s++) {
                double val = nCr(n, s) * pow(q,s) * pow(1-q, n-s);
                innerSum = innerSum + val;
            }
            functionVal = functionVal + (outerFactor * innerSum);
        }
        return functionVal;
    }

public:

    // constructor
    CoinTossInstance(double AliceH, double BobH) {
        q = AliceH;
        p = BobH;

        upperBound = getUpperBound(q, p);
        lowerBound = getLowerBound(q, p);

        // initialze the memoization vector
        for(int i=0;i<=upperBound; i++) {
            mem.push_back({});
            for(int j=0;j<=upperBound; j++) {
                mem[i].push_back(-1);
            }
        }
    }
 
    // find out the number of  optimal tosses
    int getOptimalNumberOfTosses() {

        // do binary search between upper and lower bound to get value of optimal n
        int left = lowerBound;
        int right = upperBound;
        int mid = (left+right)/2;

        while(left <= right) {
            mid = (left+right)/2;
            double midVal = getFunctionValue(mid, q, p);
            double midNegativeVal = getFunctionValue(mid-1, q, p);
            double midPositiveVal = getFunctionValue(mid+1, q, p); 

            if( (midVal > midNegativeVal && midVal > midPositiveVal) || (midVal >= midNegativeVal && midVal > midPositiveVal) || (midVal > midNegativeVal && midVal >= midPositiveVal)) {
                return mid;
            }
            else if(midVal > midNegativeVal){
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
    }

    void drawVerificationCurve(int optimalTosses) {
        // source: https://sciplot.github.io/

        int numberOfTrials = 1000000;
        vector<double> yTheory;
        vector<double> yExperiment;
        random_device rd;
        mt19937 e2(rd());
        uniform_real_distribution<> dist(0, 1);

        // compute the theoretical value of Alice's win probability
        for(int i = 1; i< optimalTosses+4; i++) {
            yTheory.push_back(getFunctionValue(i, q, p));
        }

        // for varying values of n, we run 10^6 trials
        for(int i = 1; i< optimalTosses+4; i++) {

            int numberOfAliceWins = 0;
            for(int j = 1; j< numberOfTrials; j++) {
    
                int headCountAlice = 0;
                int headCountBob = 0;
                // pick a number between 0 and 1 from a uniform distribution n times, if <q, then the player got a head
                for (int nA = 0; nA < i; nA++) {
                    if(dist(e2)<=q) headCountAlice++;
                }
                for (int nB = 0; nB < i; nB++) {
                    if(dist(e2)<=p) headCountBob++;
                }
                // If Alice gets more heads, she wins
                if(headCountAlice>headCountBob) numberOfAliceWins++;
            }
            // compute probability using frequency of Alice's wins for the given value of n
            double probAliceWins = (double)numberOfAliceWins/numberOfTrials;
            yExperiment.push_back(probAliceWins);
        }

        // plot the graph using these 2 y values
        sciplot::Vec x = sciplot::linspace(1, optimalTosses+4, optimalTosses+5);
        sciplot::Plot2D plot;
        plot.drawCurve(x, yTheory).label("Theory").lineWidth(0);
        plot.drawCurve(x, yExperiment).label("Experiment").lineWidth(0);
        plot.legend().atOutsideBottom().displayHorizontal().displayExpandWidthBy(2);
        sciplot::Figure figure = {{plot}};
        sciplot::Canvas canvas = {{figure}};
        canvas.show();
        canvas.save("TheoryVSExperimentGraph.pdf");
            
    }
};