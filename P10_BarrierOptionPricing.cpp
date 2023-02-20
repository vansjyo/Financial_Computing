#include "P10_BarrierOptionPricing.h"

using namespace std;

int main(int argc, char * const argv[]) {

    // Some code snippets in the .h file are directly copied from codes shared by Prof RS.
    // Only did part 1 of the assignment in this

    if(argc!=9) {
        cout << "ERROR: Must have 8 arguments in order -\n"
                    "1.  Expiration Time (years): \n"
                    "2.  Risk-free rate (Continuous compounding) Eg. Input 0.25 for 25%\n"
                    "3.  Volatility (expressed as a fraction of initial price of underlying) \n"
                    "4.  Initial Price of underlying\n"
                    "5.  Strike Price\n"
                    "6.  Number of trials/repetitions of Monte Carlo simulations (n) \n"
                    "7.  Number of sample-points in each sample price-path (m)\n"
                    "8.  Barrier Price B (B < S0)" << endl;
        return 0;
    }
    else if(stod(argv[2])>=1.00 || stod(argv[3])>=1.00) {
        // can add more tests
        cout << "ERROR: Invalid Argument for: 3.  Risk-free rate (Continuous compounding) Eg. Input 0.25 for 25%" << endl;
        return 0;
    }

    OptionPricing optionInstance = OptionPricing(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);

    cout << "Computing Europian and American Option Prices...\n";

    optionInstance.priceDownAndOutEuropianOptionUsingSimulation();
    optionInstance.priceDownAndOutEuropianOptionUsingAdjustmentTerm();
    optionInstance.priceDownAndOutEuropianOptionUsingClosedForm();

    return 0;
}