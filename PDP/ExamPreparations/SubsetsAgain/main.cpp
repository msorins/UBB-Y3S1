#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
using namespace std;

mutex mtx;
void subsets(vector<int> &subset, int n, int threads) {
    // Show current subset
    mtx.lock();
    for(auto elem: subset) {
        cout << elem << " ";
    }
    cout << "\n";
    mtx.unlock();

    int st = 0;
    if(subset.size()) {
        st = subset[ subset.size() - 1] + 1;
    }

    if(threads == 1) {
        for(int i = st; i < n; i++) {
            // Put i
            subset.push_back(i);
            subsets(subset, n, 1);
            subset.pop_back();

            // Do not put i
        }
    } else {
        vector<int> subsetCopy(subset);
       auto t1 = thread([n, subsetCopy, st]() mutable{
           for(int i = st + 1; i < n; i += 2) {
               // Put i
               subsetCopy.push_back(i);
               subsets(subsetCopy, n, 1);
               subsetCopy.pop_back();

               // Do not put i
           }
       });

        for(int i = st; i < n; i += 2) {
            // Put i
            subset.push_back(i);
            subsets(subset, n, 1);
            subset.pop_back();

            // Do not put i
        }
       t1.join();
    }

}
int main() {
    vector<int> v;
    subsets(v, 4, 1);
    return 0;
}