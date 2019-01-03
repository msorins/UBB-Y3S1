#include <iostream>
#include <vector>
#include <thread>
using namespace std;

vector<int> conv(vector<int> a, vector<int> b) {
    // a size must be equal to b size
    int n = (int) a.size();
    vector<int> res(n, 0);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            res[i] += a[i] * b[(i - j + n) % n];
        }
    }

    return res;
}

vector<int> convParallel(vector<int> a, vector<int> b, int threadsNumber) {
    // a size must be equal to b size
    int n = (int) a.size();
    vector<int> res(n, 0);
    vector<thread> threads;

    for(int th = 0; th < threadsNumber; th++) {
        threads.emplace_back( thread([th, n, threadsNumber,a, b, &res](){
            for(int i = th; i < n; i += threadsNumber) {
                for(int j = 0; j < n; j++) {
                    res[i] += a[i] * b[(i - j + n) % n];
                }
            }
        }));
    }

    for(int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    return res;
}

int main() {
    vector<int> a{1, 2, 3, 4, 5, 6, 99};
    vector<int> b{7, 8, 9, 10, 11, 12, 99};

    vector<int> res = conv(a, b);
    vector<int> resP = convParallel(a, b, 2);

    for(int i = 0; i < res.size(); i++) {
        if(res[i] != resP[i]) {
            cout << "wrong result \n";
        }
        cout << res[i] << " ";
    }
    cout << "\n";

    return 0;
}