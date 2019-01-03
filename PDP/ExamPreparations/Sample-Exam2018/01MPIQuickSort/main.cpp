/*
 * mpirun -np 1 /Users/so/Desktop/Y3S1/PDP/ExamPreparations/Sample-Exam2018/01MPIQuickSort/cmake-build-debug/01MPIQuickSort
*/
#include <iostream>
#include <vector>
#include <future>
#include <mpi.h>
using namespace std;

int me;
int procs;
vector<int> v;

bool sent = false;
int nextProc() {
    if(me + 1 < procs) {
        return me + 1;
    } else {
        return -1;
    }
}

int partition(int low, int high) {
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

void quickSort(int low, int high) {
    MPI_Status _;
    if(low >= high) {
        return;
    }
    cout << "quickSort process: " << me << ", low: " << low << ", high: " << high << "\n";

    int pivot = partition(low, high);

    // Right
    int toProc = nextProc();
    if(toProc == -1 || sent == true) {
        // Right do local -> no more processes left
        quickSort(pivot + 1, high);
    } else {
        // Right send to other process
        vector<int> subVector(v.begin() + pivot + 1, v.begin() + high + 1);
        int sz = subVector.size();
        if(sz) {
            sent = true;
            cout << "Sending to process " << toProc << ":            (" << pivot + 1 << ", " << high << ") \n";

            MPI_Bsend(&sz,     1, MPI_INT, toProc,  0, MPI_COMM_WORLD);
            MPI_Bsend(subVector.data(),   sz, MPI_INT, toProc,  1, MPI_COMM_WORLD);

            // Get Result
            MPI_Recv(subVector.data(), sz, MPI_INT,  toProc,  2, MPI_COMM_WORLD, &_);

            cout << "Received from process " << toProc << "\n";

            // Assign results to current data
            int i = pivot + 1;
            for(auto el: subVector) {
                v[i] = el;
                i++;
            }
        }
    }

    // Left do local
    quickSort(low, pivot - 1);
}

void masterGenerate() {
    vector<int> aux{111, 4213, 12, 667, 213, 321, 67, 78, 1231, 654, 1, 2, 88, 89953, 3242 ,12, 432, 665, 332, 126, 775, 331, 789, 221};
    v = aux;
}

void masterStartTask() {
    quickSort(0, v.size() - 1);
}

void slaveReceiveTask() {
    // Receive vector to quick sort
    MPI_Status _;
    int sz;
    MPI_Recv(&sz, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &_);
    v.resize(sz);
    MPI_Recv(v.data(), sz, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &_);

    // Do the quick sort
    quickSort(0, v.size() - 1);

    // Send the response to its father
    MPI_Bsend(v.data(),  v.size(), MPI_INT, _.MPI_SOURCE,  2, MPI_COMM_WORLD);
}

int main() {
    MPI_Init(0, 0);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    cout << "Process " << me << " started \n";

    if(me == 0) {
        masterGenerate();
        masterStartTask();

        for(auto el: v) {
            cout << el << " ";
        }
    } else {
        slaveReceiveTask();
    }

    if(!sent && nextProc() != -1) {
        vector<int> empty;
        int zero = 0;
        MPI_Bsend(&zero     ,1, MPI_INT, nextProc(),  0, MPI_COMM_WORLD);
        MPI_Bsend(empty.data(),   zero, MPI_INT, nextProc(),  1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}