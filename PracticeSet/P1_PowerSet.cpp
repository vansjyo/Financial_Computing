#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>

using namespace std;

// Took some guidance from: https://www.geeksforgeeks.org/stdnext_permutation-prev_permutation-c/ and https://cplusplus.com/reference/algorithm/next_permutation/

int main() {

    // get input size of array
    int arr_size;
    int n_pick;
    cout << "Enter array size:" << endl;
    cin >> arr_size;

    // check input
    if(arr_size < 0) {
        cout << "Invalid Input.. Exiting";
        return 0;
    }

    // define the array
    vector<int> arr(arr_size);
    for(int i = 0; i < arr_size; i++)
        arr[i] = i+1;             // keep the order same

    cout << "Power Set of array: " << endl;
    for(int n_pick = 0; n_pick <= arr_size; n_pick++) {

        // create boolean vector  
        vector<bool> arr_bool(arr_size, true);
        for(int i = 0; i < n_pick; i++)
            arr_bool[i] = false;      // instead of defining 1's (true) in the boolean array based on picked elements, I define 0's (false) 

        // printing all possible permutations in specific order
        do {
            cout << "{ ";
            for(int i = 0; i < arr_size; i++) {
                // now I print where the 0's are instead of 1's
                if(!arr_bool[i]) {
                    cout << arr[i] << " ";
                }
            }
            cout << "}" << endl;
        } while ( next_permutation(arr_bool.begin(), arr_bool.end()) );

    }
    

    return 0;
}

