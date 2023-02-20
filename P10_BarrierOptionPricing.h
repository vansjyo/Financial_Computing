#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <random>
#define _USE_MATH_DEFINES
#define EPS 0.0001f

using namespace std;

class OptionPricing {
private:

    // from input
    int numberOfTrials;
    int numberOfSamples;
    double expirationTime;
    double riskFreeInterestRate;
    double volatility;
    double initialStockPrice;
    double strikePrice;
    double barrierPrice;

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


    // Simulation Based Pricing
    double computeEuropeanDownAndOutOptionUsingSimulation(string optionType) {

        // source: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
        random_device rd; 
        mt19937 gen(rd());
        uniform_real_distribution<> uniformDist(0.0, 1.0);

        // compute R, up Factor, uptick, downtick and sametick probabilities
        double deltaT = expirationTime/(double)numberOfSamples;
        double deltaR = (riskFreeInterestRate - 0.5*pow(volatility,2.0))*deltaT;
        double deltaSD = volatility*sqrt(deltaT);

        // for computing average profit across number of trials 
        double averageOptionPriceSum = 0;
        for(int i = 0; i < numberOfTrials; i++) {
            
            double currentStockPrice = initialStockPrice;
            int flag = true;

            for (int j = 0; j < numberOfSamples && flag; j++) {
                
                // create the unit normal variates using the Box-Muller Transform
                double x = uniformDist(gen);
                double y = uniformDist(gen);
                double a = sqrt(-2.0*log(x)) * cos(6.283185307999998*y);

                currentStockPrice = currentStockPrice * exp(deltaR + deltaSD*a);

                // check if the stock price has hit the barrier
                if(currentStockPrice <= barrierPrice) flag = false;
            }

            // add the option price to compute average at the end
            double price = ((optionType=="call" && flag)?
                                        max(0.0, currentStockPrice - strikePrice):
                                        (optionType=="put" && flag)?
                                            max(0.0, strikePrice - currentStockPrice):0);

            // converting to present value
            averageOptionPriceSum += (price / exp(riskFreeInterestRate * expirationTime));
;
            
        }
        return (averageOptionPriceSum/numberOfTrials);
    }

    // Adjustment Term Based Pricing
    double computeEuropeanDownAndOutOptionUsingAdjustmentTerm(string optionType) {

        // source: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
        random_device rd; 
        mt19937 gen(rd());
        uniform_real_distribution<> uniformDist(0.0, 1.0);

        // maintaining running sum for average at the end
        double averageOptionPriceSum = 0;
        for(int i = 0; i < numberOfTrials; i++) {

            // using direct formula to copmute final price without time steps or division
            double T = expirationTime;
            double R = (riskFreeInterestRate - 0.5*pow(volatility,2.0))*T;
            double SD = volatility*sqrt(T);

            double x = uniformDist(gen);
            double y = uniformDist(gen);
            double a = sqrt(-2.0*log(x)) * cos(6.283185307999998*y);
            double directFinalPriceUsingSimulation = initialStockPrice * exp(R + SD*a); // from lesson 9

            // if final price is less than barrier, add 0 (or nothing) to the average Sum since payoff is 0
            if(directFinalPriceUsingSimulation <= barrierPrice || initialStockPrice <= barrierPrice)
                continue;

            // from lesson 7
            double probabilityOfDownAndOut = exp( (-2.0/(volatility*volatility*T)) * log(initialStockPrice/barrierPrice) * log(directFinalPriceUsingSimulation/barrierPrice) );
            double price = ((optionType=="call")?
                                max(0.0, directFinalPriceUsingSimulation - strikePrice):
                                (optionType=="put")?
                                    max(0.0, strikePrice - directFinalPriceUsingSimulation):0);

            // convert price to present value
            averageOptionPriceSum += ( (1.0-probabilityOfDownAndOut) * (price / exp(riskFreeInterestRate * expirationTime)) );
        }

        return (averageOptionPriceSum/numberOfTrials);
    }

    // Black Scholes Formula
    double computeEuropeanOptionPriceUsingBlackScholes(double myInitialStockPrice, string optionType) {
        // formulas copy pasted from Prof RS's code samples
        // note: Closed form computes Black Scholes for different Initial Prices and that is the only custom Input
        double time_sqrt = sqrt(expirationTime);
        double d1 = (log(myInitialStockPrice/strikePrice)+riskFreeInterestRate*expirationTime)/(volatility*time_sqrt) + 0.5*volatility*time_sqrt;
        double d2 = d1-(volatility*time_sqrt);
        double price = ((optionType=="call")?
                        ( (myInitialStockPrice * N(d1)) - (strikePrice * exp(-riskFreeInterestRate * expirationTime) * N(d2)) ):
                        (optionType=="put")?
                            ( (strikePrice * exp(-riskFreeInterestRate * expirationTime) * N(-d2)) - (myInitialStockPrice * N(-d1)) ):0);
        return price;
    }

