#include <iostream>
#include <vector>
#include <future>
using namespace std;

int scalarProduct(vector<int> a, vector<int> b, int threads) {
    if(a.size() == 1) {
        return a[0] * b[0];
    }

    if(threads == 1) {
        int crts = 0;
        for(int i = 0; i < a.size(); i++) {
            crts += a[i] * b[i];
        }

        return crts;
    } else {
        // Another thread
        vector<int> cpyA(a.begin(), a.begin() + a.size() / 2);
        vector<int> cpyB(b.begin(), b.begin() + b.size() / 2);
        auto f1 = async([cpyA, cpyB, threads]() {
            return scalarProduct(cpyA, cpyB, threads / 2);
        });


        // Current thread
        vector<int> cpySecondA(a.begin() + a.size() / 2, a.end());
        vector<int> cpySecondB(b.begin() + b.size() / 2, b.end());

        return scalarProduct(cpySecondA, cpySecondB, threads - threads / 2) + f1.get();
    }
}

int main() {
    vector<int> a{1, 2, 3, 4};
    vector<int> b{1, 2, 3, 4};

    cout << scalarProduct(a, b, 4);
    return 0;
}