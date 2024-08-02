#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include "helperfunctions.h"
//#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> getPowersOfTmodP(int p, int highestPower) {
    vector<vector<int>> powers(p, vector<int>(highestPower, 0));
    for (int t = 0; t < p; t++) {
        for (int power = 0; power < highestPower; power++) {
            powers[t][power] = power_mod_p(t, power, p);
        }
    }
    return powers;
}

int calculatePoly(int t, int p, const vector<vector<int>>& powersOfT, const vector<int>& coeffs) {
    int result = 0;
    int count = 0;
    for (int coeff: coeffs) {
        result += coeff * powersOfT[t][count];
        result = result % p;
        count++;
    }
    return result;
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file_name> <upper_limit> <filter_on> <a> <mod b>" << endl;
        return 1;
    }

    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB

    int highestPower = 120;


    std::vector<int> primes = generate_primes_in_range(1, n);

    vector<int> x; /*primes*/
    vector<double> y; /*A-values*/
    vector<double> z; /*A-values*/

    float runningsum = 0;
    float weightedrunningsum = 0;
    int primecount = 0;
    float weightedprimecount = 0;
    int positivecount = 0;
    int weightedpositivecount = 0;

    for (int p : primes) {

        vector<vector<int>> allPowersOfT = getPowersOfTmodP(p, highestPower);

        if (filterOn) {
            if (p % filterB != filterA || p <= 5) {
                continue;
            }
        }

        long A = 0;
        long B = 0;
        int c = 0;
        int value = 0;
        int readThing = 0;
        int repline = 0;

        /* /
        if (p % 20 == 1) {
            cout << "on prime " << p << std::endl;
        }
        /**/
        //cout << "on prime " << p << std::endl;

        string filename = "classdata/file_" + to_string(p) + ".csv";

        //ifstream file;
        //file.open(filename);

        int MAX_ROWS = 5*p;
        ifstream file(filename);
        if(!file.is_open()){
            cerr << "Error opening file!" << endl;
            return 1;
        }
    
        int data[MAX_ROWS];
        string line;
        int row = 0;
        while(getline(file,line) && row < MAX_ROWS+1){
            if (row == 0){
                row++;
                continue;
            }
            stringstream ss(line);
            string cell;
            for (int i = 0; getline(ss, cell, ','); ++i) {
                if (i == 2) {
                    data[row-1] = stoi(cell);
                    //cout << data[row-1] << "adasd" << row << "\n";
                }
            }
            row++;
            
        }
        file.close();

        /*cout << "at " << p << "\n";
        for (int i = 0; i < 3*p; i++){
            cout << data[i] << "kewdwedw" << "\n";
        }*/

        vector<int> reps = findQuarticResidueClasses(p);

        for (long t = 0; t < p; ++t) {
            A = 1;
            B = power_mod_p(t, 3, p);
            //(power_mod_p(t, 3, p)+4*power_mod_p(t, 2, p)-power_mod_p(t, 1, p) /* + 7*power_mod_p(t, 2, p)*/) % p;

            // Compute Pythagorean family
            /*int poly = 2 * t * inverse(power_mod_p(t,2,p)-1,p);
            int b = - power_mod_p(poly,2,p) + 1;
            int c = power_mod_p(poly,4,p);
            A = ((c - inverse(3,p) * power_mod_p(b, 2, p)) % p + p)%p;
            B = ((2 * inverse(27,p) * power_mod_p(b,3,p) - c * b * inverse(3,p))%p + p)%p;*/
            
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

            //step 1: figure out what residue class A is in
            for (int i = 0; i <= reps.size(); i++) {
                if (reps[i] == 0) {
                    if (A == 0) {
                        value += pow(data[B], 2);
                        break;
                    }
                }

                int Aovertest = (A * inverse(reps[i], p)) % p;

                if (isFourthPower(Aovertest, p)) {
                    int lfourth = Aovertest % p;

                    //step 2: calculate lsixth
                    int lsquare = squareroot(lfourth, p);
                    int lsixth = (lsquare * lfourth) % p;

                    //step 3: calculate B and lookup
                    B = (B * (inverse(lsixth, p)+p)) % p;

                    value += pow(data[p * i + B], 2);
                    break;
                }
            }
        }
        //cout << " p = " << p << ", sum = " << value << "\n";

        x.push_back(p);
        float normalizedSecondMoment = (value - pow(p, 2)) / pow(p, 1.5); //subtract by p^2 and divide by p^(1.5) to normalize
        runningsum += normalizedSecondMoment;
        weightedrunningsum += normalizedSecondMoment * log(p);
        primecount += 1;
        weightedprimecount += log(p);
        if (runningsum >= 0) {
            positivecount ++;
        }
        if (weightedrunningsum >= 0) {
            weightedpositivecount ++;
        }
        y.push_back(runningsum/primecount); 
        z.push_back(weightedrunningsum/weightedprimecount); 

        if (primecount % 10 == 0) {
            cout << "Just finished p=" << p << " which is the " << primecount << "th prime\n";
        }

        file.close();
    }
    cout << "Running average is positive " << 1.0 * positivecount / primecount  << " and weighted running average is positive " << 1.0 * weightedpositivecount/primecount << "\n";

    std::ofstream output_file("data.txt");
    if (!output_file.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << "," << z[i] << std::endl;
    }

    return 0;
}