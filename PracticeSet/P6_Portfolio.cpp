#include "P6_Portfolio.h"

using namespace std;

int main(int argc, char * const argv[]) {

    if(argc!=2) {
        cout <<"ERROR: Must have one argument: input file path" << endl;
        return 0;
    } 

    /* 
    For percentage of bond/cash flow 1 to buy to meet future dept obligation: I use this logic:

    How much percentage of a bond should I buy such that by the end of Q years, I have an amount equal
    to the amount of dept due:

    i.e. find alpha such that:

    alpha * ( P_1*(1+r)^Q-1 + P_2*(1+r)^Q-2 + ... P_x*(1+r)^Q-x) = amount of Dept Due

    where x = min(bond Maturity year, year when dept is due)

    If I buy alpha percent of a bond, my coupon payments will also be proportional to alpha.

    */

    Bonds cashFlows(argv[1]);
    cashFlows.computeYTM();
    cashFlows.computeDuration();
    cashFlows.computeConvexity();
    cashFlows.printInputCashFlows();
    cashFlows.computeFaceValuePercentage();
    cashFlows.computeMaxConvexityPortfolio();


    return 0;
}