    // Closed Form Based Pricing
    double computeEuropeanDownAndOutOptionUsingClosedForm(string optionType) {
        // formulas copy pasted from Prof RS's code samples
        // He took this formula from Wilmott, Howison and Dewynne, "The Mathematics of Financial Derivatives"
        if(optionType=="call") {
            double K = (2*riskFreeInterestRate)/(volatility*volatility);
            double A = computeEuropeanOptionPriceUsingBlackScholes(initialStockPrice, optionType);
            double B = (barrierPrice*barrierPrice)/initialStockPrice;
            double C = pow(initialStockPrice/barrierPrice, -(strikePrice-1));
            double D = computeEuropeanOptionPriceUsingBlackScholes(B, optionType);
            return (A - D*C);
        }
        
        // for put option
        double vanilla_put = computeEuropeanOptionPriceUsingBlackScholes(initialStockPrice, optionType);
        double put_down_in = computeDownAndInEuropeanPutOptionUsingClosedForm();
        return (vanilla_put - put_down_in);
    }


    double computeDownAndInEuropeanPutOptionUsingClosedForm()	{
        double S = initialStockPrice;
        double r = riskFreeInterestRate;
        double T = expirationTime;
        double sigma = volatility;
        double H = barrierPrice;
        double X = strikePrice;
        
        // Took these formulae from some online reference
        double lambda = (r+((sigma*sigma)/2))/(sigma*sigma);
        double temp = 2*lambda - 2.0;
        double x1 = (log(S/H)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
        double y = (log(H*H/(S*X))/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
        double y1 = (log(H/S)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
        return (-S*N(-x1) + X*exp(-r*T)*N(-x1 + sigma*sqrt(T)) + 
                S*pow(H/S, 2*lambda)*(N(y)-N(y1)) -
                X*exp(-r*T)*pow(H/S, temp)*(N(y-sigma*sqrt(T))-N(y1-sigma*sqrt(T))));
    }

public:

    // constructor
    OptionPricing(string t, string r, string v, string i, string k, string n, string m, string b) {
        cout << "Creating Instance... " << endl;
        try {
            expirationTime = stod(t);
            riskFreeInterestRate = stod(r);
            volatility = stod(v);
            initialStockPrice = stod(i);
            strikePrice =  stod(k);
            numberOfTrials = stoi(n);
            numberOfSamples = stoi(m);
            barrierPrice = stod(b);
            cout << "Instance created.\n";
        }
        catch(const std::exception& e) {
            throw out_of_range("ERROR: Invalid conversion in stoi/stod. Enter valid values for arguments");
        }

        cout << "\n Expiration Time (years): " << expirationTime <<
                "\n Risk-free rate (Continuous compounding): " << riskFreeInterestRate <<
                "\n Volatility: " << volatility <<
                "\n Initial Price of underlying: " << initialStockPrice <<
                "\n Strike Price: " << strikePrice << 
                "\n Number of trials: " << numberOfTrials <<
                "\n Number of sample-points in each sample price-path: " << numberOfSamples <<
                "\n Barrier Price B: " << barrierPrice <<
                "\n \n"; 
    }

    void priceDownAndOutEuropianOptionUsingSimulation() {
        cout << "Price of Europian Call option using simulation is: " << computeEuropeanDownAndOutOptionUsingSimulation("call") << "\n";
        cout << "Price of Europian Put option using simulation is:  " << computeEuropeanDownAndOutOptionUsingSimulation("put") << "\n\n";
    }

    void priceDownAndOutEuropianOptionUsingAdjustmentTerm() {
        cout << "Price of Europian Call option using (1-p) Adjustment term is: " << computeEuropeanDownAndOutOptionUsingAdjustmentTerm("call") << "\n";
        cout << "Price of Europian Put option using (1-p) Adjustment term is:  " << computeEuropeanDownAndOutOptionUsingAdjustmentTerm("put") << "\n\n";
    }

    void priceDownAndOutEuropianOptionUsingClosedForm() {
        cout << "Price of Europian Call option using Theoretical closed form is: " << computeEuropeanDownAndOutOptionUsingClosedForm("call") << "\n";
        cout << "Price of Europian Put option using Theoretical closed form is:  " << computeEuropeanDownAndOutOptionUsingClosedForm("put") << "\n\n";
    }
};