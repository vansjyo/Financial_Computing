#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random>
#include <math.h>
#define _USE_MATH_DEFINES
#define EPS 0.0001f

using namespace std;

class OptionPricing {
private:

    // from input
    double expirationTime;
    int numberOfStages;
    double riskFreeInterestRate;
    double volatility;
    double initialPrice;
    double strikePrice;

    // derived
    double R;
    double upFactor;
    double upTickProb; 
    double downTickProb;
    double sameTickProb;
    unordered_map<string, double> mem;

    double N(const double& z) { 
        // copy pasted from Prof RS's code samples for Lesson 6
        if(z > 6.0) { return 1.0; }; // this guards against overflow 
        if(z < -6.0) { return 0.0; }; 
        double b1 = 0.31938153, b2 = -0.356563782, b3 = 1.781477937, b4 = -1.821255978, b5 = 1.330274429; 
        double p = 0.2316419, c2 = 0.3989423; 
        double a = fabs(z), t = 1.0/(1.0+a*p), b = c2*exp((-z)*(z/2.0)); 
        double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t; 
        n = 1.0-b*n; 
        if( z < 0.0 ) n = 1.0 - n; 
        return n; 
    }

    double computeEuropianOption(int k, int i, string optionType) {
        // option Type = "put" / "call"

        // clear map when function call first time 
        if(k==0) mem.clear();
        // generate unique ID for combination of k and i
        string id = to_string(k) + "-" + to_string(i);
        double val = 0;

        // memoization
        if(mem.find(id)!=mem.end()) return mem[id];
        if(k==numberOfStages) {

            val =   (optionType=="put")?
                        max( 0.0, -1*initialPrice*pow(upFactor, (double)i) + strikePrice):
                        (optionType=="call")?
                            max( 0.0, initialPrice*pow(upFactor, (double)i) - strikePrice):0;

            mem.insert({id, val});
            return val;
        }

        val = ( (upTickProb * computeEuropianOption(k+1, i+1, optionType)) + 
                (downTickProb * computeEuropianOption(k+1, i-1, optionType)) + 
                (sameTickProb * computeEuropianOption(k+1, i, optionType)) )/R;
        mem.insert({id, val});

        return val;
    }


    double computeAmericanOption(int k, int i, string optionType) {
        // option Type = "put" / "call"

        // clear map when function call first time 
        if(k==0) mem.clear();
        // generate unique ID for combination of k and i
        string id = to_string(k) + "-" + to_string(i);
        double val = 0;

        double currentProfit =  (optionType=="put")?
                                    max( 0.0, strikePrice - initialPrice*pow(upFactor, (double)i) ):
                                    (optionType=="call")?
                                        max( 0.0, initialPrice*pow(upFactor, (double)i) - strikePrice):0;

        // memoization
        if(mem.find(id)!=mem.end()) return mem[id];
        if(k==numberOfStages) {
            mem.insert({id, currentProfit});
            return val;
        }

        val = max(  currentProfit,
                    ((upTickProb * computeAmericanOption(k+1, i+1, optionType)) + 
                    (downTickProb * computeAmericanOption(k+1, i-1, optionType)) + 
                    (sameTickProb * computeAmericanOption(k+1, i, optionType)))/R);
        mem.insert({id, val});

        return val;
    }

    double computeEuropianOptionUsingBlackScholes(string optionType){
        // optionType = "put" / "call"
        // copy pasted from Prof RS's code samples from Lesson 6
        double sqrtTime = sqrt(expirationTime);
        double d1 = (log(initialPrice/strikePrice)+riskFreeInterestRate*expirationTime)/(volatility*sqrtTime) + 0.5*volatility*sqrtTime;
        double d2 = d1-(volatility*sqrtTime);
        double optionPrice = (optionType=="put")?
                                    (strikePrice*exp(-riskFreeInterestRate*expirationTime)*N(-d2) - initialPrice*N(-d1)):
                                    (optionType=="call")?
                                        (initialPrice*N(d1) - strikePrice*exp(-riskFreeInterestRate*expirationTime)*N(d2)):0;
        return optionPrice;
    }

public:

    // constructor
    OptionPricing(string e, string n, string r, string v, string i, string k) {
        cout << "Creating Instance... " << endl;
        try {
            expirationTime = stod(e);
            numberOfStages = stoi(n);
            riskFreeInterestRate = stod(r);
            volatility = stod(v);
            initialPrice = stod(i);
            strikePrice =  stod(k);
            cout << "Instance created.\n";
        }
        catch(const std::exception& e) {
            cout << "ERROR: Invalid conversion in stoi/stod. Enter valid values for arguments" << endl;
        }

        // compute R, up Factor, uptick, downtick and sametick probabilities
        double interval = expirationTime/numberOfStages;
        R = exp( riskFreeInterestRate * interval );
        upFactor = exp( volatility * sqrt(2*interval));
        double sqrtR = sqrt(R);
        double sqrtUpFactor = sqrt(upFactor);
        upTickProb = pow( (sqrtR - 1.0/sqrtUpFactor)/(sqrtUpFactor - 1.0/sqrtUpFactor), 2); 
        downTickProb = pow( (sqrtUpFactor - sqrtR)/(sqrtUpFactor - 1.0/sqrtUpFactor), 2);
        sameTickProb = (1 - upTickProb - downTickProb);

        cout << "\n Expiration Time (years): " << expirationTime <<
                "\n Number of Stages: " << numberOfStages <<
                "\n Risk-free rate (Continuous compounding): " << riskFreeInterestRate <<
                "\n Volatility: " << volatility <<
                "\n Initial Price of underlying: " << initialPrice <<
                "\n Strike Price: " << strikePrice << 
                "\n Up Factor: " << upFactor <<
                "\n P(UpTick): " << upTickProb <<
                "\n P(DownTick: " << downTickProb <<
                "\n P(SameTick): " << sameTickProb <<
                "\n \n"; 
    }

    void priceEuropianOptionUsingTrinomialModel() {
        cout << "Price of Europian Call option is: " << computeEuropianOption(0, 0, "call") << "\n";
        cout << "Price of Europian Put option is:  " << computeEuropianOption(0, 0, "put") << "\n\n";
    }

    void priceEuropianOptionUsingBlackScholes() {
        cout << "Price of Europian Call option using Black Scholes is: " << computeEuropianOptionUsingBlackScholes("call") << "\n";
        cout << "Price of Europian Put option using Black Scholes is:  " << computeEuropianOptionUsingBlackScholes("put") << "\n\n";
    }

    void priceAmericanOptionUsingTrinomialModel() {
        cout << "Price of American Call option is: " << computeAmericanOption(0, 0, "call") << "\n";
        cout << "Price of American Put option is:  " << computeAmericanOption(0, 0, "put") << "\n\n";
    }

};