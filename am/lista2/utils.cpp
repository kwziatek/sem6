#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

using namespace std;
int distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    return static_cast<int>(round(hypot(x1 - x2, y1 - y2)));
}

int calculatePathDistance(const vector<int>& permutation, int N, const vector<int>& distMatrix) {
    int result = 0;
    for(size_t i = 1; i < permutation.size(); i++) {
        result += distMatrix[permutation.at(i) * N + permutation.at(i - 1)];
    }
    result += distMatrix[permutation.at(permutation.size() - 1) * N + permutation.at(0)];

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
