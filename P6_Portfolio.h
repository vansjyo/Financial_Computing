#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "/opt/gurobi952/linux64/include/gurobi_c++.h"

#define EPS 0.0001f

using namespace std;

// class defining attributes of a bond
class Bond {
public:
    double Vb;
    int m;
    vector<int> payments;
    double YTM;
    double duration;
    double convexity;
};

// class for defining bond portfolio
class Bonds {

private:
    int numberOfCashFlows;
    vector<Bond> cashFlows;
    int yearDebtDue;
    double amountDeptDue;
    
    // computes f(r) at r
    double getYieldFunctionValue(Bond b, double r) {
        double value = (b.Vb) * pow(1+r, b.m);
        for(int i=1; i<=b.m; i++) {
            value = value - ( (b.payments[i-1]) * pow(1+r, b.m-i) );  
        }
        return value;
    }

    // computes f'(r) at r
    double getYieldFunctionDerivativeValue(Bond b, double r) {
        double value = (b.m) * (b.Vb) * pow(1+r, b.m-1);
        for(int i=1; i<=(b.m-1); i++) {
            value = value - ( (b.payments[i-1]) * (b.m-i) * pow(1+r, b.m-i-1) );  
        }
        return value;   
    }
    
public:
    // default constructor for the class
    Bonds(string filePath) {
        ifstream inputFile(filePath);
        inputFile >> numberOfCashFlows;
        for(int i=0; i<numberOfCashFlows; i++) {
            Bond b;
            inputFile >> b.Vb >> b.m;
            for(int j=0; j<b.m; j++) {
                double p_j;
                inputFile >> p_j;
                b.payments.push_back(p_j);
            }
            cashFlows.push_back(b);
        }
        inputFile >> amountDeptDue >> yearDebtDue;
    }

    // print details of all cashflows
    void printInputCashFlows() {

        cout << "Amount of dept due: " << amountDeptDue << endl;
        cout << "Number of Years in which dept due: " << yearDebtDue << endl;
        cout << "Number of Cash Flows: " << numberOfCashFlows << "\n \n";

        cout << "************************************************************* \n";
        cout << "YTM, Duration, Convexity for each cash flow: " << endl;
        cout << "************************************************************* \n";

        for(int i=0; i<numberOfCashFlows; i++) {
            cout << "\n ------------------------------- \n";
            cout << "Cash Flow #" << i+1 << endl;
            cout << "Price: " << cashFlows[i].Vb << endl;
            cout << "Maturity: " << cashFlows[i].m << endl;
            cout << "Yield to Maturity (YTM): " << cashFlows[i].YTM << endl;
            cout << "Duration: " << cashFlows[i].duration << endl;
            cout << "Convexity: " << cashFlows[i].convexity << endl;
            cout << "Payments: ";
            for(auto i :cashFlows[i].payments) {cout << i << " ";}
        }
        cout << endl;
    }
    
    // compute YTM of all cash flows using Newton Raphson's method of computing roots
    void computeYTM() {
        for(int i=0; i < numberOfCashFlows; i++) {
            double rn = 1;
            while(abs(getYieldFunctionValue(cashFlows[i], rn)) > EPS) {
                rn = rn - (getYieldFunctionValue(cashFlows[i], rn)/getYieldFunctionDerivativeValue(cashFlows[i], rn));
            }
            cashFlows[i].YTM = rn;
        }
    } 

    // compute duration of all cash flows
    void computeDuration() {
        for(int i=0; i < numberOfCashFlows; i++) {
            double duration = 0;
            for(int j=1; j <= cashFlows[i].m; j++) {
                duration = duration + ( ( j * cashFlows[i].payments[j-1] ) / pow(1+cashFlows[i].YTM, j) );  
            }
            duration = duration/cashFlows[i].Vb;
            cashFlows[i].duration = duration;
        }
    }

    // compute Convexity of all cash flows
    void computeConvexity() {
        for(int i=0; i < numberOfCashFlows; i++) {
            double convexity = 0;
            for(int j=1; j <= cashFlows[i].m; j++) {
                convexity = convexity + ( ( j * (j+1) * cashFlows[i].payments[j-1] ) / pow(1+cashFlows[i].YTM, j+2) );  
            }
            convexity = convexity/cashFlows[i].Vb;
            cashFlows[i].convexity = convexity;
        }
    }

