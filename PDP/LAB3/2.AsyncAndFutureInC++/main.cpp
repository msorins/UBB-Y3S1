#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <future>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
using namespace std;

#define N 3000
#define NRThreads 2
#define MAXNR 1000

/*
 * N = 10000 (random numbers):
 *  1 thread = 1138 ms
 *  2 threads = 654 ms
 *  4 threads = 329 ms
 *  8 threads = 314 ms
 *  16 threads = 382 ms
 *  32 threads = 737 ms
 *  64 threads = 1028 ms ??
 *  128 threads = 988 MS
 */

/*
 *  N = 30000:
 *      1 thread: 10491 ms
 *      2 threads: 5376 ms
 *      4 threads: 2973 ms
 *      8 threads: 2771 ms
 *      16 threads: 6868 ms -> CPU 100%
 *      32 threads: 12744 ms -> CPU 100%
 */

vector< vector< int > > a(N, vector<int>(N, 0));
vector< vector< int > > b(N, vector<int>(N, 0));
vector< vector< int > > c(N, vector<int>(N, 0));

void doAddition(int const &pos) {
    int totalRepo = N * N;
    for(int crt = pos; crt < totalRepo; crt += NRThreads) {
        int i = crt / N ;
        int j = crt % N ;
        c[i][j] = a[i][j] + b[i][j];
    }
}


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
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++) {
            a[i][j] = 101;
            b[i][j] = 201;
        }

    // Start the time
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // Start the threads
    vector<future<void>> futures;
    for(int i = 0; i < NRThreads; i++) {
        futures.emplace_back(  std::async(doAddition, i) );
    }

    // Wait for all threads to finish
    for(int i = 0; i < NRThreads; i++) {
        futures[i].get();
    }

    // Stop the time
    auto t1 = Time::now();
    fsec fs = t1 - t0;

    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << d.count() << "ms\n";


    // Check to see if the results are ok
//    for(int i = 0; i < N; i++)
//        for(int j = 0; j < N; j++)  {
//            if(c[i][j] != a[i][j] + b[i][j]) {
//                throw std::runtime_error("Something was wrong with this addition");
//            }
//        }
}