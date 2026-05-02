#ifndef UTILS_H
#define UTILS_H

#include <vector>
using namespace std;

int distanceBetweenPoints(double x1, double y1, double x2, double y2);
int calculatePathDistance(const vector<int>& permutation, int N, const vector<int>& distMatrix);
vector<int> generatePermutation(int N);
int calculateTreeLength (const vector<int>& tree, int N, const vector<int>& distMatrix);

#endif