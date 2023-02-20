#include "P8_RVGeneration.h"

using namespace std;

int main(int argc, char * const argv[]) {

    

    if(argc!=2) {
        cout <<"ERROR: Must have one arguments: Number of Trials for comuting mean of Cauchy Random Variate." << endl;
        return 0;
    } 

    int numberOfCauchyTrials = stoi(argv[1]);

    if(numberOfCauchyTrials<=0) {
        cout << "Please enter a positive integer";
        return 0;
    }

    RVGenerator r = RVGenerator(numberOfCauchyTrials);

    // commenting this code since I used library "Sciplot" to plot the graph
    cout << "\n************************************************************\n";
    cout << "NOTE: Code for plotting the graph has been commented to avoid compilation errors.\n\tGraph plotted using the Sciplot package has been added to submissions" << endl;
    cout << "\n************************************************************\n";
    // r.drawVerificationCurve();

    // compute mean
    cout << "\nComputing the eperimentally observed arithmetic-mean...\n";
    double cauchyMean = r.getExperimentalMean(numberOfCauchyTrials);
    cout << "Mean for #" << numberOfCauchyTrials << " trials is " << cauchyMean << endl;

    // Generate table
    cout << "\nGenerating table... \n";
    cout << "#Trails \t Mean\n";
    cout << "--------------------------\n";
    for(int trials = 10000; trials < 100000; trials+=5000) {
        cout << trials << "\t\t" << r.getExperimentalMean(trials) << endl;
    }

    cout << "\nReason for inconsistent mean: The expected value of a Cauchy distribution does not exist, hence it does not obey the law of large numbers.\n";

    return 0;
}