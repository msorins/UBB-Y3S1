#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lockfree/queue.hpp>
#include <random>
#include <mpi.h>

using namespace std;

int me;
int procs;

#define sizePol 10
#define sizeResultPol (sizePol * 2 + 1)

vector<int> polinomA;
vector<int> polinomB;

void slaveMultiplication(vector<int> const &a, vector<int> const &b, int pozLeft) {
    vector<int> c(sizeResultPol, 0);

    for(int pozRight = 0; pozRight < b.size(); pozRight++) {
        if(b[pozRight] != 0) {
            c[pozLeft + pozRight] += (a[pozLeft] * b[pozRight]);
        }
    }

    // Send the result back to master
    cout << "Slave: " << me << " sending result \n";
    MPI_Bsend(c.data(), sizeResultPol, MPI_INT, 0,  3, MPI_COMM_WORLD);
}

void slaveReceive() {
    for(int i = me; i< sizePol; i += procs) {
        vector<int> polA(sizePol);
        vector<int> polB(sizePol);
        int pozLeft;
        MPI_Status _;

        MPI_Recv(polA.data(), sizePol, MPI_INT, 0, 0, MPI_COMM_WORLD, &_);
        MPI_Recv(polB.data(), sizePol, MPI_INT, 0, 1, MPI_COMM_WORLD, &_);
        MPI_Recv(&pozLeft,                    1, MPI_INT, 0, 2, MPI_COMM_WORLD, &_);

        slaveMultiplication(polA, polB, pozLeft);
    }
}

void masterGenerateData() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rndGen(1, 1);
    for(int i = 0; i < sizePol; i++) {
        polinomA.push_back(rndGen(gen));
        polinomB.push_back(rndGen(gen));
    }
}

void masterAssignTasks() {
    for(int i = 0; i < sizePol; i++) {
        int toProc = i % procs;
        cout << "Assigning: " << i << " to proccessor: " << toProc << "\n";
        MPI_Bsend(polinomA.data(), sizePol, MPI_INT, toProc,  0, MPI_COMM_WORLD);
        MPI_Bsend(polinomB.data(), sizePol, MPI_INT, toProc,  1, MPI_COMM_WORLD);
        MPI_Bsend(&i,                  1, MPI_INT, toProc,  2, MPI_COMM_WORLD);
    }
}

void masterCollect() {
    vector<int> polinomC(sizeResultPol, 0);

    for(int i = 0; i < sizePol; i++) {
        int fromProc = i % procs;

        vector<int>c(sizeResultPol);
        MPI_Status _;
        cout << "Waiting to receive from slave: " << fromProc <<"\n";
        MPI_Recv(c.data(), sizeResultPol, MPI_INT, fromProc, 3, MPI_COMM_WORLD, &_);
        cout << "Received from slave: " << fromProc <<"\n";

        for(int j = 0; j < sizeResultPol; j++) {
            polinomC[j] += c[j];
        }
    }

    cout << "Final answer: \n";
    for(int i = 0; i < sizeResultPol; i++)  {
        cout << polinomC[i] << " ";
    }
    cout << "\n";
}

int main() {
    // Initialsie MPI
    MPI_Init(0, 0);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    cout << me << " " << procs << "\n";

    if (!me) {
        // Master
        masterGenerateData();
        masterAssignTasks();
        slaveReceive();
        cout << "master finished \n";
        masterCollect();
    } else {
        // Slave
        slaveReceive();
    }

    MPI_Finalize();
}