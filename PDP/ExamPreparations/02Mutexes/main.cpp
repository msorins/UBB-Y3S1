#include <iostream>
#include <mutex>
#include <list>
#include <shared_mutex>

using namespace std;

mutex mtxA, mtxB, mtxC;
recursive_mutex recMTX;
shared_mutex sharedMTX;

int main() {
    sharedMTX.lock_shared();
    sharedMTX.lock();


    return 0;
}