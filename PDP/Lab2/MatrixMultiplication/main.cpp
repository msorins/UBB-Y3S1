#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
using namespace std;

#define N 1500
#define NRThreads 16
#define MAXNR 1000

/*
 * N = 1500 (random numbers):
 *  1 thread =  48320  ms
 *  2 threads = 24359 ms
 *  4 threads = 13128  ms
 *  8 threads =  12089     ms
 */

/*
 *  N = 100000:
 *      1 thread:
 *      2 threads:
 *      4 threads:
 *      8 threads:
 *      16 threads:
 *      32 threads:
 *      64 threads:
 *      128 threads:
 */

vector< vector< int > > a(N, vector<int>(N, 0));
vector< vector< int > > b(N, vector<int>(N, 0));
vector< vector< int > > c(N, vector<int>(N, 0));
vector< vector< int > > d(N, vector<int>(N, 0));

void doMultiplication(int const &pos) {
    int totalRepo = N * N;

    for(int crt = pos; crt < totalRepo; crt += NRThreads) {
        int i = crt / N ;
        int j = crt % N ;

        for(int h = 0; h < N; h++) {
            c[i][j] += a[i][h] * b[h][j];
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rndGen(1, MAXNR);

    // Initialise the matrix with some numbers
    int nr = 0;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++) {
            nr++;
            a[i][j] = nr;
            b[i][j] = nr;
        }

    // Start the time
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // Start the threads
    vector<thread> threads;
    for(int i = 0; i < NRThreads; i++) {
        threads.emplace_back(thread(doMultiplication, i) );
    }

    // Wait for all threads to finish
    for(auto &thread: threads) {
        thread.join();;
    }

    // Stop the time
    auto t1 = Time::now();
    fsec fs = t1 - t0;

    ms timed = std::chrono::duration_cast<ms>(fs);
    std::cout << timed.count() << "ms\n";

    // Compute the result of the multiplication (to check for the threads result)
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int h = 0; h < N; h++) {
                d[i][j] += a[i][h] * b[h][j];
            }
        }
    }
    // Check to see if the results are ok
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)  {
            if(d[i][j] != c[i][j]) {
                throw std::runtime_error("Something was wrong with this addition");
            }
        }
}