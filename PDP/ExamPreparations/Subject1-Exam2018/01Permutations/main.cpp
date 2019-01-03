#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;


/*
 * (3p) Write a parallel or distributed program that counts the number of permutations of N that
 * satisfy a given property.
 * You have a function (bool pred(vector <int> const& v)) that verifies if a given permutation satisfies the property.
 * Your program shall call that function once for each permutation and count the number of times it returns true.
 */

bool contains(vector<int> const &v, int elem) {
    for(auto e: v) {
        if(e == elem) {
            return true;
        }
    }
    return false;
}

atomic<int> res;

void bk(vector<int> v, int threads, int n) {
    if(v.size() == n) {
        // Solution
        res++;
        return;
    }

    if(threads == 1) {
        for(int i = 1; i <= n; i++) {
            if(!contains(v, i)) {
                v.push_back(i);
                bk(v, threads, n);
                v.pop_back();
            }
        }
    } else {
        // Split the work half and half with another thread

        // Another Thread
        vector<int> vCopy(v);
        auto thrd = thread([n, threads, v]() mutable {
            // Crt Thread
            for(int i = 2; i <= n; i += 2) {
                if(!contains(v, i)) {
                    v.push_back(i);
                    bk(v, threads - threads / 2, n);
                    v.pop_back();
                }
            }
        });


        // Crt Thread
        for(int i = 1; i <= n; i += 2) {
            if(!contains(v, i)) {
                v.push_back(i);
                bk(v, threads / 2, n);
                v.pop_back();
            }
        }

        thrd.join();
    }
}

int main() {
    vector<int> v;
    int n = 10;
    bk(v, 8, n);
    cout << res << " " << "\n";
    return 0;
}