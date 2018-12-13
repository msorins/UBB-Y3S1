// https://courses.cs.washington.edu/courses/cse332/11wi/lectures/cse332-11wi-lec19-PrefixAndSorting.pdf

#include <iostream>
#include <vector>
#include <future>
#include <cmath>
using namespace std;

#define N 8

void computeSum(vector<int> const &v, vector<int> &tree, int pos, int k) {
    // Leaf
    if(pos * 2 + 1 > k) {
        tree[pos] = v[pos - N + 1];
        return;
    }

    // Go Left
    future<void> left = std::async(computeSum, std::ref(v), std::ref(tree), pos * 2, k);

    // Go Right
    future<void> right = std::async(computeSum, std::ref(v), std::ref(tree), pos * 2 + 1, k);

    // Update current node
    left.wait();
    right.wait();
    tree[pos] = tree[pos * 2] + tree[pos * 2 + 1];
}

void computeLeft(vector<int> const &v, vector<int> const &treeSum, vector<int> &treeLeft, int pos, int k, int fromLeft) {
    // Leaf
    if(pos * 2 + 1 > k) {
        treeLeft[pos] = fromLeft;
        return;
    }

    // Go Left
    future<void> left = std::async(computeLeft, std::ref(v), std::ref(treeSum), std::ref(treeLeft), pos * 2, k,
                                   fromLeft);

    // Go Right
    future<void> right = std::async(computeLeft, std::ref(v), std::ref(treeSum), std::ref(treeLeft), pos * 2 + 1, k,
                                    fromLeft + treeSum[pos * 2]);

    // Update current node
    left.wait();
    right.wait();
    treeLeft[pos] = fromLeft;
}

void getResults(vector<int> const &treeSum, vector<int> &treeLeft, int pos, int k, vector<int> &result) {
    // Leaf
    if(pos * 2 + 1 > k) {
        result[pos - N + 1] = treeSum[pos] + treeLeft[pos];
        return;
    }

    // Go Left
    future<void> left = std::async(getResults, std::ref(treeSum), std::ref(treeLeft), pos * 2, k, std::ref(result));

    // Go Right
    future<void> right = std::async(getResults, std::ref(treeSum), std::ref(treeLeft), pos * 2 + 1, k, std::ref(result));

    left.wait();
    right.wait();
}

int main() {
    int k = pow(2, log2(N) + 1) - 1;
    int k2 = pow(2, log2(N)) ;

    vector<int> v(N, 0);
    vector<int> treeSum(k + 5, 0);
    vector<int> treeLeft(k + 5, 0);
    vector<int> result(N + 5, 0);

    for(int i = 1; i <= N; i++) {
        v[i] = i;
    }

    computeSum(v, treeSum, 1, k);
    computeLeft(v, treeSum, treeLeft, 1, k, 0);
    getResults(treeSum, treeLeft, 1, k, result);

    for(int i = 1; i <= N; i++) {
        cout << result[i] << " ";
    }

    return 0;
}