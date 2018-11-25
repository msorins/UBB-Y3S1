#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lockfree/queue.hpp>
#include <random>
using namespace std;

#define  NRThreads 8
#define  Size 30000

vector<int> polinomA = {3, 0, 4, 0, 1};
vector<int> polinomB = {3, 5, 0, 0, 0};
vector< atomic<int> > polinomC(Size * 2 + 5);

// 1 thread: 14326ms
// 2 threads: 8227ms
// 4 threads: 6604ms
// 8 threads: 4400ms

void doMultiplication( vector<int> const &a, vector<int> const &b, vector<atomic<int>> const &c, int pozLeft) {
    auto &C = const_cast<vector<atomic<int>> &>(c);

    for(int pozRight = 0; pozRight < b.size(); pozRight++) {
        if(b[pozRight] != 0) {
            C[pozLeft + pozRight] += (a[pozLeft] * b[pozRight]);
        }
    }
}

int main() {
    // Add numbers in PolinomA and B
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rndGen(1, 40);
    for(int i = 0; i < Size; i++) {
        polinomA.push_back(rndGen(gen));
        polinomB.push_back(rndGen(gen));
    }

    // Start the time
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // Run the threads
    boost::asio::thread_pool threadPool(NRThreads);
    for(int i = polinomA.size() - 1; i >= 0; --i) {
        if(polinomA[i] != 0) {
            boost::asio::post(threadPool,
                              [i]()
                              {
                                  doMultiplication(polinomA, polinomB, std::ref(polinomC), i);
                              });
        }
    }

    // Join the threads
    threadPool.join();

    // Stop the time
    auto t1 = Time::now();
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << d.count() << "ms\n";

    return 0;
}