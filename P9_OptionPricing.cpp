#include "P9_OptionPricing.h"

using namespace std;

int main(int argc, char * const argv[]) {

    if(argc!=7) {
        cout << "ERROR: Must have 6 arguments in order -\n"
                    "1.  Expiration Time (years): \n"
                    "2.  Number of Stages\n"
                    "3.  Risk-free rate (Continuous compounding) Eg. Input 0.25 for 25% \n"
                    "4.  Volatility (expressed as a fraction of initial price of underlying)\n"
                    "5.  Initial Price of underlying\n"
                    "6.  Strike Price." << endl;
        return 0;
    }
    else if(stod(argv[3])>=1.00 || stod(argv[4])>=1.00) {
        // can add more tests
        cout << "ERROR: Invalid Argument for: 3.  Risk-free rate (Continuous compounding) Eg. Input 0.25 for 25%" << endl;
        return 0;
    }

    OptionPricing optionInstance = OptionPricing(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

    cout << "Computing Europian and American Option Prices...\n";

    optionInstance.priceEuropianOptionUsingTrinomialModel();
    optionInstance.priceEuropianOptionUsingBlackScholes();
    optionInstance.priceAmericanOptionUsingTrinomialModel();

    return 0;
}