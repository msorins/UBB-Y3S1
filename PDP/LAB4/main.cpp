#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lockfree/queue.hpp>

#include <iostream>

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
using namespace std;

#define N 1000
#define NRThreads 8
#define MAXNR 1000

/*
 * N = 1000 (random numbers):
 *  1 thread: 23294ms
 *  2 threads: 15409ms
 *  4 threads: 11368ms
 *  8 threads: 9222ms
 */


vector< vector< int > > a(N, vector<int>(N, 0));
vector< vector< int > > b(N, vector<int>(N, 0));
vector< vector< int > > c(N, vector<int>(N, 0));
vector< vector< int > > resFirst(N, vector<int>(N, 0));
vector< vector< int > > resFinal(N, vector<int>(N, 0));

boost::lockfree::queue<int> queue(120);
boost::atomic<bool> done(false);

void doMultiplication(vector<vector<int>> const &a, vector<vector<int>> const &b, vector<vector<int>> const &c, int const &i, bool const &producer) {
    auto &A = const_cast<vector<vector<int>> &>(a);
    auto &B = const_cast<vector<vector<int>> &>(b);
    auto &C = const_cast<vector<vector<int>> &>(c);


    for(int j = 0; j < N; j++) {
        for(int h = 0; h < N; h++) {
            C[i][j] += A[i][h] * B[h][j];
        }
    }

    // When computation is finished, push completed line in queue
    if(producer) {
        queue.push(i);
    }
}

void consumer() {
    // Wait until there is an element in the queue
    while (!done) {

        // If queue has an element
        int computedLine;
        if(queue.pop(computedLine)) {
            doMultiplication(std::ref(resFirst), std::ref(c), std::ref(resFinal), computedLine, false);
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
            a[i][j] = 2;
            b[i][j] = 3;
            c[i][j] = 4;
            resFirst[i][j] = 0;
            resFinal[i][j] = 0;
        }

    // Start the time
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // Start the producers
    boost::asio::thread_pool poolProducer(NRThreads);
    for(int i = 0; i < N; i++) {
        boost::asio::post(poolProducer,
                          [i]()
                          {
                              doMultiplication(std::ref(a), std::ref(b), std::ref(resFirst), i, true);
                          });
    }

    // Start the consumers
    boost::thread_group groupConsumers;
    for(int i = 0; i < NRThreads; i++) {
        groupConsumers.create_thread(consumer);
    }

    // Join both the producer and the consumer
    poolProducer.join();
    done = true;
    groupConsumers.join_all();

    // Stop the time
    auto t1 = Time::now();
    fsec fs = t1 - t0;

    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << d.count() << "ms\n";
}