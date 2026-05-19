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


        /// ------------------- symulowane wyżarzanie --------------------------------------------------------------------------------------------------------------------------------------------
        // vector<int> permutation;

        // permutation = generatePermutation(cities);
        // float temperature = calculateInitTemp(permutation, cities, distMatrix);
        // float minTemperature = 0.01;
        // float coefficient = 0.9;
        // int numberOfTries = cities * cities / 16;

        // vector<int> results;
        // auto now = std::chrono::system_clock::now();
        // std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
        // for(int j = 0; j < 100; j++) {


        //     cout << argv[i] << ": " << j << " " << put_time(localtime(&timeNow), "%H:%M:%S") << "\n";
        //     random_device rd;
        //     static thread_local mt19937 gen(std::random_device{}());
        //     uniform_int_distribution<int> dist(0, cities - 1);
        //     permutation = generatePermutation(cities);
        //     int result = simulatedAnnealing(numberOfTries, temperature, coefficient, cities, minTemperature, permutation, distMatrix);
        //     results.push_back(result);
        // }

        // int bestResult = INT_MAX, meanResult, sum = 0;
        // for(size_t i = 0; i < results.size(); i++) {
        //     sum += results.at(i);
        //     if(results.at(i) < bestResult) {
        //         bestResult = results.at(i);
        //     }
        // }
        // meanResult = sum / (float) results.size();
        // cout << put_time(localtime(&timeNow), "%H:%M:%S") << ", best result: " << bestResult << ", mean result: " << meanResult << "\n";

        /// ------------------- Symulowane wyżarzanie --------------------------------------------------------------------------------------------------------------------------------------------

        /// ------------------- Tabu search ------------------------------------------------------------------------------------------------------------------------------------------------------
        vector<int> mst = findMST(cities, distMatrix);
        vector<vector<int>> listOfEdges = convertMSTToEdges(mst, cities);
        vector<vector<int>> adjencyList = convertEdgesToAdjencyList(listOfEdges, cities);
        vector<int> permutation;
        vector<bool> visited(cities, 0);
        constructHamiltionCycleThroughDFS(0, adjencyList, visited, permutation, 0); // to modyfikuje perumutation -> dostajemy 2 aproks.

        vector<int> results;
        int tabuTenure = cities/4;

        // Setup random number generator
        std::random_device rd;
        std::mt19937 rng(rd());

        auto now = std::chrono::system_clock::now();
        for(int j = 0; j < 100; j++) {

            if (j > 0) {
                int totalKicks = 3; // Number of random swaps to shake up the tour
                for (int k = 0; k < totalKicks; ++k) {
                    int idx1 = 1 + rng() % (cities - 1);
                    int idx2 = 1 + rng() % (cities - 1);
                    swap(permutation[idx1], permutation[idx2]);
                }
            }

            int result = tabuSearch(permutation, cities, tabuTenure, distMatrix);
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
        time_t timeNow = chrono::system_clock::to_time_t(now);
        cout << put_time(localtime(&timeNow), "%H:%M:%S") << ", best result: " << bestResult << ", mean result: " << meanResult << "\n";
        
        /// ------------------- Tabu search ------------------------------------------------------------------------------------------------------------------------------------------------------
    }


    
}