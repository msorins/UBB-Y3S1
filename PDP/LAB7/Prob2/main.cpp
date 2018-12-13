#include <iostream>
#include <vector>
#include <random>

#include <future>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lockfree/queue.hpp>

using namespace std;

#define NRS 3
#define DIGITS 100

void doAddition(shared_ptr<boost::lockfree::queue<int>> nrA,
                const vector< int > nrB,
                shared_ptr<boost::lockfree::queue<int>> sendResultsTo ) {

    int indexNrB = -1;
    int remainder = 0;

    // Exhaust nrA digits
    while(true) {
        int currentDigit;
        if(nrA.get()->pop(currentDigit)) {
            if(currentDigit == -1) {
                break;
            }

            indexNrB += 1;
            if(indexNrB >= nrB.size()) {
               currentDigit += remainder;
            } else {
                currentDigit += nrB[indexNrB] + remainder;
            }

            remainder = currentDigit / 10;
            currentDigit %= 10;

            // Send the current digit
            sendResultsTo->push(currentDigit);
        }
    }

    // Exhaust nrB digits
    for(int i = indexNrB + 1; i < nrB.size(); i++) {
        int currentDigit = nrB[i] + remainder;
        remainder = currentDigit / 10;
        currentDigit %= 10;

        // Send the current digit
        sendResultsTo->push(currentDigit);
    }

    // If the result is going to have one extra digit
    if(remainder) {
        sendResultsTo->push(remainder);
    }

    // Send stop signal
    sendResultsTo->push(-1);
}

int main() {
    // Populate the vector
    vector< vector<int> > v;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> digit(1, 9);
    std::uniform_int_distribution<> nrDigit(1, DIGITS);

//    for(int i = 1; i <= NRS; i++) {
//        int nrDigits = nrDigit(gen);
//
//        vector<int> crtNumber;
//        for(int j = 1; j <= nrDigits; j++) {
//            crtNumber.push_back(  digit(gen) );
//        }
//
//        v.push_back(crtNumber);
//    }
//
    vector<int> v1{1, 2, 3};
    vector<int> v2{0, 0, 1};
    vector<int> v3{0, 0, 9};
    v.push_back(v1);
    v.push_back(v2);
    v.push_back(v3);

    // Declare a vector with n - 1 lockfree queues
    vector<  shared_ptr<boost::lockfree::queue<int>> > queues;
    queues.reserve(NRS);
    for(int i = 0; i <= NRS; i++) {
        queues.emplace_back( shared_ptr< boost::lockfree::queue<int> >( new boost::lockfree::queue<int>(100) ) );
    }

    // Start the threads
    vector< future<void> > results;

    // Separately start the first thread
    for(int i = 0; i < v[0].size(); i++) {
        queues[0].get()->push( v[0][i] );
    }
    queues[0].get()->push(-1);
    results.emplace_back( std::async(doAddition, queues[0], v[1], queues[1]) );

    // Start the others
    for(int i = 2; i < NRS ; i++) {
        results.emplace_back( std::async(doAddition, queues[i - 1], v[i], queues[i]) );
    }

    // Print the result from queues[NRS]
    int resultDigit = 0;
    while(true) {
        if(queues[NRS - 1].get()->pop(resultDigit)) {
            if(resultDigit == -1) {
                break;
            }

            cout << resultDigit << " ";
        }
    }
    cout << "\n";



    return 0;
}
