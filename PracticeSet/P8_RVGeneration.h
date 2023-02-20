#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
// #include <sciplot/sciplot.hpp>
#define _USE_MATH_DEFINES

#define EPS 0.0001f

using namespace std;

class RVGenerator {

private:

    double q;
    double p;
    int numberOfTrials = 0;
    int nSamples = 100;
    int resSize = 0.1;
    vector< vector<int> > mem;

    void generateUniformRV() {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator (seed);

        uniform_real_distribution<double> distribution (0.0,100.0);

        cout << "some random numbers between 0.0 and 100.0: " << endl;
        for (int i=0; i<10; ++i) {
            cout << distribution(generator) << endl;
        }
    }

    // find out the number of  optimal tosses
    vector<double> getTheoreticalCauchyValue() {
        cout << "Computing Theoretical Cauchy values...\n";
        vector<double> p;
        for(double i = -5.0; i <= 5.0; i+=0.1) {
            // source: https://www.quantstart.com/articles/Mathematical-Constants-in-C/
            double val = pow( ( M_PI) * (1.0 + i*i), -1);
            p.push_back(val);
        }
        return p;
    }

    vector<double> getExperimentalCauchy() {
        // source: https://cplusplus.com/reference/random/cauchy_distribution/
        int n = 1000000;
        default_random_engine generator;
        cauchy_distribution<double> distribution(0.0,1.0);
        vector<double> p(nSamples+1,0.0);

        cout << "Computing Experimental Cauchy values...\n";

        for (int i=0; i<n; i++) {
            double number = distribution(generator);
            
            if ((number>=-5.0)&&(number<5.0)) {
                number*=10;
                number = round(number);
                number/=10;
                number = (number+5)*10;
                p[int(round(number))] = (p[int(round(number))] + 1.0);
            }
        }
        for (int i=0; i<p.size(); ++i) {
            p[i] = p[i]*10/(n);
        }

        return p;
    }

public:

    // constructor
    RVGenerator(int numberOfCauchyTrials) {
        numberOfTrials = numberOfCauchyTrials;
    }

    // void drawVerificationCurve() {
    //     // source: https://sciplot.github.io/
    //     // run using `g++ -std=c++17  P8_RVGeneration.cpp`

    //     vector<double> yTheory = getTheoreticalCauchyValue();
    //     vector<double> yExperiment = getExperimentalCauchy();
    //     cout << "Printing the Graph...\n";

    //     // plot the graph using these 2 y values
    //     sciplot::Vec x = sciplot::linspace(-5.0, 5.0, nSamples);
    //     sciplot::Plot2D plot;
    //     plot.drawCurve(x, yTheory).label("Theory").lineWidth(0);
    //     plot.drawCurve(x, yExperiment).label("Experiment").lineWidth(0);
    //     plot.legend().atOutsideBottom().displayHorizontal().displayExpandWidthBy(2);
    //     sciplot::Figure figure = {{plot}};
    //     sciplot::Canvas canvas = {{figure}};
    //     canvas.show();
    //     canvas.save("P8_TheoryVSExperimentGraph.pdf");
            
    // }

    double getExperimentalMean(int nTrials) {
        default_random_engine generator;
        cauchy_distribution<double> distribution(0.0,1.0);

        double sum = 0.0;
        for(int i = 0; i<nTrials; i++) {
            sum = sum + distribution(generator);
        }
        return sum/numberOfTrials;
    }
};