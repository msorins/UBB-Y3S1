#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

/*
 *  Write a parallel or distributed program that counts the number of subsets of k out of N that satisfy a given property. You have a function (bool pred(vector <int> const& v)) that verifies if a given subset satisfies the property. Your program shall call that function once for each subset of k elements and count the number of times it returns true.
 */

atomic<int> nrs;
void subsets(vector<int> v, int n, int k, int threads) {
    int sz = v.size();

    // Exit recursion
    if(sz == k) {
        nrs += 1;
        return;
    }

    // Enter recursion
    int start;
    if(sz == 0) {
        start == 0;
    } else {
        start = v[sz - 1] + 1;
    }

    if(threads == 1) {
        for(int i = start; i < n; i++) {
            v.push_back(i);
            subsets(v, n, k, threads);
            v.pop_back();
        }
    } else {
        auto t = thread([start, n, v, k, threads]() mutable {
            for(int i = start + 1; i < n; i += 2) {
                v.push_back(i);
                subsets(v, n, k, threads / 2);
                v.pop_back();
            }
        });


        for(int i = start; i < n; i += 2) {
            v.push_back(i);
            subsets(v, n, k, threads - threads / 2);
            v.pop_back();
        }
        t.join();
    }

}

int main() {
    subsets(vector<int>(), 1000, 3, 5);
    cout << nrs << "\n";
    return 0;
}