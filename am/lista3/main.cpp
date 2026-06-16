#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits.h>
#include <chrono>
#include <iomanip> // Required for std::put_time
#include "utils.h"


using namespace std;



int main(int argc, char *argv[]) {

    for(int i = 1; i < argc; i++) {
        /// FILE READING PART
        vector<double> x,y;
        int cities;
    
        ifstream input(argv[i]);
        if(!input) {
            cerr << "Nie udało się otworzyć pliku!" << endl;
            return 1;
        }

        int id;
        double tempX, tempY;

        input >> cities;

        while(input >> id >> tempX >> tempY) {
            x.push_back(tempX);
            y.push_back(tempY);
        }

        cout << "\n" << argv[i] << " " << cities << "\n";

        /// CALCULATIONS PART
        
        int N = x.size();
        vector<int> distMatrix(N * N);

        for(int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                distMatrix[i * N + j] = distanceBetweenPoints(x[i], y[i], x[j], y[j]);
            }
        }

        vector<int> results;

        auto now = std::chrono::system_clock::now();
        time_t timeNow = chrono::system_clock::to_time_t(now);
        cout << argv[i] << " - starting time: " << put_time(localtime(&timeNow), "%H:%M:%S") << "\n";

        for(int j = 0; j < 100; j++) {
            int result = geneticAlgorithm(cities, distMatrix);
            now = chrono::system_clock::now();
            time_t timeNow = chrono::system_clock::to_time_t(now);
            cout << argv[i] << ": " << j << " " << put_time(localtime(&timeNow), "%H:%M:%S") << " result: " << result << "\n";
            results.push_back(result);
        }

        int bestResult = INT_MAX, meanResult, sum = 0;
        for(size_t i = 0; i < results.size(); i++) {
            sum += results.at(i);
            if(results.at(i) < bestResult) {
                bestResult = results.at(i);
            }
        }
        meanResult = sum / (float) results.size();

        now = chrono::system_clock::now();
        timeNow = chrono::system_clock::to_time_t(now);
        cout << put_time(localtime(&timeNow), "%H:%M:%S") << ", best result: " << bestResult << ", mean result: " << meanResult << "\n";
    
    }


    
}