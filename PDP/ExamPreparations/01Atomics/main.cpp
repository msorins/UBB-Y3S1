#include <iostream>
#include <atomic>
#include <future>
#include <vector>
using namespace std;

atomic<int> nr = 0;

void incrementAndPrint() {
    nr++;
}

int main() {
    vector<future<void>> futures;

    for(int i = 1; i <= 100000; i++) {
        futures.emplace_back( std::async(incrementAndPrint) );
    }

    for(int i = 0; i < futures.size(); i++) {
        futures[i].wait();
    }
    return 0;
}