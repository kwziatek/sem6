#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <limits.h>

using namespace std;
int distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    return static_cast<int>(round(hypot(x1 - x2, y1 - y2)));
}

int calculatePathDistance(const vector<int>& tour, int N, const vector<int>& distMatrix) {
    int result = 0;
    for(size_t i = 1; i < tour.size(); i++) {
        result += distMatrix[tour.at(i) * N + tour.at(i - 1)];
    }
    result += distMatrix[tour.at(tour.size() - 1) * N + tour.at(0)];

    return result;
}

vector<int> generatePermutation(int N) {
    vector<int> tour(N);
    iota(tour.begin(), tour.end(), 0);

    // thread_local sprawia, że generator jest tworzony RAZ na każdy wątek.
    // Słowo 'static' gwarantuje, że przetrwa on między wywołaniami funkcji.
    static thread_local mt19937 g(random_device{}());

    shuffle(tour.begin(), tour.end(), g);

    return tour;
}

int calculateTreeLength (const vector<int>& tree, int N, const vector<int>& distMatrix) {
    int sum = 0;
    for(size_t v = 0; v < tree.size(); v++) {
        if(tree.at(v) != -1) {
            sum += distMatrix.at(v * N + tree.at(v));
        }
    }
    return sum;
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

void constructHamiltionCycleThroughDFS(int v, vector<vector<int>>& adjencyList, vector<bool>& visited, vector<int>& result, int v0) {
    visited[v] = true;
    result.push_back(v);
    for(size_t i = 0; i < adjencyList.at(v).size(); i++) {
        int u = adjencyList.at(v).at(i);
        if(visited.at(u) == false) {
            constructHamiltionCycleThroughDFS(u, adjencyList, visited, result, v0);
        }
    }
}


int simulatedAnnealing(int numberOfTries, float temperature, float coefficient, int cities, float minTemperature, vector<int>& tour, vector<int>& distMatrix) {
    auto getDist = [&](int i, int j) { return distMatrix[i * cities + j]; };
    random_device rd;
    static thread_local mt19937 gen(std::random_device{}());
    uniform_int_distribution<int> dist(1, cities - 1);
    
    // dopóki temperatura nie przekroczy minimalnej (warunek stopu)
    int counter = 0;
    int betterResult = 0;
    while(temperature >= minTemperature) {

        // cout << temperature << " " << minTemperature << " \n";make 

        // wykonujemy daną liczbę prób dla każdej epoki
        for(int l = 0; l < numberOfTries; l++) {
            counter++;

            int i = dist(gen);
            int j = dist(gen);

            if (i == j) continue;
            if (i > j) swap(i, j);

            int a = tour[i - 1];
            int b = tour[i];
            int c = tour[j];
            int d = (j + 1 < cities) ? tour[j + 1] : tour[0];

            int delta = (getDist(a, c) + getDist(b, d)) - (getDist(a, b) + getDist(c, d));

            
            
            betterResult++;

            // Jeśli wśród N wylosowanych znaleźliśmy jakąkolwiek poprawę zamieniemy, w przeciwnym razie zamieniamy najlepszego zgodnie z prawd. Boltzmana
            reverse(tour.begin() + i, tour.begin() + j + 1);
            bool acceptMove = false;

            if (delta < 0) {
                acceptMove = true;
            } else {
                float boltzConstant = exp( -static_cast<float>(delta) / temperature);
                // cout << "boltz: " << boltzConstant <<"\n";
                uniform_real_distribution<float> probDist(0.0f, 1.0f);

                if(probDist(gen) <= boltzConstant) {
                    acceptMove = true;
                }
            }

            if(!acceptMove) {
                reverse(tour.begin() + i, tour.begin() + j + 1);
                betterResult--;
            }
        }
        // cout << "temperature: " << temperature << ", acceptance: " << betterResult << " / " << counter << " = " << betterResult/(float) counter << ", length: " << calculatePathDistance(tour, cities, distMatrix) << "\n";

        counter = 0;
        betterResult = 0;
        // po wykonaniu wszystkich prób dla danej epoki, zmniejszamy temperature
        temperature *= coefficient;

    }

    return calculatePathDistance(tour, cities, distMatrix);
}

float calculateInitTemp(vector<int>& tour, int cities, vector<int>& distMatrix) {
    auto getDist = [&](int i, int j) { return distMatrix[i * cities + j]; };
    random_device rd;
    static thread_local mt19937 gen(std::random_device{}());
    uniform_int_distribution<int> dist(1, cities - 1);
    // Celujemy w początkową akceptację na poziomie 85%
    float targetAcceptance = 0.85f; 

    double sumDeltaOfWorseMoves = 0;
    int worseMovesCount = 0;
    int sampleSize = 500; // wystarczy kilkaset prób, by poznać skalę mapy

    for (int s = 0; s < sampleSize; ++s) {
        int i = dist(gen);
        int j = dist(gen);
        if (i == j) continue;
        if (i > j) swap(i, j);

        int a = tour[i - 1];
        int b = tour[i];
        int c = tour[j];
        int d = (j + 1 < cities) ? tour[j + 1] : tour[0];

        int delta = (getDist(a, c) + getDist(b, d)) - (getDist(a, b) + getDist(c, d));

        // Interesują nas tylko ruchy, które POGARSZAJĄ trasę (delta > 0)
        if (delta > 0) {
            sumDeltaOfWorseMoves += delta;
            worseMovesCount++;
        }
    }

    float temperature;
    if (worseMovesCount > 0) {
        double avgDelta = sumDeltaOfWorseMoves / worseMovesCount;
        // Wzór: T0 = -avgDelta / ln(P0)
        temperature = -avgDelta / log(targetAcceptance);
    } else {
        // Zabezpieczenie, gdyby jakimś cudem wszystkie ruchy były poprawiające
        temperature = 1000.0f; 
    }

    cout << "Automatycznie dobrana temperatura początkowa (T0): " << temperature << "\n";
    return temperature;
}

int tabuSearch(vector<int> tour, int cities, int tabuTenure, const vector<int>& distMatrix) {
    auto getDist = [&](int i, int j) { return distMatrix[i * cities + j]; };
    
    int currentDistance = calculatePathDistance(tour, cities, distMatrix);
    int bestDistance = currentDistance;
    
    // Tabu memory matrix: stores the future iteration number until which a move is tabu
    vector<vector<int>> tabuMatrix(cities, vector<int>(cities, 0));
    
    int maxIterations = 1000; // Let the algorithm run long enough to explore

    for (int iter = 0; iter < maxIterations; ++iter) {
        int bestI = -1, bestJ = -1;
        int minDelta = INT_MAX; // Start high to allow non-improving moves

        for (int i = 1; i < cities - 1; ++i) {
            for (int j = i + 1; j < cities; ++j) {
                int a = tour[i - 1];
                int b = tour[i];
                int c = tour[j];
                int d = (j + 1 < cities) ? tour[j + 1] : tour[0];

                double delta = (getDist(a, c) + getDist(b, d)) - (getDist(a, b) + getDist(c, d));

                // Check if this specific move is currently tabu
                bool isTabu = (iter < tabuMatrix[i][j]);

                // Aspiration Criterion: If the move is tabu but beats our ALL-TIME best, override tabu!
                if (isTabu && (currentDistance + delta < bestDistance)) {
                    isTabu = false;
                }

                // We want the lowest delta (even if it's positive/worse)
                if (!isTabu && delta < minDelta) {
                    minDelta = delta;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        if (bestI == -1) {
            break;
        } 
        reverse(tour.begin() + bestI, tour.begin() + bestJ + 1);
        currentDistance += minDelta;

        tabuMatrix[bestI][bestJ] = iter + tabuTenure;
        tabuMatrix[bestJ][bestI] = iter + tabuTenure;

        if(currentDistance < bestDistance) {
            bestDistance = currentDistance;
        }
        
    }
    return currentDistance;
}
