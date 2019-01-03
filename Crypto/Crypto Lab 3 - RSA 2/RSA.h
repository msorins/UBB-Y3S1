//
// Created by Sorin Sebastian Mircea on 12/12/2018.
//

#ifndef CRYPTO_LAB_3_RSA_RSA_H
#define CRYPTO_LAB_3_RSA_RSA_H

#endif //CRYPTO_LAB_3_RSA_RSA_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <string>
#define MaxPrimeSize 10000

using namespace std;

struct PublicKey {
    long long n;
    long long e;

    PublicKey(long long int n, long long int e) : n(n), e(e) {}

    friend ostream &operator<<(ostream &os, const PublicKey &key) {
        os << "n: " << key.n << " e: " << key.e;
        return os;
    }
};
class RSA {
public:
    RSA() {
        generatePrimes();
        generatePandQ();
        generateNAndLengthEncryption();
        generatePhiN();
        generatePublicKey();
        generatePrivateKey();
    }

    PublicKey getPublicKey() {
        return PublicKey(this->n, this->e);
    }

    long long encrypt(long long nr) {
        return this->doEncrypt( nr );
    }

    long long decrypt(long long nr) {
        return this->doDecrypt(nr);
    }

    long long int getP() const {
        return p;
    }

    long long int getQ() const {
        return q;
    }

    long long int getN() const {
        return n;
    }

    long long int getE() const {
        return e;
    }

    long long int getPhiN() const {
        return phiN;
    }

    long long int getD() const {
        return d;
    }

    long long int getLengthEncrypted() const{
        return this->lengthEncrypted;
    }

private:
    vector<long long> primes;

    // Two Distinct prime nrs of aprox same size
    long long p;
    long long q;

    // Public Key
    long long n;
    long long e;

    // (p-1)*(q-1)
    long long phiN;

    // Private Key
    long long d;

    int lengthEncrypted = 0;
private:
    void generatePrimes() {
        vector<bool> isPrime(MaxPrimeSize, true);
        for(int i = 2; i <= MaxPrimeSize; i++) {
            if(isPrime[i]) {
                for(int j = i + i; j <= MaxPrimeSize; j += i) {
                    isPrime[j] = false;
                }
            }

        }

        for(int i = 20; i <= MaxPrimeSize; i++) {

            if(isPrime[i]) {
                this->primes.push_back(i);
            }
        }
    }

    void generatePandQ(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rndGen(3, primes.size() - 4);
        std::uniform_int_distribution<> rndGenTwo(1, 3);
        std::uniform_int_distribution<> rndGenThree(0, 1);

        int pos = rndGen(gen);
        int deviation = rndGenTwo(gen);
        int sign = rndGenThree(gen) == 0 ? 1 : -1;

        this->p = this->primes[pos];
        this->q = this->primes[ pos + sign * deviation];
    };

    void generateNAndLengthEncryption() {
        this->n = this->p * this->q;

        int aux = n;
        this->lengthEncrypted = 0;
        while(aux) {
            this->lengthEncrypted += 1;
            aux /= 27;
        }
    }

    void generatePhiN() {
        this->phiN = (this->p - 1) * (this->q - 1);
    }

    void generatePublicKey() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rndGen(2, this->phiN - 1);

        do {
            this->e = rndGen(rd);
        }while( this->gcd(this->e, this->phiN) != 1 );
    }

    void generatePrivateKey() {
        this->d = getInverseMod();
    }

    long long doEncrypt(long long nr) {
        return this->moduloPow(nr, this->e, this->n);
    }

    long long doDecrypt(long long nr) {
        return this->moduloPow(nr, this->d, this->n);
    }

    long long gcd(long long a, long long b) {
        long long c;
        while(b) {
            c = a % b;
            a = b;
            b = c;
        }

        return a;
    }

    long long moduloPow(long long base, long long power, long long modulo) {
        long long last = base % modulo;
        long long crt = base % modulo;
        long long res = 1;

        while(power) {
            int crtBit = power & 1;

            if(crtBit) {
                res = ( res * crt ) % modulo;
            }

            crt = (last * last) % modulo;
            last = crt;
            power >>= 1;
        }

        return res;
    }

    void cmmdc(long long a, long long b, long long &x, long long &y) {
        if(!b){x=1;y=0;return;}
        long long X,Y;
        cmmdc(b,a%b,X,Y);
        x=Y;
        y=X-a/b*Y;
    }

    long long getInverseMod() {
        long long x, y;
        this->cmmdc(this->e, this->phiN, x, y);
        x %= this->phiN;
        if(x < 0) {
            x += this->phiN;
        }

        return x;
    }

private:
    vector<long long> msgToLong(string msg) {
        vector<long long> vl;

        for(int i = 0; i < msg.size(); i += 2) {
            int crtDigit;
            if(msg[i] == ' ') {
                crtDigit = 0;
            } else {
                crtDigit = (int)msg[i] - 'a' + 1;
            }

            if(i + 1 < msg.size()) {
                int nextDigit;
                if(msg[i + 1] == ' ') {
                    nextDigit = 0;
                } else {
                    nextDigit = (int)msg[i + 1] - 'a' + 1;
                }
                vl.push_back(27 * crtDigit + nextDigit);
            } else {
                vl.push_back(27 * crtDigit);
            }
        }
        return vl;
    }

    string nrToString(long long nr) {
        string s;
        long long leftDigit = nr / 27;
        long long rightDigit = nr % 27;

        if(leftDigit == 0) {
            s += ' ';
        } else {
            s += leftDigit + 'a' - 1;
        }

        if(rightDigit == 0) {
            s += ' ';
        } else {
            s += rightDigit + 'a' - 1;
        }

        return s;
    }

};