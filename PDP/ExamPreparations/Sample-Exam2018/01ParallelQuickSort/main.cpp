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

void quickSort(vector<int> &v, int low, int high) {
    if(low >= high) {
        return;
    }

    int pivot = partition(v, low, high);
    auto f1 = async(quickSort, ref(v), low, pivot - 1);
    auto f2 = async(quickSort, ref(v), pivot + 1, high);

    f1.get();
    f2.get();
}

int main() {
    vector<int> v{10, 80, 30, 90, 40, 50, 70};

    quickSort(v, 0, v.size() - 1);

    for(auto el: v) {
        cout << el << " ";
    }
    return 0;
}