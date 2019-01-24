#include <iostream>
#include <vector>
#include <future>
using namespace std;

int partition(vector<int> &v, int low, int high) {
    int pivot = v[high];
    int i = low - 1;

    for(int j = low; j < high; j++) {
        if(v[j] < pivot) {
            i++;
            swap(v[i], v[j]);
        }
    }
    i++;
    swap(v[i], v[high]);

    return i;
}

void quickSort(vector<int> &v, int low, int high, int threads) {
    if(low >= high) {
        return;
    }

    int pivot = partition(v, low, high);
    if(threads == 1) {
        quickSort(v, low, pivot - 1, 1);
        quickSort(v, pivot + 1, high, 1);
    } else {
        // Another thread
        auto f1 = async(quickSort, ref(v), low, pivot - 1, threads / 2);

        // Crt thread
        quickSort(v, pivot + 1, high, threads - threads / 2);

        f1.get();
    }
}

int main() {
    vector<int> v{10, 80, 30, 90, 40, 50, 70};
    for(int i = 10000; i >= 1; i--) {
        v.push_back(i);
    }

    int threads = 4;
    quickSort(v, 0, v.size() - 1, threads);

    for(auto el: v) {
        cout << el << " ";
    }
    return 0;
}