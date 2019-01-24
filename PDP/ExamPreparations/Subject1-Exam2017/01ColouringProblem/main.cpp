/*
 * (3.5p) Write a parallel (distributed or local, at your choice) program for solving the k-coloring problem.
 * That is, you are given a number k, and n objects and some pairs among them that have distinct colors.
 * Find a solution to color them with at most n colors in total, if one exists. Assume you have a function that gets a
 * vector with n integers representing the assignment of colors to objects and checks if the constraits are obeyed or not.
 */
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <cmath>
#include <mutex>
#include <algorithm>
using namespace std;

#define THREADS 4

bool checkIfColouringCorrect(vector<int> colouring) {
    // mocked
    return true;
}
vector<int> getSol(int sol, int k, int n) {
    vector<int> vecSol;

    for(int i = 1; i <= n; i++) {
        vecSol.push_back(sol % k);
        sol /= k;
    }
    reverse(vecSol.begin(), vecSol.end());

    return vecSol;
}


int main() {
    // n object, k colours
    int n, k;
    cin >> n >> k;

    // response vector
    vector<int> correctColouring;

    // need to iterate from 0 to (k^n) - 1 with a step of (k^n - 1) / THREADS
    int to = pow(k, n) - 1;
    int step = to / THREADS;

    // start the threads
    vector<future<void>> threads;
    for(int i = 0; i <= to; i += step) {
        threads.emplace_back( std::async([i, step, to, k, n, &correctColouring](){
            // Check combination j
            for(int j = i; j <= min(i + step - 1, to); j++) {
                vector<int> sol = getSol(j, k, n);
                if(checkIfColouringCorrect(sol)) {
                    correctColouring = sol;
                }
            }
        }) );

    }

    // join the threads
    for(int i = 0; i < threads.size(); i++) {
        threads[i].get();
    }

    return 0;
}