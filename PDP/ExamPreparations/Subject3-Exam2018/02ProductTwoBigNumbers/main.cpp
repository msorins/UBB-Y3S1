#include <iostream>
#include <thread>
#include <vector>
using namespace std;

vector<int> multiply(vector<int> a, vector<int> b) {
    vector<int> c(a.size() + b.size() - 1);
    for(int i = 0; i < a.size(); i++) {
        for(int j = 0; j < b.size(); j++) {
            c[i+j] += a[i] * b[j];
        }
    }

    int i = 0;
    int remainder = 0;
    for(i = 0; i < a.size() + b.size() - 1; i++) {
        c[i] += remainder;
        remainder = c[i] / 10;
        c[i] %= 10;
    }
    if(remainder) {
        c.push_back(remainder);
    }

    return c;
}

vector<atomic<int>> multiplyThreads(vector<int> a, vector<int> b, int nrThreads) {
    vector<atomic<int>> c(a.size() + b.size());
    vector<thread> threads;

    for(int t = 0; t < nrThreads; t++) {
        threads.emplace_back([a, b, &c, t, nrThreads]() {

            for(int i = t; i < a.size(); i += nrThreads) {
                for(int j = 0; j < b.size(); j++) {
                    c[i+j] += a[i] * b[j];
                }
            }

        });
    }

    for(int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    int i = 0;
    int remainder = 0;
    for(i = 0; i < a.size() + b.size() - 1; i++) {
        c[i] += remainder;
        remainder = c[i] / 10;
        c[i] = c[i] % 10;
    }
    if(remainder) {
        c[a.size() + b.size() - 1] = remainder;
    }

    return c;
}

vector<int> multiplyThreads2(vector<int> a, vector<int> b, int nrThreads) {
    vector<int> c(a.size() + b.size() - 1, 0);
    vector<thread> threads;

    for(int t = 0; t < nrThreads; t++) {
        threads.emplace_back([a, b, &c, t, nrThreads]() {

            for(int i = t; i < a.size() + b.size(); i += nrThreads) {
                for(int j = 0; j < b.size(); j++) {
                    if(i - j < 0 || i - j >= a.size()) {
                        continue;
                    }
                    c[i] += a[i - j] * b[j];
                }
            }

        });
    }

    for(int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    int i = 0;
    int remainder = 0;
    for(i = 0; i < a.size() + b.size() - 1; i++) {
        c[i] += remainder;
        remainder = c[i] / 10;
        c[i] = c[i] % 10;
    }
    if(remainder) {
        c.push_back(remainder);
    }

    return c;
}

int main() {
    vector<int>a {9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
    vector<int>b {2, 6, 4, 3, 1};

    vector<int> res = multiply(a, b);
    for(auto digit: res) {
        cout << digit << " ";
    }
    cout << "\n";


    vector<atomic<int>> resT = multiplyThreads(a, b, 5);
    for(int i = 0; i < resT.size(); i++) {
        cout << resT[i] << " ";
    }
    cout << "\n";

    vector<int> resT2 = multiplyThreads2(a, b, 5);
    for(int i = 0; i < resT2.size(); i++) {
        cout << resT2[i] << " ";
    }
    cout << "\n";
    return 0;
}