    // compute bond value percentages
    void computeFaceValuePercentage() {
        cout << "\n************************************************************* \n";
        cout << "Percentage of the face value of cash flow to purchase to meet future debt obligation: \n";
        cout << "************************************************************* \n";
        for(int i=0; i < numberOfCashFlows; i++) {
            double bondFaceValue = amountDeptDue/pow(1+cashFlows[i].YTM, yearDebtDue);
            double bondValue = 0;

            // double bondValue = bondFaceValue/pow(1+cashFlows[i].YTM, cashFlows[i].m);
            for(int j=1; j <= min(cashFlows[i].m, yearDebtDue); j++) {
                bondValue = bondValue + ( cashFlows[i].payments[j-1] / pow(1+cashFlows[i].YTM, j) );
            }
            cout << "Bond#" << i << ": " << bondFaceValue*100/bondValue << "% \n";
        }
        cout << endl;
    }

    // compute Portfolio with maximum convexity
    void computeMaxConvexityPortfolio() {
        cout << "\n************************************************************* \n";
        cout << "Bond-portfolio with maximum convexity that meets obli-gation when dept due: \n";
        cout << "************************************************************* \n";
        try {
            // Create an environment
            GRBEnv env = GRBEnv();
            env.set("LogFile", "P6_Portfolio.log");
            env.start();

            // Create an empty model
            GRBModel model = GRBModel(env);

            // create lamda percentage variables 
            GRBVar lambda[numberOfCashFlows];
            for(int i=0; i<numberOfCashFlows; i++) {
                lambda[i] = model.addVar(0.0, 1.0, cashFlows[i].convexity, GRB_CONTINUOUS);            
            }

            // Constraint 1: sum(lamda[i]*duration[i]) = N, duration is equal to year the dept is due
            GRBLinExpr expr1;
            for(int i=0; i<numberOfCashFlows; i++) {
                expr1+=(lambda[i]*cashFlows[i].duration);
            }
            model.addConstr(expr1==yearDebtDue);

            // Constraint 2: sum(lamda[i]) = 1, convexity constraint
            GRBLinExpr expr2;
            for(int i=0; i<numberOfCashFlows; i++) {
                expr2+=(lambda[i]);
            }
            model.addConstr(expr2 == 1);

            // set Objective: max (sum(lambda[i]*convexity[i])), maximize overall convexity
            GRBLinExpr obj;
            for(int i=0; i<numberOfCashFlows; i++) {
                obj+=(lambda[i]*cashFlows[i].convexity);
            }
            model.setObjective(obj, GRB_MAXIMIZE);

            // optimize and print final values
            model.optimize();

            // check if model was feasible or not
            int optimstatus = model.get(GRB_IntAttr_Status);
            if (optimstatus == GRB_INF_OR_UNBD) {
                model.set(GRB_IntParam_Presolve, 0);
                model.optimize();
                optimstatus = model.get(GRB_IntAttr_Status);
            }
            if (optimstatus == GRB_OPTIMAL) {
                cout << "----------------------------------\n";
                cout << "Optimal Portfolio: \n";
                cout << "----------------------------------\n";
                for(int i=0; i<numberOfCashFlows; i++) {
                    if(lambda[i].get(GRB_DoubleAttr_X)>EPS)
                        cout << lambda[i].get(GRB_DoubleAttr_X)*100 <<  "%" << "of Cash Flow#" << i+1 << endl;
                }
                cout << "----------------------------------\n";
                cout << "Maximum Convexity: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
            } else if (optimstatus == GRB_INFEASIBLE) {
                cout << "----------------------------------\n";
                cout << "Infeaible Dept Obligation date. No portfolio combination works\n"; 
                cout << "----------------------------------\n";
            }

        } catch(GRBException e) {
            cout << "Error code = " << e.getErrorCode() << endl;
            cout << e.getMessage() << endl;
        } catch(...) {
            cout << "Exception during optimization" << endl;
        }
    }
};