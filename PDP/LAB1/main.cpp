// https://thispointer.com/c11-multithreading-part-3-carefully-pass-arguments-to-threads/
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

using namespace std;

/*
 *      2. Bank accounts
        At a bank, we have to keep track of the balance of some accounts.
        Also, each account has an associated log (the list of records of operations performed on that account).
        Each operation record shall have a unique serial number, that is incremented for each operation performed in the bank.

        We have concurrently run transfer operations, to be executer on multiple threads. Each operation
        transfers a given amount of money from one account to someother account, and also appends the information about the
        transfer to the logs of both accounts.

        From time to time, as well as at the end of the program, a consistency check shall be executed.

        It shall verify that the amount of money in each account corresponds with the operations records associated to that account,
        and also that all operations on each account appear also in the logs of the source or destination of the transfer.

        Documentation:
        CPU: 2,2 GHz Intel Core i7
        Limbaj: C++17
        Constante:
                threadNumber 5
                transactionNumber 1000
                accountsInBank 1000
        Timp:
        20000000 transactii:
            1 thread:  11514  ms
            2 threaduri: 8222 ms
            4 threaduri: 6651 ms
            8 threaduri: 4602 ms
           16 threaduri: 4204 ms
           32 threaduri: 3871 ms
           64 threaduri: 3773 ms
           128 threaduri: 3704 ms
           500 threaduri: 3544 ms

 */

struct Account {
    int initialBalance;
    int balance;
    vector<int> logs;
    mutex *mtx;

    Account(int startBalance) : initialBalance(startBalance) {}
};


class Bank {
private:
    vector<Account> accounts; // position in the vector = account number
    vector<unique_lock<mutex>> mtx;

public:
    int addAccount(int initialBalance) {
        Account newAcc(initialBalance);
        newAcc.balance = initialBalance;
        newAcc.mtx = new mutex();

        auto da = make_unique<Account>(newAcc);
        this->accounts.push_back(newAcc);
        int accountNumber = this->accounts.size() - 1;

        return accountNumber;
    }

    void doTransaction(int fromAccountNr, int toAccountNr, int withBalance) {
        if(fromAccountNr > accounts.size() || toAccountNr > accounts.size() || fromAccountNr < 0 | toAccountNr < 0)
            throw std::invalid_argument("One of the account number is invalid");

        // Lock the mutex
        this->accounts[min(fromAccountNr, toAccountNr)].mtx->lock();
        this->accounts[max(fromAccountNr,toAccountNr)].mtx->lock();

        // Do the transaction to fromAccount
        this->accounts[fromAccountNr].balance -= withBalance;
        this->accounts[fromAccountNr].logs.push_back(-1 * withBalance);

        // Do the transaction to toAccount
        this->accounts[toAccountNr].balance += withBalance;
        this->accounts[toAccountNr].logs.push_back(withBalance);

        // Unlock the mutex
        this->accounts[min(fromAccountNr, toAccountNr)].mtx->unlock();
        this->accounts[max(fromAccountNr,toAccountNr)].mtx->unlock();
    }

    int getBalanceOfAccount(int accountNr) {
        if(accountNr > accounts.size() || accountNr < 0)
            throw std::invalid_argument("The account number is invalid");

        return this->accounts[accountNr].balance;
    }

    void checkIntegrity() {
        for (int i = 0; i < this->accounts.size(); i++) {
            int checkedBalanced = this->accounts[i].initialBalance;

            for (auto operation: this->accounts[i].logs) {
                checkedBalanced += operation;
            }

            if (checkedBalanced != this->accounts[i].balance)
                throw std::runtime_error("Balance does not match with the logs");
        }
    }

    int getNumberOfAccounts() {
        return this->accounts.size();
    }

};

#define threadNumber 64
#define transactionNumber 20000000
#define accountsInBank 1000

void doTransactionInThread(Bank const &bk) {
    Bank & bank = const_cast<Bank &>(bk);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disAccountNumber(0, bank.getNumberOfAccounts() - 1);
    std::uniform_int_distribution<> disSum(1, 10);

    int total = transactionNumber / threadNumber;
    for(int i = 1; i <= total; i++) {
        int from = 0;
        int to = 0;

        while(to == from) {
            from = disAccountNumber(gen);
            to = disAccountNumber(gen);
        }

        int money = disSum(gen);
        bank.doTransaction(from, to, money);
    }
}

int main() {
    Bank bank;

    // Create bank accounts
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    for(int i = 1; i <= accountsInBank; i++) {
        bank.addAccount(dis(gen));
    }

    // Create and run threads
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();
    vector<thread> threads;
    for(int i = 1; i <= threadNumber; i++) {
        threads.emplace_back(thread(doTransactionInThread, std::ref(bank)) );
    }

    // Wait for all threads to finish
    for(auto &thread: threads) {
        thread.join();
    }

    auto t1 = Time::now();
    fsec fs = t1 - t0;

    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << d.count() << "ms\n";

    // Run the integrity check, if crashes . program broken
    bank.checkIntegrity();

    return 0;
}