#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits.h>
#include "utils.h"


using namespace std;

vector<int> quickLocalSearch(vector<int>& tour, int N, const vector<int>& distMatrix) {
    auto getDist = [&](int i, int j) { return distMatrix[i * N + j]; };
    
    bool betterSolution = true;
    int counter = 0;

    // Generator losowy
    random_device rd;
    static thread_local mt19937 gen(std::random_device{}());
    uniform_int_distribution<int> dist(1, N - 1);

    while (betterSolution) {
        betterSolution = false;
        int bestI = -1, bestJ = -1;
        int minDelta = 0;

        // Szukamy najlepszego spośród N wylosowanych sąsiadów
        for (int k = 0; k < N; ++k) {
            int i = dist(gen);
            int j = dist(gen);

            if (i == j) continue;
            if (i > j) std::swap(i, j);
            if (j == N - 1 && i == 1) continue; // Unikamy pełnego obrotu -> to tylko zmiana kierunku, nie dystansu

            int a = tour[i - 1];
            int b = tour[i];
            int c = tour[j];
            int d = (j + 1 < N) ? tour[j + 1] : tour[0];

            int delta = (getDist(a, c) + getDist(b, d)) - (getDist(a, b) + getDist(c, d));

            if (delta < minDelta) {
                minDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }

        // Jeśli wśród N wylosowanych znaleźliśmy jakąkolwiek poprawę
        if (minDelta < 0) {
            std::reverse(tour.begin() + bestI, tour.begin() + bestJ + 1);
            betterSolution = true;
            counter++;
        }
    }

    vector<int> result;
    int finalDistance = calculatePathDistance(tour, N, distMatrix);
    result.push_back(finalDistance);
    result.push_back(counter);
    return result;
    
}


vector<int> localSearch(std::vector<int>& tour, int N, const std::vector<int>& distMatrix) {
    auto getDist = [&](int i, int j) { return distMatrix[i * N + j]; };
    
    bool betterSolution = true;
    int currentDistance = calculatePathDistance(tour, N, distMatrix);
    int counter = 0;

    while (betterSolution) {
        counter++;
        betterSolution = false;
        int bestI = -1, bestJ = -1;
        double minDelta = 0;

        for (int i = 1; i < N - 1; ++i) {
            for (int j = i + 1; j < N; ++j) {
                // Obliczamy DELTĘ zamiast całego dystansu od zera!
                // 2-opt zamienia krawędzie (i-1, i) oraz (j, j+1*) na (i-1, j) oraz (i, j+1*)
                int a = tour[i - 1];
                int b = tour[i];
                int c = tour[j];
                int d = (j + 1 < N) ? tour[j + 1] : tour[0];

                double delta = (getDist(a, c) + getDist(b, d)) - (getDist(a, b) + getDist(c, d));

                if (delta < minDelta) {
                    minDelta = delta;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        if (minDelta < -0.000001) {
            reverse(tour.begin() + bestI, tour.begin() + bestJ + 1);
            betterSolution = true;
            currentDistance += minDelta;
        }
        
    }
    // cout << counter << " " << currentDistance << "\n";
    vector<int> result;
    result.push_back(currentDistance);
    result.push_back(counter);
    return result;
}

vector<double> meanMinPaths(vector<int> minPaths) {
    double minPath = minPaths.at(0);
    double meanOfMinPaths = 0;
    for(const auto& val: minPaths) {
        meanOfMinPaths += val;
        if(val < minPath) {
            minPath = val;
        }
    }
    meanOfMinPaths /= minPaths.size();
    vector<double> result;
    result.push_back(minPath);
    result.push_back(meanOfMinPaths);
    return result;
}

double meanSteps(vector<int> steps) {
    double meanOfSteps = 0;
    for(const auto& val: steps) {
        meanOfSteps += val;
    }
    meanOfSteps /= steps.size();
    return meanOfSteps;
}

int minKey(vector<bool>& mstSet, vector<int>& key) {
    int minKey = INT_MAX;
    int index = -1;
    for(size_t v = 0; v < mstSet.size(); v++) {
        if(mstSet.at(v) == false && key.at(v) < minKey) {
            minKey = key.at(v);
            index = v;
        }
    }
    return index;
}

vector<int> findMST(int n,  vector<int>& distMatrix) {
    vector<bool> mstSet(n, 0);
    vector<int> parent(n, -1);
    vector<int> key(n);
    for(int i = 0; i < n; i++) {
        mstSet.at(i) = false;
        key.at(i) = INT_MAX;
    }

    parent.at(0) = -1;
    key.at(0) = 0;

    for(int count = 0; count < n - 1; count++) {
        int minVertex = minKey(mstSet, key);
        mstSet.at(minVertex) = true;

        for (int v = 0; v < n; v++) {
            int index = n * minVertex + v;
            if(v != minVertex && mstSet.at(v) == false && distMatrix.at(index) < key.at(v)) {
                parent.at(v) = minVertex;
                key.at(v) = distMatrix.at(index);
            }
        }
    }
    return parent;
}

vector<vector<int>> convertMSTToEdges(vector<int>& mst, int n) {
    vector<vector<int>> result;
    for(size_t i = 0; i < mst.size(); i++) {
        vector<int> edge;
        if(mst.at(i) != -1) {
            edge.push_back(i);
            edge.push_back(mst.at(i));
            result.push_back(edge);
        }
    }
    return result;
}

vector<vector<int>> convertEdgesToAdjencyList(vector<vector<int>> edges, int n) {
    vector<vector<int>> result(n);
    for(size_t e = 0; e < edges.size(); e++) {
        result.at(edges.at(e).at(0)).push_back(edges.at(e).at(1));
        result.at(edges.at(e).at(1)).push_back(edges.at(e).at(0));
    }
    return result;
}

void dfs(int v, vector<vector<int>>& adjencyList, vector<bool>& visited, vector<int>& result, int v0) {
    visited[v] = true;
    result.push_back(v);
    for(size_t i = 0; i < adjencyList.at(v).size(); i++) {
        int u = adjencyList.at(v).at(i);
        if(visited.at(u) == false) {
            dfs(u, adjencyList, visited, result, v0);
        }
    }
}

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

        vector<int> minPaths;
        vector<int> steps;

            /// ------------------- QUICK LOCAL SEARCH ----------------------------------------------------------------------------------------------------------------------
            // #pragma omp parallel for schedule(dynamic)
            // for(int i = 0; i < cities; i++) {
            //     vector<int> permutation = generatePermutation(cities);
            //     vector<int> result = quickLocalSearch(permutation, cities, distMatrix);

            //     minPaths.push_back(result.at(0));
            //     steps.push_back(result.at(1));
            // 
            // vector<double> result = meanMinPaths(minPaths);
            // double meanOfSteps = meanSteps(steps);
            // cout << "najkrótsza ścieżka: " << result.at(0) << ", średnia długość ścieżki: " << result.at(1) << ", średnia ilość kroków: " << meanOfSteps << "\n";
            /// ------------------- QUICK LOCAL SEARCH -----------------------------------------------------------------------------------------------------------------------------------------------


        /// ------------------- 2-aprox. LOCAL SEARCH --------------------------------------------------------------------------------------------------------------------------------------------

        vector<int> mst = findMST(cities, distMatrix);
        int minMST = calculateTreeLength(mst, cities, distMatrix);
        vector<vector<int>> listOfEdges = convertMSTToEdges(mst, cities);
        vector<vector<int>> adjencyList = convertEdgesToAdjencyList(listOfEdges, cities);
        

        vector<int> localMinPaths;
        vector<int> localSteps;

        for(int j = 0; (j + 1) * (j + 1) < cities; j++) {
                cout << argv[i] << ": " << j << "\n";
                random_device rd;
                static thread_local mt19937 gen(std::random_device{}());
                uniform_int_distribution<int> dist(0, cities - 1);

                int v0 = dist(gen);
                vector<bool> visited(cities, 0);
                vector<int> permutation;

                dfs(v0, adjencyList, visited, permutation, v0);


                vector<int> result = localSearch(permutation, cities, distMatrix);

                // cout << i << ": " << result.at(0) << " " << result.at(1) << "\n";
                minPaths.push_back(result.at(0));
                steps.push_back(result.at(1));
        }
        

        vector<double> meanPaths = meanMinPaths(minPaths);
        double meanOfSteps = meanSteps(steps);
        cout << "najkrótsza ścieżka: " << meanPaths.at(0) << ", średnia długość ścieżki: " << meanPaths.at(1) << ", średnia ilość kroków: " << meanOfSteps << ", mst: " << minMST << "\n";
        /// ------------------- 2-aprox. LOCAL SEARCH --------------------------------------------------------------------------------------------------------------------------------------------
    }


    
}