#ifndef UTILS_H
#define UTILS_H

#include <vector>
using namespace std;

int distanceBetweenPoints(double x1, double y1, double x2, double y2);
int calculatePathDistance(const vector<int>& permutation, int N, const vector<int>& distMatrix);
vector<int> generatePermutation(int N);
int calculateTreeLength (const vector<int>& tree, int N, const vector<int>& distMatrix);
int minKey(vector<bool>& mstSet, vector<int>& key);
vector<int> findMST(int n,  vector<int>& distMatrix);
vector<vector<int>> convertMSTToEdges(vector<int>& mst, int n);
vector<vector<int>> convertMSTToEdges(vector<int>& mst, int n);
vector<vector<int>> convertEdgesToAdjencyList(vector<vector<int>> edges, int n);
void constructHamiltionCycleThroughDFS(int v, vector<vector<int>>& adjencyList, vector<bool>& visited, vector<int>& result, int v0);
int simulatedAnnealing(int numberOfTries, float temperature, float coefficient, int cities, float minTemperature, vector<int>& tour, vector<int>& distMatrix);
float calculateInitTemp(vector<int>& tour, int cities, vector<int>& distMatrix);
int tabuSearch(vector<int> permutation, int cities, int tabuTenure, const vector<int>& distMatrix);

#endif