#include <iostream>
#include <vector>
#include <future>
using namespace std;

#define N 100000

int computeSum(int k, vector<int> const v) {
    int sum = 0;

    for(int i = 0; i <= k ; i++) {
        sum += v[i];
    }

    return sum;
}

int main() {
    vector<int> v;
    for(int i = 1; i <= N; i++) {
        v.push_back(i);
    }

    // Start the time
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // Start the threads
    vector< future<int> > results;
    for(int k = 0; k < v.size(); k++) {
        results.emplace_back( std::async(computeSum, k, v) );
    }

    // Get the results and print them
    for(int k = 0; k < v.size(); k++) {
        cout << results[k].get() << " ";
    }
    cout << "\n";

    // Stop the time
    auto t1 = Time::now();
    fsec fs = t1 - t0;

    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << d.count() << "ms\n";

    return 0;
}